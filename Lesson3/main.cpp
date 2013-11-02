#include "tools.h"
#include "task1.h"
#include "task2.h"
#include "task3.h"
#include "task3_4.h"
#include "task3_1.h"
#include "task3_2.h"
int main(int argc, char** argv) {
    Mat coins_4 = imread("coins_4.jpg");
    Mat coins_3 = cv::imread("coins_3.jpg");
    task3(coins_3,coins_4);
    waitKey(0);
}