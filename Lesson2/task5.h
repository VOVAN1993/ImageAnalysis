/* 
 * File:   task5.h
 * Author: vovan
 *
 * Created on 21 Октябрь 2013 г., 12:18
 */

#ifndef TASK5_H
#define	TASK5_H
#include "util.h"
void task5(Mat& img) {
    std::vector<Mat> gausNoiseImgs;
    std::vector<Mat> myLenaNoiseImgs;
    std::stringstream stream;
    for (int i = 0; i < 3; i++) {
        stream << "Task4img";
        stream << i + 1;
        stream << ".jpg";
        Mat tmp = imread(stream.str());
        gausNoiseImgs.push_back(tmp);
        stream.str(std::string());
        stream.clear();
    }
    for (int i = 0; i < 3; i++) {
        stream << "Task3_img_";
        stream << i + 1;
        stream << ".jpg";
        Mat tmp = imread(stream.str());
        myLenaNoiseImgs.push_back(tmp);
        stream.str(std::string());
        stream.clear();
    }

    //blur
    std::vector<Mat> resultBlur;
    std::vector<Mat> resultMedianBlur;
    std::vector<Mat> resultGaussBlur;

   
    for (int j = 0; j < 3; j++) {
        for (int i = 0; i < 3; i++) {
            int ksize = 3 + 2 * i;
            Mat tmp = Mat(img.rows, img.cols, CV_8UC1);
            cv::blur(myLenaNoiseImgs[j], tmp, cv::Size(ksize, ksize));
            resultBlur.push_back(tmp);
            tmp = Mat(img.rows, img.cols, CV_8UC1);
            cv::blur(gausNoiseImgs[j], tmp, cv::Size(ksize, ksize));
            resultBlur.push_back(tmp);

            tmp = Mat(img.rows, img.cols, CV_8UC1);
            cv::medianBlur(myLenaNoiseImgs[j], tmp, ksize);
            resultMedianBlur.push_back(tmp);
            tmp = Mat(img.rows, img.cols, CV_8UC1);
            cv::medianBlur(gausNoiseImgs[j], tmp, ksize);
            resultMedianBlur.push_back(tmp);

            tmp = Mat(img.rows, img.cols, CV_8UC1);
            cv::GaussianBlur(myLenaNoiseImgs[j], tmp, cv::Size(ksize, ksize), 0);
            resultGaussBlur.push_back(tmp);
            tmp = Mat(img.rows, img.cols, CV_8UC1);
            cv::GaussianBlur(gausNoiseImgs[j], tmp, cv::Size(ksize, ksize), 0);
            resultGaussBlur.push_back(tmp);
        }
    }
    const std::string filterMyNoise = "FilterMyNoise";
    const std::string filterGausNoise = "FilterGausNoise";
    for (int i = 0; i < 6; i++) {
        std::vector<Mat> blurMerge;
        std::vector<Mat> medianMerge;
        std::vector<Mat> gausMerge;
        for (int j = 0; j < 3; j++) {
            blurMerge.push_back(resultBlur[j * 6 + i]);
            medianMerge.push_back(resultMedianBlur[j * 6 + i]);
            gausMerge.push_back(resultGaussBlur[j * 6 + i]);
        }
        std::string nameFile = (i % 2 == 0) ? filterMyNoise : filterGausNoise;
        stream << nameFile;
        stream << i / 2 + 1;
        stream << ".jpg";
        Mat allBlur = merge(blurMerge);
        Mat allMedian = merge(medianMerge);
        Mat allGaus = merge(gausMerge);
        std::vector<Mat> arg;
        arg.push_back(allBlur);
        arg.push_back(allGaus);
        arg.push_back(allMedian);
        Mat all = mergeVertical(arg);
        imshow(stream.str(), all);
        imwrite(stream.str(), all);
        stream.str(std::string());
        stream.clear();
    }
    waitKey(0);
}


#endif	/* TASK5_H */

