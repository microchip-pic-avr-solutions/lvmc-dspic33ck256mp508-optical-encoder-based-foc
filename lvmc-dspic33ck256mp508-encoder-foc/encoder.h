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
#ifndef _ENCODER_H
#define	_ENCODER_H

#ifdef __cplusplus  // Provide C++ Compatability
    extern "C" {
#endif
// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <xc.h>
#include <stdint.h>
#include "clock.h"
#include "userparms.h"
// *****************************************************************************
// *****************************************************************************
// Section: Constants
// *****************************************************************************
// *****************************************************************************
        
/*No of delay samples used for speed measurement*/        
#define DELAY_SAMPLES          20
        
/*Speed = Theta_Diff * [60/(Delay_Samples*50us*2^16)], Here Delay_Samples = 20
 [60/(Delay_Samples*50us)] = 60000   - multiplication with Theta_diff causes overflow
So, 60000/2^5 = 1875 used for multiplication and this is compensated with right shifting*/
#define SPEED_SCALING_Q        5

#define SPEED_MULTIPLIER       (60/(DELAY_SAMPLES*0.00005))/(2<<(SPEED_SCALING_Q-1))
  
typedef struct
{
    /*Position Count Measurement*/
    int16_t positionCount;
    /*Mechanical angle of the motor*/
    uint16_t  theta_mec;
    /*Electrical angle of the motor*/
    uint16_t  theta_ele;
    /*Angle difference for the speed calculation*/
    uint16_t  theta_diff;
    /*Speed measured from encoder in RPM*/
    int16_t speed;
    /*Speed measured in per unit*/
    int16_t speed_pu;
    /*State variable of low pass filter for timer count */
    int32_t speedStateVar;
    /*Low pass filter output*/
    int16_t speedFilter;
    /*Low pass filter coefficient*/
    int16_t speedKFilter;
    /*State variable of low pass filter for timer count */
    int32_t speedStateVarPU;
    /*Buffer to store the angle samples for speed calculation*/
    uint16_t  theta_mec_buf[20];
    /*Buffer Index*/
    uint16_t  buffer_Idx;
    /*Angle offset at the starting*/
    uint16_t  theta_offset;
} ENCODER;

extern ENCODER encoder;
// *****************************************************************************
// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
// *****************************************************************************

void calc_Encoder_Angle_Speed(void);

#ifdef __cplusplus  // Provide C++ Compatibility
    }
#endif
#endif      // end of _ENCODER_H

