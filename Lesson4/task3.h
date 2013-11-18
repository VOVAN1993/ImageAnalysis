// -*- C++ -*-
/* 
 * File:   task3.h
 * Author: vovan
 *
 * Created on 17 Ноябрь 2013 г., 19:23
 */

#ifndef TASK3_H
#define	TASK3_H
#include "util.h"

bool cmpPaird(pair<string, double> a, pair < string, double> b) {
    return a.second < b.second;
}

bool cmpP(Point a, Point b) {
    return a.y < b.y;
}
int norm_count_point=500;
//вычисление вектора признаков
vector<double> calc_vec_features(Mat src) {
    //сначала необходимо выделить контур в правильной последовательсности
    // основная идея : будем для y хранить вектор все x, таких что в точке (x,y) есть точка контура.
    //Далее оставляем только два x - самый левый  и самый правый.
    // Далее в итоговый контур записываем сначала подряд точки (x1,y) по возрастанию y, потом (x2,y) по убыванию y
    // При этом будем записывать сдвинутые относитьльно центра точки.
    map<int, vector<Point> > mmap_y;
    for (int i = 0; i < src.rows; i++) {
        uchar *Mi = src.ptr(i);
        for (int j = 0; j < src.cols; j++) {
            if (Mi[j] < 255) {
                if (mmap_y.find(i) == mmap_y.end()) {
                    vector<Point> tmp;
                    tmp.push_back(Point(i, j));
                    mmap_y[i] = tmp;
                } else {
                    mmap_y[i].push_back(Point(i, j));
                }
            }
        }
    }
    vector<Point> countur;
    Point center = Point(src.cols / 2, src.rows / 2);
    for (map<int, vector<Point> >::iterator it = mmap_y.begin(); it != mmap_y.end(); ++it) {
        if (it->second.size() > 2) {
            sort(it->second.begin(), it->second.end(), cmpP);
            vector<Point> tmp;
            tmp.push_back(it->second[0]);
            tmp.push_back(it->second[it->second.size() - 1]);
            it->second = tmp;
        } else if (it->second.size() == 1) {
            it->second.push_back(it->second[0]);
        }
        countur.push_back(it->second[0] - center);
    }
    for (map<int, vector<Point> >::reverse_iterator it = mmap_y.rbegin(); it != mmap_y.rend(); ++it) {
        countur.push_back(it->second[1] - center);
    }

    //формируем сигнатуру, равномерно (всего norm_count_point точек),записывая в виде x+i*y
    vector<complex<double> > sign;
    
    int step = countur.size() / norm_count_point;
    for (int i = 0; i < norm_count_point && i * step < countur.size(); i += 1) {//TODO
        sign.push_back(complex<double>(countur[i * step].x, countur[i * step].y));
    }
    //dft
    vector<complex<double> > fourier;
    dft(sign, fourier);
    //вычисляем вектор признаков
    vector<double> ret(fourier.size());
    for (int i = 0; i < fourier.size(); i++) {
        ret[i] = abs(abs(fourier[i]) / abs(fourier[0]));
    }
    return ret;
}

void task3() {
    DIR *dir = opendir("e1");
    Mat tmp;
    string s;
    int k = 0;
    vector<pair <string, vector<double> > > mmap;
    if (dir) {
        struct dirent *entry;
        while ((entry = readdir(dir)) != 0) {
            if (entry->d_type != 4) {
                tmp = imread("e1/" + string(entry->d_name), CV_LOAD_IMAGE_GRAYSCALE);
                mmap.push_back(make_pair(entry->d_name, calc_vec_features(tmp)));
            }
        }
    }
    vector< pair<string, double> > answer;
    for (int i = 0; i < mmap.size() - 1; i++) {
        for (int j = i + 1; j < mmap.size(); j++) {
            string name = mmap[i].first + " " + mmap[j].first;
            double diff=0.0;
            for(int ii=0;ii<mmap[i].second.size();ii++){
                diff+=cv::abs(mmap[i].second[ii]-mmap[j].second[ii]);
            }
            answer.push_back(make_pair(name, diff));
        }
    }
    sort(ALL(answer), cmpPaird);
    freopen("out/task3.txt", "w", stdout);
    for (int i = 0; i < answer.size(); ++i) {
        cout << answer[i].first<<"\n";
    }
    fclose(stdout);
}


#endif	/* TASK3_H */

