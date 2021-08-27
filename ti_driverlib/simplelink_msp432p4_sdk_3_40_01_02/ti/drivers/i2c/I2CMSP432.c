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

#include <stdint.h>
#include <stdbool.h>

/*
 * By default disable both asserts and log for this module.
 * This must be done before DebugP.h is included.
 */
#ifndef DebugP_ASSERT_ENABLED
#define DebugP_ASSERT_ENABLED 0
#endif
#ifndef DebugP_LOG_ENABLED
#define DebugP_LOG_ENABLED 0
#endif

#include <ti/devices/DeviceFamily.h>

#include <ti/drivers/I2C.h>
#include <ti/drivers/i2c/I2CMSP432.h>
#include <ti/drivers/dpl/DebugP.h>
#include <ti/drivers/dpl/HwiP.h>
#include <ti/drivers/dpl/SemaphoreP.h>
#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerMSP432.h>

/* driverlib header files */
#include <ti/devices/msp432p4xx/driverlib/rom.h>
#include <ti/devices/msp432p4xx/driverlib/rom_map.h>
#include <ti/devices/msp432p4xx/driverlib/i2c.h>
#include <ti/devices/msp432p4xx/driverlib/gpio.h>
#include <ti/devices/msp432p4xx/driverlib/pmap.h>

#define PinConfigValue(config) (((config) >> 10) & 0x1F)
#define PinConfigModuleFunction(config) (((config) >> 8) & 0x3)
#define PinConfigPort(config) (((config) >> 4) & 0xF)
#define PinConfigPin(config) (1 << ((config) & 0x7))

void I2CMSP432_cancel(I2C_Handle handle);
void I2CMSP432_close(I2C_Handle handle);
int_fast16_t I2CMSP432_control(I2C_Handle handle, uint_fast16_t cmd, void *arg);
void I2CMSP432_init(I2C_Handle handle);
I2C_Handle I2CMSP432_open(I2C_Handle handle, I2C_Params *params);
int_fast16_t I2CMSP432_transfer(I2C_Handle handle,
                                I2C_Transaction *transaction, uint32_t timeout);
static void blockingTransferCallback(I2C_Handle handle, I2C_Transaction *msg,
    bool transferStatus);
static void completeTransfer(I2C_Handle handle);
static void initHw(I2CMSP432_Object *object,
        I2CMSP432_HWAttrsV1 const *hwAttrs, uint32_t inputClkFreq);
static int perfChangeNotifyFxn(unsigned int eventType, uintptr_t eventArg,
    uintptr_t clientArg);
static void primeTransfer(I2CMSP432_Object *object,
    I2CMSP432_HWAttrsV1 const *hwAttrs, I2C_Transaction *transaction);

static void mapPin(uint8_t port, uint8_t pin, uint8_t value);

/* I2C function table for I2CMSP432 implementation */
const I2C_FxnTable I2CMSP432_fxnTable = {
    I2CMSP432_cancel,
    I2CMSP432_close,
    I2CMSP432_control,
    I2CMSP432_init,
    I2CMSP432_open,
    I2CMSP432_transfer
};

static const uint32_t bitRates[] = {
    EUSCI_B_I2C_SET_DATA_RATE_100KBPS,  /*  I2C_100kHz = 0  */
    EUSCI_B_I2C_SET_DATA_RATE_400KBPS,  /*  I2C_400kHz = 1  */
    EUSCI_B_I2C_SET_DATA_RATE_1MBPS     /*  I2C_1000kHz = 2 */
};

#define ERROR_INTERRUPTS (EUSCI_B_I2C_NAK_INTERRUPT | \
    EUSCI_B_I2C_ARBITRATIONLOST_INTERRUPT)

#define ALL_INTERRUPTS (EUSCI_B_I2C_RECEIVE_INTERRUPT0  | \
    EUSCI_B_I2C_TRANSMIT_INTERRUPT0 | \
    EUSCI_B_I2C_STOP_INTERRUPT | \
    ERROR_INTERRUPTS)

/*
 *  ======== blockingTransferCallback ========
 */
static void blockingTransferCallback(I2C_Handle handle, I2C_Transaction *msg,
    bool transferStatus)
{
    I2CMSP432_Object *object = handle->object;

    DebugP_log1("I2C:(%p) posting transferComplete semaphore",
        ((I2CMSP432_HWAttrsV1 const *)(handle->hwAttrs))->baseAddr);

    /* Indicate transfer complete */
    SemaphoreP_post(object->transferComplete);
}

/*
 *  ======== completeTransfer =======
 */
