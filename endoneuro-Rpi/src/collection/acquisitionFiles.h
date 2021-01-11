#ifndef ACQUISITIONFILES_H // include guard
#define ACQUISITIONFILES_H

#include"lockingQueue.h"

#define CHANNEL_NB 4
#define SAMPLE_PER_US 400
#define FILE_DURATION_MS 5000ul
#define BUF_LENGTH (1000 * FILE_DURATION_MS * CHANNEL_NB / SAMPLE_PER_US)


class AcquisitionFiles {
    private:
        bool active = true;
        uint16_t buffer[BUF_LENGTH];
        void writeToFile();

    public:
        void start(LockingQueue* queue);
        void shutdown();
};


#endif