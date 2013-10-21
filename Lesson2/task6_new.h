/* 
 * File:   task6_new.h
 * Author: vovan
 *
 * Created on 21 Октябрь 2013 г., 16:12
 */

#ifndef TASK6_NEW_H
#define	TASK6_NEW_H
#include "tools.h"
#include "util.h"


void highOrLowPass(cv::Mat& src, int radius,int flag) {

//1=>high
    int mx = src.cols / 2;
    int my = src.rows / 2;
    int tmpx, tmpy;
    int r2 = radius * radius;

    for (int i = 0; i < src.rows; ++i) {
        float *Mi = src.ptr<float>(i);
        tmpy = my - i;
        for (int j = 0; j < src.cols; ++j) {
            tmpx = mx - j;
            if (flag) {
                if (tmpy * tmpy + tmpx * tmpx < r2) 
                    Mi[j] = 0.0;
            } else {
                if (tmpy * tmpy + tmpx * tmpx >= r2) 
                    Mi[j] = 0.0;
            }
        }
    }

}

std::vector<Mat> transformSpectrum(const Mat& image, int R, int flag) {
    std::vector<Mat> result;

    Mat local_src = image.clone(), temp;
    result.push_back(local_src);
    local_src.convertTo(local_src, CV_32FC1);
    std::vector<Mat> planes(2, Mat());
    std::vector<Mat> polar(2, Mat());

    cv::dft(local_src, temp, cv::DFT_COMPLEX_OUTPUT);
    cv::split(temp, planes);
    cv::cartToPolar(planes[0], planes[1], polar[0], polar[1]);
    temp = updateMag(polar[0]);
    result.push_back(temp);

    shift(polar[0]);
    highOrLowPass(polar[0],R,(int)(flag==1));
    shift(polar[0]);

    temp = updateMag(polar[0]);

    result.push_back(temp);

    cv::polarToCart(polar[0], polar[1], planes[0], planes[1]);
    cv::merge(planes, temp);
    idft(temp, temp, cv::DFT_SCALE | cv::DFT_REAL_OUTPUT);
    temp.convertTo(temp, CV_8U);

    result.push_back(temp);
    return result;
}

void task6_new(const Mat& src) {
    std::vector<int> arg;
    arg.push_back(5);
    arg.push_back(15);
    arg.push_back(30);
    arg.push_back(255);
    arg.push_back(380);
    std::stringstream stream;
    for (int i = 0; i < arg.size(); i++) {
        for (int j = 0; j <= 1; j++) {
            std::vector<Mat> result = transformSpectrum(src, arg[i], j);
            stream << "Task6_R=";
            stream << arg[i];
            if (j)
                stream << "LowPass";
            else
                stream << "HighPass";
            stream << ".jpg";
            imwrite(stream.str(), merge(result));
            stream.str(std::string());
            stream.clear();
        }

    }
}
#endif	/* TASK6_NEW_H */

