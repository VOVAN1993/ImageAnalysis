// -*- C++ -*-
/* 
 * File:   task2.h
 * Author: vovan
 *
 * Created on 15 Ноябрь 2013 г., 16:33
 */

#ifndef TASK2_H
#define	TASK2_H
#include "util.h"
#include "tools.h"

cv::Mat getKernel(int ks, double sig, double th, double lm, double ps) {
    int hks = (ks - 1) / 2;
    double theta = th * CV_PI / 180;
    double psi = ps * CV_PI / 180;
    double del = 2.0 / (ks - 1);
    double lmbd = lm;
    double sigma = sig / ks;
    double x_theta;
    double y_theta;
    cv::Mat kernel(ks, ks, CV_32F);
    for (int y = -hks; y <= hks; y++) {
        for (int x = -hks; x <= hks; x++) {
            x_theta = x * del * cos(theta) + y * del * sin(theta);
            y_theta = -x * del * sin(theta) + y * del * cos(theta);
            kernel.at<float>(hks + y, hks + x) = (float) exp(-0.5 * (pow(x_theta, 2) + pow(y_theta, 2)) / pow(sigma, 2)) * cos(2 * CV_PI * x_theta / lmbd + psi);
        }
    }
    return kernel;
}

Mat getGabor(double pos_th, double pos_sigma, double pos_lm, double pos_psi, int kernel_size) {
    double sig = pos_sigma;
    double lm = 0.5 + pos_lm / 100.0;
    double th = pos_th;
    double ps = pos_psi;
    cv::Mat kernel = getKernel(kernel_size, sig, th, lm, ps);
    return kernel;
}

ll diffEnergy(std::pair<string, vector<ll> > a, std::pair<string, vector<ll> > b) {
    ll res = 0;
    for (int i = 0; i < a.second.size(); ++i) {
        res += a.second[i] - b.second[i];
    }
    res = abs(res);
    return res;
}

bool cmpPair(pair<string, ll>a, pair<string, ll>b) {
    return a.second < b.second;
}

void task2() {
    vector<Mat> gabors;
    //формируем банк фильтров габора, меняя параметры тэта,лямбда,пси
    for (int i = 0; i <= 180; i += 45) {
        for (int j = 0; j <= 100; j += 50) {
            for (int k = 20; k <= 100; k+=30) {
                Mat tmp = getGabor(i, 6, j, k, 21);
                gabors.push_back(tmp);
            }
        }
    }
    vector<std::pair<string, ll> > ans;
    vector<std::pair<string, vector<ll> > > mmap;
    DIR *dir = opendir(PATH_TO_BR.c_str());
    vector<string> names;
    if (dir) {
        struct dirent *entry;
        while ((entry = readdir(dir)) != 0) {
            if (entry->d_type != 4) {
                string name = entry->d_name;
                names.push_back(name);
                
                Mat imageBGR = imread(PATH_TO_BR + '/' + name);
                cv::Mat img_gray1;
                cv::cvtColor(imageBGR, img_gray1, CV_BGR2GRAY);
                img_gray1.convertTo(imageBGR, CV_32F, 1.0 / 255.0);
                vector<ll> tmpEnergy(gabors.size());
                //применяем к изображению все фильтры из банка
                ll ret = 0;
                for (int i = 0; i < gabors.size(); i++) {
                    Mat dest;
                    cv::filter2D(imageBGR, dest, CV_32F, gabors[i]);
                    for (int ii = 0; ii < dest.rows; ii++) {
                        float *Mi = dest.ptr<float>(ii);
                        for (int j = 0; j < dest.cols; j++) {
                            if (Mi[j] < 0.0)Mi[j] = 0.0;
                        }
                    }
                    //нормализуем
                    normalize(dest, dest, 0.0, 1.0, NORM_MINMAX, CV_32F);
                    //считаем энергию (по ней будем сравнивать)
                    cv::pow(dest, 2, dest);
                    dest.convertTo(dest, CV_8UC1, 255);
                    ll sum = cv::sum(dest)[0];
                    tmpEnergy[i] = sum;
                }
                mmap.push_back(make_pair(name, tmpEnergy));
            }
        }
    }
    //компануем пары и сортируем
    vector<pair<string, ll> > toFile;
    for (int i = 0; i < mmap.size() - 1; i++) {
        string name1 = mmap[i].first;
        for (int j = i + 1; j < mmap.size(); j++) {
            string name2 = mmap[j].first;
            ll diff = diffEnergy(mmap[i], mmap[j]);
            pair<string, ll> ppair = make_pair(name1 + "::" + name2, diff);
            toFile.push_back(ppair);
        }
    }
    sort(ALL(toFile), cmpPair);
    freopen("out/task2.txt", "w", stdout);
    for (int i = 0; i < toFile.size(); i++) {
        cout << toFile[i].first << "\n";
    }
    fclose(stdout);
}


#endif	/* TASK2_H */

