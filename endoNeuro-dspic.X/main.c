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
    uint16_t periodCount = 0;
    uint16_t newSample;
    rpiCommand_t command;
    uint16_t ledCount, ledMax;
    
    
	frcPllConfig();
    
    MAIN_LOOP_LED = 0;
    MAIN_LOOP_LED_TRIS = 0;
    ACQ_LED = 0;
    ACQ_LED_TRIS = 0;
    
    AD1PCFGL = ANALOG_PINS_CFG;
//    _TRISB8 = 0;
//    _TRISB9 = 0;
    RPI_TRIG_TRIS = 0;
    RPI_TRIG = 0;
    RPI_ACTIVE_TRIS = 1;
    
    //INTCON1bits.NSTDIS = 1;
    _SPI1IP = 6;
    _SPI2IP = 5;
    _T2IP = 4;
    
    bufInit();
    spiRpiInit();
    spiAdcInit();
    
    T2CONbits.TCKPS = 1;        // prescaler = 1:8 => PR2 unit = 200nsec
    PR2 = 124;                  // Period = 125*200ns = 100us
    _T2IF = 0;

    T3CONbits.TCKPS = 3;        // prescaler = 1:256 => PR3 unit = 6.4usec
    PR3 = 5*7811u;              // 6.4us*7812 = 49.9968ms
    T3CONbits.TON = 1;
    
    ledMax = 6;
    ledCount = 0;
	while(1) {
        if (_T2IF) {
            _T2IF = 0;
            // Acquire channel 0 and set CHANNEL1 as next to be acquired
            newSample = adcSample(CHANNEL1);
            bufPushData( (uint8_t)(newSample >> 8) );
            bufPushData( (uint8_t)(newSample & 0x00FF) );
            if (periodCount != 0) {     // Usually, we only acquire CHANNEL0 and CHANNEL1
                // Acquire CHANNEL1 and set CHANNEL0 as next to be acquired
                newSample = adcSample(CHANNEL0);
                bufPushData( (uint8_t)(newSample >> 8) );
                bufPushData( (uint8_t)(newSample & 0x00FF) );
            } else {    // each 20 sampling period, we also acquire CHANNEL2 and CHANNEL3
                // Acquire CHANNEL1 and set CHANNEL2 as next to be acquired
                newSample = adcSample(CHANNEL2);
                bufPushData( (uint8_t)(newSample >> 8) );
                bufPushData( (uint8_t)(newSample & 0x00FF) );
                // Acquire CHANNEL2 and set CHANNEL3 as next to be acquired
                newSample = adcSample(CHANNEL3);
                bufPushData( (uint8_t)(newSample >> 8) );
                bufPushData( (uint8_t)(newSample & 0x00FF) );
                // Acquire CHANNEL3 and set CHANNEL0 as next to be acquired
                newSample = adcSample(CHANNEL0);
                bufPushData( (uint8_t)(newSample >> 8) );
                bufPushData( (uint8_t)(newSample & 0x00FF) );
            }
            if (++periodCount > 19) {
                periodCount = 0;
            }
            
            if (bufSize() >= packetSize) {
                RPI_TRIG = 1;
            } else {
                RPI_TRIG = 0;
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
                RPI_TRIG = 0;
                ledMax = 6;
            }
        }
	}
}
