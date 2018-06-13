//
// Created by norse on 17-4-17.
//
//if sqltypes.h is error
//      apt-get install unixodbc-dev
#include <sqltypes.h>
#include "weatherApi.h"
//#include "CvText.h"
#include "json/value.h"
Mat weatherApi::printData(const Mat screen, const string weather) {
    CvFont font;
    IplImage ipl_img(screen);
    double hScale = 1;
    double vScale = 1;
    double lineWidth = 2;
    time_t ts;
    tm *local;
    char buf[128] = {0};
    char buf_2[weather.size()] = {0};
    const char *p = buf_2;
    p = weather.c_str();
    ts = time(NULL);
    local = localtime(&ts);
    strftime(buf, 64, "%Y-%m-%d %H:%M:%S", local);
    TCHAR *showMsg = buf;
    TCHAR s_weather[1000];
    //TODO : buffer over follow
    memcpy(s_weather, weather.c_str(), weather.size());
    //CvText text("NotoSansHans-DemiLight.otf");
    const char *msg = weather.c_str();
    //text.putText(&ipl_img, msg, cvPoint(100, 150), CV_RGB(255,0,0));

    cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX | CV_FONT_ITALIC, hScale, vScale, 0, lineWidth);
    cvPutText(&ipl_img, showMsg, cvPoint(25, 25), &font, CV_RGB(255, 0, 0));
    cvPutText(&ipl_img, weather.c_str(), cvPoint(25, 75), &font, CV_RGB(255, 0, 0));
    return cvarrToMat(&ipl_img);
}