#ifndef LZ4_H_
#define LZ4_H_

//******************************************************************************
//
//! \addtogroup lz4_api
//! @{
//
//******************************************************************************

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>

//******************************************************************************
//
//! \brief Maximum size of a compressed file, one percent plus 27 bytes larger
//!        than uncompressed data.
//
//******************************************************************************
#define LZ4_COMPRESS_MAX_SIZE(n)        ((size_t)((n * 101) / 100) + 27)

//******************************************************************************
//
//! \brief LZ4 status return types.
//
//******************************************************************************
typedef enum {
    //! Successful operation.
    LZ4_SUCCESS,
    //! Data was partially decompressed due to insufficient space.
    LZ4_PARTIAL_SUCCESS,
    //! Content size is not present in LZ4 frame header.
    LZ4_NO_CONTENT_SIZE,
    //! Error in frame header.
    LZ4_FRAMING_ERROR,
    //! Incorrect block checksum.
    LZ4_BLOCK_CHECKSUM_ERROR,
    //! Incorrect content checksum.
    LZ4_CONTENT_CHECKSUM_ERROR
} LZ4_status;

//******************************************************************************
//
//! \brief Compression parameters for a LZ4 frame.
//
//******************************************************************************
typedef struct LZ4_compressParams {
    //! Pointer to the source data buffer to compress.
    const void *src;
    //! Pointer to the destination data buffer to store compressed data.
    void *dst;
    //! Length of source data buffer.
    uint32_t length;
    //! Pointer to memory block with pow2(hashLog2Size) bytes allocated.
    void *hashTable;
    //! Power of two used to determine the hash table size.
    uint16_t hashLog2Size;
    //! Add checksum to each compressed block. Decreases compression
    //! performance. Valid values are:
    //! - false
    //! - true
    bool addBlockChecksum;
    //! Add checksum of original source data buffer. Decreases compression
    //! performance. Valid values are:
    //! - false
    //! - true
    bool addContentChecksum;
    //! Add total content size to LZ4 frame. Increases total frame size by 8
    //! bytes. Valid values are:
    //! - false
    //! - true
    bool addContentSize;
} LZ4_compressParams;

//******************************************************************************
//
//! \brief Compression parameters for a single LZ4 block.
//
//******************************************************************************
typedef struct LZ4_compressBlockParams {
    //! Pointer to the source data buffer to compress.
    const void *src;
    //! Pointer to the destination data buffer to store compressed data.
    void *dst;
    //! Length of source data buffer.
    uint32_t length;
    //! Pointer to memory block with pow2(hashLog2Size) bytes allocated.
    void *hashTable;
    //! Power of two used to determine the hash table size.
    uint16_t hashLog2Size;
    //! Add checksum to each compressed block. Decreases compression
    //! performance. Valid values are:
    //! - false
    //! - true
    bool addBlockChecksum;
} LZ4_compressBlockParams;

//******************************************************************************
//
//! \brief Decompression parameters for a LZ4 frame.
//
//******************************************************************************
typedef struct LZ4_decompressParams {
    //! Pointer to the source data buffer to decompress.
    const void *src;
    //! Pointer to the destination data buffer to store compressed data.
    void *dst;
    //! Length of the destination data buffer.
    uint32_t dstLength;
    //! Enable verification of block checksum if present. Valid values are:
    //! - false
    //! - true
    bool verifyBlockChecksum;
    //! Enable verification of content checksum if present. Valid values are:
    //! - false
    //! - true
    bool verifyContentChecksum;
} LZ4_decompressParams;

//******************************************************************************
//
//! \brief Decompression parameters for a single LZ4 block.
//
//******************************************************************************
typedef struct LZ4_decompressBlockParams {
    //! Pointer to the source data buffer to decompress.
    const void *src;
    //! Pointer to the destination data buffer to store compressed data.
    void *dst;
    //! Length of the destination data buffer.
    uint32_t dstLength;
    //! Enable verification of block checksum if present. Valid values are:
    //! - false
    //! - true
    bool verifyBlockChecksum;
} LZ4_decompressBlockParams;

