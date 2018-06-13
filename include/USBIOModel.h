//
// Created by norse on 17-4-19.
//

#ifndef CAR_EYE_USBIOMODEL_H
#define CAR_EYE_USBIOMODEL_H

#include "USBIOModel.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <zconf.h>
#include "applog.h"
#include <iostream>
#include "string"
#include "map"
using namespace std;
class USBIOModel {
public:
    int findCOM();
    int sendData(char *data);
    map<string,string> getData();
    int closeCOM();
private:
    struct termios Opt;
    int port=0;
    applog loger;
    map<string,string>  serializeData(char *data);
    int search_find(const char *chr_1,const char *chr_2);
};


#endif //CAR_EYE_USBIOMODEL_H
