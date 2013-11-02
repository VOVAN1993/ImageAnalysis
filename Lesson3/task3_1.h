/* 
 * File:   task3_1.h
 * Author: vovan
 *
 * Created on 27 Октябрь 2013 г., 14:32
 */

#ifndef TASK3_1_H
#define	TASK3_1_H
#include "tools.h"
Mat laplacEdge(const Mat& src){
    Mat local_src=src.clone();
    cvtColor(local_src, local_src, CV_BGR2GRAY);
    blur(local_src,local_src,Size(3,3));
    threshold(local_src,local_src,100,255,CV_THRESH_BINARY);
    Mat abs_dst;
    Mat dst;
    Laplacian(local_src, dst, CV_16S, 3);
    
    convertScaleAbs(dst, abs_dst);
//    for(int i=0;i<src.rows;i++){
//        short *Mi=dst.ptr<short>(i);
//        uchar *Ni=abs_dst.ptr(i);
//        for(int j=0;j<src.cols;j++){
//            if(Mi[j]!=0)
//            std::cout<<(int)Mi[j]<<" "<<(int)Ni[j]<<" \n";
//        }
//    }
    return abs_dst;
}
Mat grad(const Mat& src){
    Mat local_src=src.clone();
    cvtColor(local_src, local_src, CV_BGR2GRAY);
    blur(local_src,local_src,Size(3,3));
    threshold(local_src,local_src,100,255,CV_THRESH_BINARY);
    Mat grad_x, grad_y;
  Mat abs_grad_x, abs_grad_y;
  int scale = 1;
  int delta = 0;
  int ddepth = CV_16S;
  Mat grad;
  Sobel( local_src, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
  convertScaleAbs( grad_x, abs_grad_x );

  Sobel( local_src, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
  convertScaleAbs( grad_y, abs_grad_y );

  addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );
threshold(grad,grad,100,255,CV_THRESH_BINARY);
  return grad;
}
Mat erodeEdge(const Mat& src){
    Mat local_src = src.clone();
    cvtColor(local_src, local_src, CV_BGR2GRAY);
    blur(local_src,local_src,Size(3,3));
    threshold(local_src,local_src,100,255,CV_THRESH_BINARY);
    int r=5;
    Mat element2 = getStructuringElement(MORPH_RECT,
            Size(2*r, 2*r),
            Point(r, r));
    Mat tmp;
    erode(local_src,tmp,element2);
//    imshow("asd",tmp);
    Mat res;
    absdiff(local_src,tmp,res);
//    imshow("asd1",res);
    return res;
}
void task3_1(Mat& src) {
    imshow("src",src);
    imshow("var1", laplacEdge(src));
    imshow("var2", grad(src));
    imshow("var3",erodeEdge(src));
}


#endif	/* TASK3_1_H */

