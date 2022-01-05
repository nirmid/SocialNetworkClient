//
// Created by bentzvii@wincs.cs.bgu.ac.il on 04/01/2022.
//

#include "../include/decoder.h"


using namespace std;


decoder::decoder( ConnectionHandler &_handler, std:: mutex & _lock,bool & _shouldTerminate, std::condition_variable & _conVar): handler(_handler), locker(_lock), shouldTerminate(_shouldTerminate) , conVar(_conVar) {}

void decoder::operator()() {
    while (!shouldTerminate){
        cout << "entered decode"<< endl;
        char firstBytes[2];
        short opCode = -1;
        short messageOpCode = -1;
        if(handler.getBytes(firstBytes,2))
            opCode = bytesToShort(firstBytes);
        cout << "received opCode"<< endl;
        string status = "-1";
        string content = "";
        string checker = "";
        if (opCode == 10){
            cout << "entered ACK message"<< endl;
            status = "ACK";
            if(handler.getBytes(firstBytes,2))
                messageOpCode = bytesToShort(firstBytes);
            if (messageOpCode == 3){
                unique_lock<mutex> lock(locker);
                shouldTerminate = true;
                conVar.notify_one();
                handler.getLine(checker);
            }
            else if( messageOpCode == 7 || messageOpCode == 8) {
                char check[1];
                char statBytes[8];
                if (handler.getBytes(statBytes, 8)) {
                    for (int i = 0; i < 8; i = i + 2) {
                        short stat = bytesToShort(&statBytes[i]);
                        content = content + " " + to_string(stat);
                    }
                    content = content + "\n";
                }
                do {
                    if (handler.getBytes(check, 1) && check[0] == '\0') {
                        char nextStat[12];
                        if (handler.getBytes(nextStat, 12)) {
                            content = content + "ACk ";
                            for (int i = 2; i < 12; i = i + 2) {
                                short stat = bytesToShort(&statBytes[i]);
                                content = content + " " + to_string(stat);
                            }
                            content = content + "\n";
                        }
                    }
                } while (check[0] == '\0');
                handler.getLine(checker);
            }
            else{
                handler.getLine(content);
            }
            cout << status << " " << messageOpCode << content << endl;

        }
        else if(opCode == 9){
            status = "NOTIFICATION";
            if(handler.getBytes(firstBytes,1))
                messageOpCode =  (short)(firstBytes[0] & 0xff);
            if (messageOpCode == 0)
                content = " PM";
            else if (messageOpCode == 1)
                content = " post";
            string msg = "";
            handler.getLine(msg);
            int start = 0;
            int end = msg.find('\0');
            while (end != -1){
                content = content + " " + msg.substr(start,end - start);
                start = end + 1;
                end = msg.find('\0', start);
            }
            cout << status << " " << content << endl;
        }
        else if (opCode == 11){
            status = "ERROR";
            if(handler.getBytes(firstBytes,2))
                messageOpCode = bytesToShort(firstBytes);
            if (messageOpCode == 3)
                conVar.notify_one();
            cout << status << " " << messageOpCode << endl;
            handler.getLine(content);

        }
        else
            cout << "something went wrong"<< endl;
    }
}

short decoder:: bytesToShort(char* bytesArr)
{
    short result = (short)((bytesArr[0] & 0xff) << 8);
    result += (short)(bytesArr[1] & 0xff);
    return result;
}


