//
// Created by jakub on 3/1/17.
//

#include <iostream>
#include <sstream>
#include <regex>
#include "Http.h"
#include "IPKUtils.h"

using namespace std;

HttpRequest::HttpRequest() {

}

int HttpRequest::ParseRequest(string request) {
        istringstream iss(request);
        string buff;
        string::size_type index;

        iss >> buff;
        if (!buff.compare("GET") && !buff.compare("PUT") &&
            !buff.compare("DELETE") && !buff.compare("POST")) {
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
                string p1 = buff.substr(0, index);
                headers.insert(
                        make_pair(p1, buff.substr(index + 2, buff.length() - p1.length() - 3)));
            }
        }

        body = string(iss.str().substr(iss.tellg()));

    return ParseRoute();
}

int HttpRequest::ParseRoute() {

    route = IPKUtils::DecodeURL(route);

    regex formatreg(R"rgx(/[a-zA-Z0-9]+/.*\?type=(file|folder)$)rgx");
    if (!regex_match(route, formatreg)) {
        return 3;
    }

    regex rmuser(R"rgx(^/[a-zA-Z0-9]+/)rgx");
    regex rmparams(R"rgx(\?.+)rgx");
    regex rmuserfile(R"rgx(.+\?type=)rgx");

    filename = regex_replace(route, rmuser, "/");
    filename = regex_replace(filename, rmparams, "");
    filename = filename;

    filetype = regex_replace(route, rmuserfile, "");

    smatch user;
    regex_search(route, user, rmuser);

    username = user[0];
    username = username.substr(1, username.length() - 2);

    return 0;
}

string HttpRequest::ToString() {
    string requestString = type + " " + IPKUtils::EncodeURL(route) + "?type=" + filetype + " HTTP/1.1\r\n";
    headers.insert(make_pair("Date", IPKUtils::RFC2616DateNow()));
    headers.insert(make_pair("Accept-Encoding", "identity"));
    headers.insert(make_pair("Accept", "application/octet-stream"));

    if ((!type.compare("POST") || !type.compare("PUT")) && !body.compare("")) {
        headers.insert(make_pair("Content-Length", to_string(body.length())));
        headers.insert(make_pair("Content-Type", "application/octet-stream"));
    }

    // Cycle headers:
    for (auto it = headers.cbegin(); it != headers.cend(); ++it) {
        requestString += (it->first + ": " + it->second + "\r\n");
    }

    requestString += ("\r\n" + body);

    return requestString;
}

HttpResponse::HttpResponse() {
    code = CODE_OK;
    body = "";
}

HttpResponse::HttpResponse(string code) {
    this->code = code;
    body = "";
}

string HttpResponse::ToString() {
    string responseString = "HTTP/1.0 " + code + "\r\n";
    headers.insert(make_pair("Date", IPKUtils::RFC2616DateNow()));
    headers.insert(make_pair("Accept-Encoding", "identity"));

    if (body.compare("")) {
        headers.insert(make_pair("Content-Length", to_string(body.length())));
        headers.insert(make_pair("Content-Type", "application/octet-stream"));
    }

    // Cycle headers:
    for (auto it = headers.cbegin(); it != headers.cend(); ++it) {
        responseString += (it->first + ": " + it->second + "\r\n");
    }

    responseString += ("\r\n" + body);

    return responseString;
}

int HttpResponse::ParseResponse(string response) {

    istringstream iss(response);
    string buff;
    string::size_type index;

    iss >> buff; // HTTP/1.0
    iss >> buff;

    code = buff;

    getline(iss, buff);

    while (getline(iss, buff) && buff != "\r") {
        index = buff.find(':', 0);
        if (index != string::npos) {
            string p1 = buff.substr(0, index);
            headers.insert(
                    make_pair(p1, buff.substr(index + 2, buff.length() - p1.length() - 3)));
        }
    }

    body = string(iss.str().substr(iss.tellg()));

    return 0;
}
