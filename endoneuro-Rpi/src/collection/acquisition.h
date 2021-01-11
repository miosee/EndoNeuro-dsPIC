#ifndef ACQUISITION_H // include guard
#define ACQUISITION_H

#include <stdint.h>
#include "lockingQueue.h"

#define PACKET_SIZE 200


//#include <string>
class Acquisition {
    private:
        uint8_t packet[PACKET_SIZE];
        bool active = true;
        void resetDspic();
        void stopDspic();
        void startDspic();

    public:
        void start(LockingQueue* queue);
        void shutdown();
};

#endif