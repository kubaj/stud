#include <cstdlib>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include <regex>
#include <netdb.h>
#include <iomanip>

using namespace std;

const short port = 55555;
const string xlogin = "b8c98194ac89ad057a612ce0168e931f";
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
                << "Wrong number of arguments. Expected format: \nftrest COMMAND REMOTE-PATH [LOCAL-PATH]"
                << endl;
        exit(1);
    }

    int socket = connect_to(server_ip);
    if (socket < 0) {
        // TODO exit
    }

    int err = send_hello(socket, xlogin);

    while (!(err = read_request(socket)));

    close(socket);
    cout << err << endl;

    return 0;
}


int connect_to(string ip) {

    struct sockaddr_in serveraddr;
    int socketino = socket(PF_INET, SOCK_STREAM, 0);

    serveraddr.sin_addr.s_addr = inet_addr(ip.c_str());
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(port);

    if (connect(socketino, (struct sockaddr *) &serveraddr,
                sizeof(serveraddr)) < 0) {
        return -1;
    }

    return socketino;
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

    regex byerequest(R"rgx(BYE [:xdigit:]+\n)rgx");
    regex solverequest(R"rgx(SOLVE \d+ [-\+\*\/] \d+\n)rgx");

    string n1 = "";
    string n2 = "";
    string op = "";

    cout << request << endl;
    if (regex_match(request, byerequest)) {
        cout << request.substr(6, request.length() - 7) << endl;
        return 1;
    } else if (regex_match(request, solverequest)) {
        int status = -1;
        for (auto c : request) {

            cout << (int) c << endl;

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
//        for (auto c : request) {
//
//            cout << (int) c << endl;
//        }
//        send_response(sock, error_string);
        return 0;
    }
}

int send_solve_response(int sock, string n1, string op, string n2) {


    long long pn1;
    long long pn2;

    cout << n1 << " " << op << " " <<  n2 << endl;

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

    cout << pn1 << " " << pn2 << endl;
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

        result = pn1 / pn2;
    }

    stringstream stream;
    stream << fixed << setprecision(2) << result;
    string result_string = stream.str();

    send_response(sock, "RESULT " + result_string + "\n");

    return 0;
}

int send_response(int sock, string message) {
    cout << message << endl;
    return send(sock, message.c_str(), message.length(), 0);
}