static void completeTransfer(I2C_Handle handle)
{
    I2CMSP432_Object          *object = ((I2C_Handle) handle)->object;
    I2CMSP432_HWAttrsV1 const *hwAttrs = ((I2C_Handle) handle)->hwAttrs;

    DebugP_log1("I2C:(%p) ISR Transfer Complete", hwAttrs->baseAddr);

    /* If this pointer is NULL, the transfer was canceled */
    if (object->currentTransaction == NULL) {
        return;
    }

    /* See if we need to process any other transactions */
    if (object->headPtr == object->tailPtr) {

        /* No other transactions need to occur */
        object->headPtr = NULL;
        object->tailPtr = NULL;

        /*
         * Allow callback to run. If in CALLBACK mode, the application
         * may initiate a transfer in the callback which will call
         * primeTransfer().
         */
        object->transferCallbackFxn(handle, object->currentTransaction,
            (object->mode == I2CMSP432_IDLE_MODE));

        /* Remove constraints set during transfer */
#if DeviceFamily_ID == DeviceFamily_ID_MSP432P401x
        Power_releaseConstraint(PowerMSP432_DISALLOW_DEEPSLEEP_0);
#endif
        Power_releaseConstraint(PowerMSP432_DISALLOW_PERF_CHANGES);

        DebugP_log1("I2C:(%p) ISR No other I2C transaction in queue",
            hwAttrs->baseAddr);
    }
    else {

        /* This should never happen if you are in blocking mode */
        DebugP_assert(object->transferMode == I2C_MODE_CALLBACK);

        /* Another transfer needs to take place */
        object->headPtr = object->headPtr->nextPtr;

        /*
         * Allow application callback to run. The application may
         * initiate a transfer in the callback which will add an
         * additional transfer to the queue.
         */
        object->transferCallbackFxn(handle, object->currentTransaction,
            (object->mode == I2CMSP432_IDLE_MODE));

        DebugP_log2("I2C:(%p) ISR Priming next I2C transaction (%p) from queue",
            hwAttrs->baseAddr, (uintptr_t) object->headPtr);

        primeTransfer(object, hwAttrs, object->headPtr);
    }

}

/*
 *  ======== initHW ========
 *
 *  Configures I2C peripheral
 */
static void initHw(I2CMSP432_Object *object,
        I2CMSP432_HWAttrsV1 const *hwAttrs, uint32_t inputClkFreq)
{
    eUSCI_I2C_MasterConfig   i2cConfig;

    /* Disable interrupts while being reconfigured */
    MAP_I2C_disableInterrupt(hwAttrs->baseAddr, ALL_INTERRUPTS);

    /* Configure the I2C module */
    i2cConfig.selectClockSource = hwAttrs->clockSource;
    i2cConfig.i2cClk = inputClkFreq;
    i2cConfig.dataRate = object->bitRate;
    i2cConfig.byteCounterThreshold = 0;
    i2cConfig.autoSTOPGeneration = EUSCI_B_I2C_NO_AUTO_STOP;
    MAP_I2C_initMaster(hwAttrs->baseAddr, &i2cConfig);

    /* Clear any pending interrupts & enable I2C module */
    MAP_I2C_clearInterruptFlag(hwAttrs->baseAddr, ALL_INTERRUPTS);
    MAP_I2C_enableInterrupt(hwAttrs->baseAddr, ALL_INTERRUPTS);
    MAP_I2C_enableModule(hwAttrs->baseAddr);

    DebugP_log3("I2C:(%p) CPU freq: %d; I2C freq to %d", hwAttrs->baseAddr,
        inputClkFreq, object->bitRate);
}

/*
 *  ======== perfChangeNotifyFxn ========
 *
 *  Called by Power module before and after performance level is changed.
 */
static int perfChangeNotifyFxn(unsigned int eventType, uintptr_t eventArg,
    uintptr_t clientArg)
{
    uint32_t                   clockFreq;
    PowerMSP432_Freqs          powerFreqs;
    I2CMSP432_Object          *object = ((I2C_Handle) clientArg)->object;
    I2CMSP432_HWAttrsV1 const *hwAttrs = ((I2C_Handle) clientArg)->hwAttrs;

    if (eventType == PowerMSP432_START_CHANGE_PERF_LEVEL) {
        /* Disable peripheral before performance level change */
        MAP_I2C_disableModule(hwAttrs->baseAddr);
    }
    else { /* eventType == PowerMSP432_DONE_CHANGE_PERF_LEVEL */
        /* Get new performance level clock frequencies */
        PowerMSP432_getFreqs(Power_getPerformanceLevel(), &powerFreqs);
        clockFreq = (hwAttrs->clockSource == EUSCI_B_I2C_CLOCKSOURCE_SMCLK) ?
            powerFreqs.SMCLK : powerFreqs.ACLK;

        /* Reconfigure I2C peripheral */
        initHw(object, hwAttrs, clockFreq);
    }

    return (Power_NOTIFYDONE);
}

/*
 *  ======== primeTransfer =======
 */
