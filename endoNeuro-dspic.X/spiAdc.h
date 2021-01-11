/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef SPIADC_H
#define	SPIADC_H



#include <xc.h> // include processor files - each processor file is guarded.  
#include "hardConfig.h"


#define CHANNEL0    0xC000
#define CHANNEL1    0xC400
#define CHANNEL2    0xC800
#define CHANNEL3    0xCC00

void spiAdcInit();
uint16_t adcSample(uint16_t nextChannel);


#endif	/* SPIADC_H */


/**
    <p><b>Function prototype:</b></p>
  
    <p><b>Summary:</b></p>

    <p><b>Description:</b></p>

    <p><b>Precondition:</b></p>

    <p><b>Parameters:</b></p>

    <p><b>Returns:</b></p>

    <p><b>Example:</b></p>
    <code>
 
    </code>

    <p><b>Remarks:</b></p>
 */
