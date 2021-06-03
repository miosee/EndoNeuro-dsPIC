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
#define DISABLE RPI_V2_GPIO_P1_31  // Gpio 6  for disable signal


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
    // Configure the disable pin
    bcm2835_gpio_fsel(DISABLE, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_write(DISABLE, HIGH);

    // Enable the dsPIC to start the acquisition
    bcm2835_gpio_write(DISABLE, LOW);
    // Main Loop
    while (active) {
        if (bcm2835_gpio_lev(TRIGGER) == 1) {
            packet[0] = 0x55;
            for (uint16_t i=1; i<PACKET_SIZE; i++) {
                packet[i] = 0;
            }
            bcm2835_spi_transfern((char*)packet, PACKET_SIZE);
            for (uint16_t i=3; i<PACKET_SIZE; i+=2) {
                sample = 256*packet[i] + packet[i+1]; //See datasheet
                queue->push(sample);
            }
            count++;
        }
    }
    // Disable the dsPIC to stop the acquisition
    bcm2835_gpio_write(DISABLE, HIGH);
    bcm2835_spi_end();
    bcm2835_close();
    lockPrintln("Acquisition finished, " + to_string(count) + " packets read.");
}
