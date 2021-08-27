/*
 * Copyright (c) 2015-2019, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*!****************************************************************************
 *  @file   I2CSlave.h
 *  @brief  Inter-Integrated Circuit (I2C) Slave Driver
 *
 *  @anchor ti_drivers_I2CSlave_Overview
 *  # Overview
 *
 *  The I2C Slave driver allows you to send and recieve I2C transfers.
 *  This driver complements the @ref I2C.h driver which operates
 *  as an I2C master.
 *
 *  <hr>
 *  @anchor ti_drivers_I2CSlave_Usage
 *  # Usage
 *
  *  This documentation provides a basic @ref ti_drivers_I2CSlave_Synopsis
 *  "usage summary" and a set of @ref ti_drivers_I2CSlave_Examples "examples"
 *  in the form of commented code fragments. Detailed descriptions of the
 *  APIs are provided in subsequent sections.


 *  @anchor ti_drivers_I2CSlave_Synopsis
 *  ## Synopsis #
 *  @anchor ti_drivers_I2CSlave_Synopsis_Code
 *  @code
 *  // Import I2C Slave Driver definitions
 *  #include <ti/drivers/I2CSlave.h>
 *
 *  // Define name for an index of an I2C Slave
 *  #define MASTER_BUS 0
 *
 *  // One-time init of I2C driver
 *  I2CSlave_init();
 *
 *  // initialize optional I2C parameters
 *  I2CSlave_Params params;
 *  I2CSlave_Params_init(&params);
 *  params.transferMode = I2CSLAVE_MODE_BLOCKING;
 *
 *  // Open I2C Slavefor usage
 *  I2CSlave_Handle i2cHandle = I2CSlave_open(MASTER_BUS, &params);
 *
 *  // Wait for a write from an I2C master device
 *  I2CSlave_read(i2cHandle, buffer, 4);
 *
 *  // Write to the I2C master device
 *  I2CSlave_write(i2cHandle, buffer, 2);
 *
 *  // Close I2C
 *  I2CSlave_close(i2cHandle);
 *  @endcode
 *
 *  @anchor ti_drivers_I2CSlave_Examples
 *  ## Examples
 *
 *  @li @ref ti_drivers_I2CSlave_Example_open "Getting an I2C Slave handle"
 *  @li @ref ti_drivers_I2CSlave_Example_transferring "Transferring data"
 *
 *  @anchor ti_drivers_I2CSlave_Example_open
 *  ## Opening the driver
 *
 *  @code
 *  // Define name for an index of an I2C Slave
 *  #define MASTER_BUS 0
 *
 *  I2CSlave_Handle handle;
 *  I2CSlave_Params params;
 *
 *  // One-time init of I2C Slave driver
 *  I2CSlave_init();
 *
 *  I2CSlave_Params_init(&params);
 *  params.transferMode  = I2CSLAVE_MODE_CALLBACK;
 *  params.transferCallbackFxn = callbackFxn;
 *
 *  handle = I2CSlave_open(0, &params);
 *  if (handle == NULL) {
 *      // I2C Slave failed to open
 *      while (1) {}
 *  }
 *  @endcode
 *
 *  @anchor ti_drivers_I2CSlave_Example_transferring
 *  ## Transferring data
 *
 *  @code
 *  status = I2CSlave_read(handle, buffer, 5)
 *  if (status == false) {
 *      //Unsuccessful I2CSlave read
 *  }
 *
 *  status = I2CSlave_write(handle, buffer, 3);
 *  if (status == false) {
 *      //Unsuccessful I2CSlave write
 *  }
 *  @endcode
 *
 *  <hr>
 *  @anchor ti_drivers_I2CSlave_Configuration
 *  # Configuration
 *
 *  Refer to the @ref driver_configuration "Driver's Configuration" section
 *  for driver configuration information.
 *  <hr>
 ******************************************************************************
 */