//******************************************************************************
//
//! \brief Compress a block of data to a LZ4 frame.
//!
//! Compress a block of data using LZ4 compression and add LZ4 framing. This
//! API will compress data to a valid LZ4 file and contains several parameters
//! to enable or disable features of the LZ4 framing specification such as
//! content and block checksum using the xxHash algorithm or block size. See
//! the LZ4_compressParams structure documentation for more details about the
//! available parameters.
//!
//! A block compressed with this method can be saved as a binary .lz4 file and
//! extracted using the LZ4 command line utility.
//!
//! \param params Pointer to the LZ4 compression parameter structure.
//! \param status Pointer to a LZ4 status that will contain the result status.
//! \return The total compressed frame size.
//
//******************************************************************************
extern uint32_t LZ4_compress(const LZ4_compressParams *params, LZ4_status *status);

//******************************************************************************
//
//! \brief Compress a block of data to a single LZ4 block without framing.
//!
//! Compress a block of data using only LZ4 compression and block format. This
//! API can be used to compress data and create a custom framing scheme using an
//! alternative checksum method. The LZ4 block format has a single parameter
//! for enabling block checksum computation with the xxHash algorithm. The
//! block checksum is computed on the compressed data block. See the
//! LZ4_compressBlockParams structure documentation for more details about the
//! available parameters.
//!
//! \param params Pointer to the LZ4 compression block parameter structure.
//! \param status Pointer to a LZ4 status that will contain the result status.
//! \return The total compressed block size.
//
//******************************************************************************
extern uint32_t LZ4_compressBlock(const LZ4_compressBlockParams *params, LZ4_status *status);

//******************************************************************************
//
//! \brief Decompress a LZ4 frame to a block of data.
//!
//! Decompress a LZ4 frame to an uncompressed data block. This API contains
//! parameters to enable checking of content and block checksum. When enabled
//! the xxHash algorithm is used to verify the checksum. While not required it
//! is the application programmers responsibility to determine if validating
//! the checksum is necessary. See the LZ4_decompressParams structure
//! documentation for more details about the available parameters.
//!
//! A .lz4 file compressed with the LZ4 command line utility can be
//! decompressed using this API.
//!
//! \param params Pointer to the LZ4 decompression parameter structure.
//! \param status Pointer to a LZ4 status that will contain the result status.
//! \return The total decompressed data block size.
//
//******************************************************************************
extern uint32_t LZ4_decompress(const LZ4_decompressParams *params, LZ4_status *status);

//******************************************************************************
//
//! \brief Decompress a single LZ4 block without framing to a block of data.
//!
//! Decompress a LZ4 block to an uncompressed data block. This API contains
//! a single parameter to enable checking of the block checksum. When enabled
//! the xxHash algorithm is used to verify the checksum. While not required it
//! is the application programmers responsibility to determine if validating
//! the checksum is necessary. See the LZ4_decompressBlockParams structure
//! documentation for more details about the available parameters.
//!
//! \param params Pointer to the LZ4 decompression block parameter structure.
//! \param status Pointer to a LZ4 status that will contain the result status.
//! \return The total decompressed data block size.
//
//******************************************************************************
extern uint32_t LZ4_decompressBlock(const LZ4_decompressBlockParams *params, LZ4_status *status);

//******************************************************************************
//
//! \brief Get the content size of a compressed LZ4 frame.
//!
//! Get the size of the original uncompressed data block if it is present. The
//! content size is an optional parameter when compressing but it is recommended
//! to enable it and verify the uncompressed data block fits in the allocated
//! buffer before decompressing.
//!
//! \param src Pointer to the LZ4 frame.
//! \param status Pointer to a LZ4 status that will contain the result status.
//! \return The total decompressed content size. The content size is stored as
//! 64-bit but will be truncated to 32-bit.
//
//******************************************************************************
extern uint32_t LZ4_getContentSize(const uint8_t *src, LZ4_status *status);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

//******************************************************************************
//
// Close the Doxygen group.
//! @}
//
//******************************************************************************

#endif /* LZ4_H_ */
