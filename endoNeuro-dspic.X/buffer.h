/* 
 * File:   buffer.h
 * Author: mosee
 *
 * Created on 17 janvier 2020, 11:01
 */

#ifndef BUFFER_H
#define	BUFFER_H

#include <xc.h>

#define BUF_LENGTH  8000
#define bufData_t   uint8_t


typedef enum {
    NO_ERROR = 0,
    BUFFER_FULL = -1,
    BUFFER_EMPTY = -2
} bufErr_t;


void     bufInit( void );
bufErr_t bufPushData( bufData_t data );
bufErr_t bufPopData( bufData_t *data );
uint16_t bufSize( void );
uint16_t bufEmpty( void );
uint16_t bufFull( void );



#endif	/* BUFFER_H */

