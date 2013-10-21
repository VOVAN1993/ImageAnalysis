/* 
 * File:   task1.h
 * Author: vovan
 *
 * Created on 21 Октябрь 2013 г., 11:51
 */

#ifndef TASK1_H
#define	TASK1_H
#include "util.h"

void task1(const Mat &image) {
    std::vector<Mat> dsts,channels;
    Mat diff;
    for (int i = 0; i < 5; i++) {
        dsts.push_back(Mat::zeros(image.size(), image.type()));
    }
    
    Mat testCvt;
    image.convertTo(testCvt, -1, 2, 100);
    Mat testMyCvt = Mat::zeros(image.size(), image.type());
    scaleImg(image,testMyCvt,2,100);
    absdiff(testCvt,testMyCvt,diff);
    split(diff,channels);
    for(int i=0;i<channels.size();i++){
        int mmax = *(std::max_element(channels[i].begin<uchar>(), channels[i].end<uchar>()));
        std::cout<<mmax<<" ";
    }
    
    std::vector<std::pair<double, int> > arrParam;
    arrParam.push_back(std::make_pair(0.5, 0));
    arrParam.push_back(std::make_pair(1.0, -125));
    arrParam.push_back(std::make_pair(2.0, 0));
    arrParam.push_back(std::make_pair(2.0, -255));
    arrParam.push_back(std::make_pair(1.0, 125));

    for (int i = 0; i < arrParam.size(); i++) {
        scaleImg(image,dsts[i],arrParam[i].first,arrParam[i].second);
    }
    std::vector<Mat> param;
    param.push_back(concatCh(dsts[0]));
    param.push_back(concatCh(dsts[1]));
    Mat res1 = mergeVertical(param);
    param.clear();
    imwrite("Task1Img1+2.jpg",res1);
    param.push_back(concatCh(dsts[2]));
    param.push_back(concatCh(dsts[3]));
    param.push_back(concatCh(dsts[4]));
    Mat res2 = mergeVertical(param);
    imwrite("Task1Img3+4+5.jpg",res2);
    waitKey(0);
}


#endif	/* TASK1_H */

