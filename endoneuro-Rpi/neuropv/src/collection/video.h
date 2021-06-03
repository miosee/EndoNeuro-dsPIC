#ifndef VIDEO_H // include guard
#define VIDEO_H


class Video {
    private:
        bool active = true;

    public:
        void start(int fileDurationInSeconds);
        void shutdown();
};


#endif
