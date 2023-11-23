/*******************************************************************************
  Source File for Quadrature Encoder Interface (QEI)

  File Name:
    qei.c

  Summary:
    This file includes subroutine for initializing  Quadrature Encoder Interface (QEI)

  Description:
    Definitions in the file are for dsPIC33CK256MP508 on Motor Control 
    Development board from Microchip

*******************************************************************************/
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
#include "userparms.h"
#include "qei.h"

void InitQEI(void)
{
    int32_t temp = 0;

    QEI1CON = 0;
    
    QEI1CONbits.PIMOD = 0b110;  // Modulo Count mode
    QEI1CONbits.INTDIV = 0b100; //1:16 prescale value
    POS1CNTL = 0;
    POS1CNTH = 0;
    
/*
 * In module count mode:
 * 
 * In CW direction, assume that the POS count in UP (increment).
 * The Position Counter is loaded with the contents of the QEIxLEC register when the Position Counter value equals the QEIxGEC register value and a count up pulse is detected.
 * When starting from 0 and CW rotation (UP count or increment) --> 0, 1, 2...96, 97, 98, -100, -99, -98, -97....-3, -2, -1, 0, 1, 2, 3, 4.....
 * 
 * In CCW direction, assume that the POS count in DOWN (decrement).
 * The counter is loaded with the contents of the QEIxGEC register when the Position Counter value equals the QEIxLEC register value and a count down pulse is detected.
 * When starting from 0 and CCW rotation (DOWN count or decrement) --> 0, -1, -2, -3....-97, -98, -99, 99, 98, 97....3, 2, 1, 0, -1, -2, -3, -4.....
 */    
    temp = -ELECTRICAL_CYCLE_COUNT_PI_RADS;     // -100
    QEI1LECH = (int16_t)(temp >> 16);
    QEI1LECL = (int16_t)(temp);  
    
    temp = (ELECTRICAL_CYCLE_COUNT_PI_RADS-1);      // +99
    QEI1GECH = (int16_t)(temp >> 16);
    QEI1GECL = (int16_t)(temp);
    
    QEI1IOCbits.SWPAB = 0;      // Not swapped
    
    QEI1CONbits.QEIEN = 1;
}
