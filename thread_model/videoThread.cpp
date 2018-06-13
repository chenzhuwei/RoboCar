//
// Created by norse on 17-4-16.
//
#include <thread>
#include "detcetModel.h"
#include "videoThread.h"
#include "applog.h"
int videoThread::startVideo() {
    if(this->cap_id==666)
        this->cap_devices.open("./road.mp4");
    else
        this->cap_devices.open(this->cap_id);
    loger.printInfo("find capture success");
    this->cap_open = (this->cap_devices.isOpened());
    if (this->cap_open) {
        loger.printSuccess("Load capture");
    } else {
        loger.printError("Can not load capture");
    }
}
/**
 * get video from capture
 *
 * @return
 */
Mat videoThread::getVideo() {
    Mat cap_mat;
    imageController img_controller;
    if (this->cap_open) {
        this->cap_devices>>cap_mat;
        Detcet dec;
        Mat cap_rio_mat;
        Size imageSize = Size(width, hight);
        resize(cap_mat, cap_rio_mat, imageSize);
        dec.detcetBody(cap_rio_mat);
        dec.detcetLines(cap_rio_mat,this->width,this->hight/2);
        return cap_rio_mat;
    }
}
int videoThread::setConfig(map<string, string> config) {
    try {
        this->weather = config["weather_model"];
        this->width = atoi(config["width"].c_str());
        this->hight = atoi(config["hight"].c_str());
        if(config["camera_type"]=="1"){
            this->cap_id = atoi(config["camera"].c_str());
        } else{
            this->cap_id = 666;
        }
    } catch (Exception e) {
        loger.logError("Cap Config Error");
    }

}