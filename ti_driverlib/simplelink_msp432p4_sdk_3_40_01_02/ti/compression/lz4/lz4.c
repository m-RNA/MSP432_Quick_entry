#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "lz4.h"
#include "lz4_common.h"
#include "lz4_xxhash.h"

uint32_t LZ4_compress(const LZ4_compressParams *params, LZ4_status *status)
{
    uint8_t *dstPtr;
    uint8_t *posPtr;
    uint8_t flgByte;
    uint32_t dstLength;
    uint32_t checksum;
    uint32_t blockLength;
    const uint8_t *srcPtr;
    const uint8_t *blockPtr;
    LZ4_compressBlockParams blockParams;
    
    /* Initialize local pointers. */
    srcPtr = params->src;
    dstPtr = params->dst;
    posPtr = dstPtr;
    
    /* Set frame magic number */
    posPtr = write32(posPtr, LZ4_MAGIC_NUMBER);
    
    /* Set LZ4 frame FLG byte. */
    flgByte = LZ4_FLG_VERSION | LZ4_FLG_BLOCK_INDEP;
    if (params->addBlockChecksum) {
        flgByte |= LZ4_FLG_BLOCK_CHECKSUM;
    }
    if (params->addContentSize) {
        flgByte |= LZ4_FLG_CONTENT_SIZE;
    }
    if (params->addContentChecksum) {
        flgByte |= LZ4_FLG_CONTENT_CHECKSUM;
    }
    *posPtr++ = flgByte;
    
    /* Set LZ4 fram BD byte. */
    *posPtr++ = LZ4_BD_BYTE;
    
    /* Set content size if parameter is enabled. */
    if (params->addContentSize) {
        posPtr = write32(posPtr, params->length);
        posPtr = write32(posPtr, 0);
    }
    
    /* Calculate and set header checksum */
    checksum = LZ4_xxHashCompute((void *)(dstPtr+4), (uintptr_t)posPtr - (uintptr_t)(dstPtr+4), 0);
    *posPtr++ = (checksum >> 8) & 0xff;
    
    /* Compress data blocks. */
    blockPtr = params->src;
    blockLength = params->length;
    blockParams.hashTable = params->hashTable;
    blockParams.hashLog2Size = params->hashLog2Size;
    blockParams.addBlockChecksum = params->addBlockChecksum;
    while (blockLength > LZ4_MAX_BLOCK_SIZE) {
        blockParams.src = blockPtr;
        blockParams.dst = posPtr;
        blockParams.length = LZ4_MAX_BLOCK_SIZE;
        dstLength = LZ4_compressBlock(&blockParams, status);
        
        /* Increment pointer by compressed size. */
        posPtr += dstLength;
        
        /* Increment block pointer and decrement block length. */
        blockPtr += LZ4_MAX_BLOCK_SIZE;
        blockLength -= LZ4_MAX_BLOCK_SIZE;
    }
    
    /* Compress final block. */
    blockParams.src = blockPtr;
    blockParams.dst = posPtr;
    blockParams.length = blockLength;
    dstLength = LZ4_compressBlock(&blockParams, status);
        
    /* Increment pointer by compressed size. */
    posPtr += dstLength;
    
    /* Set end mark. */
    posPtr = write32(posPtr, 0);
    
    /* Set content checksum if parameter is enabled. */
    if (params->addContentChecksum) {
        /* Calculate content checksum and add to end of the frame. */
        checksum = LZ4_xxHashCompute((void *)srcPtr, params->length, 0);
        posPtr = write32(posPtr, checksum);
    }
    
    /* Return total frame size. */
    return ((uintptr_t)posPtr - (uintptr_t)dstPtr);
}

