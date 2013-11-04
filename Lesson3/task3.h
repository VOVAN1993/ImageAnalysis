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

Mat colouring(int n, Mat& markers) {
    Mat dst = Mat::zeros(markers.size(), CV_8UC3);
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
                dst.at<Vec3b > (i, j) = colorTab[index - 1];
            else
                dst.at<Vec3b > (i, j) = Vec3b(0, 0, 0);
        }
    }
    return dst;
}

void part2(Mat& image) {
    Mat binary;
    int thresh_value=13;
    int erosion_size = 7;
    Mat eros_element = getStructuringElement(MORPH_RECT,
            Size(2 * erosion_size + 1, 2 * erosion_size + 1));
    binary=Scalar::all(255)-image;
    cvtColor(binary, binary, CV_BGR2GRAY);
    threshold(binary, binary, thresh_value, thresh_max_value, CV_THRESH_BINARY);
    medianBlur(binary, binary, 5);    
    
    erode(binary, binary, eros_element);

    distanceTransform(binary, binary, CV_DIST_L2, 3);
    normalize(binary, binary, 0, 1., NORM_MINMAX);
    threshold(binary, binary, .6, 1, CV_THRESH_BINARY);
    int erosion_size2 = 12;
    Mat element2 = getStructuringElement(MORPH_ELLIPSE,
            Size(erosion_size2, erosion_size2));
    
    erode(binary, binary, element2);

    VVP contours;
    binary.convertTo(binary, CV_8UC1);
    findContours(binary, contours, noArray(), CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
    std::vector<Point2f>center(contours.size());
    std::vector<float>radius(contours.size());
    
    for (int i = 0; i < contours.size(); i++) {
        minEnclosingCircle((Mat) contours[i], center[i], radius[i]);
    }
    
    Mat drawing = Mat::zeros(binary.size(), CV_8UC3);
    for (int i = 0; i < contours.size(); i++) {
        radius[i]+=20;
        Scalar color = Scalar::all(255);
        circle(drawing, center[i], radius[i], color);
        floodFill(drawing, center[i], color);
    }
    cvtColor(drawing, drawing, CV_BGR2GRAY);
    drawing.convertTo(drawing, CV_8U);

    findContours(drawing, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    Mat markers = Mat::zeros(binary.size(), CV_32SC1);
    for (int i = 0; i < contours.size(); i++)
        drawContours(markers, contours, i, Scalar::all(i + 1), -1);
    circle(markers, Point(10, 10), 3, Scalar::all(255), 1);
    watershed(image, markers);
    Mat res =  colouring(contours.size(), markers);
    imwrite("out/task3_coins_4.jpg", res);
}

void part1(Mat& coins) {
    Mat src = coins.clone();
    Mat bw;
    
    GaussianBlur(src, src, Size(3, 3), 0.);
    cvtColor(src, bw, CV_BGR2GRAY);

    threshold(bw, bw, 200, 255, CV_THRESH_BINARY);
    GaussianBlur(bw, bw, Size(7, 7), 0.);
    threshold(bw, bw, 200, 255, CV_THRESH_BINARY);
    bitwise_not(bw, bw);
    distanceTransform(bw, bw, CV_DIST_L2, 3);

    normalize(bw, bw, 0, 1., NORM_MINMAX);

    threshold(bw, bw, .5, 1., CV_THRESH_BINARY);

    bw.convertTo(bw, CV_8U);
    VVP contours;
    findContours(bw, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    Mat markers = Mat::zeros(bw.size(), CV_32SC1);

    for (int i = 0; i < contours.size(); i++)
        drawContours(markers, contours, i, Scalar::all(i + 1), -1);

    circle(markers, Point(10, 10), 3, CV_RGB(255, 254, 25), -1);

    watershed(src, markers);
    Mat dst =  colouring(contours.size(), markers);

    imwrite("out/task3_coins_3.jpg", dst);
}

void task3(Mat& coins_3, Mat& coins_4) {
    CV_Assert(!coins_3.empty());
    CV_Assert(!coins_4.empty());
    part2(coins_4);
    part1(coins_3);
    
}


#endif	/* TASK3_H */

