/* 
 * File:   util.h
 * Author: vovan
 *
 * Created on 21 Октябрь 2013 г., 11:54
 */

#ifndef UTIL_H
#define	UTIL_H
#include "tools.h"

void myConvert(const Mat& src, Mat& dst, double alpha, double beta) {
    dst = src * alpha + Scalar(beta, beta, beta);
}

Mat mergeVertical(const std::vector<Mat>& imgs) {
    for (uint i = 1; i < imgs.size(); i++) {
        if (imgs[i].size() != imgs[i - 1].size()) {
            throw std::invalid_argument("Images must be the same size");
        }
    }
    Mat imgResult(imgs.size() * imgs[0].rows, imgs[0].cols, imgs[0].type());
    std::vector<Mat> imgPartRec;
    for (int i = 0; i < imgs.size(); i++) {
        imgPartRec.push_back(imgResult(Rect(0, i * imgs[0].rows, imgs[0].cols, imgs[0].rows)));
    }
    std::vector<Mat> imgPart;
    for (int i = 0; i < imgs.size(); i++) {
        imgPart.push_back(imgs[i](Rect(0, 0, imgs[i].cols, imgs[i].rows)));
        imgPart[i].copyTo(imgPartRec[i]);
    }
    return imgResult;
}

Mat merge(const std::vector<Mat>& imgs) {
    //горизонтальное соединение изображений
    for (uint i = 1; i < imgs.size(); i++) {
        if (imgs[i].size() != imgs[i - 1].size() || imgs[i].size() != imgs[i - 1].size()) {
            throw std::invalid_argument("Images must be the same size");
        }
    }
    Mat imgResult(imgs[0].rows, imgs.size() * imgs[0].cols, imgs[0].type());
    std::vector<Mat> imgPartRec;
    for (int i = 0; i < imgs.size(); i++) {
        imgPartRec.push_back(imgResult(Rect(i * imgs[0].cols, 0, imgs[0].cols, imgs[0].rows)));
    }
    std::vector<Mat> imgPart;

    for (int i = 0; i < imgs.size(); i++) {
        imgPart.push_back(imgs[i](Rect(0, 0, imgs[i].cols, imgs[i].rows)));
        imgPart[i].copyTo(imgPartRec[i]);
    }
    return imgResult;
}

Mat concatCh(const Mat& src) {
    std::vector<Mat> channels;
    split(src, channels);
    Mat res = merge(channels);
    return res;
}

void addGausNoise(const Mat& src, Mat& dst, double mean, double stddev) {
    dst = src.clone();
    cv::Mat noise(dst.rows, dst.cols, dst.type(), cv::Scalar(0));
    cv::randn(noise, mean, stddev);
    dst += noise;
}

void addNoise(Mat& chan, const double p, const double q) {
    const int int_p = (int) (1000 * p);
    const int int_q = (int) (1000 * q);
    srand(time(NULL));
    for (uint i = 0; i < chan.rows; i++) {
        uchar *Mi = chan.ptr(i);
        for (uint j = 0; j < chan.cols; j++) {
            bool b1 = (rand() % 1000 + 1) <= int_p;
            if (b1)Mi[j] = 0;
            b1 = (rand() % 1000 + 1) <= int_q;
            if (b1)Mi[j] = 255;
        }
    }
}

Mat mythreshold(const Mat& src, const int arg) {
    Mat dst = Mat::zeros(src.size(), src.type());
    for (int i = 0; i < src.rows; i++) {
        const uchar *Mi = src.ptr(i);
        uchar *Ni = dst.ptr(i);
        for (int j = 0; j < src.cols; j++) {
            Ni[j] = ((int)Mi[j] <= arg) ? 0 : 255;
        }
    }
    return dst;
}

#endif	/* UTIL_H */

