//
// Created by jakub on 3/1/17.
//

#ifndef PROJEKT1_HTTP_H
#define PROJEKT1_HTTP_H


#include <ctime>
#include <vector>
#include <string>
#include <map>
#include <fstream>

using namespace std;

const string CODE_NOT_FOUND = "404 Not Found";
const string CODE_OK = "200 OK";
const string CODE_UNAUTHORIZED = "401 Unauthorized";
const string CODE_BAD_REQUEST = "400 Bad Request";
const string CODE_NOT_ALLOWED = "405 Method Not Allowed";

const string TYPE_POST = "POST";
const string TYPE_PUT = "PUT";
const string TYPE_GET = "GET";
const string TYPE_DEL = "DELETE";

const string FILE_TYPE = "file";
const string FOLDER_TYPE = "folder";

class HttpRequest {
private:
    int ParseRoute();
public:
    HttpRequest();
    int ParseRequest(string request);
    string ToString();

    int socket;
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
    int ParseResponse(string response);
    string ToString();

    int socket;
    map<string, string> headers;
    string code;
    string body;
};


#endif //PROJEKT1_HTTP_H
