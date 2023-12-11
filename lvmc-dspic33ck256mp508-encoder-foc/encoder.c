/*******************************************************************************
* Copyright (c) 2017 released Microchip Technology Inc.  All rights reserved.
*
* SOFTWARE LICENSE AGREEMENT:
* 
* Microchip Technology Incorporated ("Microchip") retains all ownership and
* intellectual property rights in the code accompanying this message and in all
* derivatives hereto.  You may use this code, and any derivatives created by
* any person or entity by or on your behalf, exclusively with Microchip's
* proprietary products.  Your acceptance and/or use of this code constitutes
* agreement to the terms and conditions of this notice.
*
* CODE ACCOMPANYING THIS MESSAGE IS SUPPLIED BY MICROCHIP "AS IS".  NO
* WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
* TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A
* PARTICULAR PURPOSE APPLY TO THIS CODE, ITS INTERACTION WITH MICROCHIP'S
* PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.
*
* YOU ACKNOWLEDGE AND AGREE THAT, IN NO EVENT, SHALL MICROCHIP BE LIABLE,
* WHETHER IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR BREACH OF
* STATUTORY DUTY),STRICT LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE,
* FOR ANY INDIRECT, SPECIAL,PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL
* LOSS, DAMAGE, FOR COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE CODE,
* HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR
* THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT ALLOWABLE BY LAW,
* MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS CODE,
* SHALL NOT EXCEED THE PRICE YOU PAID DIRECTLY TO MICROCHIP SPECIFICALLY TO
* HAVE THIS CODE DEVELOPED.
*
* You agree that you are solely responsible for testing the code and
* determining its suitability.  Microchip has no obligation to modify, test,
* certify, or support the code.
*
*******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "xc.h"
#include <stdint.h>
#include "encoder.h"
#include "userparms.h"
#include "qei.h"
#include "pwm.h"
#include "port_config.h"

int32_t getQEICount(void)
{
    int32_t pos;
    int32_t posHigh;
    
    pos = POS1CNTL;
    posHigh = POS1HLD;
    posHigh = posHigh << 16;
    return(posHigh | pos);
}

int32_t getQEIVelocity(void)
{
    int32_t velo;
    int32_t veloHigh;
    
    velo = VEL1CNT;
    veloHigh = VEL1HLD;
    veloHigh = veloHigh << 16;
    return(veloHigh | velo);
}

int32_t getQEITimer(void)
{
    int32_t timer;
    int32_t timerHigh;
    
    timer = INT1HLDL;
    timerHigh = INT1HLDH;
    timerHigh = timerHigh << 16;
    return(timerHigh | timer);
}

void clearQEICount(void)
{
    POS1CNTL = 0;
    POS1CNTH = 0; 
    QEI1CONbits.QEIEN = 1;
}

/*****************************************************************************
 *Function:
 *      calcEncoderAngle()
 * 
 * Summary:
 *      Function calculates the motor rotor angle from the encoder
 *      
 *
 *****************************************************************************/
void calcEncoderAngle(void)
{
    encoder.positionCount = (int16_t)getQEICount();
    encoder.Theta = encoder.positionCount * 327;
}

/*****************************************************************************
 *Function:
 *      calcEncoderSpeed()
 * 
 * Summary:
 *      Function calculates the motor speed from the encoder
 *      
 *
 *****************************************************************************/
void calcEncoderSpeed(void)
{     
    encoder.velocityDelayCounter++;
    if(encoder.velocityDelayCounter>=(PWMFREQUENCY_HZ/VELOCITY_CONTROL_ESEC_FREQ_HZ))
    {    
        encoder.velocityCount = (int16_t)getQEIVelocity();
        encoder.velCntSpeed = encoder.velocityCount * SPEED_MULTI_VELCNT;
        encoder.velocityDelayCounter = 0;
    }
    if(encoder.velocityCount >= 0)
    {
        encoder.timerCount = (uint16_t)getQEITimer();  
    }
    else
    {
        encoder.timerCount = -(uint16_t)getQEITimer();  
    }
    encoder.timerStateVar += (((long int)encoder.timerCount - 
                (long int)encoder.timerFilter)*(int)(encoder.timerKFilter));
    encoder.timerFilter = (int)(encoder.timerStateVar>>15); 
    if(encoder.timerCount != 0)
    {
        encoder.tmrCntSpeed =  (__builtin_divud((uint32_t)SPEED_MULTI_TMRCNT,
                                        (uint16_t)(encoder.timerFilter)));
    }
    if(encoder.tmrCntSpeed < 5000)
    {
        encoder.Speed = encoder.tmrCntSpeed;
    }
    else
    {
        encoder.Speed = encoder.velCntSpeed;
    }
}