uint32_t LZ4_compressBlock(const LZ4_compressBlockParams *params, LZ4_status *status)
{
    uint8_t token;
    uint8_t *dstPtr;
    uint8_t *dstOrigin;
    const uint8_t *srcPtr;
    const uint8_t *posPtr;
    const uint8_t *matchPtr;
    const uint8_t *srcPtrEnd;
    uint16_t index;
    uint16_t posOffset;
    uint16_t backOffset;
    uint16_t matchOffset;
    uint16_t *hashTable;
    uint16_t hashMask;
    uint16_t hashLength;
    uint16_t matchLength;
    uint16_t literalLength;
    uint16_t remainingLength;
    uint32_t checksum;
    uint32_t blockLength;
    
    /* Initialize local pointers and counters and masks. */
    dstOrigin = params->dst;
    dstPtr = dstOrigin+4;
    srcPtr = params->src;
    posPtr = params->src;
    matchPtr = params->src;
    srcPtrEnd = srcPtr + params->length - LZ4_LAST_MATCH_DIST;

    /* Initialize the provided hash table */
    hashTable = (uint16_t *)params->hashTable;
    hashLength = 1 << (params->hashLog2Size - 1);
    hashMask = hashLength - 1;
    while (hashLength--) {
        *hashTable++ = 0xffff;
    }
    hashTable = (uint16_t *)params->hashTable;
          
    /* Run until the end of input */
    while (matchPtr < srcPtrEnd) {
        /* Calculate the hash index. */
        index = calculateHash(read32(matchPtr)) & hashMask;
        
        /* Check if index is used */
        posOffset = (uintptr_t)matchPtr - (uintptr_t)srcPtr;
        matchOffset = hashTable[index];
        backOffset = posOffset - matchOffset;
        if ((matchOffset != 0xffff) && (backOffset != 0)) {
            /* Check if there is a match. */
            matchLength = checkMatch(matchPtr, matchPtr-backOffset);
            if (matchLength >= LZ4_MIN_MATCH) {
                /* Saturate match length to end of src block */
                if ((matchPtr + matchLength) > (srcPtrEnd + (LZ4_LAST_MATCH_DIST-LZ4_LAST_LITERAL_LENGTH))) {
                    matchLength = srcPtrEnd + (LZ4_LAST_MATCH_DIST-LZ4_LAST_LITERAL_LENGTH) - matchPtr;
                }
                
                /* Calculate token length values. */
                matchLength -= LZ4_MIN_MATCH;
                literalLength = (uintptr_t)matchPtr - (uintptr_t)posPtr;
                
                /* Calculate token value and write it to destination. */
                if (matchLength >= 15) {
                    token = LZ4_TOKEN_MATCH_MASK;
                }
                else {
                    token = matchLength & LZ4_TOKEN_MATCH_MASK;
                }
                if (literalLength >= 15) {
                    /* Add additional literal length codes */
                    *dstPtr++ = LZ4_TOKEN_LITERAL_MASK | token;
                    literalLength -= 15;
                    while (literalLength >= 255) {
                        *dstPtr++ = 255;
                        literalLength -= 255;
                    }
                    *dstPtr++ = literalLength;
                }
                else {
                    *dstPtr++ = ((literalLength << 4) & LZ4_TOKEN_LITERAL_MASK) | token;
                }
                
                /* Copy the literals. */
                while (posPtr != matchPtr) {
                    *dstPtr++ = *posPtr++;
                }
                
                /* Add the 16-bit back pointer */
                dstPtr = write16(dstPtr, backOffset);
                    
                /* Add additional literal length codes if necessary. */
                if (matchLength >= 15) {
                    uint16_t matchLengthCopy = matchLength;
                    matchLengthCopy -= 15;
                    while (matchLengthCopy >= 255) {
                        *dstPtr++ = 255;
                        matchLengthCopy -= 255;
                    }
                    *dstPtr++ = matchLengthCopy;
                }
                
                /* Store position on hash */
                hashTable[index] = posOffset;
                
                /* Move positional pointer forwards past match and update hash table. */
                matchPtr += matchLength + LZ4_MIN_MATCH;
                posPtr = matchPtr;
                posOffset = (uintptr_t)posPtr - (uintptr_t)srcPtr;
                index = calculateHash(read32(posPtr-2)) & hashMask;
                hashTable[index] = posOffset-2;
            }
            else {
                /* Store the current back pointer and increment match pointer. */
                hashTable[index] = posOffset;
                matchPtr++;
            }
        }
        else {
            /* Store the current back pointer and increment match pointer. */
            hashTable[index] = posOffset;
            matchPtr++;
        }
    }
    
    /* Calculate remaining literals length. */
    srcPtrEnd += LZ4_LAST_MATCH_DIST;
    remainingLength = (uintptr_t)srcPtrEnd - (uintptr_t)posPtr;
    
    /* Set remaining literals token and additional length bytes if necessary. */
    if (remainingLength >= 15) {
        *dstPtr++ = LZ4_TOKEN_LITERAL_MASK;
        remainingLength -= 15;
        while (remainingLength >= 255) {
            *dstPtr++ = 255;
            remainingLength -= 255;
        }
        *dstPtr++ = remainingLength;
    }
    else {
        *dstPtr++ = ((remainingLength << 4) & LZ4_TOKEN_LITERAL_MASK);
    }
                
    /* Copy the literals. */
    while (posPtr < srcPtrEnd) {
        *dstPtr++ = *posPtr++;
    }
    
    /* Write the block size. */
    blockLength = (uintptr_t)dstPtr - (uintptr_t)(dstOrigin+4);
    write32(dstOrigin, blockLength);
        
    /* Set block checksum if parameter is enabled. */
    if (params->addBlockChecksum) {
        /* Calculate block checksum and add to end of block. */
        checksum = LZ4_xxHashCompute((void *)(dstOrigin+4), blockLength, 0);
        dstPtr = write32(dstPtr, checksum);
    }
    
    /* Set status and return size. */
    *status = LZ4_SUCCESS;
    return ((uintptr_t)dstPtr - (uintptr_t)dstOrigin);
}

