#include <iostream>
#include <regex>
#include "Http.h"
#include "FileController.h"
#include "IPKUtils.h"
//
// Created by jakub on 3/4/17.
//

int main() {
    using namespace std;

    const char* s = "POST /user/path/asd/asdkjsad/asdsahj/asdjdsa?type=folder HTTP/1.1\r\n"
            "Content-Type: image/jpeg; charset=utf-8\r\n"
            "Content-Length: 19912\r\n\r\nand body";

    cout << IPKUtils::RFC2616DateNow() << endl;


    return 0;
}