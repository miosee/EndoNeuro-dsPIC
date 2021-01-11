#!/bin/bash

update_linux() {
    sudo apt-get update && sudo apt-get upgrade
}

install_cmake() {
    sudo apt install cmake
}

install_BCM2835() {
    wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.68.tar.gz 
    tar xvfz bcm2835-1.68.tar.gz
    cd bcm2835-1.68
    ./configure
    make
    sudo make install
    cd ..
}

install_openCV() {
    sudo apt-get install libjpeg-dev libpng-dev libtiff-dev
    sudo apt-get install libavcodec-dev libavformat-dev libswscale-dev libv4l-dev
    sudo apt-get install libxvidcore-dev libx264-dev
    sudo apt-get install libgtk-3-dev
    sudo apt-get install libcanberra-gtk*
    sudo apt-get install libatlas-base-dev gfortran

    wget -O opencv.zip https://github.com/opencv/opencv/archive/4.0.0.zip
    wget -O opencv_contrib.zip https://github.com/opencv/opencv_contrib/archive/4.0.0.zip
    unzip opencv.zip
    unzip opencv_contrib.zip
    mv opencv-4.0.0 opencv
    mv opencv_contrib-4.0.0 opencv_contrib

    cd ~/opencv
    mkdir build
    cd build
    cmake -D CMAKE_BUILD_TYPE=RELEASE \
        -D CMAKE_INSTALL_PREFIX=/usr/local \
        -D OPENCV_EXTRA_MODULES_PATH=~/opencv_contrib/modules \
        -D ENABLE_NEON=ON \
        -D ENABLE_VFPV3=ON \
        -D BUILD_TESTS=OFF \
        -D OPENCV_ENABLE_NONFREE=ON \
        -D INSTALL_PYTHON_EXAMPLES=OFF \
        -D BUILD_EXAMPLES=OFF ..

    make -j$(nproc)

    sudo make install
    sudo ldconfig
}

if [ "$1" == "update" ] then
    update_linux
elif [ "$1" == "cmake" ] then
    install_cmake
elif [ "$1" == "BCM2835" ] then
    install_BCM2835
elif [ "$1" == "openCV"] then
    install_openCV 
elif [ "$1" == "all"] then
    update_linux
    install_cmake
    install_BCM2835
    install_openCV 
else
    echo "use update/BCM2835/openCV/all as parameter"
fi