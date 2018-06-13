//
// Created by Summer-V on 17-4-18.
//

#include <opencv/highgui.h>
#include <iostream>
#include <gdkmm/pixbuf.h>
#include <sys/ioctl.h>
#include "mainWindow.h"
#include "USBIOModel.h"
#include "videoThread.h"
#include "map"
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <xcb/xcb.h>
#include <sys/mman.h>
#include <math.h>
#include <time.h>

#define LOADING_IMAGE "./1.jpg"
using namespace std;
/**
 * create gtk window thread
 */
void mainWindow::runMainWindow() {
    GtkWidget *window;
    //this->singal.serial.findCOM();
    int i, screenNum;
    xcb_connection_t *connection = xcb_connect(NULL, &screenNum);
    const xcb_setup_t *setup = xcb_get_setup(connection);
    xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);
    for (i = 0; i < screenNum; ++i) {
        xcb_screen_next(&iter);
    }
    xcb_screen_t *screen = iter.data;


    loger.logInfo("Screen Wdith " + to_string(screen->width_in_pixels));
    loger.logInfo("Screen Height " + to_string(screen->height_in_pixels));
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    //window_model = gtk_window_new(GTK_WINDOW_POPUP);
    gtk_window_set_title(GTK_WINDOW(window), "CarEyeSystem");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), screen->width_in_pixels, screen->height_in_pixels);
    gtk_container_set_border_width(GTK_CONTAINER(window), 5);
    g_signal_connect (G_OBJECT(window), "delete_event", G_CALLBACK("delete_event"), NULL);
    g_signal_connect (G_OBJECT(window), "destroy", G_CALLBACK("destroy"), NULL);

    GtkWidget *table = gtk_table_new(20, 4, TRUE);
    gtk_container_add(GTK_CONTAINER(window), table);

    GdkPixbuf *src = gdk_pixbuf_new_from_file(LOADING_IMAGE, NULL);
    GdkPixbuf *dst = gdk_pixbuf_scale_simple(src, 640, 480, GDK_INTERP_BILINEAR);
    GtkWidget *image = gtk_image_new_from_pixbuf(dst);
    GtkWidget *label_tempture = gtk_label_new("温度");
    GtkWidget *label_gps = gtk_label_new("GPS");
    GtkWidget *label_temp_now = gtk_label_new("天气");
    GtkWidget *speed = gtk_label_new("速度");
    GtkWidget *gama = gtk_label_new("gama波");
    GtkWidget *alpha = gtk_label_new("alpha波");
    GtkWidget *beta = gtk_label_new("beta波");
    GtkWidget *weather_now = gtk_label_new(this->config["weather_model"].c_str());

    gtk_table_attach_defaults(GTK_TABLE(table), image, 0, 4, 0, 10);
    gtk_table_attach_defaults(GTK_TABLE(table), label_gps, 0, 2, 10, 11);
    gtk_table_attach_defaults(GTK_TABLE(table), label_temp_now, 2, 4, 10, 11);
    gtk_table_attach_defaults(GTK_TABLE(table), label_tempture, 0, 2, 11, 12);
    gtk_table_attach_defaults(GTK_TABLE(table), speed, 2, 4, 11, 12);

    gtk_table_attach_defaults(GTK_TABLE(table), alpha, 0, 1, 12, 13);
    gtk_table_attach_defaults(GTK_TABLE(table), beta, 1, 2, 12, 13);
    gtk_table_attach_defaults(GTK_TABLE(table), gama, 3, 4, 12, 13);

    gtk_table_attach_defaults(GTK_TABLE(table), weather_now, 0, 4, 13, 14);

    g_object_unref(src);  //free buff
    g_object_unref(dst);
    gtk_widget_show_all(window);
    this->singal.imageController = image;
    this->singal.gpsController = label_gps;
    this->singal.temp_nowController = label_temp_now;
    this->singal.temputerController = label_tempture;
    this->singal.speed = speed;
    this->singal.alpha = alpha;
    this->singal.beta = beta;
    this->singal.gama = gama;
    this->singal.video = &this->video;
    this->singal.config = this->config;

    g_timeout_add(50, changeState, (gpointer) &this->singal);
    g_timeout_add(100, changeImage, (gpointer) &this->singal);
    g_timeout_add(100, getSerinal, (gpointer) &this->singal);
    gtk_main();
}

