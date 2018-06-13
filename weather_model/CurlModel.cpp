//
// Created by norse on 17-4-17.
//

#include <iostream>
#include "CurlModel.h"
#include "curl/curl.h"
#include <fstream>
#include <sstream>
#include <iostream>

#include <stdlib.h>

string CurlModel::getData(const char* url) {
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    FILE *fp;
    fp = fopen("temp/data", "w+");
    ifstream is;
    if (curl != NULL) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    ostringstream buf;
    fseek(fp, 0, SEEK_END);
    int lSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char *szBuf = new char[lSize + 1];
    fread(szBuf, 1, lSize, fp);
    fclose(fp);
    szBuf[lSize] = 0;
    string str_native_json = szBuf;
    delete szBuf;
    return str_native_json;
}

string CurlModel::postData(map<string, string>) {

}