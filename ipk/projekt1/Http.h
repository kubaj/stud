//
// Created by jakub on 3/1/17.
//

#ifndef PROJEKT1_HTTP_H
#define PROJEKT1_HTTP_H


#include <ctime>
#include <vector>
#include <string>
#include <map>

using namespace std;

const string CODE_NOT_FOUND = "404 Not Found";
const string CODE_OK = "200 OK";
const string CODE_UNAUTHORIZED = "401 Unauthorized";
const string CODE_NOTFOUND = "404 Not Found";
const string CODE_BADREQUEST = "400 Bad Request";

class HttpRequest {
private:
    int ParseRoute();
public:
    HttpRequest();
    int ParseRequest(string request);

    map<string,string> headers;
    string type;
    string body;
    string route;
    string filename;
    string username;
    string filetype;
};

class HttpResponse {
public:
    HttpResponse();
    HttpResponse(string code);
    string ToString();

    map<string, string> headers;
    string code;
    string body;
};


#endif //PROJEKT1_HTTP_H
