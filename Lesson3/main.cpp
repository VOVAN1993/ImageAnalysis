#include "tools.h"
#include "task1.h"
#include "task2.h"
#include "task3.h"
#include "task3_4.h"
#include "task3_1.h"
#include "task3_2.h"
#include "task3_5.h"
#include "task3_3.h"


int main(int argc, char** argv) {
//    Mat coins_1=imread("coins_1.jpg");
//    task1(coins_1);
//    Mat coins2=imread("coins_2.jpg");
//    task2(coins2);
    Mat coins_4 = imread("coins_4.jpg");
    Mat coins_3 = cv::imread("coins_3.jpg");
    task3(coins_3,coins_4);
    
//    Mat circles = imread("circles2.jpg");
//    task3_5(circles);
//    Mat circles_1 = imread("circles.jpg");
//    task3_4(circles_1);
//    Mat coins = imread("coins_5.jpg");
//    Mat n;
//    cvtColor(coins,n,CV_BGR2GRAY);
//    GaussianBlur(n,n,Size(5,5),0);
//    adaptiveThreshold(n,n,255,CV_ADAPTIVE_THRESH_MEAN_C,CV_THRESH_BINARY,11,0);
//    int r=1;
//    cv::Mat element3 = cv::getStructuringElement(cv::MORPH_RECT,
//            cv::Size(r+2, r+2));
//    
//    dilate(n,n,element3);
//    erode(n,n,element3);
//    element3 = cv::getStructuringElement(cv::MORPH_RECT,
//            cv::Size(2*(r+1)+1, 2*(r+1)+1));
//    dilate(n,n,element3);
//    erode(n,n,element3);
//GaussianBlur(n,n,Size(5,5),0);
//    adaptiveThreshold(n,n,255,CV_ADAPTIVE_THRESH_GAUSSIAN_C,CV_THRESH_BINARY,81,0);
    
//    threshold(n,n,100,255,CV_THRESH_BINARY);
    
//    imshow("qw",n);
//    imshow("coins",coins);

    waitKey(0);
}