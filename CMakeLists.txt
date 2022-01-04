cmake_minimum_required(VERSION 3.20)
project(spl3Client)

set(CMAKE_CXX_STANDARD 11)

add_executable(spl3Client main.cpp include/connectionHandler.h src/connectionHandler.cpp src/echoClient.cpp include/Encoder.h src/Encoder.cpp)
