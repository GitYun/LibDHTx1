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

int8_t DHTx1_Init(tDHTx1Sensor* psDHTx1, tDHTx1Gpio* psGpio)
{
    memset(psDHTx1, 0, sizeof(tDHTx1Sensor));

    psDHTx1->psGPIO = psGpio;

    DHTx1_Close(psDHTx1);

    return 0;
}
/*-----------------------------------------------------------------------------------------------*/

tDHTx1Status DHTx1_Open(tDHTx1Sensor* psDHTx1)
{

}
/*-----------------------------------------------------------------------------------------------*/

bool DHTx1_Close(tDHTx1Sensor* psDHTx1) 
{
    DHTx1_DATA_PIN_OUTPUT_MODE(GPIO_PIN(psDHTx1->psGPIO));
    DHTx1_DATA_PIN_HIGH(GPIO_PIN(psDHTx1->psGPIO));

    return true;
}
/*-----------------------------------------------------------------------------------------------*/

uint8_t DHTx1_ReadByte(tDHTx1Sensor* psDHTx1)
{
    uint8_t ui8Result = 0;

    uint8_t idx = 0;
    for (idx = 0; idx < 8; ++idx)
    {
        ui8Result <<= idx;

        // Wait 50us before read one-wire bus data
        while (DHTx1_DATA_PIN_READ(GPIO_PIN(psDHTx1->psGPIO)) == 0);
        
        // Delay 30us, aim to jump data '0'
        DHTx1_DelayUs(30);

        if (DHTx1_DATA_PIN_READ(GPIO_PIN(psDHTx1->psGPIO)) == 1) // Read data '1'
        {
            ui8Result |= 0x01;
        }
        
        // Wait data '1' finish        
        while (DHTx1_DATA_PIN_READ(GPIO_PIN(psDHTx1->psGPIO)) == 1);
    }

    return ui8Result;
}
/*-----------------------------------------------------------------------------------------------*/

bool DHTx1_IsShortOfReadTime(void)
{

}
/*-----------------------------------------------------------------------------------------------*/

bool DHTx1_IsDataChanged(tDHTx1Sensor* psDHTx1)
{
    
}
/*-----------------------------------------------------------------------------------------------*/

/*************************************************************************************************
************* Copyright 2020 (C), Sichuan Deyang DeYi Electronic Instrument Co., Ltd *************
*************************************************************************************************/