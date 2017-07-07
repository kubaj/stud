#include <cstdlib>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <regex>
#include <iomanip>
#include <cmath>
#include "md5.h"

using namespace std;

const string port = "55555";
const string error_string = "RESULT ERROR\n";

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

int send_solve_response(int sock, string n1, string op, string n2);

int send_response(int sock, string message);


int main(int argc, char *argv[]) {


    string server_ip = "";

    if (argc == 2) {
        server_ip = string(argv[1]);
    } else {
        cerr
                << "Wrong number of arguments. Expected format: \nipk-client IP-ADDRESS"
                << endl;
        exit(1);
    }

    int socket = connect_to(server_ip);
    if (socket < 0) {
        cerr << "* Sad trumpet sound *" << endl << "Cannot connect to the server." << endl;
        exit(1);
    }

    string xlogin = md5("xkulic03");
    send_hello(socket, xlogin);

    int err;

    while (!(err = read_request(socket)));

    close(socket);

    if (err < 0) {
        cerr << "error happened" << endl;
        exit(1);
    }

    return 0;
}


int connect_to(string ip) {

    int status;
    struct addrinfo hints;
    struct addrinfo *res;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(ip.c_str(), port.c_str(), &hints, &res)) != 0) {
        return -1;
    }

    int clientSocket = socket(res->ai_family, SOCK_STREAM, 0);
    if (clientSocket < 0)
    {
        return -1;
    }

    if (connect(clientSocket, res->ai_addr, res->ai_addrlen) < 0)
    {
        return -1;
    }

    return clientSocket;
}

int send_hello(int sock, string login) {
    string hello = "HELLO " + login + "\n";
    return send_response(sock, hello);
}

int read_request(int sock) {
    char server_reply[99999];

    if (recv(sock, server_reply, 99999, 0) < 0) {
        perror("error receiving");
    }

    string request = string(server_reply);

    string n1 = "";
    string n2 = "";
    string op = "";

    if (!request.substr(0, 3).compare("BYE")) {
      string forprint = "";
        for (auto c : request.substr(4)) {
          if (c == '\n')
          {
            break;
          }
          forprint += c; 
        }

        cout << forprint;
        return 1;
    } else if (!request.substr(0, 5).compare("SOLVE")) {
        int status = -1;
        for (auto c : request) {

            if (c == ' ') {
                status += 1;
            } else {
                if (status == 0) {
                    n1 += c;
                } else if (status == 1) {
                    op += c;
                } else if (status == 2) {
                    n2 += c;
                }
            }
        }

        return send_solve_response(sock, n1, op, n2);
    } else {
        return 0;
    }
}

int send_solve_response(int sock, string n1, string op, string n2) {


    long long pn1;
    long long pn2;

    try {
        pn1 = stoll(n1);
    } catch (exception e) {
        send_response(sock, error_string);
        return 0;
    }

    try {
        pn2 = stoll(n2);
    } catch (exception e) {
        send_response(sock, error_string);
        return 0;
    }

    double result = 0;

    if (!op.compare("+")) {
        result = pn1 + pn2;

        if (pn1 > 0 && pn2 > 0 && result < 0) {
            send_response(sock, error_string);
            return 0;
        }

        if (pn1 < 0 && pn2 < 0 && result > 0) {
            send_response(sock, error_string);
            return 0;
        }
    } else if (!op.compare("-")) {
        result = pn1 - pn2;

        if (pn1 < 0 && pn2 > 0 && result > 0) {
            send_response(sock, error_string);
            return 0;
        }
    } else if (!op.compare("*")) {
        result = pn1 * pn2;

        if (pn1 > 0 && pn2 > 0 && result < 0) {
            send_response(sock, error_string);
            return 0;
        }

        if (pn1 < 0 && pn2 < 0 && result > 0) {
            send_response(sock, error_string);
            return 0;
        }
    } else if (!op.compare("/")) {
        if (pn2 == 0) {
            send_response(sock, error_string);
            return 0;
        }

        result = (double) pn1 / pn2;
    }

    stringstream stream;
    stream << fixed << setprecision(2) << floor(result * 100) / 100;
    string result_string = stream.str();

    send_response(sock, "RESULT " + result_string + "\n");

    return 0;
}

int send_response(int sock, string message) {
    return send(sock, message.c_str(), message.length(), 0);
}
