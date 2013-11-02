// -*- C++ -*-
/* 
 * File:   task2.h
 * Author: vovan
 *
 * Created on 25 Октябрь 2013 г., 22:04
 */

#ifndef TASK2_H
#define	TASK2_H
#include "tools.h"

void task2(Mat& src) {
    CV_Assert(!src.empty());
    Mat local_src=Mat::zeros(src.size(),CV_8UC1);
    cvtColor(src,local_src,CV_BGR2GRAY);
    GaussianBlur(local_src,local_src,Size(3,3),0);
    threshold(local_src,local_src,250,255,CV_THRESH_BINARY);
    Mat element_dil = getStructuringElement(MORPH_RECT,
            Size(13,13),
            Point(6, 6));
    Mat element_er = getStructuringElement(MORPH_RECT,
            Size(15, 15),
            Point(7, 7));
    Mat test=Mat::zeros(src.size(),src.type());
    dilate( local_src, test, element_dil);
    erode(test,test,element_er);
    
    bitwise_not(test,test);
    imshow("asd",test);
    Mat res(src.size(),src.type(),Scalar(255,255,255));
    src.copyTo(res,test);
    
    Mat src2=src.clone();
    Mat diff;
    absdiff(res,src2,diff);
    bitwise_not(diff,diff);
    imshow("out/task1_2Text.jpg",diff);
    imshow("out/task1_2Coins.jpg",res);
}



#endif	/* TASK2_H */

