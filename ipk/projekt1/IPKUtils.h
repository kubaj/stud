#include <string>
//
// Created by jakub on 3/5/17.
//

#ifndef PROJEKT1_IPKUTILS_H
#define PROJEKT1_IPKUTILS_H

using namespace std;

class IPKUtils {
public:
    static string RFC2616DateNow();

    static int GetAddressPosition(string url);

    static const string EncodeURL(const string &s);

    static const string DecodeURL(const string &str);

};
#endif //PROJEKT1_IPKUTILS_H
