#include "video.h" // header in local directory
#include <chrono>
#include <ctime>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <sys/syscall.h>
#include <syslog.h>
#include <unistd.h>

using namespace N;
using namespace std;
using namespace cv;

namespace N {
    void video::start()
    {

        std::ostringstream tmpStr;
        pid_t tid;
        tid = syscall(SYS_gettid);
        cout << "Video PID :  " << tid << endl;
        setlogmask(LOG_UPTO(LOG_NOTICE));

        openlog("video", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);

        // Create a VideoCapture object and use camera to capture the video
        VideoCapture cap(-1);
        // Check if camera opened successfully
        if (!cap.isOpened()) {
            cout << "Error opening video stream" << endl;
        }
        else {

            int frame_width = 640;
            int frame_height = 480;

            uint64_t curTime, lastTime = 0;
            cout << "Capturing video" << endl;
            while (1) {
                tmpStr.str("");
                tmpStr << "Rpi1-";
                curTime = std::chrono::duration_cast<std::chrono::seconds>(
                              std::chrono::system_clock::now().time_since_epoch())
                              .count();
                tmpStr << curTime;
                tmpStr << ".mp4";
                if (lastTime != curTime) {

                    VideoWriter video(tmpStr.str(),
                                      VideoWriter::fourcc('m', 'p', '4', 'v'), 25,
                                      Size(frame_width, frame_height));
                    Mat frame;
                    // Capture frame-by-frame
                    cap >> frame;
                    // If the frame is empty, break immediately
                    if (frame.empty()){
                        cout << "empty" << endl;
                        continue;
                    }

                    // // Write the frame into the file 'outcpp.avi'
                    video.write(frame);

                    // When everything done, release the video capture and write object
                    video.release();
                    lastTime = curTime;
                }
            }
            cap.release();
        }
    }

} // namespace  N