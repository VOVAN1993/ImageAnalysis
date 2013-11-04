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
    Mat local_src, text, res(src.size(), src.type(), Scalar(255, 255, 255));
    const int size_elem = 13;
    Mat element_dil = getStructuringElement(MORPH_RECT,
            Size(size_elem, size_elem));
    Mat element_er = getStructuringElement(MORPH_RECT,
            Size(size_elem + 2, size_elem + 2));
    
    cvtColor(src, local_src, CV_BGR2GRAY);
    GaussianBlur(local_src, local_src, Size(3, 3), 0);
    threshold(local_src, local_src, 250, 255, CV_THRESH_BINARY);
    dilate(local_src, local_src, element_dil);
    erode(local_src, local_src, element_er);

    bitwise_not(local_src, local_src);
    src.copyTo(res, local_src);
    absdiff(res, src, text);
    bitwise_not(text, text);

    imwrite("out/task2_coins.jpg", res);
    imwrite("out/task2_text.jpg", text);
}



#endif	/* TASK2_H */

