/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef SPIRPI_H
#define	SPIRPI_H



#include <xc.h> // include processor files - each processor file is guarded.  
#include "hardConfig.h"


#define NO_ERROR   0x00
#define PERIOD_ERR 0x01
#define BUFFER_ERR 0x02
/* each packet contains 
 * 1 ID byte (rolling from 0 to 255)
 * 1 error byte
 * 2 identical sequences of 20 sampling results:
 *   on the first sampling period, we acquire the 4 channels
 *   on the 19 following periods, we acquire only channels 1 and 2
 *   Each sample being coded on 2 bytes, these sequences are coded on 84 bytes
 * Hence, one packet contains 2+2*84 = 170 bytes
 */
#define BUFFER_SIZE     170
#define BUFFER_NUMBER   80

extern volatile uint8_t buffer[BUFFER_NUMBER][BUFFER_SIZE];
extern volatile uint16_t bufToSend;

void spiRpiInit();




#endif	/* SPIRPI_H */


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
