#include <xc.h>
#define FCY 40000000
#include <libpic30.h>
#include <p33FJ128MC802.h>
#include "hardConfig.h"
#include "spiRpi.h"
#include "buffer.h"
#include "spiAdc.h"


// GLOBAL VARIABLES
volatile uint16_t packetSize = 200;
volatile uint16_t bufCurSize = 0;
volatile uint16_t bufferMax = 0;



int main(void) {
    uint16_t newSample;
    rpiCommand_t command;
    bufErr_t bufErr;
    uint16_t bufCurSize;
    uint16_t ledCount, ledMax;
    
    
	frcPllConfig();
    
    MAIN_LOOP_LED = 0;
    MAIN_LOOP_LED_TRIS = 0;
    ACQ_LED = 0;
    ACQ_LED_TRIS = 0;
    
    AD1PCFGL = ANALOG_PINS_CFG;
    _TRISB8 = 0;
    _TRISB9 = 0;
    RPI_TRIG_TRIS = 0;
    RPI_TRIG = 0;

    
    //INTCON1bits.NSTDIS = 1;
    
    bufInit();
    spiRpiInit();
    spiAdcInit();
    
    T2CONbits.TCKPS = 1;        // prescaler = 1:8 => PR2 unit = 200nsec
    PR2 = 499;//499;                  // Period = 500*200ns = 100us
    _SPI1IP = 6;
    _T2IP = 5;
    _T2IF = 0;
    _T2IE = 1;

    T3CONbits.TCKPS = 3;        // prescaler = 1:256 => PR3 unit = 6.4usec
    PR3 = 5*7811u;              // 6.4us*7812 = 49.9968ms
    T3CONbits.TON = 1;
    
    ledMax = 6;
    ledCount = 0;
	while(1) {
        if (newSampleFlag) {
            _SPI2IE = 0;
            newSample = spi2IsrData;
            _SPI2IE = 1;
            newSampleFlag = 0;
            //_SPI1IE = 0;
            bufErr = bufPushData( (uint8_t)(newSample >> 8) );
            //_SPI1IE = 1;
            if (bufErr == NO_ERROR) {
                //_SPI1IE = 0;
                bufErr = bufPushData( (uint8_t)(newSample & 0x00FF) );
                //_SPI1IE = 1;
            }
            bufCurSize = bufSize();
            if ( bufErr == NO_ERROR ) {
                if (bufCurSize > bufferMax) {
                    bufferMax = bufCurSize;
                }
                if (bufCurSize >= packetSize) {
                    //SPI1BUF = 0xAA;
                    RPI_TRIG = 1;
                    ACQ_LED = 1;
                } else {
                    RPI_TRIG = 0;
                    ACQ_LED = 0;
                }
            } else {
                // ERROR_LED = 1;
                T2CONbits.TON = 0;
                RPI_TRIG = 0;
                ACQ_LED = 0;
            }
        }
        
        if (IFS0bits.T3IF) {
            IFS0bits.T3IF = 0;
            if (++ledCount > ledMax) {
                ledCount = 0;
            }
            if (ledCount == 0) {
                MAIN_LOOP_LED = 1;
            }
            else {
                MAIN_LOOP_LED = 0;
            }
        }

        if (commandAvailable()) {
            command = getCommand();
            if (command.cmd == CMD_START) {
                T2CONbits.TON = 1;
                ledMax = 2;
            } else if (command.cmd == CMD_STOP) {
                T2CONbits.TON = 0;
                ledMax = 4;
            } else if (command.cmd == CMD_RESET) {
                T2CONbits.TON = 0;
                _SPI1IE = 0;
                bufInit();
                _SPI1IE = 1;
                newSample = 0;
                newSampleFlag = 0;
                RPI_TRIG = 0;
                ACQ_LED = 0;
                ledMax = 6;
            }
        }
	}
}
