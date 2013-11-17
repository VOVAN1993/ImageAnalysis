/* 
 * File:   task1.h
 * Author: vovan
 *
 * Created on 11 Ноябрь 2013 г., 19:27
 */

#ifndef TASK1_H
#define	TASK1_H
#include "util.h"
const string name_L1=PATH_TO_OUT+"/"+"L1.txt";
const string name_X2=PATH_TO_OUT+"/"+"X2.txt";

vector<int> calculateHist(Mat src, const uchar kolX, const uchar kolY,
        const uchar kolZ) {
    
    const int lenX = 180 / kolX;
    const int lenY = 256 / kolY;
    const int lenZ = 256 / kolZ;
    const int len = kolX * kolY * (255/lenZ) + kolX*(255/lenY)+180/lenX +1 ;
    vector<int> histogram(len, 0);
 
    for (int x = 0; x < src.rows; ++x) {
        uchar *Mi = src.ptr(x);
        for (int y = 0; y < src.cols; ++y) {

            int h = Mi[3 * y] / lenX;
            int s = Mi[3 * y + 1] / lenY;
            int v = Mi[3 * y + 2] / lenZ;
            int pos = v * kolX * kolY + s * kolX + h;
            ++histogram[pos];
        }
    }

    return histogram;
}

double compareByL1(const vector<double>& histogram1, const vector<double>& histogram2) {
    double res = 0;
    for (int i = 0; i < histogram1.size(); ++i) {
        res += std::min(histogram1[i], histogram2[i]);
    }
    return 1.0 - res;
}

double compareByX2(const vector<double>& histogram1, const vector<double>& histogram2) {
    double res = 0;
    for (int i = 0; i < histogram1.size(); ++i) {
        if (histogram1[i] + histogram2[i] != 0) {
            res += pow(histogram1[i] - histogram2[i], 2)
                    / (histogram1[i] + histogram2[i]);
        }
    }
    return res * 0.5;
}

vector<double> normalizeHist(const vector<int>& hist, int div) {
    vector<double> nHist(hist.size());
    for (int i = 0; i < hist.size(); ++i) {
        nHist[i] = (double)hist[i] / (double)div;
    }
    return nHist;
}



void task1(uint Kx, uint Ky, uint Kz) {
    Mat src = imread(PATH_TO_COREL+string("/TN_28008.JPG"));
    Mat hsv;
    cvtColor(src, hsv, CV_BGR2HSV);
    vector<int> hist = calculateHist(hsv, Kx, Ky, Kz);

    vector<double> nHistSample(hist.size());
    nHistSample = normalizeHist(hist, hsv.rows * hsv.cols * hsv.channels());

    DIR *dir = opendir(PATH_TO_COREL.c_str());
    
    vector<Image1*> vecL11,vecX22; 
    if (dir) {
        struct dirent *entry;
        while ((entry = readdir(dir)) != 0) { 
            if (entry->d_type != 4) {
                string s=entry->d_name;

                Mat imageBGR = imread(PATH_TO_COREL+'/'+s,CV_LOAD_IMAGE_COLOR);
                Mat imageHSV;

                cvtColor(imageBGR, imageHSV, CV_BGR2HSV);
                vector<int> hist1 = calculateHist(imageHSV, Kx, Ky, Kx);
                vector<double> nHist1 = normalizeHist(hist1, imageHSV.rows * imageHSV.cols * imageHSV.channels());
                
                vecL11.push_back(
                        new Image1(compareByL1(nHistSample, nHist1),
                        string(entry->d_name)));

                vecX22.push_back(
                        new Image1(compareByX2(nHistSample, nHist1),
                        string(entry->d_name)));
            }
        }
    }

    std::sort(ALL(vecL11), cmpImage);
    std::sort(ALL(vecX22), cmpImage);
    print_vector_to_file(name_L1,vecL11);
    print_vector_to_file(name_X2,vecX22);

    waitKey(0);
}


#endif	/* TASK1_H */

