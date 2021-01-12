#include <xc.h>
#define FCY 40000000
#include <libpic30.h>
#include <p33FJ128MC802.h>
#include "hardConfig.h"
#include "spiRpi.h"
#include "spiAdc.h"



uint16_t bufIndex;
uint16_t curBuffer;
uint16_t bufCount;
uint8_t status;
    
    
void bufInit() {
    bufToSend = 0;
    curBuffer = 0;
    bufCount = 0;
    status = STATUS_OK;
    buffer[curBuffer][0] = bufCount++;
    buffer[curBuffer][1] = status;
    bufIndex = 2;
}


int main(void) {
    uint16_t periodCount = 0;
    uint16_t newSample;
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
    RPI_DISABLE_TRIS = 1;
    RPI_DISABLE_PUE = 1;
    
    //INTCON1bits.NSTDIS = 1;
    _SPI1IP = 6;
    _SPI2IP = 5;
    _T2IP = 4;
    
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
    bufInit();
	while(1) {
        if (_T2IF) {
            _T2IF = 0;
            // Acquire channel 0 and set CHANNEL1 as next to be acquired
            newSample = adcSample(CHANNEL1);
            buffer[curBuffer][bufIndex++] = (uint8_t)(newSample >> 8);
            buffer[curBuffer][bufIndex++] = (uint8_t)(newSample & 0x00FF);
            if (periodCount != 0) {     // Usually, we only acquire CHANNEL0 and CHANNEL1
                // Acquire CHANNEL1 and set CHANNEL0 as next to be acquired
                newSample = adcSample(CHANNEL0);
                buffer[curBuffer][bufIndex++] = (uint8_t)(newSample >> 8);
                buffer[curBuffer][bufIndex++] = (uint8_t)(newSample & 0x00FF);
            } else {    // each 20 sampling period, we also acquire CHANNEL2 and CHANNEL3
                // Acquire CHANNEL1 and set CHANNEL2 as next to be acquired
                newSample = adcSample(CHANNEL2);
                buffer[curBuffer][bufIndex++] = (uint8_t)(newSample >> 8);
                buffer[curBuffer][bufIndex++] = (uint8_t)(newSample & 0x00FF);
                // Acquire CHANNEL2 and set CHANNEL3 as next to be acquired
                newSample = adcSample(CHANNEL3);
                buffer[curBuffer][bufIndex++] = (uint8_t)(newSample >> 8);
                buffer[curBuffer][bufIndex++] = (uint8_t)(newSample & 0x00FF);
                // Acquire CHANNEL3 and set CHANNEL0 as next to be acquired
                newSample = adcSample(CHANNEL0);
                buffer[curBuffer][bufIndex++] = (uint8_t)(newSample >> 8);
                buffer[curBuffer][bufIndex++] = (uint8_t)(newSample & 0x00FF);
            }
            if (++periodCount > 19) {
                periodCount = 0;
            }
            
            if (bufIndex > BUFFER_SIZE) {
                bufIndex = 0;
                if (++curBuffer > BUFFER_NUMBER) {
                    curBuffer = 0;
                }
                buffer[curBuffer][0] = 0;
                buffer[curBuffer][1] = bufCount++;
                buffer[curBuffer][2] = status;
                bufIndex = 3;
                bufToSend++;
                if (bufToSend >= BUFFER_NUMBER) {
                    status = STATUS_ERR;
                } else {
                    status = STATUS_OK;
                }
                RPI_TRIG = 1;
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

        if (RPI_DISABLE == 0) {
            T2CONbits.TON = 1;
            ledMax = 2;
        } else {
            T2CONbits.TON = 0;
            ledMax = 6;
            bufInit();
            RPI_TRIG = 0;
        }
	}
}
