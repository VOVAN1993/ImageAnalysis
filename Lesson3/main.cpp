#include "tools.h"
#include "task1.h"
///
int main( int argc, char** argv ){
    Mat img = imread("coins_1.jpg");
    task1(img);
 
    
    waitKey(0);
}