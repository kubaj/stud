//
// Created by jakub on 3/4/17.
//

#include <cstring>
#include <iostream>
#include <fstream>
#include <pwd.h>
#include "FileController.h"

FileController::FileController(string serverPath) {
    this->serverPath = serverPath;
}

HttpResponse *FileController::HandleRequest(HttpRequest request) {
    HttpResponse *response = new HttpResponse();

    string fullPath = serverPath + request.username + '/' + request.filename;

    if (!request.filetype.compare("file")) {
        if (!request.type.compare("POST") || !request.type.compare("PUT")) {
            ofstream ofs;
            ofs.open(fullPath);
            if (!ofs.is_open()) {
                response->code = CODE_NOT_FOUND;
                return response;
            }

            ofs << request.body;
            ofs.close();
        } else if (!request.type.compare("GET")) {
            ifstream ifs;
            ifs.open(fullPath);
            if (!ifs.is_open()) {
                response->code = CODE_NOT_FOUND;
                return response;
            }
            response->body = string((istreambuf_iterator<char>(ifs)),(istreambuf_iterator<char>()));
        } else if (!request.type.compare("DELETE")) {
            int err = remove((fullPath).c_str());
            if (err != 0) {
                response->code = CODE_NOT_FOUND;
            }
        }

    } else if (!request.filetype.compare("folder")) {

    }

    return response;
}

int FileController::CheckUser(string username) {
    if (getpwnam(username.c_str()) == NULL) {
        return 1;
    }

    return 0;
}

