
#ifndef SPL3CLIENT_ENCODERDECODER_H
#define SPL3CLIENT_ENCODERDECODER_H

#include "connectionHandler.h"
#include <vector>

class encoderDecoder {
public:
    encoderDecoder();
    std::vector<char> encode();
    void decode();

private:
    char encBuf;
    char decBuf;
};


#endif //SPL3CLIENT_ENCODERDECODER_H
