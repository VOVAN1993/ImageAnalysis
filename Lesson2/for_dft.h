/* 
 * File:   for_dft.h
 * Author: vovan
 *
 * Created on 21 Октябрь 2013 г., 12:25
 */

#ifndef FOR_DFT_H
#define	FOR_DFT_H
#include "util.h"

Mat updateResult(Mat complex) {
    Mat work;
   // idft(complex, work);
    cv::dft(complex, work, cv::DFT_SCALE | cv::DFT_INVERSE | cv::DFT_REAL_OUTPUT);
    Mat planes[] = {Mat::zeros(complex.size(), CV_32F), Mat::zeros(complex.size(), CV_32F)};
    split(work, planes); // planes[0] = Re(DFT(I)), planes[1] = Im(DFT(I))

    magnitude(planes[0], planes[1], work); // === sqrt(Re(DFT(I))^2 + Im(DFT(I))^2)
    normalize(work, work, 0, 1, NORM_MINMAX);
    return work;
}

void shift(Mat magI) {

    magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));

    int cx = magI.cols / 2;
    int cy = magI.rows / 2;

    Mat q0(magI, Rect(0, 0, cx, cy)); // Top-Left - Create a ROI per quadrant
    Mat q1(magI, Rect(cx, 0, cx, cy)); // Top-Right
    Mat q2(magI, Rect(0, cy, cx, cy)); // Bottom-Left
    Mat q3(magI, Rect(cx, cy, cx, cy)); // Bottom-Right

    Mat tmp; // swap quadrants (Top-Left with Bottom-Right)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);
    q1.copyTo(tmp); // swap quadrant (Top-Right with Bottom-Left)
    q2.copyTo(q1);
    tmp.copyTo(q2);
}

Mat updateMag(Mat complex) {

    Mat magI;
    Mat planes[] = {Mat::zeros(complex.size(), CV_32F), Mat::zeros(complex.size(), CV_32F)};
    split(complex, planes); // planes[0] = Re(DFT(I)), planes[1] = Im(DFT(I))

    magnitude(planes[0], planes[1], magI); // sqrt(Re(DFT(I))^2 + Im(DFT(I))^2)

    // switch to logarithmic scale: log(1 + magnitude)
    magI += Scalar::all(1);
    log(magI, magI);

    shift(magI);
    normalize(magI, magI, 1, 0, NORM_INF); // Transform the matrix with float values into a
    return magI;
}



Mat computeDFT(Mat image) {
    Mat padded; //expand input image to optimal size
    int m = getOptimalDFTSize(image.rows);
    int n = getOptimalDFTSize(image.cols); // on the border add zero values
    copyMakeBorder(image, padded, 0, m - image.rows, 0, n - image.cols, BORDER_CONSTANT, Scalar::all(0));
    std::vector<Mat> planes;
    planes.push_back(Mat_<float>(padded));
    planes.push_back(Mat::zeros(padded.size(), CV_32F));
    Mat complex;
    merge(planes, complex); // Add to the expanded another plane with zeros
    dft(complex, complex, DFT_COMPLEX_OUTPUT); // furier transformъ
    std::cout<<complex.channels();
    return complex;
}

Mat implementFiltering(const Mat &src,int R,Size size,int flag,int y=0){
    Mat local_src = src.clone();
    Mat mask = createMask(size, R, 5, size.height / 2, size.width / 2,flag);
    imshow("asd",mask);
    Mat kernel_spec;
    std::vector<Mat> planes(2);
    shift(mask);
    planes[0] = mask; // real
    planes[1] = mask; // imaginar
    merge(planes, kernel_spec);
    mulSpectrums(local_src, kernel_spec, local_src, DFT_ROWS);
    Mat mag = updateMag(local_src);
    Mat res = updateResult(local_src);
    std::vector<Mat> params;
    params.push_back(mag);
    params.push_back(res);
    Mat ret_img = merge(params);
    return ret_img;
}

#endif	/* FOR_DFT_H */

