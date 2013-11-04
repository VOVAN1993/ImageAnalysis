/* 
 * File:   task3_5.h
 * Author: vovan
 *
 * Created on 3 Ноябрь 2013 г., 1:32
 */

#ifndef TASK3_5_H
#define	TASK3_5_H
#include "tools.h"
void task3_5(Mat& src){
    int r=14;
    Mat big;
    Mat element1 = getStructuringElement(MORPH_ELLIPSE,
            Size(2*r+1, 2*r+1),
            Point(r, r)); 
    dilate(src,big,element1);
    erode(big,big,element1);
    threshold(big,big,50,255,CV_THRESH_BINARY);
    Mat tmp_src = src.clone();
    Mat minCir;    
    bitwise_not(tmp_src,tmp_src);
    Mat n;
    bitwise_and(src.clone(),src,n);
    r=80;
    element1 = getStructuringElement(MORPH_ELLIPSE,
            Size(2*r+1, 2*r+1),
            Point(r, r));
    erode(big,big,element1);
    dilate(big,big,element1);
    floodFill(big,Point(big.cols-10,big.rows-10),Scalar(0,255,255));
    element1 = getStructuringElement(MORPH_ELLIPSE,
            Size(11, 11),
            Point(5, 5));
    erode(big,big,element1);
    Mat nn;
    erode(big,nn,element1);
    floodFill(big, Point(0,0), Scalar(255,255,0));
    threshold(src,src,50,255,CV_THRESH_BINARY);

    bitwise_not(src,src);
    Mat res = src+big;
    res=Scalar::all(255)-res;
    imwrite("out/task3_5.jpg",res);

}

#endif	/* TASK3_5_H */

