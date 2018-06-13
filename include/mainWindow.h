//
// Created by Summer-V on 17-4-18.
//

#ifndef CAR_EYE_MAINWINDOW_H
#define CAR_EYE_MAINWINDOW_H

#include <opencv/highgui.h>
#include "gtk/gtk.h"
#include "map"
#include "applog.h"
#include "videoThread.h"
#include "USBIOModel.h"

using namespace std;
struct singalData{
    videoThread *video;
    GtkWidget *imageController;
    GtkWidget *gpsController;
    GtkWidget *temp_nowController;
    GtkWidget *temputerController;
    GtkWidget *speed;
    GtkWidget *alpha;
    GtkWidget *beta;
    GtkWidget *gama;
    map<string, string> config;
    USBIOModel serial;
};
class mainWindow {
public:
    void runMainWindow();
    int setConfig(map<string, string> info);
private:
    applog loger;
    string weather;
    int width, hight;
    videoThread video;
    struct singalData singal;
    map<string, string> config;
    gint delete_event(GtkWidget *widget, gpointer data);
    void  getTime();
    void destroy(GtkWidget *widget, gpointer data);
    static gint changeImage(gpointer data);
    static gint getSerinal(gpointer data);
    static gint changeState(gpointer data);
};


#endif //CAR_EYE_MAINWINDOW_H
