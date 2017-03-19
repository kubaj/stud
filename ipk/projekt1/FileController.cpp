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

    mkdir((serverPath + '/' + request.username).c_str(), 0777);

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
            int length = strtol(request.headers.find("Content-Length")->second.c_str(), NULL, 10);
            length -= request.body.length();

            char buffer[99999];

            while (length > 0) {
                recv(request.socket, buffer, 99999, 0);
                ofs << buffer;
                length -= 99999;
            }

            ofs.close();
        } else if (!request.type.compare("GET")) {
            ifstream ifs;
            struct stat s;
            if( stat(fullPath.c_str(),&s) == 0 )
            {
                if( s.st_mode & S_IFDIR )
                {
                    response->code = CODE_NOT_ALLOWED;
                    return response;
                }
            }

            response->headers.insert(
                    make_pair("Filename", string(basename(fullPath.c_str()))));

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

            struct stat s;
            if( stat(fullPath.c_str(),&s) == 0 )
            {
                if( s.st_mode & S_IFREG )
                {
                    response->code = CODE_NOT_ALLOWED;
                    return response;
                }
            }

            dpdf = opendir(fullPath.c_str());
            if (dpdf != NULL){
                while ((epdf = readdir(dpdf))) {
                    response->body += (string(epdf->d_name) + "\t");
                }
            }
            response->body = response->body.substr(0, response->body.length() - 3);
        } else if (!request.type.compare("DELETE")) {

            if (!request.filename.compare("/")) {
                response->code = CODE_BAD_REQUEST;
                return response;
            }
            DIR *dpdf;
            dpdf = opendir(fullPath.c_str());

            int n = 0;
            while (readdir(dpdf) != NULL) {
                if(++n > 2)
                    break;
            }
            if (n > 2) {
                response->code = CODE_NOT_ALLOWED;
                return response;
            }

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

int FileController::HandleResponse(HttpResponse response, string command) {

    if (!response.code.compare("401")) {
        cerr << "User Account Not Found" << endl;
        return 1;
    } else if (!response.code.compare("400")) {
        cerr << "Unknown error." << endl;
        return 1;
    }
    if (!command.compare("del")) {
        if (!response.code.compare("404")) {
            cerr << "File not found." << endl;
            return 1;
        }
    } else if (!command.compare("get")) {

        if (!response.code.compare("404")) {
            cerr << "File not found." << endl;
            return 1;
        } else if (!response.code.compare("405")) {
            cerr << "Not a file." << endl;
            return 1;
        }

        ofstream ofs;

        if (!serverPath.compare("")) {
            serverPath = response.headers.find("Filename")->second;
        }

        ofs.open(serverPath);
        if (!ofs.is_open()) {
            return -1;
        }

        ofs << response.body;
        int length = strtol(response.headers.find("Content-Length")->second.c_str(), NULL, 10);
        length -= response.body.length();

        char buffer[99999];

        while (length > 0) {
            recv(response.socket, buffer, 99999, 0);
            ofs << buffer;
            length -= 99999;
        }

        ofs.close();
    } else if (!command.compare("put")) {
        if (!response.code.compare("404")) {
            cerr << "Already exists." << endl;
            return 1;
        }

    } else if (!command.compare("lst")) {
        if (!response.code.compare("404")) {
            cerr << "Directory not found." << endl;
            return 1;
        } else if (!response.code.compare("405")) {
            cerr << "Not a directory." << endl;
            return 1;
        }
        cout << response.body << endl;

    } else if (!command.compare("mkd")) {
        if (!response.code.compare("404")) {
            cerr << "Already exists." << endl;
            return 1;
        }
    } else if (!command.compare("rmd")) {
        if (!response.code.compare("404")) {
            cerr << "Directory not found." << endl;
            return 1;
        } else if (!response.code.compare("405")) {
            cerr << "Directory not empty." << endl;
            return 1;
        }
    }

    return 0;
}

