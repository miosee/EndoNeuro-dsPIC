// include library
#include "acquisition.h"
#include <math.h>
#include <complex.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include <time.h> //<sys/time.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <ratio>
#include <chrono>
#include <stdio.h>  
#include <stdint.h>
#include <iomanip>
//#include "../libs/Spi.h"
#include <sys/syscall.h>
#include <bcm2835.h>
//#include <future>
#include "utils.h"


using namespace std;

#define TRIGGER RPI_V2_GPIO_P1_29  // Gpio 5  for trigger


void Acquisition::shutdown() {
    active = false;
}


void Acquisition::start(LockingQueue* queue) {
    string msg;
    uint16_t sample;
    uint64_t count = 0;

    msg = "Acquisition PID :  " + to_string(syscall(SYS_gettid));
    lockPrintln(msg);


    if (!bcm2835_init()) {
        msg = "Failed to initialise the GPIO";
        lockPrintln(msg);
    }
    if (!bcm2835_spi_begin()) {
        msg = "Failed to initialise the SPI";
        lockPrintln(msg);
    }
    // Configure the SPI
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE1);
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_512);
    bcm2835_spi_chipSelect(BCM2835_SPI_CS0);                      // The default
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);      // the default
    // Configure the trigger pin
    bcm2835_gpio_fsel(TRIGGER, BCM2835_GPIO_FSEL_INPT); // Set RPI pin P1-5  to be an input Trigger
    bcm2835_gpio_set_pud(TRIGGER, BCM2835_GPIO_PUD_UP);  //  with a pullup

    // Reset acquisition to start capturing data
    resetDspic();
    // Start acquisition
    startDspic();

    // Main Loop
    while (active) {
        if (bcm2835_gpio_lev(TRIGGER) == 1) {
            for (uint16_t i=0; i<PACKET_SIZE; i++) {
                packet[i] = 0;
            }
            bcm2835_spi_transfern((char*)packet, PACKET_SIZE);
            for (uint16_t i=0; i<PACKET_SIZE; i+=2) {
                sample = 256*packet[i] + packet[i+1];
                queue->push(sample);
            }
            count++;
        }
    }
    // Finish acquisition
    stopDspic();
    bcm2835_spi_end();
    bcm2835_close();
    lockPrintln("Acquisition finished, " + to_string(count) + " packets read.");
}

// Define acquistion parameter
#define START_CMD           1
#define STOP_CMD            2
#define RESET_CMD           3
#define SAMPLING_PERIOD_CMD 4


// Communicate to Dspic to Start Acquisition
void Acquisition::startDspic(void) {
    uint8_t frame[5] = {0x55, START_CMD, 0, 0, 0xAA};

    bcm2835_spi_transfern((char*)frame, 5);
}


// Communicate to Dspic to stop Acquisition
void Acquisition::stopDspic(void) {
    uint8_t frame[5] = {0x55, STOP_CMD, 0, 0, 0xAA};

    bcm2835_spi_transfern((char*)frame, 5);
}


// Communicate to Dspic to reset Acquisition
void Acquisition::resetDspic(void) {
    uint8_t frame[5] = {0x55, RESET_CMD, 0, 0, 0xAA};

    bcm2835_spi_transfern((char*)frame, 5);
}
