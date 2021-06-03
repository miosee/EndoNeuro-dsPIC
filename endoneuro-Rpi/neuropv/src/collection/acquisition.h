#ifndef ACQUISITION_H // include guard
#define ACQUISITION_H

#include <stdint.h>
#include "lockingQueue.h"

#define PACKET_SIZE 187


//#include <string>
class Acquisition {
    private:
        uint8_t packet[PACKET_SIZE];
        bool active = true;

    public:
        void start(LockingQueue* queue);
        void shutdown();
};

#endif