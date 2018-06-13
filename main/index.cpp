//
// Created by norse on 17-5-25.
//
#include "iostream"
#include "stdlib.h"
#include "string"

#include "vector"
using namespace std;
int main(){
    string str;
    str="";
    str += getenv("QUERY_STRING");
    cout << "Content-type:text/html\r\n\r\n";
    cout << "<html>\n";
    cout << "<head>\n";
    cout << "<meta charset='utf-8'>\n";
    cout << "<title>学习妹妹女装吧</title>\n";
    cout << "</head>\n";
    cout << "<body>\n";
    cout << "<p>主机名："<<getenv("SERVER_NAME")<<"</p>"<<endl;
    cout << "<p>主机程序："<<getenv("SERVER_SOFTWARE")<<"</p>"<<endl;
    cout << "<p>当前主机IP："<<getenv("SERVER_ADDR")<<"</p>"<<endl;
    cout << "<p>当前主机端口号："<<getenv("SERVER_PORT")<<"</p>"<<endl;
    cout << "<p>请求方法："<<getenv("REQUEST_METHOD")<<"</p>"<<endl;
    cout << "<p>客户端IP："<<getenv("REMOTE_HOST")<<"</p>"<<endl;
    cout << "<p>客户端地址："<<getenv("REMOTE_ADDR")<<"</p>"<<endl;
    cout << "<p>客户端设备："<<getenv("HTTP_USER_AGENT")<<"</p>"<<endl;
    cout << "<h2>学习妹妹的女装照"<<str<<"</h2>\n";
    cout << "</body>\n";
    cout << "</html>\n";
    return 0;
}

