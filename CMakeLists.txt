cmake_minimum_required(VERSION 3.20)
project(spl3Client)

set(CMAKE_CXX_STANDARD 11)

add_executable(spl3Client include/connectionHandler.h src/connectionHandler.cpp src/echoClient.cpp include/Encoder.h src/Encoder.cpp include/decoder.h src/decoder.cpp)
cmake_minimum_required(VERSION 3.20)
project(spl3Client)

set(BOOST_ROOT "C:Boost")
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME off)
find_package(Boost REQUIRED COMPONENTS system)

target_include_directories(spl3Client PUBLIC ${Boost_INCLUDE_DIRD})
target_link_libraries(spl3Client ${Boost_LIBRARIES})

SET(LDFLAGS "-lboost_system -lboost_thread -pthread")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LDFLAGS}")
