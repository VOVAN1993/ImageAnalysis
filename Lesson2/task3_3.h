/* 
 * File:   task3_3.h
 * Author: vovan
 *
 * Created on 21 Октябрь 2013 г., 12:22
 */

#ifndef TASK3_3_H
#define	TASK3_3_H
#include "util.h"


void task3_3( const Mat& src) {
    std::vector<int> args;
    src.convertTo(src,CV_8UC1);
    args.push_back(20);
    args.push_back(30);
    args.push_back(40);
    args.push_back(100);
    std::vector<Mat> result;
    for (int i = 0; i < args.size(); i++) {
        Mat tmp = mythreshold(src, args[i]);
        result.push_back(tmp);
    }
    for (int i = 0; i < args.size(); i++) {
        Mat tmp;
        threshold(src, tmp, args[i], 255, CV_THRESH_BINARY);
        cv::Mat diff = tmp != result[i];
        bool eq = cv::countNonZero(diff) == 0;//количество ненулевых элементов для проверки
        if (!eq) {
            throw std::invalid_argument("Error task3_3");
        }
    }
    Mat mergeResult = merge(result);
    imwrite("out/Task3_3.jpg", mergeResult);
    waitKey(0);
}


#endif	/* TASK3_3_H */

