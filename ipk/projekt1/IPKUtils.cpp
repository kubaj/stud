//
// Created by jakub on 3/5/17.
//

#include "IPKUtils.h"
#include <ctime>
#include <sstream>

using namespace std;

string IPKUtils::RFC2616DateNow() {
    struct tm *timeinfo;
    char buffer[30];
    time_t t;
    time(&t);

    timeinfo = gmtime(&t);
    strftime(buffer, 30, "%a, %d %b %Y %H:%M:%S GMT", timeinfo);

    return string(buffer);
}

int IPKUtils::GetAddressPosition(string url) {
    if (url.substr(0, 7).compare("http://") != 0) {
        return 0;
    }

    return (int) url.find("/", 8);
}

inline unsigned char to_hex(unsigned char x) {
    return x + (x > 9 ? ('A' - 10) : '0');
}

const string IPKUtils::EncodeURL(const string &s) {
    ostringstream os;

    for (string::const_iterator ci = s.begin(); ci != s.end(); ++ci) {
        if ((*ci >= 'a' && *ci <= 'z') ||
            (*ci >= 'A' && *ci <= 'Z') ||
            (*ci >= '0' && *ci <= '9')) { // allowed
            os << *ci;
        } else if (*ci == ' ') {
            os << '+';
        } else {
            os << '%' << to_hex(*ci >> 4) << to_hex(*ci % 16);
        }
    }

    return os.str();
}

inline unsigned char from_hex(unsigned char ch) {
    if (ch <= '9' && ch >= '0')
        ch -= '0';
    else if (ch <= 'f' && ch >= 'a')
        ch -= 'a' - 10;
    else if (ch <= 'F' && ch >= 'A')
        ch -= 'A' - 10;
    else
        ch = 0;
    return ch;
}

const string IPKUtils::DecodeURL(const string &str) {
    string result;
    string::size_type i;
    for (i = 0; i < str.size(); ++i) {
        if (str[i] == '+') {
            result += ' ';
        } else if (str[i] == '%' && str.size() > i + 2) {
            const unsigned char ch1 = from_hex(str[i + 1]);
            const unsigned char ch2 = from_hex(str[i + 2]);
            const unsigned char ch = (ch1 << 4) | ch2;
            result += ch;
            i += 2;
        } else {
            result += str[i];
        }
    }
    return result;
}