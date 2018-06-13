//
// Created by norse on 17-4-16.
//

#ifndef CAR_EYE_IMAGECONTROLLER_H
#define CAR_EYE_IMAGECONTROLLER_H


#include "opencv/cv.hpp"
#include "weatherApi.h"

using namespace cv;

class imageController {
public:
    /**
     * image merge
     *
     * @param cap_1 frame 1 from capture
     * @param cap_2 frame 2 from capture
     * @param width width of output image
     * @param hight hight of output image
     * @param weather text of output image (ps:weather is the api of weather now)
     * @return output image
     */
    Mat image2Merge(Mat cap_1, Mat cap_2, int width, int hight,string weather) ;
    /**
     * get the p_hashcode from the image mat
     *
     * @param src sourse image
     * @return hashCode
     */
    string p_hashCode(Mat src);
    /**
     * get the p_hashcode from the image mat
     *
     * @param src sourse image
     * @return hashCode
     */
    string a_hashCode(Mat src);
    /**
     * get the HanmingDistance between two image mat
     *
     * @param str1 hashcode of image_1
     * @param str2 hashcode of image_2
     * @return HanmingDistance
     */
    double HanmingDistance(string &str1,string &str2);
};


#endif //CAR_EYE_IMAGECONTROLLER_H
