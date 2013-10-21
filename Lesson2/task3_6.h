/* 
 * File:   task3_6.h
 * Author: vovan
 *
 * Created on 21 Октябрь 2013 г., 12:36
 */

#ifndef TASK3_6_H
#define	TASK3_6_H
#include "util.h"

void task3_6(Mat& src, Mat noise) {
    std::vector<Mat> planes;
    split(noise, planes);

    medianBlur(planes[0], planes[0], 3);
    medianBlur(planes[1], planes[1], 3);
    medianBlur(planes[2], planes[2], 3);

    merge(planes, noise);

    GaussianBlur(noise, noise, Size(9, 9), 0, 2);

    Mat tmp;
    cv::matchTemplate(src, noise, tmp, CV_TM_SQDIFF);
    std::cout << "RMSE: " << tmp / (src.rows * src.cols * src.channels()) << std::endl;
    imwrite("Task3_6.jpg",noise);
}


#endif	/* TASK3_6_H */

