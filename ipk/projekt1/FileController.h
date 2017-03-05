//
// Created by jakub on 3/4/17.
//

#ifndef PROJEKT1_FILECONTROLLER_H
#define PROJEKT1_FILECONTROLLER_H


#include <evdns.h>
#include "Http.h"

class FileController {
private:
    string serverPath;
public:
    FileController(string serverPath);
    HttpResponse * HandleRequest(HttpRequest request);

};


#endif //PROJEKT1_FILECONTROLLER_H
