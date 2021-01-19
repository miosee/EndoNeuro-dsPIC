#ifndef VIDEO_H // include guard
#define VIDEO_H


class Video {
    private:
        bool active = true;

    public:
        void start();
        void shutdown();
};


#endif