static void primeTransfer(I2CMSP432_Object *object,
    I2CMSP432_HWAttrsV1 const *hwAttrs, I2C_Transaction *transaction)
{
    PowerMSP432_Freqs          powerFreqs;
    uint32_t                   clockFreq;

    /* If the last transfer timed-out, reset the peripheral */
    if(object->mode == I2CMSP432_TIMEOUT) {
        /* disable interrupts, send STOP to try to complete all transfers */
        MAP_I2C_disableInterrupt(hwAttrs->baseAddr, ALL_INTERRUPTS);
        MAP_I2C_masterReceiveMultiByteStop(hwAttrs->baseAddr);

        /* remove Power constraints set for I2C_transfer() */
    #if DeviceFamily_ID == DeviceFamily_ID_MSP432P401x
        Power_releaseConstraint(PowerMSP432_DISALLOW_DEEPSLEEP_0);
    #endif
        Power_releaseConstraint(PowerMSP432_DISALLOW_PERF_CHANGES);

        /* re-initialize the I2C peripheral */
        PowerMSP432_getFreqs(Power_getPerformanceLevel(), &powerFreqs);
        clockFreq = (hwAttrs->clockSource == EUSCI_B_I2C_CLOCKSOURCE_SMCLK) ?
            powerFreqs.SMCLK : powerFreqs.ACLK;
        MAP_I2C_disableModule(hwAttrs->baseAddr);
        initHw(object, hwAttrs, clockFreq);
    }

    /* Store the new internal counters and pointers */
    object->currentTransaction = transaction;

    object->writeBufIdx = transaction->writeBuf;
    object->writeCountIdx = transaction->writeCount;

    object->readBufIdx = transaction->readBuf;
    object->readCountIdx = transaction->readCount;

    DebugP_log2("I2C:(%p) Starting transaction to slave: 0x%x",
        hwAttrs->baseAddr, object->currentTransaction->slaveAddress);

    MAP_I2C_setSlaveAddress(hwAttrs->baseAddr,
        object->currentTransaction->slaveAddress);

    /* Clear the STOP condition and enable the STOP interrupt */
    BITBAND_PERI(EUSCI_B_CMSIS(hwAttrs->baseAddr)->CTLW0, EUSCI_B_CTLW0_TXSTP_OFS) = 0;
    MAP_I2C_clearInterruptFlag(hwAttrs->baseAddr, EUSCI_B_I2C_STOP_INTERRUPT);
    MAP_I2C_enableInterrupt(hwAttrs->baseAddr, EUSCI_B_I2C_STOP_INTERRUPT);

    /* Start transfer in Transmit mode */
    if (object->writeCountIdx) {
        object->mode = I2CMSP432_WRITE_MODE;
        MAP_I2C_setMode(hwAttrs->baseAddr, EUSCI_B_I2C_TRANSMIT_MODE);

        /* Start the I2C transfer by sending the start bit */
        MAP_I2C_masterSendStart(hwAttrs->baseAddr);

        DebugP_log1("I2C:(%p) I2CMSP432_IDLE_MODE: -> I2CMSP432_WRITE_MODE; "
            "Writing w/ START", hwAttrs->baseAddr);
    }
    /* Start transfer in Receive mode */
    else {
        object->mode = I2CMSP432_READ_MODE;
        MAP_I2C_setMode(hwAttrs->baseAddr, EUSCI_B_I2C_RECEIVE_MODE);
        MAP_I2C_masterReceiveStart(hwAttrs->baseAddr);

        if (object->readCountIdx > 1) {
            DebugP_log1("I2C:(%p) I2CMSP432_IDLE_MODE: -> I2CMSP432_READ_MODE; "
                "Reading w/ ACK", hwAttrs->baseAddr);
        }
        else {
            /*
             * Need to wait for the start bit to have transmitted before we can
             * tell it to send a stop bit.
             */
            while (MAP_I2C_masterIsStartSent(hwAttrs->baseAddr)) {}

            MAP_I2C_masterReceiveMultiByteStop(hwAttrs->baseAddr);

            DebugP_log1("I2C:(%p) I2CMSP432_IDLE_MODE: -> I2CMSP432_READ_MODE; "
                "Reading w/ NACK", hwAttrs->baseAddr);
        }
    }
}

/*
 *  ======== I2CMSP432_cancel ========
 */
