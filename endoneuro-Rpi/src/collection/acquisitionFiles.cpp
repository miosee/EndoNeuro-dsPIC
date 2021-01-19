#include "acquisitionFiles.h" // header in local directory
#include "acquisition.h"
#include "utils.h"
#include <chrono>
#include <cstdio>
#include <ctime>
#include <fcntl.h> // open
#include <fstream>
#include <future>
#include <iostream>
#include <sstream>
#include <sys/sendfile.h> // sendfile
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <syslog.h>
#include <unistd.h>
#include <filesystem>

namespace fs = std::filesystem;
using namespace std;
using namespace std::chrono;


void AcquisitionFiles::shutdown() {
    active = false;
}


void AcquisitionFiles::start(LockingQueue* queue) {
    string msg;
    uint64_t i;
    uint64_t count = 0;


    // Set Log configuration
    setlogmask(LOG_UPTO(LOG_NOTICE));
    openlog("files", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
    syslog(LOG_NOTICE, "Program started by User %d", getuid());

    msg = "files PID :  " + to_string(syscall(SYS_gettid));
    lockPrintln(msg);
    //lockPrintln(to_string(BUF_LENGTH));

    i = 0;
    while (active) {
        if ( !queue->empty() ) {
            buffer[i++] = queue->front();
            queue->pop();
            if (i >= FILE_SIZE) {
                writeToFile();
                count++;
                i = 0;
            }
        }
    }
    lockPrintln("AcquisitionFiles finished, " + to_string(count) + " pairs of files created.");
}


void AcquisitionFiles::writeToFile() {
    string trgt = "/home/pi/Documents/DataShare/data/"; // Path
    string filename;
    uint64_t curTime;
    uint64_t i;
    uint32_t j;

    // Get current time
    curTime = std::chrono::duration_cast<std::chrono::seconds>(
                    std::chrono::system_clock::now().time_since_epoch())
                    .count();
    filename = trgt + "Rpi12-" + to_string(curTime) + ".csv";
    ofstream ch12File(filename);
    filename = trgt + "Rpi34-" + to_string(curTime) + ".csv";
    ofstream ch34File(filename);
    i = 0;
    while (i < FILE_SIZE) {
      // first period: 4 channels sampled
      ch12File << to_string(buffer[i++]) << ",";
      ch12File << to_string(buffer[i++]) << endl;
      ch34File << to_string(buffer[i++]) << ",";
      ch34File << to_string(buffer[i++]) << endl;
      // The next 19 periods: 2 channels sampled 
      for (j = 0; j < 19; j++) {
          ch12File << to_string(buffer[i++]) << ",";
          ch12File << to_string(buffer[i++]) << endl;
      }
    }
    ch12File.close();   // Close file when data acquisition is complete
    ch34File.close();   // Close file when data acquisition is complete
}
