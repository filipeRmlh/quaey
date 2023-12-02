cmake_minimum_required(VERSION 3.25)
project(quaey)

set(CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

include_directories(.)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package( Threads REQUIRED )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(quaey quaey.cpp
        canny/nonMaximumSuppression.h
        canny/canny.h
        utils/accessUtils.h
        canny/hysteresisThresholding.h
        canny/sobel.h
        quadrangleFormation/houghLines/houghLines.h
        utils/drawUtils.h
        utils/colorConverter.h
)

target_link_libraries(quaey PRIVATE Threads::Threads ${OpenCV_LIBS})

