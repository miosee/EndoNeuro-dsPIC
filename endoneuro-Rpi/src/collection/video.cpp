#include "video.h" // header in local directory
#include "utils.h"
#include <chrono>
#include <ctime>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <sys/syscall.h>
#include <syslog.h>
#include <unistd.h>


using namespace std;
using namespace cv;


void Video::shutdown() {
    active = false;
}

void Video::start() {
    string msg;
    int frame_width = 640;
    int frame_height = 480;
    string trgt = "/home/pi/Documents/DataShare/data/"; // Path
    string filename;
    uint64_t curTime, lastTime=0;
    Mat frame;
    uint64_t count = 0;


    msg = "video PID :  " + to_string(syscall(SYS_gettid));
    lockPrintln(msg);

    setlogmask(LOG_UPTO(LOG_NOTICE));

    openlog("video", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);

    // Create a VideoCapture object and use camera to capture the video
    VideoCapture cap(-1);
    // Check if camera opened successfully
    if (!cap.isOpened()) {
        lockPrintln("Error opening video stream");
    } else {
        while (active) {
            // Get current time
            curTime = std::chrono::duration_cast<std::chrono::seconds>(
                      std::chrono::system_clock::now().time_since_epoch())
                      .count();
            if (lastTime != curTime) {
                filename = trgt + "RpiVid-" + to_string(curTime) + ".mp4";
                VideoWriter video(filename,
                                  VideoWriter::fourcc('m', 'p', '4', 'v'), 25,
                                  Size(frame_width, frame_height));
                // Capture frame-by-frame
                cap >> frame;
                // If the frame is empty, break immediately
                if (frame.empty()){
                    lockPrintln("empty");
                    continue;
                }
                // Write the frame into the file
                video.write(frame);
                // Close the file
                video.release();
                count++;
                lastTime = curTime;
            }
        }
        cap.release();
    }
    lockPrintln("Video finished, " + to_string(count) + " files created.");
}
