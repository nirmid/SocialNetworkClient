#include <stdlib.h>
#include <thread>
#include "../include/connectionHandler.h"
#include "../include/Encoder.h"
#include "../include/decoder.h"

/**
* This code assumes that the server replies the exact text the client sent it (as opposed to the practical session example)
*/
int main (int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " host port" << std::endl << std::endl;
        return -1;
    }
    std::string host = argv[1];
    short port = atoi(argv[2]);
    
    ConnectionHandler connectionHandler(host, port);
    if (!connectionHandler.connect()) {
        std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
        return 1;
    }


    bool terminate = false;
    mutex mtx;
    condition_variable cv;
    Encoder encoder(connectionHandler,terminate,mtx,cv);
    decoder dec(connectionHandler,mtx,terminate,cv);
    std::thread inputFromClient(std::ref(encoder));
    std::thread decodeFromServer(std::ref(dec));

    inputFromClient.join();
    //decodeFromServer.join();
    dec.operator()();

    return 0;
}


