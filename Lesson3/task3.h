/*
 * File:   task3.h
 * Author: vovan
 *
 * Created on 26 Октябрь 2013 г., 17:25
 */

#ifndef TASK3_H
#define	TASK3_H
#include "tools.h"
#include "util.h"

Mat createColorVetor(int n, Mat& markers) {
    cv::Mat dst = cv::Mat::zeros(markers.size(), CV_8UC3);
    std::vector<Vec3b> colorTab;
    for (int i = 0; i < n; i++) {
        int b = theRNG().uniform(0, 255);
        int g = theRNG().uniform(0, 255);
        int r = theRNG().uniform(0, 255);

        colorTab.push_back(Vec3b((uchar) b, (uchar) g, (uchar) r));
    }


    for (int i = 0; i < markers.rows; i++) {
        for (int j = 0; j < markers.cols; j++) {
            int index = markers.at<int>(i, j);
            if (index > 0 && index <= n)
                dst.at<cv::Vec3b > (i, j) = colorTab[index - 1];
            else
                dst.at<cv::Vec3b > (i, j) = cv::Vec3b(0, 0, 0);
        }
    }
    return dst;
}

void part2(Mat& image) {
    Mat binary,dist,dist_8u;
    int thresh_value=13;
    int erosion_size = 7;
    cv::Mat eros_element = cv::getStructuringElement(cv::MORPH_RECT,
            cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1));
    cv::subtract(cv::Scalar::all(255), image, binary);
    cvtColor(binary, binary, CV_BGR2GRAY);
    threshold(binary, binary, thresh_value, thresh_max_value, CV_THRESH_BINARY);
    medianBlur(binary, binary, 5);    
    
    cv::erode(binary, binary, eros_element);

    cv::distanceTransform(binary, binary, CV_DIST_L2, 3);
    cv::normalize(binary, binary, 0, 1., NORM_MINMAX);
    cv::threshold(binary, dist, .6, 1, CV_THRESH_BINARY);
    int erosion_size2 = 12;
    cv::Mat element2 = cv::getStructuringElement(cv::MORPH_ELLIPSE,
            cv::Size(erosion_size2, erosion_size2));
    
    cv::erode(dist, dist, element2);

    VVP contours;
    dist.convertTo(dist, CV_8UC1);
    findContours(dist, contours, noArray(), CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
    std::vector<Point2f>center(contours.size());
    std::vector<float>radius(contours.size());
    
    for (int i = 0; i < contours.size(); i++) {
        minEnclosingCircle((Mat) contours[i], center[i], radius[i]);
    }
    
    Mat drawing = Mat::zeros(dist.size(), CV_8UC3);
    for (int i = 0; i < contours.size(); i++) {
        radius[i]+=20;
        Scalar color = Scalar::all(255);
        circle(drawing, center[i], radius[i], color);
        floodFill(drawing, center[i], color);
    }
    cvtColor(drawing, drawing, CV_BGR2GRAY);
    drawing.convertTo(dist_8u, CV_8U);

    cv::findContours(dist_8u, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    cv::Mat markers = cv::Mat::zeros(dist.size(), CV_32SC1);
    for (int i = 0; i < contours.size(); i++)
        cv::drawContours(markers, contours, i, Scalar::all(i + 1), -1);
    cv::circle(markers, cv::Point(10, 10), 3, Scalar::all(255), 1);
    cv::watershed(image, markers);
    Mat res = createColorVetor(contours.size(), markers);
    imshow("out/coins_4.jpg", res);
}

void part1(Mat& coins) {
    cv::Mat src = coins.clone();

    GaussianBlur(src, src, Size(3, 3), 0.);
//    cv::imshow("src", src);

    cv::Mat bw;
    cv::cvtColor(src, bw, CV_BGR2GRAY);

    threshold(bw, bw, 200, 255, CV_THRESH_BINARY);
    GaussianBlur(bw, bw, Size(7, 7), 0);
    threshold(bw, bw, 200, 255, CV_THRESH_BINARY);
    bitwise_not(bw, bw);
    cv::Mat dist;
    cv::distanceTransform(bw, dist, CV_DIST_L2, 3);

    cv::normalize(dist, dist, 0, 1., cv::NORM_MINMAX);

    cv::threshold(dist, dist, .5, 1., CV_THRESH_BINARY);

    cv::Mat dist_8u;
    dist.convertTo(dist_8u, CV_8U);
    bw /= 255;
    std::vector<std::vector<cv::Point> > contours;
    cv::findContours(dist_8u, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    int ncomp = contours.size();
    cv::Mat markers = cv::Mat::zeros(dist.size(), CV_32SC1);

    for (int i = 0; i < ncomp; i++)
        cv::drawContours(markers, contours, i, cv::Scalar::all(i + 1), -1);

    cv::circle(markers, cv::Point(10, 10), 3, CV_RGB(255, 254, 25), -1);

    cv::watershed(src, markers);
    Mat dst = createColorVetor(ncomp, markers);



    cv::imwrite("out/dst.jpg", dst);
}

void task3(Mat& coins_3, Mat& coins_4) {
    CV_Assert(!coins_3.empty());
    
    //    CV_Assert(!src.empty());
    part2(coins_4);
//    part1(coins_3);
    
}


#endif	/* TASK3_H */