gint mainWindow::getSerinal(gpointer data) {
    struct singalData *singal = (struct singalData *) data;
    USBIOModel *serial = (USBIOModel *) &singal->serial;
    //map<string,string> serial_data=serial->getData();
    int size=1;//serial_data.size();
    if(size!=0){
	//外接GPS模块
        string gpsController = "GPS->";
        //gpsController += serial_data["E"];
        //gpsController += "";
        //gpsController += serial_data["N"];
        string temp_nowController = "TempNow->";
        //temp_nowController += serial_data["TEMPERTURE"];
        string temputerController = "Tempture->";
        //temputerController += serial_data["TEMPERTURE_HUMIDDITY"];
        string spped_controller = "Speed->";
        //spped_controller+=serial_data["KM"];
        //spped_controller+="   "+serial_data["M"];

        gtk_label_set_text(GTK_LABEL(singal->gpsController), gpsController.c_str());
        gtk_label_set_text(GTK_LABEL(singal->temp_nowController), temp_nowController.c_str());
        gtk_label_set_text(GTK_LABEL(singal->temputerController), temputerController.c_str());
        gtk_label_set_text(GTK_LABEL(singal->speed), spped_controller.c_str());
        return TRUE;
    }
    return TRUE;
}
/**
*脑电波模块（未接入）
*/
gint mainWindow::changeState(gpointer data) {
    struct singalData *singal = (struct singalData *) data;
    string gama_state="gama波";
    string alpha_state="alpha波";
    string beta_state="beta波";

    double random(double, double);
    srand(time(NULL));
    double alpha = rand() % (30 + 1) / (float) (30 + 1);
    alpha_state+=to_string(alpha);
    gtk_label_set_text(GTK_LABEL(singal->alpha), alpha_state.c_str());

    double random(double, double);
    srand(time(NULL));
    double beta = rand() % (30 + 1) / (float) (30 + 1);
    beta_state+=to_string(beta);
    gtk_label_set_text(GTK_LABEL(singal->beta), beta_state.c_str());

    double random(double, double);
    srand(time(NULL));
    double gama = rand() % (30 + 1) / (float) (30 + 1);
    gama_state+=to_string(gama);
    gtk_label_set_text(GTK_LABEL(singal->gama), gama_state.c_str());

    return TRUE;
}

/**
 * refresh main camera image from opencv
 *
 * @param data thread data singal
 * @return
 */
gint mainWindow::changeImage(gpointer data) {
    applog logger;
    using namespace std;
    using namespace Gdk;
    struct singalData *singal = (struct singalData *) data;
    Mat image = singal->video->getVideo();
    IplImage img = IplImage(image);
    cvCvtColor(&img, &img, CV_RGB2BGR);
    guchar *char_i = (guchar *) (img.imageData);
    GdkPixbuf *img_org;
    img_org = gdk_pixbuf_new_from_data(
            (guchar *) img.imageData, GDK_COLORSPACE_RGB, FALSE, 8,
            img.width, img.height, img.widthStep, NULL, NULL
    );
    GdkPixbuf *dst = gdk_pixbuf_scale_simple(img_org, 640 * 2, 480, GDK_INTERP_BILINEAR);
    gtk_image_set_from_pixbuf(GTK_IMAGE(singal->imageController), dst);
    //std::cout << gpsController.c_str() << std::endl;
    return TRUE;
}

gint mainWindow::delete_event(GtkWidget *widget, gpointer data) {
    gtk_main_quit();
    return FALSE;
}


void mainWindow::destroy(GtkWidget *widget, gpointer data) {
    this->singal.serial.closeCOM();
    gtk_main_quit();
}

/**
 * output local time
 */
void mainWindow::getTime() {
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    std::cout << asctime(timeinfo) << std::endl;
}

/**
 * set the window config
 *
 * @param info info list
 * @return state of operation
 */
int mainWindow::setConfig(map<string, string> info) {
    try {
        this->config = info;
        this->width = atoi(info["width"].c_str()) * 2;
        this->hight = atoi(info["hight"].c_str()) + 100;
        this->video.setConfig(info);
        this->video.startVideo();
        loger.printSuccess("Window get config_model success");
        return 1;
    } catch (Exception e) {
        loger.printError(e.msg);
        return 0;
    }
}
