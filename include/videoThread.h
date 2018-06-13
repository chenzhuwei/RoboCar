//
// Created by norse on 17-4-16.
//

#ifndef CAR_EYE_VIDEOTHREAD_H
#define CAR_EYE_VIDEOTHREAD_H

#include "imageController.h"
#include <iostream>
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "map"
#include "applog.h"

using namespace std;
using namespace cv;

class videoThread {
public:
    int startVideo();
    Mat getVideo();
    int setConfig(map<string,string> config);
private:
    int width,hight,cap_id;
    string weather;
    bool cap_open;
    applog loger;
    VideoCapture cap_devices;

    static void *cap_thread(void *object);
};

#endif //CAR_EYE_VIDEOTHREAD_H
