// Import library
#include "collection/acquisition.h"
#include "collection/acquisitionFiles.h"
#include "collection/video.h"
#include "collection/lockingQueue.h"
#include "collection/utils.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mount.h>
#include <syslog.h>
#include <thread>
#include <unistd.h>
#include <iostream>

//#include "libs/Spi.h"

using namespace std;

#define FILE_LENGTH_IN_SEC  5

LockingQueue acqQueue;
Acquisition acq;
AcquisitionFiles acqFiles;
Video video;


void acqTask(LockingQueue *queue) {
  acq.start(queue);
}

void acqFilesTask(LockingQueue *queue) {
  acqFiles.start(queue);
}

void videoTask(int fileDurationInSeconds) {
  video.start(fileDurationInSeconds);
}

int main() {
    cout << "Main PID :  " << getpid() << endl;

    setlogmask(LOG_UPTO(LOG_NOTICE));
    openlog("Endoneuro", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);

    const char* src = "//10.0.0.2/raspberry3";
    const char* trgt = "/home/pi/Documents/DataShare/";
    const char* type = "cifs"; // Type of mounted hdd
    const unsigned long mntflags = 0;
    const char* opts = "user=pi,pass=kiwi1050,file_mode=0777,dir_mode=0777,noserverino,nounix"; // Params to mount cifs HDD

    int result = mount(src, trgt, type, mntflags, opts);

    if (result == 0) {
        cout << "Mount created at  " << trgt << endl;
    } else {
        cout << "Error : Failed to mount " << src << endl;
        cout << "Reason: " << strerror(errno) << " [" << errno << "]\n";
        umount(trgt); // Umount HDD
        return -1;    // Exit app
    }

    cout << "Starting app.." << endl;
    syslog(LOG_NOTICE, "Starting Endoneuro");

    // Start threads
    thread acqThread(acqTask, &acqQueue);
    thread acqFilesThread(acqFilesTask, &acqQueue);
    thread videoThread(videoTask, FILE_LENGTH_IN_SEC);


    lockPrintln("Press a key to end the acquisition.");
    //char c;
    getchar();

    acq.shutdown();
    acqThread.join();               // join the thread to wait its end before continuing
    acqFiles.shutdown();
    acqFilesThread.join();
    video.shutdown();
    videoThread.join();
    umount2(trgt, MNT_FORCE); // Umount HDD

    return 0;
}