void I2CMSP432_cancel(I2C_Handle handle)
{
    I2CMSP432_HWAttrsV1 const *hwAttrs = handle->hwAttrs;
    I2CMSP432_Object          *object = handle->object;
    PowerMSP432_Freqs          powerFreqs;
    uint32_t                   clockFreq;
    uintptr_t                  key;

    /* just return if no transfer is in progress */
    if (!object->headPtr) {
        return;
    }

    /* disable interrupts, send STOP to try to complete all transfers */
    key = HwiP_disable();
    MAP_I2C_disableInterrupt(hwAttrs->baseAddr, ALL_INTERRUPTS);
    MAP_I2C_masterReceiveMultiByteStop(hwAttrs->baseAddr);

    /* call the transfer callback for the current transfer, indicate failure */
    object->transferCallbackFxn(handle, object->currentTransaction, false);

    /* also dequeue and call the transfer callbacks for any queued transfers */
    while (object->headPtr != object->tailPtr) {
        object->headPtr = object->headPtr->nextPtr;
        object->transferCallbackFxn(handle, object->headPtr, false);
    }

    /* clean up object */
    object->currentTransaction = NULL;
    object->headPtr = NULL;
    object->tailPtr = NULL;
    object->mode = I2CMSP432_IDLE_MODE;

    /* remove Power constraints set for I2C_transfer() */
#if DeviceFamily_ID == DeviceFamily_ID_MSP432P401x
    Power_releaseConstraint(PowerMSP432_DISALLOW_DEEPSLEEP_0);
#endif
    Power_releaseConstraint(PowerMSP432_DISALLOW_PERF_CHANGES);

    /* re-initialize the I2C peripheral */
    PowerMSP432_getFreqs(Power_getPerformanceLevel(), &powerFreqs);
    clockFreq = (hwAttrs->clockSource == EUSCI_B_I2C_CLOCKSOURCE_SMCLK) ?
        powerFreqs.SMCLK : powerFreqs.ACLK;
    MAP_I2C_disableModule(hwAttrs->baseAddr);
    initHw(object, hwAttrs, clockFreq);

    HwiP_restore(key);
}

/*
 *  ======== I2CMSP432_close ========
 */
void I2CMSP432_close(I2C_Handle handle)
{
    unsigned int               i;
    I2CMSP432_Object          *object = handle->object;
    I2CMSP432_HWAttrsV1 const *hwAttrs = handle->hwAttrs;

    /* Check to see if a I2C transaction is in progress */
    DebugP_assert(object->headPtr == NULL);

    /* Disable interrupts & the I2C */
    MAP_I2C_disableInterrupt(hwAttrs->baseAddr, ALL_INTERRUPTS);
    MAP_I2C_disableModule(hwAttrs->baseAddr);

    /* Destruct driver resources */
    if (object->hwiHandle) {
        HwiP_delete(object->hwiHandle);
        object->hwiHandle = NULL;
    }
    if (object->mutex) {
        SemaphoreP_delete(object->mutex);
        object->mutex = NULL;
    }
    if (object->transferComplete) {
        SemaphoreP_delete(object->transferComplete);
        object->transferComplete = NULL;
    }

    /* Remove power constraints */
    for (i = 0; object->perfConstraintMask; i++) {
        if (object->perfConstraintMask & 0x01) {
            Power_releaseConstraint(PowerMSP432_DISALLOW_PERFLEVEL_0 + i);
        }
        object->perfConstraintMask >>= 1;
    }
    Power_unregisterNotify(&object->perfChangeNotify);

    object->isOpen = false;

    DebugP_log1("I2C: Object closed 0x%x", hwAttrs->baseAddr);
}

/*
 *  ======== I2CMSP432_control ========
 *  @pre    Function assumes that the handle is not NULL
 */
int_fast16_t I2CMSP432_control(I2C_Handle handle, uint_fast16_t cmd, void *arg)
{
    /* No implementation yet */
    return (I2C_STATUS_UNDEFINEDCMD);
}

/*
 *  ======== I2CMSP432_hwiIntFxn ========
 *  Hwi interrupt handler to service the I2C peripheral
 *
 *  The handler is a generic handler for a I2C object.
 */
