#ifndef LZ4_COMMON_H_
#define LZ4_COMMON_H_

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

#define LZ4_MAGIC_NUMBER                0x184d2204

#define LZ4_MAX_BLOCK_SIZE              0x00100000

#define LZ4_FLG_VERSION                 0x40
#define LZ4_FLG_BLOCK_INDEP             0x20
#define LZ4_FLG_BLOCK_CHECKSUM          0x10
#define LZ4_FLG_CONTENT_SIZE            0x08
#define LZ4_FLG_CONTENT_CHECKSUM        0x04
#define LZ4_BD_BYTE                     0x70

#define LZ4_MIN_MATCH                   4
#define LZ4_LAST_MATCH_DIST             12
#define LZ4_LAST_LITERAL_LENGTH         5

#define LZ4_TOKEN_LITERAL_MASK          0xf0
#define LZ4_TOKEN_MATCH_MASK            0x0f

#define LZ4_HASH_VALUE                  0x9E3779B1

static inline uint16_t calculateHash(uint32_t input)
{
    return (uint16_t)(((input) * LZ4_HASH_VALUE) >> 16);
}

static inline uint16_t read16(const uint8_t* ptr)
{
    /* Odd address */
    if ((uintptr_t)ptr & 1) {
        uint16_t value = ptr[0];
        value += (uint16_t)ptr[1] << 8;
        return value;
    }
    /* Even address */
    else {
        return *(uint16_t *)ptr;
    }
}

static inline uint32_t read32(const uint8_t* ptr)
{
    /* Odd address */
    if ((uintptr_t)ptr & 1) {
        uint32_t value = ptr[0];
        value += (uint32_t)(*(uint16_t *)(ptr+1)) << 8;
        value += (uint32_t)(ptr[3]) << 24;
        return value;
    }
    /* Even address */
    else {
        return *(uint32_t *)ptr;
    }
}

static inline uint8_t *write16(uint8_t *ptr, uint16_t data)
{
    if ((uintptr_t)ptr & 1) {
        *ptr++ = data & 0xff;
        *ptr++ = (data >> 8) & 0xff;
        return ptr;
    }
    else {
        *((uint16_t *)ptr) = data;
        return ptr+2;
    }
}

static inline uint8_t *write32(uint8_t *ptr, uint32_t data)
{
    if ((uintptr_t)ptr & 1) {
        *ptr++ = data & 0xff;
        *ptr++ = (data >> 8) & 0xff;
        *ptr++ = (data >> 16) & 0xff;
        *ptr++ = (data >> 24) & 0xff;
        return ptr;
    }
    else {
        *((uint32_t *)ptr) = data;
        return ptr+4;
    }
}

static inline uint16_t checkMatch(const uint8_t *pos1, const uint8_t *pos2)
{
    uint16_t length = 0;
    while ((*pos1++ == *pos2++) && (length != 0xffff)) {
        length++;
    }
    return length;
}

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

#endif /* LZ4_COMMON_H_ */
