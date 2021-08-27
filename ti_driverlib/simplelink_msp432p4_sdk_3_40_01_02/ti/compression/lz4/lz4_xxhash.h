#ifndef LZ4_XXHASH_H_
#define LZ4_XXHASH_H_

//******************************************************************************
//
//! \addtogroup lz4_xxhash_api
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
//! \brief Compute xxhash checksum for a block of data.
//!
//! Used to compute and verify checksums for the LZ4 frame and block format.
//!
//! \param src Pointer to the data block.
//! \param length Length of data block.
//! \param seed Initialization value.
//! \return The computed xxhash checksum.
//
//******************************************************************************
extern uint32_t LZ4_xxHashCompute(const void *src, uint32_t length, uint32_t seed);

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

#endif /* LZ4_XXHASH_H_ */
