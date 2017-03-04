//
// Created by jakub on 3/1/17.
//

#include <iostream>
#include <sstream>
#include <regex>
#include "http.h"

using namespace std;

HttpRequest::HttpRequest() {

}

int HttpRequest::ParseRequest(string request) {
    istringstream iss(request);
    string buff;
    string::size_type index;

    iss >> buff;
    if (!buff.compare("GET") && !buff.compare("POST") && !buff.compare("PUT") &&
        !buff.compare("DELETE")) {
        return 1;
    }

    type = buff;
    iss >> route;

    iss >> buff;
    if (!buff.compare("HTTP/1.1") && !buff.compare("HTTP/1.0")) {
        return 2;
    }

    getline(iss, buff);

    while (getline(iss, buff) && buff != "\r") {
        index = buff.find(':', 0);
        if (index != string::npos) {
            headers.insert(
                    make_pair(buff.substr(0, index), buff.substr(index + 2)));
        }
    }

    for (auto it = headers.cbegin(); it != headers.cend(); ++it) {
        std::cout << it->first << " " << it->second << "\n";
    }

    body = string(iss.str().substr(iss.tellg()));

    cout << body << endl;

    return 0;
}