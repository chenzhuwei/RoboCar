//
// Created by Summer-V on 17-6-8.
//

#include <cstring>
#include <iostream>
#include <fstream>
#include "conf.h"
#include "applog.h"
using namespace std;

Conf::Conf() {
    //TODO: get|load|set the car.conf
    loger.logInfo("------------------Load Config------------------\n");
    string defaultConfig[] = {
            "weatherapi=http://www.weather_model.com.cn/data/cityinfo/101270101.html",
            "facecascade=./haarcascades/haarcascade_frontalface_alt.xml",
            "eyecascade=./haarcascades/haarcascade_eye_tree_eyeglasses.xml",
            "bodycascade=./haarcascades/haarcascade_fullbody.xml",
            "camera1=0",
            "camera2=1",
            "width=640",
            "hight=480"
    };
    for (string x:defaultConfig) {
        this->defaultConfig.push_front(x);
    }
    fstream _file;
    _file.open(CONFIG_FILE_PATH, ios::in);
    if (!_file) {
        loger.printInfo("Config file is not exits");
        _file.close();
        fstream fout(CONFIG_FILE_PATH, ios::out);
        if (fout) {
            for (auto i:defaultConfig) {
                fout <<i+"\n";
            }
            fout.close();
        loger.printInfo("Config file had been create");
        }
    }
}

int Conf::reConfig() {
    fstream fout(CONFIG_FILE_PATH, ios::binary);
    fout.seekg(0);
    if (fout) {
        for (auto i:this->defaultConfig) {
            const char*  content=i.data();
            fout << content<<'\n';
        }
        loger.printInfo("rewrite the config_model file success");
        return 1;
    } else return 0;
}
    int Conf::load() {
        realpath(filename, this->filePath);
        fstream loadFile;
        loadFile.open(this->filePath,ios::in);
        if(loadFile) {
            loadFile.seekg(0, ios::end);
            ios::pos_type ss = loadFile.tellg();
            int nFilesize = (int) ss;
            //cout<<"File Size:"<<nFilesize<<endl;
            if (ss == 0) {
                loger.printInfo("Load error File is distoried");
                return -1;
            }
            loadFile.seekg(0);
            while ((!loadFile.eof())) {
                char buff[DEFAULT_BUFFER_SIZE];
                char buff_2[DEFAULT_BUFFER_SIZE];
                loadFile >> buff;
                char *key_c = this->configFindKey(buff, DEFAULT_BUFFER_SIZE);
                char *value_c = this->cinfigProfileFind(buff, DEFAULT_BUFFER_SIZE, key_c);
                string value = value_c;
                string key = key_c;
                string info = key + "------------->" + value;
                loger.printInfo(info);
                if (realpath(value_c, buff_2))
                    this->ConfigInfo.insert(pair<string, string>(key, (string) buff_2));
                else this->ConfigInfo.insert((pair<string, string>(key, value)));
            }
            loadFile.close();
            loger.printSuccess("Config ready loading");
            return 1;
        } else return -1;
    }
    int Conf::configFind(char *buffer, unsigned long len, char *keyword) {
        for (int i = 0; i < len; i++) {
            char *buffer2 = buffer + i;
            bool key = strncmp(buffer2, keyword, strlen(keyword)) == 0;
            if (key) {
                return i;
            }
        }
        return -1;
    }

    char *Conf::configFindKey(char *buffer, unsigned long buflen) {
        char *out_buffer=new char [buflen];
        //out_buffer={'\0'};
        int i=configFind(buffer,DEFAULT_BUFFER_SIZE,"=");
        memcpy(out_buffer, buffer, i);
        return out_buffer;
        return (char *) "";
    }

    int Conf::cinfigFindInt(char *buffer, unsigned long buflen, char *keyword) {
        int firstpos = this->configFind(buffer, buflen, keyword);
        if (firstpos == -1)
            return -65535;
        firstpos += strlen(keyword);
        unsigned long kwlen = this->configFind((buffer + firstpos), buflen - firstpos, (char *) "\n");
        if (kwlen == -1)
            kwlen = buflen - firstpos;
        char *val1 = new char[kwlen];
        memcpy(val1, (buffer + firstpos), kwlen);
        int ivalue = atol(val1);
        delete[]val1;
        return ivalue;
    }
    char *Conf::cinfigProfileFind(char *buffer, unsigned long buflen, char *keyword) {
        int firstpos = this->configFind(buffer, buflen, keyword);
        if (firstpos == -1)
            return (char *) "";
        firstpos += strlen(keyword)+1;
        unsigned long kwlen = this->configFind((buffer + firstpos), buflen - firstpos, (char *) "\r");
        if (kwlen == -1)
            kwlen = buflen - firstpos;
        char *val1 = new char[kwlen + 1];
        memcpy(val1, (buffer + firstpos), kwlen);
        val1[kwlen] = '\0';
        return val1;

    }