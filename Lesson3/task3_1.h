/* 
 * File:   task3_1.h
 * Author: vovan
 *
 * Created on 27 Октябрь 2013 г., 14:32
 */

#ifndef TASK3_1_H
#define	TASK3_1_H
#include "tools.h"

Mat laplacEdge(const Mat& src) {
    Mat local_src = src.clone(), abs_dst, dst;
    blur(local_src, local_src, Size(3, 3));
    threshold(local_src, local_src, 100, 255, CV_THRESH_BINARY);
    Laplacian(local_src, dst, CV_16S, 3);

    convertScaleAbs(dst, abs_dst);
    return abs_dst;
}

Mat grad(const Mat& src) {
    Mat local_src = src.clone();
    blur(local_src, local_src, Size(3, 3));
    threshold(local_src, local_src, 100, 255, CV_THRESH_BINARY);
    Mat grad_x, grad_y;
    Mat abs_grad_x, abs_grad_y;
    int scale = 1;
    int delta = 0;
    int ddepth = CV_16S;
    Mat grad;
    Sobel(local_src, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
    convertScaleAbs(grad_x, abs_grad_x);

    Sobel(local_src, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
    convertScaleAbs(grad_y, abs_grad_y);

    addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);
    threshold(grad, grad, 100, 255, CV_THRESH_BINARY);
    return grad;
}

Mat erodeEdge(const Mat& src) {
    Mat local_src = src.clone(),res,tmp;
    int r = 2;
    Mat element = getStructuringElement(MORPH_RECT,
            Size(2 * r+1, 2 *r+1));
    blur(local_src, local_src, Size(3, 3));
    threshold(local_src, local_src, 100, 255, CV_THRESH_BINARY);
    
    erode(local_src, tmp, element);
    absdiff(local_src, tmp, res);
    return res;
}

void task3_1(Mat& src) {
    imwrite("out/task3_1_var_1.jpg", laplacEdge(src));
    imwrite("out/task3_1_var_2.jpg", grad(src));
    imwrite("out/task3_1_var_3.jpg", erodeEdge(src));
}


#endif	/* TASK3_1_H */