void I2CMSP432_hwiIntFxn(uintptr_t arg)
{
    uint_fast16_t              intStatus;
    unsigned int               key;
    I2CMSP432_Object          *object = ((I2C_Handle) arg)->object;
    I2CMSP432_HWAttrsV1 const *hwAttrs = ((I2C_Handle) arg)->hwAttrs;
    bool status = true;

    /* Get the interrupt status of the I2C controller */
    intStatus = MAP_I2C_getEnabledInterruptStatus(hwAttrs->baseAddr);
    MAP_I2C_clearInterruptFlag(hwAttrs->baseAddr, intStatus);

    /* Filter any spurious interrupts */
    if (!(intStatus & ALL_INTERRUPTS)) {
        return;
    }

    /* Check for I2C Errors or timeout */
    if ((intStatus & ERROR_INTERRUPTS) || object->mode == I2CMSP432_TIMEOUT) {

        /* Some sort of error occurred */
        object->mode = I2CMSP432_ERROR;

        /* If we haven't seen the stop interrupt, send a stop bit */
        if(!(intStatus & EUSCI_B_I2C_STOP_INTERRUPT)) {
            /* Try to send a STOP bit to end all I2C communications immediately. */
            MAP_I2C_masterReceiveMultiByteStop(hwAttrs->baseAddr);
        }

        DebugP_log2("I2C:(%p) ISR I2C Bus fault (Status Reg: 0x%x)",
            hwAttrs->baseAddr, intStatus);

        return;
    }

    /* No errors, now check what we need to do next */
    switch (object->mode) {
        /*
         * ERROR case is OK because if an Error is detected, a STOP bit is
         * sent; which in turn will call another interrupt. This interrupt
         * call will then post the transferComplete semaphore to unblock the
         * I2C_transfer function
         */
        case I2CMSP432_ERROR:
        case I2CMSP432_TIMEOUT:
        case I2CMSP432_IDLE_MODE:
            completeTransfer((I2C_Handle) arg);
            break;

        case I2CMSP432_WRITE_MODE:
            if (object->writeCountIdx) {
                /* If on the last byte to write and have NO data to read */
                if ((object->writeCountIdx == 1) && (!object->readCountIdx)) {
                    /* Next state: Idle mode */
                    object->mode = I2CMSP432_IDLE_MODE;

                    /* Send last byte with STOP bit */
                    status = MAP_I2C_masterSendMultiByteFinish(hwAttrs->baseAddr,
                        *(object->writeBufIdx));

                    if (status == false) {
                        object->mode = I2CMSP432_ERROR;
                    }

                    MAP_I2C_clearInterruptFlag(hwAttrs->baseAddr,
                                               EUSCI_B_I2C_TRANSMIT_INTERRUPT0);

                    DebugP_log2("I2C:(%p) ISR I2CMSP432_WRITE_MODE:"
                        "Data to write: 0x%x; Writing w/ STOP bit",
                        hwAttrs->baseAddr, *(object->writeBufIdx));
                }
                else {
                    /* Write data contents into data register */
                    MAP_I2C_masterSendMultiByteNext(hwAttrs->baseAddr,
                        *(object->writeBufIdx));

                    DebugP_log3("I2C:(%p) ISR I2CMSP432_WRITE_MODE: "
                        "Data to write: 0x%x; To slave: 0x%x",
                        hwAttrs->baseAddr, *(object->writeBufIdx),
                        object->currentTransaction->slaveAddress);

                    object->writeBufIdx++;
                    object->writeCountIdx--;
                }
            }
            /*
             * Done writing data to the I2C slave. If no data need be read,
             * the ISR would not get here as it would have finished the I2C
             * transfer in the logical check above.
             */
            else {
                /* Next state: Receive mode */
                object->mode = I2CMSP432_READ_MODE;
                MAP_I2C_setMode(hwAttrs->baseAddr, EUSCI_B_I2C_RECEIVE_MODE);

                if (object->readCountIdx > 1) {
                    /* Start the I2C transfer in master receive mode */
                    MAP_I2C_masterReceiveStart(hwAttrs->baseAddr);
                    DebugP_log1("I2C:(%p) I2CMSP432_WRITE_MODE: -> I2CMSP432_READ_MODE; "
                        "Reading w/ RESTART", hwAttrs->baseAddr);
                }
                else {
                    /*
                     * Start the I2C transfer in master receive mode by sending
                     * a START and STOP bit since its the last byte
                     */
                    MAP_I2C_masterReceiveStart(hwAttrs->baseAddr);

                    while (MAP_I2C_masterIsStartSent(hwAttrs->baseAddr)) {}
                    MAP_I2C_masterReceiveMultiByteStop(hwAttrs->baseAddr);

                    DebugP_log1("I2C:(%p) I2CMSP432_WRITE_MODE: -> I2CMSP432_READ_MODE; "
                        "Reading w/ RESTART and STOP", hwAttrs->baseAddr);
                }
            }
            break; /* I2CMSP432_WRITE_MODE */

        case I2CMSP432_READ_MODE:
            /* Data read from RXBUF and next byte has already been shifted */
            object->readCountIdx--;

            if (object->readCountIdx == 1) {
                /* Timing critical block which mustn't be simplified - START */
                key = HwiP_disable();

                MAP_I2C_masterReceiveMultiByteStop(hwAttrs->baseAddr);
                *(object->readBufIdx) =
                    MAP_I2C_masterReceiveMultiByteNext(hwAttrs->baseAddr);

                HwiP_restore(key);
                /* Timing critical block which mustn't be simplified - END */

                DebugP_log2("I2C:(%p) ISR I2CMSP432_READ_MODE: Read data byte: 0x%x",
                    hwAttrs->baseAddr, *(object->readBufIdx));

                object->readBufIdx++;
            }
            else {
                *(object->readBufIdx) =
                    MAP_I2C_masterReceiveMultiByteNext(hwAttrs->baseAddr);

                DebugP_log2("I2C:(%p) ISR I2CMSP432_READ_MODE: "
                    "Read data byte: 0x%x ", hwAttrs->baseAddr,
                    *(object->readBufIdx));

                if (object->readCountIdx) {
                    /* Increment buffer pointer */
                    object->readBufIdx++;
                }
                else {
                    /* Next state: Idle mode */
                    object->mode = I2CMSP432_IDLE_MODE;

                    /* If we have sent the stop bit, complete the transfer */
                    if(MAP_I2C_masterIsStopSent(hwAttrs->baseAddr) == EUSCI_B_I2C_STOP_SEND_COMPLETE) {

                        /*
                         * At this point, we have sent the STOP condition on the I2C Bus.
                         * Disable the STOP interrupt from occurring
                         */
                        MAP_I2C_disableInterrupt(hwAttrs->baseAddr, EUSCI_B_I2C_STOP_INTERRUPT);

                        /* Complete the transfer */
                        completeTransfer((I2C_Handle) arg);
                    }
                    else {
                        MAP_I2C_masterReceiveMultiByteStop(hwAttrs->baseAddr);
                    }
                }
            }
            break; /* I2CMSP432_READ_MODE */

        default:
            object->mode = I2CMSP432_ERROR;
            break;
    }
}

