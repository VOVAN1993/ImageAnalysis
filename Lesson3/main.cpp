#include "tools.h"
#include "task1.h"
#include "task2.h"
#include "task3.h"
#include "task3_4.h"
///
int main( int argc, char** argv ){
//    Mat img = imread("coins_3.jpg");
//    task1(img);
//    task2(img);
//    task3(img);
    Mat table_img = imread("table.jpg");
    task3_4(table_img);
    waitKey(0);
}