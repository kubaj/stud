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

class HttpRequest {
private:
    map<string,string> headers;
    string type;
    string body;
    string route;
public:
    HttpRequest();
    int ParseRequest(string request);
};

class HttpResponse {

};


#endif //PROJEKT1_HTTP_H
