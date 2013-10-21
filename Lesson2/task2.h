/* 
 * File:   task2.h
 * Author: vovan
 *
 * Created on 21 Октябрь 2013 г., 12:10
 */

#ifndef TASK2_H
#define	TASK2_H
#include "util.h"
void task2(const Mat& image) {
    Mat gray,dst_eq,temp,diff,dst_norm;
    cvtColor(image, gray, CV_BGR2GRAY);

    cv::equalizeHist(gray, dst_eq);

    normalize(gray, dst_norm, 0, 255, NORM_MINMAX, CV_8UC1);
    absdiff(dst_norm, dst_eq, diff);
    std::vector<Mat> v;
    v.push_back(dst_norm);
    v.push_back(dst_eq);
    v.push_back(diff);
    Mat allImgs = merge(v);
    imwrite("out/Task2.jpg", allImgs);
}

#endif	/* TASK2_H */

