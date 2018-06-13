//
// Created by Summer-V on 17-4-14.
//

#include <iostream>
#include "detcetModel.h"
#include "opencv2/imgproc.hpp"
#include "opencv2/objdetect.hpp"

using namespace cv;
using namespace std;
struct line{
    int theta;
    int r;
};
Mat Detcet::detcetFace(Mat frame) {
    CascadeClassifier face_cascade;
    CascadeClassifier eyes_cascade;
    if( !face_cascade.load( "./haarcascades/haarcascade_frontalface_alt.xml" ) ){ loger.printError("Error loading face cascade");}
    if( !eyes_cascade.load( "./haarcascades/haarcascade_eye_tree_eyeglasses.xml" ) ){ loger.printError("Error loading eyes cascade");}
    std::vector<Rect> faces;
    Mat frame_gray;
    cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );
    face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(30, 30) );
    for ( size_t i = 0; i < faces.size(); i++ )
    {
        Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
        ellipse( frame, center, Size( faces[i].width/2, faces[i].height/2 ), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
        Mat faceROI = frame_gray( faces[i] );
        std::vector<Rect> eyes;
        eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CASCADE_SCALE_IMAGE, Size(30, 30) );
        for ( size_t j = 0; j < eyes.size(); j++ )
        {
            Point eye_center( faces[i].x + eyes[j].x + eyes[j].width/2, faces[i].y + eyes[j].y + eyes[j].height/2 );
            int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
            circle( frame, eye_center, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );
        }
    }
}

void Detcet::detcetBody(Mat frame) {
    CascadeClassifier body_cascade;
    if( !body_cascade.load( "./haarcascades/haarcascade_fullbody.xml" ) ){ printf("--(!)Error loading face cascade\n");}
    //body_cascade.load("./haarcascades/haarcascade_frontalface_alt.xml");
    std::vector<Rect> faces;
    Mat frame_gray;
    cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
    equalizeHist(frame_gray, frame_gray);
    body_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0, Size(30, 30));
    for (size_t i = 0; i < faces.size(); i++) {
        Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
        ellipse(frame, center, Size(faces[i].width / 2, faces[i].height / 2), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);
    }
}
Mat Detcet::detcetLines(Mat src,int width,int height) {
    Rect select;
    select.x = 0;
    select.y = height;
    select.width =width;
    select.height =height;
    Mat roi1;
    src(select).copyTo(roi1);
    Mat dst,edge,gray,binary;
    dst.create( roi1.size(), roi1.type() );
    cvtColor( roi1, gray, CV_BGR2GRAY );
    threshold(gray,binary,178,255,CV_THRESH_BINARY);
    blur( binary, edge, Size(5,5) );
    Canny( edge, edge, 3, 9,3 );
    dst = Scalar::all(0);
    roi1.copyTo( dst, edge);
    vector<Vec4i> lines;
    HoughLinesP(binary,lines,1,CV_PI/180,80,50,10);
    Scalar sca(0,255,0);
    drawDetectLines(roi1,lines,sca);
    rectangle(src,select,Scalar(255,255,255),3,2,0);
    Mat out_put_image=src(select);
    roi1.copyTo(out_put_image);
    return src;
}
Mat Detcet::detcetSingal(Mat frame) {
    CascadeClassifier body_cascade;
    if( !body_cascade.load( "./haarcascades/haarcascade_fullbody.xml" ) ){ loger.printError("Error loading eyes cascade");}
    //body_cascade.load("./haarcascades/haarcascade_fullbody.xml");
    std::vector<Rect> faces;
    Mat frame_gray;
    cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
    equalizeHist(frame_gray, frame_gray);
    body_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0, Size(30, 30));
    for (size_t i = 0; i < faces.size(); i++) {
        Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
        ellipse(frame, center, Size(faces[i].width / 2, faces[i].height / 2), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);
        Mat faceROI = frame_gray(faces[i]);
    }
    return frame;
}
void Detcet::drawDetectLines(Mat& image,const vector<Vec4i>& lines,Scalar & color)
{
    // 将检测到的直线在图上画出来
    vector<Vec4i>::const_iterator it=lines.begin();
    while(it!=lines.end())
    {
        Point pt1((*it)[0],(*it)[1]);
        Point pt2((*it)[2],(*it)[3]);
        cv::line(image,pt1,pt2,color,2);
        ++it;
    }
}
vector<struct line> Detcet::detectLine(Mat &img, int threshold) {
    vector<struct line> lines;
    int diagonal = floor(sqrt(img.rows*img.rows + img.cols*img.cols));
    vector< vector<int> >p(360, vector<int>(diagonal));
    //统计数量
    for (int j = 0; j < img.rows; j++) {
        for (int i = 0; i < img.cols; i++) {
            if (img.at<unsigned char>(j, i) > 0){
                for (int theta = 0; theta < 360; theta++){
                    int r = floor(i*cos(theta / RADIAN) + j*sin(theta / RADIAN));
                    if (r < 0)
                        continue;
                    p[theta][r]++;
                }
            }
        }
    }
    for (int theta = 0; theta < 360; theta++){
        for (int r = 0; r < diagonal; r++){
            int thetaLeft = max(0, theta - 1);
            int thetaRight = min(359, theta + 1);
            int rLeft = max(0, r - 1);
            int rRight = min(diagonal - 1, r + 1);
            int tmp = p[theta][r];
            if (tmp > threshold
                && tmp > p[thetaLeft][rLeft] && tmp > p[thetaLeft][r] && tmp > p[thetaLeft][rRight]
                && tmp > p[theta][rLeft] && tmp > p[theta][rRight]
                && tmp > p[thetaRight][rLeft] && tmp > p[thetaRight][r] && tmp > p[thetaRight][rRight]){
                struct line newline;
                newline.theta = theta;
                newline.r = r;
                lines.push_back(newline);
            }
        }
    }
    return lines;
}