/*
 *  ======== I2CMSP432_init ========
 */
void I2CMSP432_init(I2C_Handle handle)
{
}

/*
 *  ======== I2CMSP432_open ========
 */
I2C_Handle I2CMSP432_open(I2C_Handle handle, I2C_Params *params)
{
    unsigned int               i;
    uintptr_t                  key;
    uint32_t                   clockFreq;
    uint8_t                    numPerfLevels;
    union {
        SemaphoreP_Params      semParams;
        HwiP_Params            hwiParams;
    } portsParams;
    PowerMSP432_Freqs          powerFreqs;
    I2CMSP432_Object          *object = handle->object;
    I2CMSP432_HWAttrsV1 const *hwAttrs = handle->hwAttrs;
    uint16_t                   pin;
    uint16_t                   port;
    uint16_t                   value;
    uint16_t                   moduleFunction;

    /*
     * Check that a callback is provided if using callback mode.
     * Check for supported bitrate.
     */
    if ((params->transferMode == I2C_MODE_CALLBACK &&
        params->transferCallbackFxn == NULL) ||
        (params->bitRate > I2C_1000kHz)) {

        return (NULL);
    }

    /* Determine if the driver was already opened */
    key = HwiP_disable();

    if (object->isOpen) {
        HwiP_restore(key);
        DebugP_log1("I2C:(%p) Error! Already in use.", hwAttrs->baseAddr);
        return (NULL);
    }
    object->isOpen = true;

    HwiP_restore(key);

    /* Ensure a supported clock source is used */
    if (hwAttrs->clockSource != EUSCI_B_I2C_CLOCKSOURCE_ACLK &&
        hwAttrs->clockSource != EUSCI_B_I2C_CLOCKSOURCE_SMCLK) {
        DebugP_log1("I2C:(%p) Error! Using unsupported clock source.",
            hwAttrs->baseAddr);
        object->isOpen = false;
        return (NULL);
    }

    /*
     * Add power management support - Disable performance transitions while
     * opening the driver.
     */
    Power_setConstraint(PowerMSP432_DISALLOW_PERF_CHANGES);

    /* Map the pins */
    /* Clock */
    port = PinConfigPort(hwAttrs->clkPin);
    value = PinConfigValue(hwAttrs->clkPin);

    if (value != 0) {
        moduleFunction = GPIO_PRIMARY_MODULE_FUNCTION;
        pin = (hwAttrs->clkPin) & 0x7;
        mapPin(port, pin, value);
    }
    else {
        moduleFunction = PinConfigModuleFunction(hwAttrs->clkPin);
    }

    pin  = PinConfigPin(hwAttrs->clkPin);
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(port,
            pin, moduleFunction);

    /* Data */
    port = PinConfigPort(hwAttrs->dataPin);
    value = PinConfigValue(hwAttrs->dataPin);

    if (value != 0) {
        moduleFunction = GPIO_PRIMARY_MODULE_FUNCTION;
        pin = (hwAttrs->dataPin) & 0x7;
        mapPin(port, pin, value);
    }
    else {
        moduleFunction = PinConfigModuleFunction(hwAttrs->dataPin);
    }

    pin  = PinConfigPin(hwAttrs->dataPin);
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(port,
            pin, moduleFunction);

    /*
     * Verify that the driver can be opened at current performance level and
     * set constraints for other performance levels.
     */
    object->bitRate = bitRates[params->bitRate];
    numPerfLevels = PowerMSP432_getNumPerfLevels();
    PowerMSP432_getFreqs(Power_getPerformanceLevel(), &powerFreqs);
    if (hwAttrs->clockSource == EUSCI_B_I2C_CLOCKSOURCE_ACLK) {
        /*
         * Verify if driver can be opened with ACLK; ACLK does not change
         * in any performance level.
         */
        if (object->bitRate >= powerFreqs.ACLK) {
            DebugP_log3("I2C:(%p) ACLK input (%d) too low for bit rate (%d)",
                hwAttrs->baseAddr, powerFreqs.ACLK, object->bitRate);
            Power_releaseConstraint(PowerMSP432_DISALLOW_PERF_CHANGES);
            object->isOpen = false;
            return (NULL);
        }
        clockFreq = powerFreqs.ACLK;
    }
    else {    /* hwAttrs->clockSource == EUSCI_B_I2C_CLOCKSOURCE_SMCLK */
        if (object->bitRate >= powerFreqs.SMCLK) {
            DebugP_log3("I2C:(%p) SMCLK input (%d) too low for bit rate (%d)",
                hwAttrs->baseAddr, powerFreqs.SMCLK, object->bitRate);
            Power_releaseConstraint(PowerMSP432_DISALLOW_PERF_CHANGES);
            object->isOpen = false;
            return (NULL);
        }
        clockFreq = powerFreqs.SMCLK;

        /*
         * SMCLK changes with performance levels.  Set constraints for
         * unsupported performance levels.
         */
        for (i = 0; i < numPerfLevels; i++) {
            PowerMSP432_getFreqs(i, &powerFreqs);
            if (object->bitRate >= powerFreqs.SMCLK) {
                /* Set constraint and keep track of it in perfConstraintMask */
                object->perfConstraintMask |= (1 << i);
                Power_setConstraint(PowerMSP432_DISALLOW_PERFLEVEL_0 + i);
            }
        }
    }

    /* Register function to reconfigure peripheral on perf level changes */
    Power_registerNotify(&object->perfChangeNotify,
        PowerMSP432_START_CHANGE_PERF_LEVEL |PowerMSP432_DONE_CHANGE_PERF_LEVEL,
        perfChangeNotifyFxn, (uintptr_t) handle);

    /* Create Hwi object for this I2C peripheral */
    HwiP_Params_init(&(portsParams.hwiParams));
    portsParams.hwiParams.arg = (uintptr_t) handle;
    portsParams.hwiParams.priority = hwAttrs->intPriority;
    object->hwiHandle = HwiP_create(hwAttrs->intNum, I2CMSP432_hwiIntFxn,
        &(portsParams.hwiParams));
    if (object->hwiHandle == NULL) {
        DebugP_log1("I2C:(%p) HwiP_create() failed.", hwAttrs->baseAddr);
        Power_releaseConstraint(PowerMSP432_DISALLOW_PERF_CHANGES);
        I2CMSP432_close(handle);
        return (NULL);
    }

    /*
     * Create threadsafe handles for this I2C peripheral. Semaphore ensures
     * exclusive access to the I2C peripheral.
     */
    SemaphoreP_Params_init(&(portsParams.semParams));
    (portsParams.semParams).mode = SemaphoreP_Mode_BINARY;
    object->mutex = SemaphoreP_create(1, &(portsParams.semParams));
    if (object->mutex == NULL) {
        DebugP_log1("I2C:(%p) mutex Semaphore_create() failed: %d.",
            hwAttrs->baseAddr);
        Power_releaseConstraint(PowerMSP432_DISALLOW_PERF_CHANGES);
        I2CMSP432_close(handle);
        return (NULL);
    }

    /* Configure driver to Callback or Blocking operating mode */
    if (params->transferMode == I2C_MODE_CALLBACK) {
        object->transferCallbackFxn = params->transferCallbackFxn;
        DebugP_log1("I2C:(%p) in I2C_MODE_CALLBACK mode", hwAttrs->baseAddr);
    }
    else {
        /* Semaphore to block task for the duration of the I2C transfer */
        object->transferComplete = SemaphoreP_create(0, &(portsParams.semParams));
        if (object->transferComplete == NULL) {
            DebugP_log1("I2C:(%p) transfer SemaphoreP_create() failed: %d.",
                hwAttrs->baseAddr);
            Power_releaseConstraint(PowerMSP432_DISALLOW_PERF_CHANGES);
            I2CMSP432_close(handle);
            return (NULL);
        }
        object->transferCallbackFxn = blockingTransferCallback;
        DebugP_log1("I2C:(%p) in I2C_MODE_BLOCKING mode", hwAttrs->baseAddr);
    }

    /*
     * Store I2C parameters & initialize peripheral.  These are used to
     * re/initialize the peripheral when opened or changing performance level.
     */
    object->mode = I2CMSP432_IDLE_MODE;
    object->transferMode = params->transferMode;
    object->headPtr = NULL;
    object->tailPtr = NULL;
    initHw(object, hwAttrs, clockFreq);

    /* Allow performance level changes  */
    Power_releaseConstraint(PowerMSP432_DISALLOW_PERF_CHANGES);

    DebugP_log1("I2C: Object opened 0x%x", hwAttrs->baseAddr);

    return (handle);
}

