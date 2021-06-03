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

void Video::start(int fileDurationInSeconds) {
    string msg;
    int frameWidth, frameHeight, framesPerSecond;
    string trgt = "/home/pi/Documents/DataShare/data/"; // Path
    string filename;
    uint64_t curTime;
    Mat frame;
    uint64_t count = 0;
    bool success;


    msg = "video PID :  " + to_string(syscall(SYS_gettid));
    lockPrintln(msg);
    setlogmask(LOG_UPTO(LOG_NOTICE));
    openlog("video", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);

    // Create a VideoCapture object and use camera to capture the video
    VideoCapture cap(0);
    
    // Check if camera opened successfully
    if (!cap.isOpened()) {
        lockPrintln("Error opening video stream");
    } else {
        // get video resolution
        frameWidth = static_cast<int>(cap.get(CAP_PROP_FRAME_WIDTH));
        frameHeight = static_cast<int>(cap.get(CAP_PROP_FRAME_HEIGHT));
        Size frameSize(frameWidth, frameHeight);
        // Set the frame rate at 10fps and read it back for confirmation
        cap.set(CAP_PROP_FPS, 10);
        framesPerSecond = static_cast<int>(cap.get(CAP_PROP_FPS));

        while (active) {
            // Get current time
            curTime = std::chrono::duration_cast<std::chrono::seconds>(
                      std::chrono::system_clock::now().time_since_epoch())
                      .count();
            
            // create the new file name, using current time
            filename = trgt + "RpiVid-" + to_string(curTime) + ".mp4";
            VideoWriter video(filename, VideoWriter::fourcc('m', 'p', '4', 'v'),
                              framesPerSecond, frameSize, true);

            // grab the video
            for (int i=0; i<fileDurationInSeconds*framesPerSecond; i++) {
                success = cap.read(frame);
                // If the frame is empty, break immediately
                if (!success){
                    lockPrintln("empty");
                    continue;
                }
                // Write the frame into the file
                video.write(frame);
            }
            // Close the file
            video.release();
            count++;
        }
        cap.release();
    }
    lockPrintln("Video finished, " + to_string(count) + " files created.");
}
