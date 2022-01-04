
#include "../include/encoder.h"
using boost::asio::ip::tcp;

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::string;
using namespace std;

enum string_opcode{
    register_,login,logout,follow,post,PM,LOGSTAT,STAT,block,null
};

string_opcode hashit(string const& s){
    if(s == "REGISTER") return register_;
    if(s == "LOGIN") return login;
    if(s == "LOGOUT") return logout;
    if(s == "FOLLOW") return follow;
    if(s == "POST") return post;
    if(s == "PM") return PM;
    if(s == "LOGSTAT") return LOGSTAT;
    if(s == "STAT") return STAT;
    if(s == "BLOCK") return block;
    return null;

}

encoder::encoder(ConnectionHandler &connectionHandler_, bool &terminate_, mutex &mtx_, condition_variable &cv_)
:connectionHandler(connectionHandler_) ,terminate(terminate_) ,mtx(mtx), cv(cv_){}

std::string encoder::encodeAndSend(std::string &line) {
    while(!terminate) {
        vector<string> input;
        stringstream ss(line);
        string word;
        while (getline(ss, word, ' '))
            input.push_back(word);
        switch (hashit(input.at(0))) {
            case register_: {
                string username = input.at(1);
                string password = input.at(2);
                const char *usernameB = input.at(1).c_str();
                const char *passwordB = input.at(2).c_str();
                char msg[2 + username.length() + 1 + password.length() + 1];
                int index = 0;
                char opcode[2];
                shortToBytes(1, opcode);
                index = copyIntoMsg(msg, opcode, index, 2);
                index = copyIntoMsg(msg, usernameB, index, username.length());
                msg[index++] = '\0';
                index = copyIntoMsg(msg, passwordB, index, password.length());
                msg[index++] = '\0';
                connectionHandler.sendBytes(msg, index);
                break;
            }
            case login: {
                string username = input.at(1);
                string password = input.at(2);
                const char *usernameB = input.at(1).c_str();
                const char *passwordB = input.at(2).c_str();
                char msg[2 + username.length() + 1 + password.length() + 1 + 1];
                int index = 0;
                char opcode[2];
                shortToBytes(2, opcode);
                index = copyIntoMsg(msg, opcode, index, 2);
                index = copyIntoMsg(msg, usernameB, index, username.length());
                msg[index++] = '\0';
                index = copyIntoMsg(msg, passwordB, index, password.length());
                msg[index++] = '\0';
                const char *captcha = input.at(3).c_str();
                msg[index++] = captcha[0];
                connectionHandler.sendBytes(msg, index);
                break;
            }
            case logout: {
                char msg[2];
                int index = 0;
                char opcode[2];
                shortToBytes(3, opcode);
                index = copyIntoMsg(msg, opcode, index, 2);
                connectionHandler.sendBytes(msg, index);
                unique_lock<mutex> uniqueLock(mtx);
                cv.wait(uniqueLock);
                break;
            }
            case follow: {
                string username = input.at(2);
                const char *usernameB = username.c_str();
                char msg[2 + 1 + username.length()];
                int index = 0;
                char opcode[2];
                shortToBytes(4, opcode);
                index = copyIntoMsg(msg, opcode, index, 2);
                string command = input[1];
                const char *commandB = command.c_str();
                index = copyIntoMsg(msg, commandB, index, 1);
                index = copyIntoMsg(msg, usernameB, index, username.length());
                connectionHandler.sendBytes(msg, index);
                break;
            }
            case post: {
                string content = input.at(1);
                const char *contentB = content.c_str();
                char msg[2 + content.length() + 1];
                int index = 0;
                char opcode[2];
                shortToBytes(5, opcode);
                index = copyIntoMsg(msg, opcode, index, 2);
                index = copyIntoMsg(msg, contentB, index, content.length());
                msg[index++] = '\0';
                connectionHandler.sendBytes(msg, index);
                break;
            }
            case PM: {
                string username = input.at(1);
                string content = input.at(2);
                string date = getTime();
                const char *usernameB = username.c_str();
                const char *contentB = content.c_str();
                const char *dateB = date.c_str();
                char msg[2 + username.length() + 1 + content.length() + 1 + date.length() + 1];
                int index = 0;
                char opcode[2];
                shortToBytes(6, opcode);
                index = copyIntoMsg(msg, opcode, index, 2);
                index = copyIntoMsg(msg, usernameB, index, username.length());
                msg[index++] = '\0';
                index = copyIntoMsg(msg, contentB, index, content.length());
                msg[index++] = '\0';
                index = copyIntoMsg(msg, dateB, index, date.length());
                msg[index++] = '\0';
                connectionHandler.sendBytes(msg, index);
                break;
            }
            case LOGSTAT: {
                char msg[2];
                int index = 0;
                char opcode[2];
                shortToBytes(7, opcode);
                index = copyIntoMsg(msg, opcode, index, 2);
                connectionHandler.sendBytes(msg, index);
                break;
            }
            case STAT: {
                string username = input.at(1);
                const char *usernameB = username.c_str();
                char msg[2 + username.length() + 1];
                int index = 0;
                char opcode[2];
                shortToBytes(8, opcode);
                index = copyIntoMsg(msg, opcode, index, 2);
                index = copyIntoMsg(msg, usernameB, index, username.length());
                msg[index++] = '\0';
                connectionHandler.sendBytes(msg, index);
                break;
            }
            case block: {
                string username = input.at(1);
                const char *usernameB = username.c_str();
                char msg[2 + username.length() + 1];
                int index = 0;
                char opcode[2];
                shortToBytes(12, opcode);
                index = copyIntoMsg(msg, opcode, index, 2);
                index = copyIntoMsg(msg, usernameB, index, username.length());
                msg[index++] = '\0';
                connectionHandler.sendBytes(msg, index);
                break;
            }
            case null:
                break;
        }
    }



}

int encoder::copyIntoMsg(char *msg, const char *toCopy, int index, int length) {
    for(int i =0; i<length; i=i+1){
        msg[index++] = toCopy[i];
    }
}

std::string encoder::getTime() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    string year = to_string(1900+ltm->tm_year);
    string month = to_string(1 + ltm->tm_mon );
    string day = to_string(ltm->tm_mday);
    string date = day+"-"+month+"-"+year;
    return date;
}
