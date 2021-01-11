#include "transfer.h" // header in local directory
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

void Transfer::shutdown() {
    active = false;
}

void Transfer::start() {
    string trgt = "/home/pi/Documents/DataShare/data/"; // Path
    std::string path = "/home/pi/Documents/localData/"; 
    std::ostringstream tmpStr;
    struct stat info;
    //struct stat stat_source;
    // Set Log configuration
    setlogmask(LOG_UPTO(LOG_NOTICE));
    openlog("transfer", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
    syslog(LOG_NOTICE, "Program started by User %d", getuid());
    pid_t tid;
    tid = syscall(SYS_gettid);
    lockPrintln("Transfer PID : " + to_string(tid));

    while (active) {
        for (const auto & entry : fs::directory_iterator(path)) {
            std::string s = entry.path().filename();
            if (s.rfind("Rpi", 0) == 0) {
                string old_name = entry.path().filename().c_str();
                string new_name = trgt + entry.path().filename().c_str();
                if ((stat(old_name.c_str(), &info) == 0) == 1) { // If file is found move it
                    if (int(info.st_size) > 0) { // Send only if file is big than 0
                        int source = open(old_name.c_str(), O_RDONLY, 0);
                        int dest = open(new_name.c_str(), O_WRONLY | O_CREAT /*| O_TRUNC/**/, 0644);
                        sendfile(dest, source, 0, info.st_size);
                        close(source);
                        close(dest);
                        std::future<int> result_future =
                            std::async(remove, old_name.c_str()); // Remove function can take ms we launch it on asyncronous way
                    }

                }
            }
        }
        sleep(1);
    }
}
