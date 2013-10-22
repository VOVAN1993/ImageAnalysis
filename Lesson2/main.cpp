#include <opencv2/highgui/highgui_c.h>

#include "task1.h"
#include "task2.h"
#include "task3.h"
#include "task4.h"
#include "tools.h"
#include "task5.h"
#include "task3_1.h"
#include "task3_2.h"
#include "task3_3.h"
#include "task3_6.h"
#include "task6_new.h"
#include "task3_5.h"

int main(int argc, char** argv) {
    //подгружаем все изображения
    //сохранять будет в папку /out
    Mat dst_fornoise = imread("lena_color_512.tif");
    Mat lena = imread("Lena.jpg");
    Mat lena_gray = imread("Lena.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    Mat src_noise = imread("lena_color_512-noise.tif");
    Mat src_diag = imread("lena_diagonal.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    Mat dst_diag = imread("lena_gray_512.tif", CV_LOAD_IMAGE_GRAYSCALE);
    
    task1(lena);
    task2(lena);
    task3(lena);
    task4(lena);
    task5(lena);
    task6_new(lena);
    task3_1(lena_gray);
    task3_2(lena_gray);
    task3_3(lena_gray);
    task3_5(src_diag, dst_diag);
    task3_6(dst_fornoise, src_noise);
    return 0;
}
