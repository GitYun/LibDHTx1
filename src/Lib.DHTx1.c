/**
 * \file Lib.DHTx1.c
 * \author vEmagic (admin@vemgic.com)
 * \brief 
 * \version 0.1.0
 * \date 2020-10-29
 * 
 * Copyright 2020 (C), Sichuan Deyang DeYi Electronic Instrument Co., Ltd.
 * 
 */

#include <string.h>
#include "Lib.DHTx1.h"

#define GPIO_PIN(psGpio) ((psGpio)->sPort, (psGpio)->sPin)

/* Initial DHTx1 Device */
int8_t DHTx1_Init(tDHTx1Sensor* psDHTx1)
{
    psDHTx1->ui32LastReadTime = 0;
    memset(psDHTx1->pui8Data, 0, sizeof(psDHTx1->pui8Data));

    if (psDHTx1->sId != eDHT11 && psDHTx1->sId != eDHT21)
    {
        return -1;
    }

    DHTx1_Close(psDHTx1);
    psDHTx1->sStatus = eDHTx1_CLOSED;

    return 0;
}
/*-----------------------------------------------------------------------------------------------*/

/* Open the DHTx1 Device */
tDHTx1Status DHTx1_Open(tDHTx1Sensor* psDHTx1)
{
    /* Start condition: I/O pin output low level 20ms */
    DHTx1_DATA_PIN_OUTPUT_MODE(GPIO_PIN(psDHTx1->psGPIO));
    DHTx1_DATA_PIN_LOW(GPIO_PIN(psDHTx1->psGPIO));
    DHTx1_DelayUs(20000);

    DHTx1_DATA_PIN_HIGH(GPIO_PIN(psDHTx1->psGPIO));
    DHTx1_DelayUs(20); // Pull-hgih level 20-40us

    DHTx1_DATA_PIN_INPUT_MODE(GPIO_PIN(psDHTx1->psGPIO));

    if (DHTx1_DATA_PIN_READ(GPIO_PIN(psDHTx1->psGPIO)) == 0)
    {
        uint8_t ui8Timeout = 0;
        while (DHTx1_DATA_PIN_READ(GPIO_PIN(psDHTx1->psGPIO)) == 0 && ++ui8Timeout < 100)
        {
            DHTx1_DelayUs(1);
        }

        if (ui8Timeout > 90)
        {
            return eDHTx1_OPEN_RSP_LOW_ERROR;
        }

        ui8Timeout = 0;
        while (DHTx1_DATA_PIN_READ(GPIO_PIN(psDHTx1->psGPIO)) == 1 && ++ui8Timeout < 100)
        {
            DHTx1_DelayUs(1);
        }

        if (ui8Timeout > 90)
        {
            return eDHTx1_OPEN_RSP_HIGH_ERROR;
        }

        return eDHTx1_OPENED;
    }
}
/*-----------------------------------------------------------------------------------------------*/

/* Close the DHTx1 Device */
bool DHTx1_Close(tDHTx1Sensor* psDHTx1) 
{
    DHTx1_DATA_PIN_OUTPUT_MODE(GPIO_PIN(psDHTx1->psGPIO));
    DHTx1_DATA_PIN_HIGH(GPIO_PIN(psDHTx1->psGPIO));

    return true;
}
/*-----------------------------------------------------------------------------------------------*/

/* Read a byte data from the DHTx1 Device */
uint8_t DHTx1_ReadByte(tDHTx1Sensor* psDHTx1)
{
    uint8_t ui8Result = 0;
    uint8_t ui8Timeout = 0;

    uint8_t idx;
    for (idx = 0; idx < 8; ++idx)
    {
        ui8Result <<= 1;

        // Wait 50us before read one-wire bus data
        while (DHTx1_DATA_PIN_READ(GPIO_PIN(psDHTx1->psGPIO)) == 0 && ++ui8Timeout < 60);
        {
            DHTx1_DelayUs(1);
        }

        if (ui8Timeout >= 60)
        {
            return 0;
        }

        // Delay 30us, aim to jump data '0'
        DHTx1_DelayUs(30);

        if (DHTx1_DATA_PIN_READ(GPIO_PIN(psDHTx1->psGPIO)) == 1) // Read data '1'
        {
            ui8Result |= 0x01;
        }
        
        // Wait data '1' finish
        ui8Timeout = 0;
        while (DHTx1_DATA_PIN_READ(GPIO_PIN(psDHTx1->psGPIO)) == 1 && ++ui8Timeout < 60)
        {
            DHTx1_DelayUs(1);
        }

        if (ui8Timeout >= 60)
        {
            return 0;
        }
    }

    return ui8Result;
}
/*-----------------------------------------------------------------------------------------------*/

