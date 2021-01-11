cmake_minimum_required(VERSION 3.10)

#Set up project
project(EndoNeuro VERSION 1.0)

#Set executable
set(SOURCES
	src/main.cpp
	src/collection/acquisition.cpp
	src/collection/acquisitionFiles.cpp
  src/collection/lockingQueue.cpp
	src/collection/transfer.cpp
 	src/collection/utils.cpp
	src/collection/video.cpp
)

add_executable(
	${PROJECT_NAME}
	src/main.cpp
	src/collection/acquisition.cpp
	src/collection/acquisitionFiles.cpp
  src/collection/lockingQueue.cpp
	src/collection/transfer.cpp
 	src/collection/utils.cpp
	src/collection/video.cpp
)

set(HEADERS
	src/collection/acquisition.h
	src/collection/acquisitionFiles.h
  src/collection/lockingQueue.h
	src/collection/transfer.h
 	src/collection/utils.h
  src/collection/video.h
)








#Set C++17
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)

#Set flags
if(MSVC)
  	target_compile_options(${PROJECT_NAME} PRIVATE
    	/W4 /WX)
else()
  	target_compile_options(${PROJECT_NAME} PRIVATE
		-Wall -Wextra -pedantic)
endif()


INCLUDE_DIRECTORIES(;/usr/local/include)
LINK_DIRECTORIES("/usr/local/lib")
INCLUDE_DIRECTORIES(src/libs)
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${pigpio_INCLUDE_DIR})


#Find libraries
find_package(Threads)
find_package(OpenCV)


find_path(pigpio_INCLUDE_DIR 
	NAMES pigpio.h pigpiod_if.h pigpiod_if2.h
	HINTS /usr/local/include)
	
find_library(pigpio_LIBRARY 
	NAMES libpigpio.so
	HINTS /usr/local/lib)
find_library(pigpiod_if_LIBRARY 
	NAMES libpigpiod_if.so
	HINTS /usr/local/lib)
find_library(pigpiod_if2_LIBRARY 
	NAMES libpigpiod_if2.so
	HINTS /usr/local/lib)
    

set(pigpio_INCLUDE_DIRS ${pigpio_INCLUDE_DIR})
set(pigpio_INCLUDES     ${pigpio_INCLUDE_DIR})

#Finally
target_link_libraries(
	${PROJECT_NAME}
	Threads::Threads
	${OpenCV_LIBS}
	pigpio
	pthread
	atomic
	bcm2835
	stdc++fs
)