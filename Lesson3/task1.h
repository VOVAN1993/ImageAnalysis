/* 
 * File:   task1.h
 * Author: vovan
 *
 * Created on 23 Октябрь 2013 г., 16:34
 */

#ifndef TASK1_H
#define	TASK1_H

#include "tools.h"
bool cmpPointX(const cv::Point& p1, const cv::Point& p2) {
    return p1.x < p2.x;
}
bool cmpPointY(const cv::Point& p1, const cv::Point& p2) {
    return p1.y < p2.y;
}
bool cmpCircle(const std::vector<Point>& a, const std::vector<Point>& b) {
    return a.size() > b.size();
}
std::string genNumberString(int number) {
    char buf[16];
    sprintf(buf, "%d", number);
    return std::string(buf);
}

cv::Point calculateCoordCenter(const std::vector<Point>& cnt) {
    int xMax = (*std::max_element(cnt.begin(), cnt.end(), cmpPointX)).x;
    int xMin = (*std::min_element(cnt.begin(), cnt.end(), cmpPointX)).x;
    int yMax = (*std::max_element(cnt.begin(), cnt.end(), cmpPointY)).y;
    int yMin = (*std::min_element(cnt.begin(), cnt.end(), cmpPointY)).y;

    return Point((xMax + xMin) / 2, (yMax + yMin) / 2);
}
void task1(Mat& src) {
    Mat src_gray = imread("src");
    cvtColor(src, src_gray, CV_BGR2GRAY);
    blur(src_gray,src_gray,Size(3,3));
    Mat canny_output;
    std::vector<std::vector<Point> > contours;
    
    Canny(src_gray, canny_output, 100, 100 * 2, 3);
    findContours(canny_output, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
    std::sort(contours.begin(), contours.end(), cmpCircle);
    
    Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
    RNG rng;
    std::stringstream stream;
    for (int i = 0; i < contours.size(); i++) {
        stream<<i+1;
        drawContours(drawing, contours, i, Scalar(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255)), CV_FILLED);
        cv::putText(drawing, stream.str(), calculateCoordCenter(contours[i])+Point(-10,5) , 
                FONT_HERSHEY_SIMPLEX,
                1.0, cv::Scalar(0, 0, 0),2);
        stream.str(std::string());
        stream.clear();
    }
    imshow("cont", canny_output);
}

#endif	/* TASK1_H */

