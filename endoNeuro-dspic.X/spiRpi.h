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


#define CMD_START   1
#define CMD_STOP    2
#define CMD_RESET   3


typedef struct {
    uint8_t cmd;
    uint16_t param;
} rpiCommand_t;


void spiRpiInit();
uint16_t commandAvailable(void);
rpiCommand_t getCommand(void);



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