/* Read the DHTx1 Device Data */
bool DHTx1_ReadData(tDHTx1Sensor* psDHTx1)
{
    bool bRet = false;

    if (psDHTx1->sStatus == eDHTx1_OPENED)
    {
        memset(psDHTx1->pui8Data, 0, sizeof(psDHTx1->pui8Data));

        uint8_t idx;
        for (idx = 0; idx < 5; ++idx)
        {
            psDHTx1->pui8Data[idx] = DHTx1_ReadByte(psDHTx1);
        }

        bRet = psDHTx1->pui8Data[4] == (uint8_t)(psDHTx1->pui8Data[0] + psDHTx1->pui8Data[1]
            +psDHTx1->pui8Data[2] + psDHTx1->pui8Data[2]);
    }

    return bRet;
}

/* Check that the interval between reading data is too short */
bool DHTx1_IsShortOfReadTime(tDHTx1Sensor* psDHTx1, uint32_t ui32IntervalMilliSeconds)
{
    bool bRet = false;
    uint32_t ui32CurrentReadTime = DHTx1_Millis();

    if ((uint32_t)(ui32CurrentReadTime - psDHTx1->ui32LastReadTime) < ui32IntervalMilliSeconds)
    {
        bRet = true;
    }
    else
    {
        psDHTx1->ui32LastReadTime = ui32CurrentReadTime;
    }

    return bRet;
}
/*-----------------------------------------------------------------------------------------------*/

/* Check the data whether has been changed */
bool DHTx1_IsDataChanged(tDHTx1Sensor* psDHTx1, uint16_t ui16PreviousData)
{
    return true;
}
/*-----------------------------------------------------------------------------------------------*/

/* Get the temperature value, the value scale 10 time */
uint16_t DHTx1_GetTemperature(tDHTx1Sensor* psDHTx1)
{    
    uint16_t ui16Temperature = 0;

    if (psDHTx1->sId == eDHT11)
    {
        ui16Temperature = (uint16_t)psDHTx1->pui8Data[2] * 10 + psDHTx1->pui8Data[3];
    }
    else if (psDHTx1->sId == eDHT21)
    {
        ui16Temperature = ((uint16_t)psDHTx1->pui8Data[2] << 8) | psDHTx1->pui8Data[3];
    }

    return ui16Temperature;
}
/*-----------------------------------------------------------------------------------------------*/

/* Get the humidity value, the value scale 10 time */
uint16_t DHTx1_GetHumidity(tDHTx1Sensor* psDHTx1)
{
    uint16_t ui16Humidity = 0;

    if (psDHTx1->sId == eDHT11)
    {
        ui16Humidity = (uint16_t)psDHTx1->pui8Data[0] * 10 + psDHTx1->pui8Data[1];
    }
    else if (psDHTx1->sId == eDHT21)
    [
        ui16Humidity = ((uint16_t)psDHTx1->pui8Data[0] << 8) | psDHTx1->pui8Data[1];
    ]

    return ui16Humidity;
}
/*-----------------------------------------------------------------------------------------------*/

/* Convert the temperature from celsius to fahrenheit */
float DHTx1_ConvertC2F(uint16_t ui16Celsius)
{
    return (float)1.8 * ui16Celsius / 10.0 + 32.0;
}
/*-----------------------------------------------------------------------------------------------*/

/* Convert the temperature from celsius to kai */
float DHTx1_ConvertC2K(uint16_t ui16Celsius)
{
    return (float)ui16Celsius / 10.0 + 273.15;
}
/*-----------------------------------------------------------------------------------------------*/

// 露点（点在此温度时，空气饱和并产生露珠）
// 参考: http://wahiduddin.net/calc/density_algorithms.htm
float DHTx1_DewPoint(uint16_t ui16Celsius, uint16_t ui16Humidity)
{
    return 0;
}
/*-----------------------------------------------------------------------------------------------*/

// 快速计算露点，速度是5倍的DHTx1_DewPoint()
// 参考: http://en.wikipedia.org/wiki/Dew_point
float DHTx1_DewPointFast(uint16_t ui16Celsius, uint16_t ui16Humidity)
{
    float f32Celsius = (float)ui16Celsius / 10.0;
    float f32humidity = (float)ui16Humidity / 10.0;

    float f32Tmp = (17.271 * f32Celsius) / (237.7 + f32Celsius) + log(f32humidity / 100);
    
    return (237.7 * f32Tmp) / (17.271 - f32Tmp);
}
/*-----------------------------------------------------------------------------------------------*/

/*************************************************************************************************
************* Copyright 2020 (C), Sichuan Deyang DeYi Electronic Instrument Co., Ltd *************
*************************************************************************************************/