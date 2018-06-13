//
// Created by norse on 17-4-14.
//

#ifndef CAR_EYE_DETCETMODEL_H
#define CAR_EYE_DETCETMODEL_H

#include <opencv2/core/mat.hpp>
#include "stdio.h"
#include "vector"
#include "opencv2/highgui.hpp"
#include "applog.h"
using namespace cv;
using namespace std;
class Detcet {
public:

    /**
     * load the face xml too detcet the face of human
     *
     * @param frame frame stream from capture
     * @return print detcet output image
     */
    Mat detcetFace(Mat frame);
    /**
     * load the face xml too detcet the body of human
     *
     * @param frame frame stream from capture
     * @return print detcet output image
     */
    void detcetBody(Mat frame);
    /**
     * load the face xml too detcet the sign from the roade
     *
     * @param frame frame stream from capture
     * @return print detcet output image
     */
    Mat detcetSingal(Mat frame);
/**
 * detcet line inthe road
 *
 * @param src
 * @param width
 * @param height
 * @return
 */
    Mat detcetLines(Mat src,int width,int height);
private:
    vector<struct line> detectLine(Mat &img, int threshold);
    void drawDetectLines(Mat& image,const vector<Vec4i>& lines,Scalar & color);
    const double pi = 3.1415926f;
    const double RADIAN = 180.0 / pi;
    applog loger;
};


#endif //CAR_EYE_DETCETMODEL_H
