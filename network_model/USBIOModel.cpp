//
// Created by norse on 17-4-19.
//

#include "USBIOModel.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <zconf.h>
#include <iostream>
#include <cstring>
#include "applog.h"
#include "map"
#define BUFFER_SIZE 80
#define SUB_BIT {'\r','\n'}
#define SEARCH_SIZE 2

map<string,string> USBIOModel::getData() {
    int fd;
    fd = this->port;
    if (fd) {
        char recv_buf[BUFFER_SIZE] = {'\0'};
        if (read(fd, recv_buf, BUFFER_SIZE) > 0) {
            //write(STDOUT_FILENO,ch,BUFFER_SIZE);
            //printf("--------%s\n",recv_buf);
            return this->serializeData(recv_buf);
        }
    }
    map<string,string> empty;
    return empty;
}

int USBIOModel::sendData(char *buffer) {

    int fd, wr_num, rd_num, status;
    wr_num = write(fd, buffer, BUFFER_SIZE);
    if (wr_num > 0) {
        loger.logSuccess("write success!");
    } else {
        loger.logError("Faild Write");
    }
    return 0;

}

int USBIOModel::search_find(const char *chr_1, const char *chr_2) {
    int num_chr_1 = 0, num_chr_2 = 0;
    for (int i = 0; i < SEARCH_SIZE; i++) {
        num_chr_1 += (int) chr_1[i];
        num_chr_2 += (int) chr_2[i];
    }
    return num_chr_1 - num_chr_2;
}
using namespace std;
map<string,string> USBIOModel::serializeData(char *data) {
    map<string,string> empty;
    char search[SEARCH_SIZE] = SUB_BIT;
    int search_flag = -1;
    int top_c = 0;
    int head_flag = 0;
    for (int i = 0; i < BUFFER_SIZE; i++) {
        char find[SEARCH_SIZE] = {'\0'};
        for (int count_c = 0; count_c < SEARCH_SIZE; count_c++) {
            find[count_c] = data[top_c + count_c];
        }
        if (top_c == 0 && search_find(find, search) == 0) {
            head_flag = 1;
        }
        if (head_flag == 0)
            return empty;
        top_c++;
        if (search_find(find, search) == 0) {
            search_flag += 1;
        }
        if (search_flag == 1)break;
    }
    char buffer_from_serial[top_c] = {'0'};
    for (int i = 0; i < top_c + 1; i++) {
        buffer_from_serial[i] = data[i];
    }
    if (head_flag == 1 && search_flag == 1) {
        map<string, string> data_map;
        string GPS_E;
        string GPS_N;
        string GPS_M;
        string GPS_KM;
        string GPS_TEMPERTURE;
        string GPS_TEMPERTURE_HUMIDDITY;
        int GPS_E_flag=0;
        int GPS_N_flag=0;
        int GPS_M_flag=0;
        int GPS_KM_flag=0;
        int GPS_TEMPERTURE_flag=0;
        int GPS_TEMPERTURE_HUMIDDITY_flag=0;
        for(int i=0;i<top_c;i++){
            if(buffer_from_serial[i]=='E'){
                int str_top=i;
                int count=0;
                while(buffer_from_serial[str_top]!='\n'){
                    str_top--;
                    count++;
                }
                for(int gps_find=1;gps_find<count+1;gps_find++){
                    char c=buffer_from_serial[str_top+gps_find];
                    GPS_E+=c;
                }
                GPS_E_flag=1;
            }
            if(buffer_from_serial[i]=='N'){
                int str_top=i;
                int count=0;
                while(buffer_from_serial[str_top]!=' '){
                    str_top--;
                    count++;
                }
                for(int gps_find=1;gps_find<count+1;gps_find++){
                    char c=buffer_from_serial[str_top+gps_find];
                    GPS_N+=c;
                }
                GPS_N_flag=1;
            }
            if(GPS_M_flag==0&&buffer_from_serial[i]=='m'){
                int str_top=i;
                int count=0;
                while(buffer_from_serial[str_top]!=' '){
                    str_top--;
                    count++;
                }
                for(int gps_find=1;gps_find<count+1;gps_find++){
                    char c=buffer_from_serial[str_top+gps_find];
                    GPS_M+=c;
                }
                GPS_M_flag=1;
            }
            if(buffer_from_serial[i]=='h'){
                int str_top=i;
                int count=0;
                while(buffer_from_serial[str_top]!=' '){
                    str_top--;
                    count++;
                }
                for(int gps_find=1;gps_find<count+1;gps_find++){
                    char c=buffer_from_serial[str_top+gps_find];
                    GPS_KM+=c;
                }
                GPS_KM_flag=1;
            }
            if(buffer_from_serial[i]=='%'){
                int str_top=i;
                int count=0;
                while(buffer_from_serial[str_top]!=' '){
                    str_top--;
                    count++;
                }
                for(int gps_find=1;gps_find<count+1;gps_find++){
                    char c=buffer_from_serial[str_top+gps_find];
                    GPS_TEMPERTURE+=c;
                }
                GPS_TEMPERTURE_flag=1;
            }
            if(buffer_from_serial[i]=='C'){
                int str_top=i;
                int count=0;
                while(buffer_from_serial[str_top]!=' '){
                    str_top--;
                    count++;
                }
                for(int gps_find=1;gps_find<count+1;gps_find++){
                    char c=buffer_from_serial[str_top+gps_find];
                    GPS_TEMPERTURE_HUMIDDITY+=c;
                }
                GPS_TEMPERTURE_HUMIDDITY_flag=1;
            }
        }
        if( GPS_E_flag==1&&
                GPS_N_flag==1&&
                GPS_M_flag==1&&
                GPS_KM_flag==1&&
                GPS_TEMPERTURE_flag==1&&
                GPS_TEMPERTURE_HUMIDDITY_flag==1){
            data_map.insert(pair<string, string>("E",GPS_E));
            data_map.insert(pair<string, string>("N",GPS_N));
            data_map.insert(pair<string, string>("M",GPS_E));
            data_map.insert(pair<string, string>("KM",GPS_KM));
            data_map.insert(pair<string, string>("TEMPERTURE",GPS_TEMPERTURE));
            data_map.insert(pair<string, string>("TEMPERTURE_HUMIDDITY",GPS_TEMPERTURE_HUMIDDITY));
            return data_map;
        }
    }
    return empty;
}
int USBIOModel::findCOM() {
    int fd;
    struct termios st;
    char ch[BUFFER_SIZE];
    if ((fd = open("/dev/ttyUSB0", O_RDWR | O_NONBLOCK)) < 0) {
        loger.printError("Cant open USB serial");
        exit(1);
    }
    loger.printSuccess("Open USB serial");
    st.c_iflag = 1;
    st.c_oflag = 0;
    st.c_cflag = 0;
    CS8 | CREAD | CLOCAL;
    cfsetospeed(&st, B9600);
    tcsetattr(fd, TCSANOW, &st);
    this->port = fd;
}

int USBIOModel::closeCOM() {
    if (this->port) {
        tcflush(this->port, TCIOFLUSH);
        close(this->port);
    }
}