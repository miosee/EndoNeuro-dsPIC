#ifndef TRANSFER_H // include guard
#define TRANSFER_H
#include <iostream>

using namespace std;

class Transfer {
    private:
        bool active = true;

    public:
        void start();
        void shutdown();
};


#endif