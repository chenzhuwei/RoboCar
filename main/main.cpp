//
// Created by Summer-V on 17-6-8.
//
#include <iostream>
#include <mainWindow.h>
#include <gtk/gtk.h>
#include <fstream>
#include <json/writer.h>
#include "opencv2/imgproc.hpp"
//#include "ekho.h"
#include "CurlModel.h"
#include "conf.h"
#include "applog.h"
#include "json/value.h"
#include "json/reader.h"

using namespace std;
using namespace cv;

int main(int argc, char *argv[]) {
    //TODO: init the system
    string type,state;
    int model=0,net=0;

    if (argc > 1) {
        type = string(argv[1]);
    }
    int ch;
    opterr = 0; //选项错误时不让报错
    if (argc > 1) {
        while ((ch = getopt(argc, argv, "t:i:")) != -1) {
            switch (ch) {
                case 't':
                    model=atoi(optarg);
                    break;
                case 'i':
                    net=atoi(optarg);
                    break;
            }
        }

    }
    gtk_init(&argc, &argv);
    CurlModel curl;
    Conf config;
    applog loger;
    if (1) {
        config.ConfigInfo.insert(
                pair<string, string>("weatherapi", "http://www.weather.com.cn/data/sk/101270101.html"));
        if(model==0)
            config.ConfigInfo.insert(pair<string, string>("camera_type", "1"));
        else
            config.ConfigInfo.insert(pair<string, string>("camera_type", "0"));
        config.ConfigInfo.insert(pair<string, string>("camera", "0"));

        config.ConfigInfo.insert(pair<string, string>("hight", "480"));
        config.ConfigInfo.insert(pair<string, string>("width", "640"));
        const char *url = config.ConfigInfo["weatherapi"].c_str();
        string weather ="No Internet";
        if(net==1) {
            weather = curl.getData(url);
            loger.printSuccess("Load Config");
            Json::Reader reader;
            Json::Value root;
            weather.pop_back();
            weather = weather.substr(15);
            string city="";
            string temp_1="";
            string temp_2="";
            string weather_now="";
            if (reader.parse(weather, root));
            {
                city = root["city"].asString();
                temp_1 = root["temp"].asString();
                temp_2 = root["WD"].asString();
                weather_now = root["njd"].asString();
            }
            weather = city + " " + temp_1 + "~" + temp_2 + " " + weather_now;
        }
        config.ConfigInfo.insert(pair<string, string>("weather_model",weather));
        mainWindow window;
        if (window.setConfig(config.ConfigInfo))
            window.runMainWindow();
    }
    return 0;
}
