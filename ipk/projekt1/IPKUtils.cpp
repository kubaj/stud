//
// Created by jakub on 3/5/17.
//

#include "IPKUtils.h"
#include <ctime>

string IPKUtils::RFC2616DateNow() {
    struct tm *timeinfo;
    char buffer[30];
    time_t t;
    time(&t);

    timeinfo = gmtime(&t);
    strftime(buffer, 30, "%a, %d %b %Y %H:%M:%S GMT", timeinfo);

    return string(buffer);
}