uint32_t LZ4_decompress(const LZ4_decompressParams *params, LZ4_status *status)
{
    uint8_t *dstPtr;
    const uint8_t *srcPtr;
    uint8_t *dstOrigin;
    uint8_t flgByte;
    uint32_t checksum;
    uint32_t dstLength;
    uint32_t blockLength;
    uint32_t dstBlockLength;
    LZ4_decompressBlockParams blockParams;
    
    /* Initialize local pointers and variables. */
    srcPtr = params->src;
    dstOrigin = params->dst;
    dstPtr = dstOrigin;
    dstLength = params->dstLength;
    
    /* Check frame magic number */
    if (read32(srcPtr) != LZ4_MAGIC_NUMBER) {
        /* ERROR, frame magic number mismatch. */
        *status = LZ4_FRAMING_ERROR;
        return 0;
    }
    srcPtr += 4;
    
    /* Read LZ4 frame FLG and BD bytes. */
    flgByte = *srcPtr;
    
    /* Verfy header checksum */
    if (flgByte & LZ4_FLG_CONTENT_SIZE) {
        checksum = LZ4_xxHashCompute((void *)srcPtr, 10, 0);
        if (srcPtr[10] != ((checksum >> 8) & 0xff)) {
            /* ERROR, frame checksum mismatch. */
            *status = LZ4_FRAMING_ERROR;
            return 0;
        }
        srcPtr += 11;
    }
    else {
        checksum = (LZ4_xxHashCompute((void *)srcPtr, 2, 0) >> 8) & 0xff;
        if (srcPtr[2] != checksum) {
            /* ERROR, frame checksum mismatch. */
            *status = LZ4_FRAMING_ERROR;
            return 0;
        }
        srcPtr += 3;
    }
    
    /* Process data blocks */
    blockParams.verifyBlockChecksum = params->verifyBlockChecksum && (flgByte & LZ4_FLG_BLOCK_CHECKSUM);
    blockLength = read32(srcPtr);
    while (blockLength != 0) {
        /* Add checksum size to block length if present. */
        if (flgByte & LZ4_FLG_BLOCK_CHECKSUM) {
            blockLength += 4;
        }
        
        /* Set block parameters. */
        blockParams.dst = dstPtr;
        blockParams.src = srcPtr;
        blockParams.dstLength = dstLength;
            
        /* Decompress block. */
        dstBlockLength = LZ4_decompressBlock(&blockParams, status);
        
        /* Increment source and destination pointers. */
        srcPtr += blockLength + 4;
        dstPtr += dstBlockLength;
        dstLength -= dstBlockLength;
        
        /* Check status of block decompress operation. */
        if (*status != LZ4_SUCCESS) {
            /* Return partial decompressed length. */
            dstLength = (uintptr_t)dstPtr - (uintptr_t)dstOrigin;
            return dstLength;
        }
        
        /* Read next block size. */
        blockLength = read32(srcPtr);
    }
    
    /* Get total decompressed size. */
    dstLength = (uintptr_t)dstPtr - (uintptr_t)dstOrigin;
            
    /* Verfy content checksum if enabled. */
    if ((flgByte & LZ4_FLG_CONTENT_CHECKSUM) && params->verifyContentChecksum) {
        checksum = LZ4_xxHashCompute((void *)dstOrigin, dstLength, 0);
        if (checksum != read32(srcPtr+4)) {
            /* ERROR, content checksum mismatch. */
            *status = LZ4_CONTENT_CHECKSUM_ERROR;
            return 0;
        }
    }
    
    /* Return the total decompressed length. */
    return dstLength;
}

