#include "buffer.h"


volatile bufData_t   buffer[BUF_LENGTH];
volatile uint16_t    bufDataNb;
volatile uint16_t    bufStart, bufEnd;


void bufInit( void ) {
    bufDataNb = 0;
    bufStart = 0;
    bufEnd = 0;
}


bufErr_t bufPushData( bufData_t data ){
    if (bufDataNb < BUF_LENGTH) {
        buffer[bufEnd++] = data;
        if (bufEnd >= BUF_LENGTH) {
            bufEnd = 0;
        }
        bufDataNb++;
        return (NO_ERROR);
    }
    else {
        return(BUFFER_FULL);
    }
}


bufErr_t bufPopData( bufData_t *data ) {
    if (bufDataNb > 0) {
        *data = buffer[bufStart++];
        if (bufStart >= BUF_LENGTH) {
            bufStart = 0;
        }
        bufDataNb--;
        return(NO_ERROR);
    }
    else {
        return(BUFFER_EMPTY);
    }
}


uint16_t bufSize( void ) {
    return(bufDataNb);
}


uint16_t bufEmpty( void ) {
    return(bufDataNb == 0);
}


uint16_t bufFull( void ) {
    return(bufDataNb >= BUF_LENGTH);
}
