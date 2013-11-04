/* 
 * File:   task1.h
 * Author: vovan
 *
 * Created on 23 Октябрь 2013 г., 16:34
 */

#ifndef TASK1_H
#define	TASK1_H

#include "tools.h"
#include "util.h"
bool cmpPointX(const Point& p1, const Point& p2) {
    return p1.x < p2.x;
}
bool cmpPointY(const Point& p1, const Point& p2) {
    return p1.y < p2.y;
}
bool cmpCircle(const VP& a, const VP& b) {
    return a.size() > b.size();
}
Point calculateCoordCenter(const VP& cnt) {
    int xMax = (*max_element(ALL(cnt), cmpPointX)).x;
    int xMin = (*min_element(ALL(cnt), cmpPointX)).x;
    int yMax = (*max_element(ALL(cnt), cmpPointY)).y;
    int yMin = (*min_element(ALL(cnt), cmpPointY)).y;

    return Point((xMax + xMin) / 2, (yMax + yMin) / 2);
}
void task1(Mat& src) {
    CV_Assert(!src.empty());
    Mat src_gray,canny_output,res;
    VVP contours;
    RNG rng;
    double thresh = 100.;
    std::stringstream stream;
    
    cvtColor(src, src_gray, CV_BGR2GRAY);
    blur(src_gray,src_gray,Size(3,3));
    Canny(src_gray, canny_output, thresh, thresh * 2, 3);
    findContours(canny_output, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    std::sort(contours.begin(), contours.end(), cmpCircle);
    
    Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
    for (int i = 0; i < contours.size(); i++) {
        stream<<i+1;
        drawContours(drawing, contours, i, Scalar(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255)), CV_FILLED);
        cv::putText(drawing, stream.str(), calculateCoordCenter(contours[i])+Point(-10,5) , 
                FONT_HERSHEY_SIMPLEX,
                1.0, cv::Scalar(0, 0, 0),2);
        stream.str(std::string());
        stream.clear();
    }
    hconcat(src,drawing,res);
    imwrite("out/task1.jpg", res);
}

#endif	/* TASK1_H */

