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

char *ROOT;

int sendRequest(HttpRequest request, string address, string port);

int main(int argc, char *argv[]) {
    using namespace std;

    string command = "";
    string remotepath = "";
    string localpath = "";

    if (argc == 3 || argc == 4) {
        command = string(argv[1]);
        remotepath = string(argv[2]);

        if (argc == 4) {
            localpath = string(argv[3]);
        }
    } else {
        cerr << "Wrong number of arguments. Expected format: \nftrest COMMAND REMOTE-PATH [LOCAL-PATH]" << endl;
        exit(1);
    }

    int routePos = IPKUtils::GetAddressPosition(remotepath);

    HttpRequest httpRequest;
    httpRequest.route = remotepath.substr(routePos);

    string address = "";
    string port = "";

    string remoteaddr = remotepath.substr(7, routePos - 7);
    int portPos = remoteaddr.find(":");
    if (portPos == string::npos) {
        port = "80";
        address = remoteaddr;
    } else {
        port = remoteaddr.substr(portPos + 1);
        address = remoteaddr.substr(0, portPos);
    }

    if (!command.compare("del")) {
        httpRequest.type = TYPE_DEL;
        httpRequest.filetype = FILE_TYPE;
    } else if (!command.compare("get")) {
        httpRequest.type = TYPE_GET;
        httpRequest.filetype = FILE_TYPE;

    } else if (!command.compare("put")) {
        httpRequest.type = TYPE_POST;
        httpRequest.filetype = FILE_TYPE;
        ifstream ifs;
        ifs.open(localpath);
        if (!ifs.is_open()) {
            return -1;
        }
        httpRequest.body = string((istreambuf_iterator<char>(ifs)),(istreambuf_iterator<char>()));

    } else if (!command.compare("lst")) {
        httpRequest.type = TYPE_GET;
        httpRequest.filetype = FOLDER_TYPE;

    } else if (!command.compare("mkd")) {
        httpRequest.type = TYPE_POST;
        httpRequest.filetype = FOLDER_TYPE;

    } else if (!command.compare("rmd")) {
        httpRequest.type = TYPE_DEL;
        httpRequest.filetype = FOLDER_TYPE;
    }

    char server_reply[99999];

    int s = sendRequest(httpRequest, address, port);

    if (s < 0) {
        perror("Error " + s);
    }

    if (recv(s, server_reply, 99999, 0) < 0) {
        perror("error receiving");
    } else {

        HttpResponse response;
        response.socket = s;

        int err = response.ParseResponse(string(server_reply));

        if (err) {
            return err;
        }

        FileController fileController(localpath);

        err = fileController.HandleResponse(response, command);

        if (err) {
            return err;
        }
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