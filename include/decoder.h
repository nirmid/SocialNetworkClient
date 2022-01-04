//
// Created by bentzvii@wincs.cs.bgu.ac.il on 04/01/2022.
//

#ifndef SPL3CLIENT_DECODER_H
#define SPL3CLIENT_DECODER_H


#include "connectionHandler.h"
#include <mutex>
#include <condition_variable>


class decoder {
public:
    decoder(ConnectionHandler& , std::mutex&, bool&, std::condition_variable&);
    ~decoder() = default;
    decoder(const decoder&)= default;
    decoder &operator = (const decoder&) = default;
    void operator()();
    short bytesToShort(char *byteArr);

private:
    ConnectionHandler& handler;
    std::mutex& locker;
    bool& shouldTerminate;
    std::condition_variable & conVar;

};


#endif //SPL3CLIENT_DECODER_H
