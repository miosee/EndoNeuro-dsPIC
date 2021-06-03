#ifndef ACQUISITIONFILES_H // include guard
#define ACQUISITIONFILES_H

#include"lockingQueue.h"

#define FILE_LENGTH_IN_SEC  5
/* The 2 VENG channels are sampled at 20kHz,
 * The 6 EEG channels are sampled at 1kHz.
 * -> 1 sec <=> 2*20.000 + 6*1000 = 46.000 samples */
#define FILE_SIZE  (46000ul*FILE_LENGTH_IN_SEC)


class AcquisitionFiles {
    private:
        bool active = true;
        uint16_t buffer[FILE_SIZE];
        void writeToFile();

    public:
        void start(LockingQueue* queue);
        void shutdown();
};


#endif
