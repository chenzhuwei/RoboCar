//
// Created by norse on 17-4-17.
//

#ifndef CAR_EYE_CURLMODEL_H
#define CAR_EYE_CURLMODEL_H

#include <map>
#include "string"

using namespace std;

class CurlModel {
public:
    string getData(const char* url);
    string postData(map<string, string>);
};


#endif //CAR_EYE_CURLMODEL_H
