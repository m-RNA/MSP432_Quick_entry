#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "lz4_stream.h"
#include "lz4_common.h"
#include "lz4_xxhash.h"

void LZ4_streamDecompressBlockInit(const LZ4_streamDecompressBlockParams *params, LZ4_streamDecompressBlockState *state, LZ4_status *status)
{
    /* Initialize LZ4 state */
    state->dstPtr = (uint8_t *)params->dst;
    state->dstLength = params->dstLength;
    state->dstOrigin = (uint8_t *)params->dst;
    if (params->containsBlockSize) {
        state->state = LZ4_BLOCK_SIZE;
        state->literalLength = 4;
        state->matchLength = 0;
        state->matchOffset = 0;
    }
    else {
        state->state = LZ4_TOKEN;
        state->literalLength = 0;
        state->matchLength = 0;
        state->matchOffset = 0;
    }

    /* Return success */
    *status = LZ4_SUCCESS;
    return;
}

uint32_t LZ4_streamDecompressBlock(LZ4_streamDecompressBlockState *state, const void *data, uint16_t length, LZ4_status *status)
{
    uint8_t temp;
    uint8_t token;
    uint8_t *dataPtr;
    uint16_t offset;

    /* Initialize data pointer */
    dataPtr = (uint8_t *)data;

    while (length--) {
        switch (state->state) {
        case LZ4_BLOCK_SIZE:
            /* Decrement block size length and set next state when done. */
            temp = *dataPtr++;
            if (--state->literalLength == 0) {
                state->state = LZ4_TOKEN;
            }
            break;
        case LZ4_LITERAL_LENGTH:
            /* Read in additional literal length byte. */
            temp = *dataPtr++;
            state->literalLength += temp;

            /* Set next state if done reading in literal length. */
            if (temp != 255) {
                state->state = LZ4_LITERAL;
            }
            break;
        case LZ4_LITERAL:
            /* Read in literal byte if there are enough bytes remaining. */
            if (state->dstLength > 0) {
                temp = *dataPtr++;
                *state->dstPtr++ = temp;
                state->dstLength--;
            }
            else {
                /* Set status as partial and return the total block length. */
                *status = LZ4_PARTIAL_SUCCESS;
                return ((uintptr_t)state->dstPtr - (uintptr_t)state->dstOrigin);
            }

            /* Set next state if done reading in literal bytes. */
            if (--state->literalLength == 0) {
                /* Set state to read in match offset address. */
                state->state = LZ4_MATCH_OFFSET_LOW;
            }
            break;
        case LZ4_MATCH_LENGTH:
            /* Read in additional match length byte. */
            temp = *dataPtr++;
            state->matchLength += temp;

            /* Perform copy if done reading match length. */
            if (temp != 255) {
                /* Check that there are enough bytes remaining in the dst buffer. */
                if ((int32_t)state->matchLength > state->dstLength) {
                    /* Copy as many bytes as possible. */
                    while (state->dstLength) {
                        *state->dstPtr = *(state->dstPtr-state->matchOffset);
                        state->dstPtr++;
                        state->dstLength--;
                    }

                    /* Set status as partial and return the total block length. */
                    *status = LZ4_PARTIAL_SUCCESS;
                    return ((uintptr_t)state->dstPtr - (uintptr_t)state->dstOrigin);
                }

                /* Copy match bytes to destination buffer. */
                state->dstLength -= state->matchLength;
                while (state->matchLength--) {
                    *state->dstPtr = *(state->dstPtr-state->matchOffset);
                    state->dstPtr++;
                }
                state->state = LZ4_TOKEN;
            }
            break;
        case LZ4_MATCH_OFFSET_LOW:
            /* Read in match offset low byte and set next state. */
            offset = *dataPtr++;
            state->matchOffset = offset;
            state->state = LZ4_MATCH_OFFSET_HIGH;
            break;
        case LZ4_MATCH_OFFSET_HIGH:
            /* Read in match offset high byte. */
            offset = *dataPtr++;
            state->matchOffset += offset << 8;

            /* Set the next state or perform copy. */
            if (state->matchLength == 15 + LZ4_MIN_MATCH) {
                /* Set state to read in more match length bytes. */
                state->state = LZ4_MATCH_LENGTH;
            }
            else {
                /* Check that there are enough bytes remaining in the dst buffer. */
                if ((int32_t)state->matchLength > state->dstLength) {
                    /* Copy as many bytes as possible. */
                    while (state->dstLength) {
                        *state->dstPtr = *(state->dstPtr-state->matchOffset);
                        state->dstPtr++;
                        state->dstLength--;
                    }

                    /* Set status as partial and return the total block length. */
                    *status = LZ4_PARTIAL_SUCCESS;
                    return ((uintptr_t)state->dstPtr - (uintptr_t)state->dstOrigin);
                }

                /* Copy match bytes to destination buffer. */
                state->dstLength -= state->matchLength;
                while (state->matchLength--) {
                    *state->dstPtr = *(state->dstPtr-state->matchOffset);
                    state->dstPtr++;
                }
                state->state = LZ4_TOKEN;
            }
            break;
        case LZ4_TOKEN:
        default:
            /* Read token byte and set lengths. */
            token = *dataPtr++;
            state->literalLength = (token & LZ4_TOKEN_LITERAL_MASK) >> 4;
            state->matchLength = (token & LZ4_TOKEN_MATCH_MASK) + LZ4_MIN_MATCH;

            /* Set next state */
            if (state->literalLength == 15) {
                /* Set state to read in more literal length bytes. */
                state->state = LZ4_LITERAL_LENGTH;
            }
            else if (state->literalLength != 0) {
                /* Set state to read in literal bytes. */
                state->state = LZ4_LITERAL;
            }
            else {
                /* Set state to read in match offset address. */
                state->state = LZ4_MATCH_OFFSET_LOW;
            }
            break;
        }
    }

    /* Return ongoing and length of decompressed data */
    *status = LZ4_SUCCESS;
    return ((uintptr_t)state->dstPtr - (uintptr_t)state->dstOrigin);
}
