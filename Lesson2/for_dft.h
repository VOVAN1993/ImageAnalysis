/* 
 * File:   for_dft.h
 * Author: vovan
 *
 * Created on 21 Октябрь 2013 г., 12:25
 */

#ifndef FOR_DFT_H
#define	FOR_DFT_H
#include "util.h"

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

Mat updateMag(Mat& complex) {

    Mat magI = complex.clone();
    magI += Scalar::all(1);
    log(magI, magI);

    shift(magI);
    normalize(magI, magI, 1, 0, NORM_INF); // Transform the matrix with float values into a
    magI.convertTo(magI,CV_8UC1,255);
    return magI;
}
#endif	/* FOR_DFT_H */

