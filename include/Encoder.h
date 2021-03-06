
#ifndef SPL3CLIENT_ENCODER_H
#define SPL3CLIENT_ENCODER_H
#include <string>
#include <iostream>
#include <boost/asio.hpp>
#include <mutex>
#include <condition_variable>
#include "connectionHandler.h"

using namespace std;

class Encoder {
private:
    ConnectionHandler &connectionHandler;
    bool& terminate;
    mutex& mtx;
    condition_variable& cv;
    std::string getTime();
    void shortToBytes(short num, char* bytesArr);
    int copyIntoMsg(char *msg, const char *toCopy, int index, int length);

public:
    Encoder(ConnectionHandler& connectionHandler_, bool& terminate_, mutex& mtx_, condition_variable& cv_);
    void operator()();





};


#endif //SPL3CLIENT_ENCODER_H
