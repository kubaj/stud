#include <cstdlib>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <stdio.h>

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
int socketino;

int main(int argc, char *argv[]) {
    using namespace std;

    struct sockaddr_in clientaddr;
    socklen_t addrlen;

    char server_reply[2000];

    ROOT = getenv("PWD");

    string server = "localhost:9876";

    socketino = socket(PF_INET, SOCK_STREAM, 0);
    clientaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    clientaddr.sin_family = AF_INET;
    clientaddr.sin_port = htons(9876);

    if (connect(socketino, (struct sockaddr *)&clientaddr, sizeof(clientaddr)) < 0) {
        perror("connect failed");
    }

    string request = "GET /root/?type=folder HTTP/1.1\r\n\r\n";

    if (send(socketino, request.c_str(), request.length(), 0) < 0) {
        perror("errror sending");
    }

    if (recv(socketino, server_reply, 2000, 0) < 0) {
        perror("error receiving");
    }
    cout << server_reply << endl;

    close(socketino);
    return 0;
}