#ifndef ti_drivers_I2CSLAVE__include
#define ti_drivers_I2CSLAVE__include

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  @defgroup I2CSLAVE_CONTROL I2CSlave_control command and status codes
 *  These I2CSlave macros are reservations for I2CSlave.h
 *  @{
 */
/*!
 * Common I2CSlave_control command code reservation offset.
 * I2CSlave driver implementations should offset command codes with
 * I2CSLAVE_CMD_RESERVED growing positively
 *
 * Example implementation specific command codes:
 * @code
 * #define I2CSLAVEXYZ_COMMAND0          I2CSLAVE_CMD_RESERVED + 0
 * #define I2CSLAVEXYZ_COMMAND1          I2CSLAVE_CMD_RESERVED + 1
 * @endcode
 */
#define I2CSLAVE_CMD_RESERVED             (32)

/*!
 * Common I2CSlave_control status code reservation offset.
 * I2CSlave driver implementations should offset status codes with
 * I2CSLAVE_STATUS_RESERVED growing negatively.
 *
 * Example implementation specific status codes:
 * @code
 * #define I2CSLAVEXYZ_STATUS_ERROR0     I2CSLAVE_STATUS_RESERVED - 0
 * #define I2CSLAVEXYZ_STATUS_ERROR1     I2CSLAVE_STATUS_RESERVED - 1
 * #define I2CSLAVEXYZ_STATUS_ERROR2     I2CSLAVE_STATUS_RESERVED - 2
 * @endcode
 */
#define I2CSLAVE_STATUS_RESERVED         (-32)

/**
 *  @defgroup I2CSLAVE_STATUS Status Codes
 *  I2CSLAVE_STATUS_SUCCESS_* macros are general status codes returned by I2CSlave_control()
 *  @{
 *  @ingroup I2CSLAVE_CONTROL
 */
/*!
 * @brief    Successful status code returned by I2CSlave_control().
 *
 * I2CSlave_control() returns I2CSLAVE_STATUS_SUCCESS if the control code was
 * executed successfully.
 */
#define I2CSLAVE_STATUS_SUCCESS          (0)

/*!
 * @brief    Generic error status code returned by I2CSlave_control().
 *
 * I2CSlave_control() returns I2CSLAVE_STATUS_ERROR if the control code was not
 * executed successfully.
 */
#define I2CSLAVE_STATUS_ERROR           (-1)

/*!
 * @brief    An error status code returned by I2CSlave_control() for undefined
 * command codes.
 *
 * I2CSlave_control() returns I2CSLAVE_STATUS_UNDEFINEDCMD if the control code
 * is not recognized by the driver implementation.
 */
#define I2CSLAVE_STATUS_UNDEFINEDCMD   (-2)
/** @}*/

/**
 *  @defgroup I2CSLAVE_CMD Command Codes
 *  I2CSlave_CMD_* macros are general command codes for I2CSlave_control(). Not all I2CSlave
 *  driver implementations support these command codes.
 *  @{
 *  @ingroup I2CSLAVE_CONTROL
 */

/* Add I2CSLAVE_CMD_<commands> here */

/** @}*/

/** @}*/

/*!
 *  @brief      A handle that is returned from a I2CSlave_open() call.
 */
typedef struct I2CSlave_Config_      *I2CSlave_Handle;

/*!
 *  @private
 *  @brief  I2CSlave mode
 *
 *  This enum defines the state of the I2CSlave driver's state-machine.
 */
typedef enum {
    I2CSLAVE_IDLE_MODE = 0,  /*!< I2CSlave is not performing a transaction */
    I2CSLAVE_WRITE_MODE = 1,  /*!< I2CSlave is currently performing write */
    I2CSLAVE_READ_MODE = 2,  /*!< I2CSlave is currently performing read */
    I2CSLAVE_START_MODE = 3,  /*!< I2CSlave received a START from a master */
    I2CSLAVE_ERROR = 0xFF  /*!< I2CSlave error has occurred, exit gracefully */
} I2CSlave_Mode;

/*!
 *  @brief    Return behavior of I2CSlave_write() and I2CSlave_read()
 *  specified in the #I2CSlave_Params.
 *
 *  @sa  #I2CSlave_Params
 */
typedef enum {
    /*
     *  In #I2CSLAVE_MODE_BLOCKING, calls to I2CSlave_read() and
     *  I2CSlave_write() block until the transfer completes. Other threads
     *  calling I2CSlave_read() or I2CSlave_write() while a transfer is in
     *  progress are also placed into a blocked state. If multiple threads are
     *  blocked, the thread with the highest priority will be unblocked first.
     *  This implies that arbitration will not be executed in chronological
     *  order.
     *
     *  @note When using #I2CSLAVE_MODE_BLOCKING, I2CSlave_write() and
     *  I2CSlave_read() must be called from a thread context.
     */
    I2CSLAVE_MODE_BLOCKING,

    /*
     *  In #I2CSLAVE_MODE_CALLBACK, calls to I2CSlave_read() and
     *  I2CSlave_write() return immediately. The application's callback
     *  function, #I2CSlave_Params.transferCallbackFxn, is called when the
     *  transfer is complete. The #I2CSlave_Params.transferCallbackFxn function
     *  will be called asynchronously as each transaction is completed.
     */
    I2CSLAVE_MODE_CALLBACK
} I2CSlave_TransferMode;

/*!
 *  @brief  The definition of a callback function.
 *
 *  When operating in #I2CSLAVE_MODE_CALLBACK, the callback function is called
 *  when a transfer completes. The application is responsible for
 *  declaring an #I2CSlave_CallbackFxn function and providing a pointer
 *  in #I2CSlave_Params.transferCallbackFxn.
 *
 *  @warning  The callback function is called from an interrupt context.
 *
 *  @param[out]  handle    #I2CSlave_Handle used with the initial
 *  call to I2CSlave_read() or I2CSlave_write()

 *  @param [out]  status    Boolean indicating if the I2C transfer was
 *  successful. If @p true, the transfer was successful. If @p false,
 *  the transfer failed.
 */
typedef void (*I2CSlave_CallbackFxn)(I2CSlave_Handle handle, bool status);

/*!
 *  @brief I2CSlave parameters used with I2CSlave_open().
 *
 *  I2CSlave_Params_init() must be called prior to setting fields in
 *  this structure.
 *
 *  @sa  I2CSlave_Params_init()
 */
typedef struct {
    /*! Specifies the #I2CSlave_TransferMode */
    I2CSlave_TransferMode   transferMode;

    /*! Pointer to a #I2CSlave_CallbackFxn to be invoked after a
     *  transfer completes when operating in #I2CSLAVE_MODE_CALLBACK. */
    I2CSlave_CallbackFxn    transferCallbackFxn;

    /*! Custom argument used by a device specific implementation */
    void                   *custom;
} I2CSlave_Params;

/*!
 *  @private
 *  @brief      A function pointer to a driver specific implementation of
 *              I2CSlave_close().
 */
typedef void        (*I2CSlave_CloseFxn)    (I2CSlave_Handle handle);

/*!
 *  @private
 *  @brief      A function pointer to a driver specific implementation of
 *              I2CSlave_control().
 */
typedef int_fast16_t (*I2CSlave_ControlFxn)  (I2CSlave_Handle handle,
                                        uint_fast16_t cmd,
                                        void *arg);

/*!
 *  @private
 *  @brief      A function pointer to a driver specific implementation of
 *              I2CSlave_init().
 */
typedef void        (*I2CSlave_InitFxn)     (I2CSlave_Handle handle);

/*!
 *  @private
 *  @brief      A function pointer to a driver specific implementation of
 *              I2CSlave_open().
 */
typedef I2CSlave_Handle  (*I2CSlave_OpenFxn)     (I2CSlave_Handle handle,
                                        I2CSlave_Params *params);

/*!
 *  @private
 *  @brief      A function pointer to a driver specific implementation of
 *              I2CSlave_WriteTransaction().
 */
typedef bool        (*I2CSlave_WriteFxn) (I2CSlave_Handle handle,
        const void *buffer, size_t size);


/*!
 *  @private
 *  @brief      A function pointer to a driver specific implementation of
 *              I2CSlave_ReadFxn().
 */
typedef bool        (*I2CSlave_ReadFxn) (I2CSlave_Handle handle, void *buffer,
        size_t size);


/*!
 *  @brief      The definition of a I2CSlave function table that contains the
 *              required set of functions to control a specific I2CSlave
 *              driver implementation.
 */
typedef struct {
    /*! Function to close the specified peripheral */
    I2CSlave_CloseFxn        closeFxn;

    /*! Function to implementation specific control function */
    I2CSlave_ControlFxn      controlFxn;

    /*! Function to initialize the given data object */
    I2CSlave_InitFxn         initFxn;

    /*! Function to open the specified peripheral */
    I2CSlave_OpenFxn         openFxn;

    /*! Function to initiate a I2CSlave data read */
    I2CSlave_ReadFxn     readFxn;

    /*! Function to initiate a I2CSlave data write */
    I2CSlave_WriteFxn  writeFxn;
} I2CSlave_FxnTable;

/*!
 *  @brief I2C Slave driver's custom @ref driver_configuration "configuration"
 *  structure.
 *
 *  @sa  I2CSlave_init()
 *  @sa  I2CSlave_open()
 */
typedef struct I2CSlave_Config_ {
    /*! Pointer to a @ref driver_function_table "function pointer table"
     *  with driver-specific implementations of I2CSlave APIs */
    I2CSlave_FxnTable const *fxnTablePtr;

    /*! Pointer to a driver specific @ref driver_objects "data object". */
    void               *object;

    /*! Pointer to a driver specific @ref driver_hardware_attributes
     *  "hardware attributes structure". */
    void         const *hwAttrs;
} I2CSlave_Config;


/*!
 *  @brief  Function to close an I2CSlave driver instance
 *
 *  @pre  I2CSlave_open() has to be called first.
 *
 *  @param[in]  handle     An #I2CSlave_Handle returned from I2CSlave_open()
 */
extern void I2CSlave_close(I2CSlave_Handle handle);

/*!
 *  @brief  Function performs implementation specific features on a
 *          driver instance.
 *
 *  @pre    I2CSlave_open() has to be called first.
 *
 *  @param[in]  handle   An #I2CSlave_Handle returned from I2CSlave_open()
 *
 *  @param[in]  cmd     A command value defined by the device specific
 *                      implementation
 *
 *  @param[in]  arg     An optional R/W (read/write) argument that is
 *                      accompanied with @p cmd
 *
 *  @return Implementation specific return codes. Negative values indicate
 *          unsuccessful operations.
 *
 *  @retval #I2CSLAVE_STATUS_UNDEFINEDCMD  The @p cmd value is not supported
 *  by the device specific implementation.
 */
extern int_fast16_t I2CSlave_control(I2CSlave_Handle handle, uint_fast16_t cmd,
    void *arg);

/*!
 *  @brief  Function to initialize the I2C Slave driver.
 *
 *  This function must also be called before any other I2CSlave driver APIs.
 */
extern void I2CSlave_init(void);

/*!
 *  @brief  Function to initialize the I2CSlave peripheral
 *
 *  Function to initialize the I2CSlave peripheral specified by the
 *  particular index value.
 *
 *  @pre    I2CSlave_init() has been called
 *
 *
 *  @param[in]  index     Index in the @p I2CSlave_Config[] array.
 *  @param[in]  params    Pointer to an initialized #I2CSlave_Params structure.
 *                        If NULL, the default #I2CSlave_Params values are used.
 *
 *  @return An #I2CSlave_Handle on success or NULL on error.
 *
 *  @sa     I2CSlave_init()
 *  @sa     I2CSlave_close()
 */
extern I2CSlave_Handle I2CSlave_open(uint_least8_t index,
    I2CSlave_Params *params);

/*!
 *  @brief  Initialize an #I2CSlave_Params structure to its default values.
 *
 *  @param[in]  params  A pointer to an #I2CSlave_Params structure.
 *
 *  Defaults values are:
 *  @arg #I2CSlave_Params.transferMode = #I2CSLAVE_MODE_BLOCKING
 *  @arg #I2CSlave_Params.transferCallbackFxn = NULL
 *  @arg #I2CSlave_Params.custom = NULL
 *
 *  @sa  I2CSlave_open()
 */
extern void I2CSlave_Params_init(I2CSlave_Params *params);

/*!
 *  @brief  Perform an I2C read from an I2C master
 *
 *  This function will perform an I2C read transfer.
 *
 *  @note When using #I2CSLAVE_MODE_BLOCKING, this must be called from a thread
 *  context.
 *
 *  The data written by the I2CSlave is synchronized with the START and STOP
 *  condition from an I2C master.
 *
 *  @param[in]  handle    An #I2CSlave_Handle returned from I2CSlave_open()
 *
 *  @param[in,out]  buffer    A pointer to buffer to store data read
 *
 *  @param[in]  size    The number of bytes to read into @p buffer
 *
 *  @retval  true on successful read
 *  @retval  false on an error
 *
 *  @sa  I2CSlave_read
 *  @sa  I2CSlave_TransferMode
 */
extern bool I2CSlave_read(I2CSlave_Handle handle, void *buffer,
        size_t size);

/*!
 *  @brief  Perform an I2C write to an I2C master
 *
 *  This function will perform an I2C write transfer.
 *
 *  @note When using #I2CSLAVE_MODE_BLOCKING, this must be called from a thread
 *  context.
 *
 *  The data written by the I2CSlave is synchronized with the START and STOP
 *  conditions from an I2C master.
 *
 *  @param[in]  handle    An #I2CSlave_Handle
 *
 *  @param[in]  buffer    A pointer to buffer containing data to write
 *
 *  @param[in]  size    The number of bytes in @p buffer to write. If the I2C
 *  master requests more than @p size bytes, this driver will transmit @p 0xFF
 *  until a STOP condition is recieved.
 *
 *  @retval  true on successful write
 *  @retval  false on an error
 *
 *  @sa  I2CSlave_write
 *  @sa  I2CSlave_TransferMode
 */
extern bool I2CSlave_write(I2CSlave_Handle handle, const void *buffer,
        size_t size);

#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_I2CSLAVE__include */
