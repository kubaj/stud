#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<fcntl.h>
#include <thread>
#include <iostream>
#include "Http.h"
#include "FileController.h"

#define BYTES 1024

char *ROOT;
int listenfd;

//void error(char *);
void startServer(char *);

void respond(int);

int main(int argc, char *argv[]) {
    using namespace std;

    struct sockaddr_in clientaddr;
    socklen_t addrlen;
    int c;

    //Default Values PATH = ~/ and PORT=10000
    char PORT[6];
    ROOT = getenv("PWD");
    strcpy(PORT, "8814");

    int slot = 0;

    //Parsing the command line arguments
    while ((c = getopt(argc, argv, "p:r:")) != -1)
        switch (c) {
            case 'r':
                ROOT = (char *) malloc(strlen(optarg));
                strcpy(ROOT, optarg);
                break;
            case 'p':
                strcpy(PORT, optarg);
                break;
            case '?':
                fprintf(stderr, "Wrong arguments given!!!\n");
                exit(1);
            default:
                exit(1);
        }

    printf("Server started at port no. %s%s%s with root directory as %s%s%s\n",
           "\033[92m", PORT, "\033[0m", "\033[92m", ROOT, "\033[0m");

    startServer(PORT);

    //thread worker(handleClient, client);

    // ACCEPT connections
    while (1) {
        addrlen = sizeof(clientaddr);
        //clients[slot] =

        int conn = accept(listenfd, (struct sockaddr *) &clientaddr,
                               &addrlen);

        if (conn < 0) {
            perror("accept() error");
            exit(1);
        }

        thread worker(respond, conn);
        worker.detach();
    }

    return 0;
}

//start server
void startServer(char *port) {
    struct addrinfo hints, *res, *p;

    // getaddrinfo for host
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if (getaddrinfo(NULL, port, &hints, &res) != 0) {
        perror("getaddrinfo() error");
        exit(1);
    }
    // socket and bind
    for (p = res; p != NULL; p = p->ai_next) {
        listenfd = socket(p->ai_family, p->ai_socktype, 0);
        if (listenfd == -1) continue;
        if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0) break;
    }
    if (p == NULL) {
        perror("socket() or bind()");
        exit(1);
    }

    freeaddrinfo(res);

    // listen for incoming connections
    if (listen(listenfd, 1000000) != 0) {
        perror("listen() error");
        exit(1);
    }
}

//client connection
void respond(int sock) {
    char mesg[99999], *reqline[3], data_to_send[BYTES], path[99999];
    int fd, bytes_read;
    ssize_t rcvd;

    memset((void *) mesg, (int) '\0', 99999);

    rcvd = recv(sock, mesg, 99999, 0);

    if (rcvd < 0)    // receive error
        fprintf(stderr, ("recv() error\n"));
    else if (rcvd == 0)    // receive socket closed
        fprintf(stderr, "Client disconnected unexpectedly.\n");
    else    // message received
    {
        HttpRequest request;
        int err = request.ParseRequest(string(mesg));

        if (err) {
            cout << err << endl;
            write(sock, "HTTP/1.0 400 Bad Request\n", 25);
        }

        FileController fileController(ROOT);

        HttpResponse *response = fileController.HandleRequest(request);

        if (response == NULL) {
            write(sock, "HTTP/1.0 400 Bad Request\n", 25);
        } else {
            write(sock, response->ToString().c_str(), response->ToString().length());
        }


        if (0) {

            //printf("%s", mesg);
            reqline[0] = strtok(mesg, " \t\n");
            if (strncmp(reqline[0], "GET\0", 4) == 0) {
                reqline[1] = strtok(NULL, " \t");
                reqline[2] = strtok(NULL, " \t\n");
                if (strncmp(reqline[2], "HTTP/1.0", 8) != 0 &&
                    strncmp(reqline[2], "HTTP/1.1", 8) != 0) {

                } else {
                    if (strncmp(reqline[1], "/\0", 2) == 0)
                        reqline[1] = "/index.html";        //Because if no file is specified, index.html will be opened by default (like it happens in APACHE...

                    strcpy(path, ROOT);
                    strcpy(&path[strlen(ROOT)], reqline[1]);
                    printf("file: %s\n", path);

                    if ((fd = open(path, O_RDONLY)) != -1)    //FILE FOUND
                    {
                        send(sock, "HTTP/1.0 200 OK\n\n", 17, 0);
                        while ((bytes_read = read(fd, data_to_send, BYTES)) > 0)
                            write(sock, data_to_send, bytes_read);
                    } else
                        write(sock, "HTTP/1.0 404 Not Found\n",
                              23); //FILE NOT FOUND
                }
            }
        }
    }

    //Closing SOCKET
    shutdown(sock, SHUT_RDWR);
    close(sock);
}