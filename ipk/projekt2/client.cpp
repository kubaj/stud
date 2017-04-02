#include <cstdlib>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include <regex>
#include <netdb.h>
#include "Http.h"
#include "IPKUtils.h"
#include "FileController.h"

#define BYTES 1024

const short port = 55555;
const string xlogin = "xkulic03";

/*
 *               ,_---~~~~~----._
 *         _,,_,*^____      _____``*g*\"*,
 *        / __/ /'     ^.  /      \ ^@q   f
 *       [  @f | @))    |  | @))   l  0 _/
 *        \`/   \~_____/ __ \_____/    \
 *         |           _l__l_           I
 *         }          [______]          I    GO FTW!
 *         ]            | | |           |
 *         ]             ~ ~            |
 *         |                            |
 *          |                          |
 */

int connect_to(string ip);
int send_hello(int sock, string login);
int read_request(int sock);

int sendRequest(HttpRequest request, string address, string port);

int main(int argc, char *argv[]) {
    using namespace std;

    string server_ip = "";

    if (argc == 2) {
        server_ip = string(argv[1]);
    } else {
        cerr << "Wrong number of arguments. Expected format: \nftrest COMMAND REMOTE-PATH [LOCAL-PATH]" << endl;
        exit(1);
    }

    int socket = connect_to(server_ip);
    if (socket < 0) {
        // TODO exit
    }

    int err = send_hello(socket, xlogin);

    bool sessionEnd = false;
    while (!sessionEnd) {
        // wait for server request, parse, send response or finish session
    }

    char server_reply[99999];

    if (recv(s, server_reply, 99999, 0) < 0) {
        perror("error receiving");
    }

    return 0;
}

int sendRequest(HttpRequest request, string address, string port) {

    struct sockaddr_in serveraddr;

    short p = (short)stoul(port);

    struct hostent *host = gethostbyname(address.c_str());
    memcpy(&serveraddr.sin_addr, host->h_addr, host->h_length);

    int socketino = socket(PF_INET, SOCK_STREAM, 0);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(p);

    if (connect(socketino, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0) {
        return -1;
    }

    if (send(socketino, request.ToString().c_str(), request.ToString().length(), 0) < 0) {
        close(socketino);
        return -2;
    }

    return socketino;
}

int connect_to(string ip) {

    struct sockaddr_in serveraddr;
    int socketino = socket(PF_INET, SOCK_STREAM, 0);

    // TODO SET IP
    // serveraddr.sin_addr.s_addr = ip;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(port);

    if (connect(socketino, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0) {
        return -1;
    }

    return socketino;
}

int send_hello(int sock, string login) {
    // TODO: hash
    string hello = "HELLO " + login  + "\n";
    return send(sock, hello.c_str(), hello.length(), 0);
}

int read_request(int sock) {
    char server_reply[99999];

    if (recv(sock, server_reply, 99999, 0) < 0) {
        perror("error receiving");
    }

    string request = string(server_reply);

    // TODO: check regexes
    regex byerequest(R"rgx(BYE [a-e0-9]+)rgx");
    regex solverequest(R"rgx(SOLVE \d+ [+-*/] \d+)rgx");

    if (regex_match(request, byerequest)) {

    } else if (regex_match(request, solverequest)) {

    } else {
        // error
    }
}

