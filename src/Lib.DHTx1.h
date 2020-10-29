/**
 * \file Lib.DHTx1.h
 * \author vEmagic (admin@vemgic.com)
 * \brief 
 * \version 0.1.0
 * \date 2020-10-29
 * 
 * Copyright 2020 (C), Sichuan Deyang DeYi Electronic Instrument Co., Ltd.
 * 
 */

#ifndef __LIB_DHTx1_H
#define __LIB_DHTx1_H

// #include "main.h"
#include "stm32f0xx_hal.h"

typedef GPIO_TypeDef tPort;
typedef uint32_t tPin;

typedef float tTemperature;
typedef float tHumidity;

#define DHTx1_DelayUs(us)

#define DHTx1_DATA_PIN_OUTPUT_MODE(port, pin)
#define DHTx1_DATA_PIN_INPUT_MODE(port, pin)

#define DHTx1_DATA_PIN_HIGH(port, pin)
#define DHTx1_DATA_PIN_LOW(port, pin)

#define DHTx1_DATA_PIN_READ(port, pin)
/*-----------------------------------------------------------------------------------------------*/

typedef enum {    
    eDataCheckSumCorrect = 0,
    eDataCheckSumError,
    eShortOfReadTime,
    eMissStartCondition1,
    eMissStartCondition2,
} tDHTx1Status;

typedef struct {
    tPort sPort;
    tPin sPin;
} tDHTx1Gpio;

typedef struct {
    uint8_t ui8Data[5];
    tDHTx1Gpio* psGPIO;    

    tTemperature sTemperature;
    tHumidity sHumidity;

    uint32_t ui32LastReadTime;
} tDHTx1Sensor;
/*-----------------------------------------------------------------------------------------------*/

/**
 * \fn int8_t DHTx1_Init(tDHTx1Sensor* psDHTx1)
 * 
 * \brief Initial DHTx1 Device
 * 
 * \param sDHTx1 \ref tDHTx1Sensor Instance
 * \return int8_t 0 - Success, or Failed
 */
int8_t DHTx1_Init(tDHTx1Sensor* psDHTx1, tDHTx1Gpio* psGpio);
/*-----------------------------------------------------------------------------------------------*/

/**
 * \fn tDHTx1Status DHTx1_Open(tDHTx1Sensor* psDHTx1)
 * 
 * \brief 
 * 
 * \param psDHTx1 
 * \return tDHTx1Status 
 */
tDHTx1Status DHTx1_Open(tDHTx1Sensor* psDHTx1);
/*-----------------------------------------------------------------------------------------------*/

/**
 * \fn bool DHTx1_Close(tDHTx1Sensor* psDHTx1)
 * \brief 
 * 
 * \param psDHTx1 
 * \return true 
 * \return false 
 */
bool DHTx1_Close(tDHTx1Sensor* psDHTx1);
/*-----------------------------------------------------------------------------------------------*/

/**
 * \fn uint8_t DHTx1_ReadByte(tDHTx1Sensor* psDHTx1)
 * \brief 
 * 
 * \param psDHTx1 
 * \return uint8_t 
 */
uint8_t DHTx1_ReadByte(tDHTx1Sensor* psDHTx1);
/*-----------------------------------------------------------------------------------------------*/

/**
 * \fn bool DHTx1_IsShortOfReadTime(void)
 * \brief 
 * 
 * \return true 
 * \return false 
 */
bool DHTx1_IsShortOfReadTime(void);
/*-----------------------------------------------------------------------------------------------*/

/**
 * 
 * \brief bool DHTx1_IsDataChanged(tDHTx1Sensor* psDHTx1)
 * 
 * \param psDHTx1 
 * \return true 
 * \return false 
 */
bool DHTx1_IsDataChanged(tDHTx1Sensor* psDHTx1);
/*-----------------------------------------------------------------------------------------------*/

/**
 * 
 * \brief tTemperature DHTx1_GetTemperature(tDHTx1Sensor* psDHTx1)
 * 
 * \param psDHTx1 
 * \return tTemperature 
 */
tTemperature DHTx1_GetTemperature(tDHTx1Sensor* psDHTx1);
/*-----------------------------------------------------------------------------------------------*/

/**
 * \fn tHumidity DHTx1_GetHumidity(tDHTx1Sensor* psDHTx1)
 * \brief 
 * 
 * \param psDHTx1 
 * \return tHumidity 
 */
tHumidity DHTx1_GetHumidity(tDHTx1Sensor* psDHTx1);
/*-----------------------------------------------------------------------------------------------*/

/**
 * \fn 
 * \brief tTemperature DHTx1_ConvertC2F(tTemperature sCelsius)
 * 
 * \param sCelsius 
 * \return tTemperature 
 */
tTemperature DHTx1_ConvertC2F(tTemperature sCelsius);
/*-----------------------------------------------------------------------------------------------*/

/**
 * \fn 
 * \brief tTemperature DHTx1_ConvertC2K(tTemperature sCelsius)
 * 
 * \param sCelsius 
 * \return tTemperature 
 */
tTemperature DHTx1_ConvertC2K(tTemperature sCelsius);
/*-----------------------------------------------------------------------------------------------*/

/**
 * \fn tHumidity DHTx1_DewPoint(tTemperature sCelsius, tHumidity sHumidity)
 * \brief 
 * 
 * \param sCelsius 
 * \param sHumidity 
 * \return tHumidity 
 */
tHumidity DHTx1_DewPoint(tTemperature sCelsius, tHumidity sHumidity);
/*-----------------------------------------------------------------------------------------------*/

/**
 * \fn tHumidity DHTx1_DewPointFast(tTemperature sCelsius, tHumidity sHumidity)
 * \brief 
 * 
 * \param sCelsius 
 * \param sHumidity 
 * \return tHumidity 
 */
tHumidity DHTx1_DewPointFast(tTemperature sCelsius, tHumidity sHumidity);
/*-----------------------------------------------------------------------------------------------*/

#endif

/*************************************************************************************************
************* Copyright 2020 (C), Sichuan Deyang DeYi Electronic Instrument Co., Ltd *************
*************************************************************************************************/