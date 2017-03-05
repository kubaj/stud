//
// Created by jakub on 3/4/17.
//

#include <cstring>
#include <iostream>
#include <fstream>
#include "FileController.h"

FileController::FileController(string serverPath) {
    this->serverPath = serverPath;
}

HttpResponse *FileController::HandleRequest(HttpRequest request) {
    HttpResponse *response = new(HttpResponse);

    if (!request.filetype.compare("file")) {
        if (!request.type.compare("POST") || !request.type.compare("PUT")) {
            ofstream ofs;
            ofs.open(serverPath + request.filename);
            if (!ofs.is_open()) {
                return NULL;
            }

            ofs << request.body;
            ofs.close();
        } else if (!request.type.compare("GET")) {
            ifstream ifs;
            ifs.open(serverPath + request.filename);
            if (!ifs.is_open()) {
                response->code = CODE_NOT_FOUND;
                return response;
            }
            response->body = string((istreambuf_iterator<char>(ifs)),(istreambuf_iterator<char>()));
        } else if (!request.type.compare("DELETE")) {
            int err = remove((serverPath + request.filename).c_str());
            if (err != 0) {
                response->code = CODE_NOT_FOUND;
            }
        }

    } else if (!request.filetype.compare("folder")) {

    }

    return response;
}