/*
 *  ======== I2CMSP432_transfer ========
 */
int_fast16_t I2CMSP432_transfer(I2C_Handle handle, I2C_Transaction *transaction, uint32_t timeout)
{
    uintptr_t                  key;
    int_fast16_t               ret = I2C_STATUS_ERROR;
    I2CMSP432_Object          *object = handle->object;
    I2CMSP432_HWAttrsV1 const *hwAttrs = handle->hwAttrs;

    /* Check if anything needs to be written or read */
    if ((!transaction->writeCount) && (!transaction->readCount)) {
        return (ret);
    }

    key = HwiP_disable();

    if (object->transferMode == I2C_MODE_CALLBACK) {
        /* Check if a transfer is in progress */
        if (object->headPtr) {
            /*
             * Update the message pointed by the tailPtr to point to the next
             * message in the queue
             */
            object->tailPtr->nextPtr = transaction;

            /* Update the tailPtr to point to the last message */
            object->tailPtr = transaction;

            HwiP_restore(key);
            return (I2C_STATUS_SUCCESS);
        }
    }

    /* Store the headPtr indicating I2C is in use */
    object->headPtr = transaction;
    object->tailPtr = transaction;

    HwiP_restore(key);

    /* Get the lock for this I2C handle */
    if (SemaphoreP_pend(object->mutex, SemaphoreP_NO_WAIT) == SemaphoreP_TIMEOUT) {

        /* An I2C_transfer() may complete before the calling thread post the
         * mutex due to preemption. We must not block in this case. */
        if (object->transferMode == I2C_MODE_CALLBACK) {
            return (I2C_STATUS_ERROR);
        }

        if(SemaphoreP_pend(object->mutex, timeout) == SemaphoreP_TIMEOUT){
            return (I2C_STATUS_TIMEOUT);
        }
    }

    if (object->transferMode == I2C_MODE_BLOCKING) {
       /*
        * In the case of a timeout, It is possible for the timed-out transaction
        * to call the hwi function and post the object->transferComplete Semaphore
        * To clear this, we simply do a NO_WAIT pend on (binary)
        * object->transferComplete, so that it resets the Semaphore count.
        */
        SemaphoreP_pend(object->transferComplete, SemaphoreP_NO_WAIT);
    }

    /*
     * Set power constraints to keep peripheral active during transfer
     * and to prevent a performance level change
     */
#if DeviceFamily_ID == DeviceFamily_ID_MSP432P401x
    Power_setConstraint(PowerMSP432_DISALLOW_DEEPSLEEP_0);
#endif
    Power_setConstraint(PowerMSP432_DISALLOW_PERF_CHANGES);

    /*
     * I2CMSP432_primeTransfer is a longer process and
     * protection is needed from the I2C interrupt
     */
    key = HwiP_disable();
    MAP_I2C_disableInterrupt(hwAttrs->baseAddr, ALL_INTERRUPTS);
    primeTransfer(object, hwAttrs, transaction);
    MAP_I2C_enableInterrupt(hwAttrs->baseAddr, ALL_INTERRUPTS);
    HwiP_restore(key);

    if (object->transferMode == I2C_MODE_BLOCKING) {
        DebugP_log1("I2C:(%p) Pending on transferComplete semaphore",
            hwAttrs->baseAddr);
        /*
         * Wait for the transfer to complete here.
         * It's OK to block from here because the I2C's Hwi will unblock
         * upon errors
         */
        if(SemaphoreP_pend(object->transferComplete, timeout) == SemaphoreP_TIMEOUT) {
            /*
             * Timeout occurred, set current state to TIMEOUT and
             * return false to indicate failure. When the next transaction occurs,
             * primeTransfer will re-initialize the peripheral.
             */
            object->mode = I2CMSP432_TIMEOUT;
            ret = I2C_STATUS_TIMEOUT;
        }

        DebugP_log1("I2C:(%p) Transaction completed", hwAttrs->baseAddr);

        /* Hwi handle has posted a 'transferComplete' check for Errors */
        if (object->mode == I2CMSP432_IDLE_MODE) {
            DebugP_log1("I2C:(%p) Transfer OK", hwAttrs->baseAddr);
            ret = I2C_STATUS_SUCCESS;
        }
    }
    else {
        /* Always return true if in Asynchronous mode */
        ret = I2C_STATUS_SUCCESS;
    }

    /* Release the lock for this particular I2C handle */
    SemaphoreP_post(object->mutex);

    /* Return the number of bytes transfered by the I2C */
    return (ret);
}

/*
 *  ======== mapPin ========
 *  NOTE: This function may go away when DriverLib supports mapping
 *  an individual pin.
 */
static void mapPin(uint8_t port, uint8_t pin, uint8_t value)
{
    volatile uint8_t                     pmap;

    pmap = port * 0x8;  // 2 -> 0x10, 3 -> 0x18, 7 -> 0x38


    /*  Code from pmap.c: */
    //Get write-access to port mapping registers:
    PMAP->KEYID = PMAP_KEYID_VAL;

    PMAP->CTL = (PMAP->CTL & ~PMAP_CTL_PRECFG) | PMAP_ENABLE_RECONFIGURATION;
    HWREG8((uint32_t)PMAP_BASE + pin + pmap) = value;
    //Disable write-access to port mapping registers:

    PMAP->KEYID = 0;
}
