//
// Created by Summer-V on 17-4-16.
//

#include <iostream>
#include "imageController.h"
#define HASH_SIZE 64
/**
 * Merge two image
 *
 * @param cap_1 image 1
 * @param cap_2 image 2
 * @param width target width
 * @param hight target height
 * @param weather print text
 * @return distance martix
 */
Mat imageController::image2Merge(Mat cap_1, Mat cap_2, int width, int hight, string weather) {
    Size imageSize = Size(width, hight);
    Mat re_frame_1;
    Mat re_frame_2;
    resize(cap_2, re_frame_2, imageSize);
    resize(cap_1, re_frame_1, imageSize);
    string hash_1 = this->p_hashCode(re_frame_1);
    string hash_2 = this->p_hashCode(re_frame_2);
    Size size(re_frame_1.cols + re_frame_2.cols, MAX(re_frame_1.rows, re_frame_2.rows));
    Mat img_merge;
    Mat outImg_left, outImg_right;
    img_merge.create(size, CV_MAKETYPE(re_frame_1.depth(), 3));
    img_merge = Scalar::all(0);
    outImg_left = img_merge(Rect(0, 0, re_frame_1.cols, re_frame_1.rows));
    outImg_right = img_merge(Rect(re_frame_1.cols, 0, re_frame_2.cols, re_frame_2.rows));
    if (re_frame_1.type() == CV_8U) {
        cvtColor(re_frame_1, outImg_left, CV_GRAY2BGR);
    } else {
        re_frame_1.copyTo(outImg_left);
    }

    if (re_frame_2.type() == CV_8U) {
        cvtColor(re_frame_2, outImg_right, CV_GRAY2BGR);
    } else {
        re_frame_2.copyTo(outImg_right);
    }
    weatherApi text;
    Mat dst=text.printData(img_merge, to_string(HanmingDistance(hash_1, hash_2)));
    return dst;
}
/**
 * get p_hashcode
 *
 * @param src sourse martix
 * @return p_hashcode of the sourse martix
 */
string imageController::p_hashCode(Mat src) {
    Mat img, dst;
    string rst(HASH_SIZE*HASH_SIZE, '\0');
    double dIdex[HASH_SIZE*HASH_SIZE];
    double mean = 0.0;
    int k = 0;
    if (src.channels() == 3) {
        cvtColor(src, src, CV_BGR2GRAY);
        img = Mat_<double>(src);
    } else {
        img = Mat_<double>(src);
    }

    resize(img, img, Size(HASH_SIZE, HASH_SIZE));

    dct(img, dst);

    //get averange
    for (int i = 0; i < HASH_SIZE; ++i) {
        for (int j = 0; j < HASH_SIZE; ++j) {
            dIdex[k] = dst.at<double>(i, j);
            mean += dst.at<double>(i, j) / (HASH_SIZE*HASH_SIZE);
            ++k;
        }
    }
    //get hashcode
    for (int i = 0; i < (HASH_SIZE*HASH_SIZE); ++i) {
        if (dIdex[i] >= mean) {
            rst[i] = '1';
        } else {
            rst[i] = '0';
        }
    }
    return rst;
}

/**
 * get a_hashcode
 *
 * @param src source martix
 * @return string of a_hashcode
 */
string imageController::a_hashCode(Mat src) {
    string rst(HASH_SIZE, '\0');
    Mat img;
    if (src.channels() == 3)
        cvtColor(src, img, CV_BGR2GRAY);
    else
        img = src.clone();

    resize(img, img, Size(HASH_SIZE, HASH_SIZE));

    uchar *pData;
    for (int i = 0; i < img.rows; i++) {
        pData = img.ptr<uchar>(i);
        for (int j = 0; j < img.cols; j++) {
            pData[j] = pData[j] / (HASH_SIZE/2);
        }
    }
   //get the averange in gray space
    int average = mean(img).val[0];
    //
    Mat mask = (img >= (uchar) average);
    //get hash code
    int index = 0;
    for (int i = 0; i < mask.rows; i++) {
        pData = mask.ptr<uchar>(i);
        for (int j = 0; j < mask.cols; j++) {
            if (pData[j] == 0)
                rst[index++] = '0';
            else
                rst[index++] = '1';
        }
    }
    return rst;
}

/**
 * get the Hanming Distance of the tow hashcode whitch have the same length
 *
 * @param str1 hashcode 1
 * @param str2 hashcode 2
 * @return Hanming distance
 */
double imageController::HanmingDistance(string &str1, string &str2) {
    if ((str1.size() != (HASH_SIZE*HASH_SIZE)) || (str2.size() != (HASH_SIZE*HASH_SIZE)))
        return -1;
    int difference = 1;
    for (int i = 0; i < (HASH_SIZE*HASH_SIZE); i++) {
        if (str1[i] != str2[i])
            difference++;
    }
    return (1-((double)difference)/(HASH_SIZE*HASH_SIZE));
}