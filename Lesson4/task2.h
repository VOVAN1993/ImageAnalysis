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
std::pair<string,vector<ll> > first_elem;
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
bool cmpEnergy(std::pair<string,vector<ll> > a,std::pair<string,vector<ll> > b){
    ll res_a=0,res_b=0;
    for(int i=0;i<a.second.size();++i){
        res_a+=a.second[i]-first_elem.second[i];
        res_b+=b.second[i]-first_elem.second[i];
    }
    res_a=abs(res_a);
    res_b=abs(res_b);
    return res_a<res_b;
}

void task2() {
    vector<Mat> gabors;
    for (int i = 0; i <= 180; i += 45) {
        for (int j = 0; j <= 100; j += 25) {
            for (int k = 3; k <= 8; k++) {
                for(int h=10;h<=100;h+=20){
                Mat tmp = getGabor(i, k, j, h, 21);
                gabors.push_back(tmp);
            }
        }
    }
    }
    vector<std::pair<string,long long> > ans;
    vector<std::pair<string,vector<ll> > > mmap;
    DIR *dir = opendir(PATH_TO_COREL.c_str());
    vector<string> names;
    if (dir) {
        struct dirent *entry;
        while ((entry = readdir(dir)) != 0) {
            if (entry->d_type != 4) {
                string name = entry->d_name;
                names.push_back(name);
                Mat imageBGR = imread(PATH_TO_COREL + '/' + name);
                cv::Mat img_gray1;
                cv::cvtColor(imageBGR, img_gray1, CV_BGR2GRAY);
                img_gray1.convertTo(imageBGR, CV_32F, 1.0 / 255.0);
                vector<ll> tmpEnergy(gabors.size());
                ll ret = 0;
                for (int i = 0; i < gabors.size(); i++) {
                    Mat dest;
                    cv::filter2D(imageBGR, dest, CV_32F, gabors[i]);
                    cv::pow(dest, 2, dest);
                    dest.convertTo(dest, CV_8UC1, 255);
                    ll sum = cv::sum(dest)[0];
                    tmpEnergy[i]=sum;
                }
                mmap.push_back(make_pair(name,tmpEnergy));
                cout<<name<<"\n";
            }
        }
    }
    stringstream answer;
    for(int i=0;i<names.size();i++){
        string name=names[i];
        int j=0;
        for(;j<mmap.size();j++){
            if(mmap[j].first!=name)continue;
            break;
        }
        std::swap(mmap[j],mmap[0]);
        first_elem=mmap[0];
        sort(ALL(mmap),cmpEnergy);
        answer<<"for "<<first_elem.first<<":::\n";
        for(int k=0;k<mmap.size();k++){
            answer<<mmap[k].first<<"\n";
        }
        answer<<"---------------------\n";
        cout<<i<<"\n";
    }
    cout<<answer.str();
    freopen("res1.txt", "w", stdout);
    cout<<answer.str();
    fclose(stdout);
    waitKey(0);
}


#endif	/* TASK2_H */

