//
// Created by jakub on 3/4/17.
//

#include <cstring>
#include <iostream>
#include <fstream>
#include <pwd.h>
#include <sys/stat.h>
#include <dirent.h>
#include "FileController.h"

FileController::FileController(string serverPath) {
    this->serverPath = serverPath;
}

HttpResponse *FileController::HandleRequest(HttpRequest request) {
    HttpResponse *response = new HttpResponse();

    string fullPath = serverPath + '/' + request.username + '/' + request.filename;

    if (mkdir((serverPath + '/' + request.username).c_str(), 0777)) {
        cout << "Error creating folder " << (serverPath + '/' + request.username) << endl;
    }

    if (!request.filetype.compare("file")) {
        if (!request.filename.compare("") || !request.filename.compare("/")) {
            response->code = CODE_BAD_REQUEST;
            return response;
        }

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
        if (!request.type.compare("POST") || !request.type.compare("PUT")) {
            int err = mkdir(fullPath.c_str(), 0777);
            if (err) {
                response->code = CODE_NOT_FOUND;
            }
        } else if (!request.type.compare("GET")) {
            DIR *dpdf;
            struct dirent *epdf;

            dpdf = opendir(fullPath.c_str());
            if (dpdf != NULL){
                while ((epdf = readdir(dpdf))) {
                    response->body += (string(epdf->d_name) + "\n");
                }
            }
            response->body = response->body.substr(0, response->body.length() - 3);
        } else if (!request.type.compare("DELETE")) {
            int err = remove((fullPath).c_str());
            if (err != 0) {
                response->code = CODE_NOT_FOUND;
            }
        }
    }

    return response;
}

int FileController::CheckUser(string username) {
    if (getpwnam(username.c_str()) == NULL) {
        return 1;
    }

    return 0;
}

