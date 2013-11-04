/* 
 * File:   task3_4.h
 * Author: vovan
 *
 * Created on 26 Октябрь 2013 г., 21:23
 */

#ifndef TASK3_4_H
#define	TASK3_4_H
#include "tools.h"
Mat rotateImage(const Mat& source, double angle) {
    //поворот изображения
    Point2f src_center(source.cols / 2.0F, source.rows / 2.0F);
    Mat rot_mat = getRotationMatrix2D(src_center, angle, 1.0);
    Mat dst;
    warpAffine(source, dst, rot_mat, source.size());
    return dst;
}
void task3_4(const Mat& src){
    Mat tmp=src.clone();
    medianBlur(tmp,tmp,5);
    threshold(tmp,tmp,100,255,CV_THRESH_BINARY);
    int r=5000;
    bitwise_not(tmp,tmp);
    Mat element1 = getStructuringElement(MORPH_CROSS,
            Size(r,1),
            Point(r-1, 0));
//    element1 = rotateImage(element1,45.);
//    erode(tmp,tmp,element1);
//    dilate(tmp,tmp,element1);
//    erode(tmp,tmp,element1);
//    imshow("src",src);
    imshow("tmp",1000*element1);

}


#endif	/* TASK3_4_H */