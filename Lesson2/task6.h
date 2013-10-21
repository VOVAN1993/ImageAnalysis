/* 
 * File:   task6.h
 * Author: vovan
 *
 * Created on 21 Октябрь 2013 г., 12:28
 */

#ifndef TASK6_H
#define	TASK6_H
#include "for_dft.h"
void task6(Mat& src){
    cv::cvtColor(src, src, CV_BGR2GRAY);
    std::vector<int> arg;
    arg.push_back(5);
    arg.push_back(15);
    arg.push_back(30);
    arg.push_back(255);
    arg.push_back(380);
    Mat complex = computeDFT(src);
    Mat spec = updateMag(complex); // show spectrum
    std::cout<<spec.type()<<" "<<CV_32FC1<<" \n";
    spec.convertTo(spec,CV_32FC1,255.0);
    spec.convertTo(spec,CV_8UC1);
    std::vector<Mat> src_merge_img_vec;
    src_merge_img_vec.push_back(src);
    src_merge_img_vec.push_back(spec);
    Mat src_merge_img = merge(src_merge_img_vec);
    Size src_size = src.size();
    std::stringstream stream;
    int flag=0;
    for(int i=0;i<arg.size();i++){
        for(int j=0;j<=1;j++){
            stream<<"Result_R=";
            stream<<arg[i];
            stream<<"j=";
            stream<<j;
            stream<<".jpg";
            Mat res = implementFiltering(complex,arg[i],src_size,j);
            res.convertTo(res,CV_32FC1,255.0);
            res.convertTo(res,CV_8UC1);
            std::vector<Mat> vert_merge;
            vert_merge.push_back(src_merge_img);
            vert_merge.push_back(res);
            Mat show_img = mergeVertical(vert_merge);
            imwrite(stream.str(),show_img);
            stream.str(std::string());
            stream.clear();
        }
    }
    
}


#endif	/* TASK6_H */

