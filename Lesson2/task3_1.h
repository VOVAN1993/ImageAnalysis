/* 
 * File:   task3_1.h
 * Author: vovan
 *
 * Created on 21 Октябрь 2013 г., 12:19
 */

#ifndef TASK3_1_H
#define	TASK3_1_H
#include "util.h"
void task3_1(const Mat& src) {
    Mat floatImg;
    src.convertTo(floatImg, CV_32FC1, 1.0 / 255.0);
    
    Mat result;
    floatImg+=1.0;
    
    cv::log(floatImg,result);
    result.convertTo(result,CV_32FC1, 255.0);
    result.convertTo(result,CV_8UC1);
    imwrite("out/LenaTask3_1.jpg", result);
}


#endif	/* TASK3_1_H */

