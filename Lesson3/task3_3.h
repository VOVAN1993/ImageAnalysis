/* 
 * File:   task3_3.h
 * Author: vovan
 *
 * Created on 3 Ноябрь 2013 г., 2:05
 */

#ifndef TASK3_3_H
#define	TASK3_3_H

void task3_3(const Mat& src){
    Mat local_src = src.clone();
    threshold(local_src,local_src,230,255,CV_THRESH_BINARY);
    int r=10;
    cvtColor(local_src,local_src,CV_BGR2GRAY);

    Mat tmp1 = Mat::zeros(local_src.size(),local_src.type());
    Mat tmp2 = Mat::zeros(local_src.size(),local_src.type());
    Mat element1 = getStructuringElement(MORPH_RECT,
            Size(1, 2*r),
            Point(0, r));  
    Mat element2 = getStructuringElement(MORPH_RECT,
            Size(2*r, 1),
            Point(r, 0));  //(2*r)x1
    morphologyEx(local_src,tmp1,MORPH_CLOSE,element1);
    morphologyEx(local_src,tmp2,MORPH_CLOSE,element2);
    Mat res;
    bitwise_and(tmp1,tmp2,res);
    
    
    morphologyEx(local_src,tmp1,MORPH_OPEN,element2);
    Mat element_vert = getStructuringElement(MORPH_RECT,
            Size(r, 2*r),
            Point(r/2, r));//2r x r
    morphologyEx(tmp1,tmp1,MORPH_CLOSE,element_vert);
    bitwise_and(res,tmp1,res);
    imwrite("out/table.jpg",res);
    imshow("tmp1",local_src);
    

}

#endif	/* TASK3_3_H */

