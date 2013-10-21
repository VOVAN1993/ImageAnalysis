/* 
 * File:   task3_2.h
 * Author: vovan
 *
 * Created on 21 Октябрь 2013 г., 12:20
 */

#ifndef TASK3_2_H
#define	TASK3_2_H
#include "util.h"
void task3_2(const Mat& src) {
    Mat local_src=src.clone();
    src.convertTo(local_src,CV_32FC1,1.0/255.0);
    std::vector<int> arg;
    arg.push_back(2);
    arg.push_back(4);
    arg.push_back(6);
    std::stringstream stream;
    for (int i = 0; i < arg.size(); i++) {
        Mat res;
        cv::pow(local_src,arg[i],res);
        stream << "out/LenaTask3_2_n=";
        stream << arg[i];
        stream << ".jpg";
        res.convertTo(res,CV_8UC1,255.0);
        imwrite(stream.str(), res);
        stream.str(std::string());
        stream.clear();
    }
}


#endif	/* TASK3_2_H */

