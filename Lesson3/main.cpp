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
    Mat coins_1=imread("coins_1.jpg");
    task1(coins_1);
    Mat coins2=imread("coins_2.jpg");
    task2(coins2);
    Mat coins_4 = imread("coins_4.jpg");
    Mat coins_3 = imread("coins_3.jpg");
    task3(coins_3,coins_4);
    Mat portrait = imread("Portrait.jpg",CV_LOAD_IMAGE_GRAYSCALE);
    task3_1(portrait);
    

    Mat table = imread("table.jpg",CV_LOAD_IMAGE_GRAYSCALE);
    task3_3(table);
       
    Mat circles = imread("circles2.jpg");
    task3_5(circles);    
    Mat circles_1 = imread("circles.jpg");
    task3_4(circles_1);


    waitKey(0);
}