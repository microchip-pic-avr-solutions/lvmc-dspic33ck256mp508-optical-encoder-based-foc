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

/*****************************************************************************
 *Function:
 *      calc_Encoder_Angle_Speed()
 * 
 * Summary:
 *      Function calculates the motor rotor angle and speed from the encoder
 *      
 *
 *****************************************************************************/
void calc_Encoder_Angle_Speed(void)
{
    encoder.positionCount = POS1CNTL;
    
    /*Position count to mechanical angle conversion from 0 to 65535
     *When PositionCount = 999, theta _mec = 65535.
     *So, 65535 = (16794*999) >> 8 */
    encoder.theta_mec = (uint16_t)(__builtin_mulss(encoder.positionCount, 16794) >> 8);
    
    /*Mechanical Angle to Electrical Angle conversion*/
    encoder.theta_ele = (uint16_t)(__builtin_mulss(encoder.theta_mec, NOPOLESPAIRS));
    
    /*Angle difference between 20 Samples for speed calculation*/
    encoder.theta_diff = encoder.theta_mec - encoder.theta_mec_buf[encoder.buffer_Idx];
    
    /*Speed = Theta_Diff * [60/(Delay_Samples*50us*65535)], Here Delay_Samples = 20
     [60/(Delay_Samples*50us)] = 60000   - multiplication with Theta_diff causes overflow
     So, 60000/2^5 = 1875 used for multiplication and this is compensated with right shifting*/
    encoder.speed = (int16_t)(__builtin_mulss(encoder.theta_diff, 
                                   SPEED_MULTIPLIER) >> (16 - SPEED_SCALING_Q)); 
    /*Low pass filter for the speed*/
    encoder.speedStateVar += (((int32_t)encoder.speed - 
                (int32_t)encoder.speedFilter)*(int16_t)(encoder.speedKFilter));
    encoder.speedFilter = (int16_t)(encoder.speedStateVar>>15);
    
    encoder.theta_mec_buf[encoder.buffer_Idx] = encoder.theta_mec;
    if(encoder.buffer_Idx < (DELAY_SAMPLES-1))
    {
        encoder.buffer_Idx++;
    }
    else
    {
        encoder.buffer_Idx = 0;
    }
}