uint32_t LZ4_decompressBlock(const LZ4_decompressBlockParams *params, LZ4_status *status)
{
    uint8_t token;
    uint8_t *dstPtr;
    uint8_t *dstOrigin;
    const uint8_t *srcPtr;
    const uint8_t *srcEnd;
    uint16_t matchOffset;
    uint16_t matchLength;
    uint16_t literalLength;
    int32_t dstLength;
    uint32_t checksum;
    uint32_t blockLength;
    
    /* Initialize local pointers and variables. */
    srcPtr = params->src;
    dstOrigin = params->dst;
    dstPtr = dstOrigin;
    dstLength = params->dstLength;
    
    /* Read block length and increment source pointer. */
    blockLength = read32(srcPtr);
    srcPtr += 4;
    srcEnd = srcPtr + blockLength;
    
    /* Check if checksum is present */
    if (params->verifyBlockChecksum) {
        checksum = LZ4_xxHashCompute((void *)(srcPtr), blockLength, 0);
        if (checksum != read32(srcPtr+blockLength)) {
            /* ERROR, block checksum mismatch. */
            *status = LZ4_BLOCK_CHECKSUM_ERROR;
            return 0;
        }
    }
    
    /* Decompress the data block. */
    while (srcPtr < srcEnd) {
        /* Read in token byte and extract literal and match lengths. */
        token = *srcPtr++;
        matchLength = (token & LZ4_TOKEN_MATCH_MASK) + LZ4_MIN_MATCH;
        literalLength = (token & LZ4_TOKEN_LITERAL_MASK) >> 4;
        
        /* Read in additional literal length bytes if necessary. */
        if (literalLength == 15) {
            while (*srcPtr == 255) {
                literalLength += *srcPtr++;
            }
            literalLength += *srcPtr++;
        }
        
        /* Check that there are enough bytes remaining in the dst buffer. */
        if ((int32_t)literalLength > dstLength) {
            /* Copy as many bytes as possible. */
            while (dstLength--) {
                *dstPtr++ = *srcPtr++;
            }
            
            /* Set status as partial and return the total block length. */
            *status = LZ4_PARTIAL_SUCCESS;
            return ((uintptr_t)dstPtr - (uintptr_t)dstOrigin);
        }
        
        /* Copy literal bytes to destination buffer. */
        dstLength -= literalLength;
        while (literalLength--) {
            *dstPtr++ = *srcPtr++;
        }
        
        /* Check if we're at the end of the block since last token does not contain match. */
        if (srcPtr >= srcEnd) {
            break;
        }
        
        /* Read in match offset. */
        matchOffset = read16(srcPtr);
        srcPtr += 2;
        
        /* Read in additional match length bytes if necessary. */
        if (matchLength == (15 + LZ4_MIN_MATCH)) {
            while (*srcPtr == 255) {
                matchLength += *srcPtr++;
            }
            matchLength += *srcPtr++;
        }
        
        /* Check that match offset is within range. */
        if (matchOffset > ((uintptr_t)dstPtr - (uintptr_t)dstOrigin)) {
            *status = LZ4_PARTIAL_SUCCESS;
            return 0;
        }
        
        /* Check that there are enough bytes remaining in the dst buffer. */
        if ((int32_t)matchLength > dstLength) {
            /* Copy as many bytes as possible. */
            while (dstLength--) {
                *dstPtr = *(dstPtr-matchOffset);
                dstPtr++;
            }
            
            /* Set status as partial and return the total block length. */
            *status = LZ4_PARTIAL_SUCCESS;
            return ((uintptr_t)dstPtr - (uintptr_t)dstOrigin);
        }
        
        /* Copy match bytes to destination buffer. */
        dstLength -= matchLength;
        while (matchLength--) {
            *dstPtr = *(dstPtr-matchOffset);
            dstPtr++;
        }
    }
    
    /* Set status as success and return the total block length. */
    *status = LZ4_SUCCESS;
    return ((uintptr_t)dstPtr - (uintptr_t)dstOrigin);
}

uint32_t LZ4_getContentSize(const uint8_t *src, LZ4_status *status)
{
    const uint8_t *srcPtr;
    uint32_t contentSize;
    
    /* Initialize local pointers and variables. */
    srcPtr = src;
    
    /* Check frame magic number to make sure header is valid. */
    if (read32(srcPtr) != LZ4_MAGIC_NUMBER) {
        /* ERROR, frame magic number mismatch. */
        *status = LZ4_FRAMING_ERROR;
        return 0;
    }
    srcPtr += 4;
    
    /* Check if content size flag is set. */
    if (*srcPtr & LZ4_FLG_CONTENT_SIZE) {
        /* Read and return content size. */
        contentSize = read32(srcPtr+2);
        *status = LZ4_SUCCESS;
        return contentSize;
    }
    else {
        /* Content size flag is not set. */
        *status = LZ4_NO_CONTENT_SIZE;
        return 0;
    }
}
