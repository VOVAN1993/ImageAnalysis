/* 
 * File:   task3_4.h
 * Author: vovan
 *
 * Created on 26 Октябрь 2013 г., 21:23
 */

#ifndef TASK3_4_H
#define	TASK3_4_H
#include "tools.h"

void task3_4(const Mat& src){
    Scalar value = Scalar(255,255,255);
    Mat dst, gray;
    
    cvtColor(src,gray,CV_BGR2GRAY);
    threshold(gray,gray,100,255,CV_THRESH_BINARY);
    cvtColor(gray,gray,CV_GRAY2BGR);
    Mat local_gray=gray.clone();
    //вставляем оконтовку, а потом заливаем ее черным цветом
    copyMakeBorder( gray, gray, 10, 10, 10, 10, BORDER_CONSTANT, value );
    //находим компоненты связности
    floodFill(gray, Point(0,0), Scalar(0,0,0));
    dst = gray(Rect(10,10,src.cols,src.rows)).clone();
    Mat diff;
    absdiff(local_gray,dst,diff);
    imwrite("out/task3_4_1.jpg",diff);
    
    local_gray-=diff;
    //MAGIC
    Mat element = getStructuringElement(MORPH_ELLIPSE, 
            Size(3,20));
    Mat tmp;
    erode(local_gray, tmp, element);
    element = getStructuringElement(MORPH_ELLIPSE, 
            Size(30,49));
    dilate(tmp, tmp, element);
    element = getStructuringElement(MORPH_ELLIPSE, 
            Size(4,4));
    
    Mat neww;
    local_gray.copyTo(neww,tmp);
  
    morphologyEx(neww,neww,MORPH_OPEN,element);
    Mat element1_1 = getStructuringElement(MORPH_ELLIPSE, 
            Size(20,3));
    Mat tmp1;
    erode(local_gray, tmp1, element1_1);
    element1_1 = getStructuringElement(MORPH_ELLIPSE, 
            Size(49,30));
    dilate(tmp1, tmp1, element1_1);
    element1_1 = getStructuringElement(MORPH_ELLIPSE, 
            Size(4,4));
    
    Mat neww1;
    local_gray.copyTo(neww1,tmp1);
    morphologyEx(neww1,neww1,MORPH_OPEN,element1_1);
    
    Mat rr=neww+neww1;
    element1_1 = getStructuringElement(MORPH_ELLIPSE, 
            Size(11,11));
    morphologyEx(rr,rr,MORPH_OPEN,element1_1);
    
    Mat res=local_gray-rr;
    element1_1 = getStructuringElement(MORPH_ELLIPSE, 
            Size(7,7));
    morphologyEx(res,res,MORPH_OPEN,element1_1);    
    rr=local_gray-res;
    element1_1 = getStructuringElement(MORPH_ELLIPSE, 
            Size(3,3));
    //маленькая зачистка
    morphologyEx(rr,rr,MORPH_OPEN,element1_1);
    res=local_gray-rr;
    //маленькая зачистка
    morphologyEx(res,res,MORPH_OPEN,element1_1);
    imwrite("out/task_3_4_3.jpg",rr);
    imwrite("out/task3_4_2.jpg",res);
    
}


#endif	/* TASK3_4_H */