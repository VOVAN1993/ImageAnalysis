#include <opencv/cv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/stitching.hpp>
#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <utility>
#include <stdexcept>
#include <time.h>       
using namespace cv;

void linearFilter(const Mat &src, Mat &dst, const double alpha, const double beta) {
    int cols = src.cols, rows = src.rows;
    for (int i = 0; i < rows; i++) {
        const uchar* Mi = src.ptr<uchar > (i);
        uchar* Ni = dst.ptr<uchar > (i);
        for (int j = 0; j < 3 * cols; j++) {
            Ni[j] = cv::saturate_cast<uchar > (alpha * (Mi[j]) + beta);
        }
    }
}

void addNoise(Mat& chan,const double p,const double q){
    const int int_p = (int)(1000*p);
    const int int_q = (int)(1000*q);
    srand(time(NULL));
    for(uint i=0;i<chan.rows;i++){
        uchar *Mi = chan.ptr(i);
        for(uint j=0;j<chan.cols;j++){
            bool b1 = (rand()%1000+1)<=int_p;
            if(b1)Mi[j]=0;
            b1 = (rand()%1000+1)<=int_q;
            if(b1)Mi[j]=255;
        }
    }
}
void task1(const Mat &image) {
    std::vector<Mat> dsts;
    for (int i = 0; i < 5; i++) {
        dsts.push_back(Mat::zeros(image.size(), image.type()));
    }

    std::vector<std::pair<double, int> > arrParam;
    arrParam.push_back(std::make_pair(0.5, 0));
    arrParam.push_back(std::make_pair(1.0, -125));
    arrParam.push_back(std::make_pair(2.0, 0));
    arrParam.push_back(std::make_pair(2.0, -255));
    arrParam.push_back(std::make_pair(1.0, 125));

    for (int i = 0; i < 5; i++) {
        linearFilter(image, dsts[i], arrParam[i].first, arrParam[i].second);
    }
    imshow("SRC", image);
    std::stringstream stream;
    for (int i = 0; i < 5; i++) {
        stream << "result ";
        stream << i + 1;
        imshow(stream.str(), dsts[i]);
        stream.str(std::string());
        stream.clear();
    }
    waitKey(0);
}
Mat mergeVertical(const std::vector<Mat>& imgs){
    for (uint i = 1; i < imgs.size(); i++) {
        if (imgs[i].size() != imgs[i - 1].size()) {
            throw std::invalid_argument("Images must be the same size");
        }
    }
    Mat imgResult(imgs.size() * imgs[0].rows, imgs[0].cols, imgs[0].type());
    std::vector<Mat> imgPartRec;
    for (int i = 0; i < imgs.size(); i++) {
        imgPartRec.push_back(imgResult(Rect(0, i*imgs[0].rows, imgs[0].cols, imgs[0].rows)));
    }
    std::vector<Mat> imgPart;
    for (int i = 0; i < imgs.size(); i++) {
        imgPart.push_back(imgs[i](Rect(0, 0, imgs[i].cols, imgs[i].rows)));
        imgPart[i].copyTo(imgPartRec[i]);
    }
    return imgResult;
}
Mat merge(const std::vector<Mat>& imgs) {
    for (uint i = 1; i < imgs.size(); i++) {
        if (imgs[i].size() != imgs[i - 1].size()) {
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

void task2(const Mat& image) {
    Mat gray = Mat::zeros(image.size(), image.type());
    cvtColor(image, gray, CV_BGR2GRAY);
    Mat dst_eq = Mat::zeros(gray.size(), gray.type());
    Mat dst_norm = Mat::zeros(gray.size(), gray.type());
    Mat temp = Mat::zeros(gray.size(), gray.type());
    Mat result = Mat::zeros(gray.size(), gray.type());


    cv::equalizeHist(gray, dst_eq);

    normalize(gray, dst_norm, 0, 255, NORM_MINMAX, CV_8UC1);
    absdiff(dst_norm, dst_eq, result);
    std::vector<Mat> v;
    v.push_back(dst_eq);
    v.push_back(dst_norm);
    v.push_back(result);
    Mat allImgs = merge(v);

    imwrite("Norm&Equal.jpg", allImgs);
    imshow("all", allImgs);

    waitKey(0);
}

void task3(Mat& image,double p,double q) {
    std::vector<std::pair<double,double> > arg;
    arg.push_back(std::make_pair(0.05,0.05));
    arg.push_back(std::make_pair(0.1,0.1));
    arg.push_back(std::make_pair(0.15,0.15));
    
    std::vector<Mat> chan;
    split(image,chan);
    std::stringstream stream;
    for(int j=0;j<arg.size();j++){
        for(int i=0;i<chan.size();i++){
            addNoise(chan[i],arg[j].first,arg[j].second);
        }
        Mat img_result;
        cv::merge(chan,img_result);
 
        stream << "MyNoiseLena";
        stream << j+1;
        stream <<".jpg";

        imwrite(stream.str(),img_result);
        stream.str(std::string());
        stream.clear();
    }   
}

void addGausNoise(Mat& src,int mean,int stddev){
    Mat noise = Mat(src.size(),CV_8UC1);
    cv::randn(noise,mean,stddev);
    for(int i=0;i<src.rows;i++){
        uchar *Mi = src.ptr(i);
        const uchar *Ni = noise.ptr(i);
        for(int j=0;j<src.cols;j++){
            if(Ni[j]!=0){
                Mi[j]=Ni[j];
            }
        }
    }
}
void task4(Mat& image){
//    mean=0, stddev=30,
    std::vector<std::pair<int,int> > arg;
//    1) mean=0, stddev=30, 2) mean=0, stddev=60, 3) mean=50, stddev=30. 
    arg.push_back(std::make_pair(0,30));
    arg.push_back(std::make_pair(0,60));
    arg.push_back(std::make_pair(50,30));
    
    std::vector<Mat> ch;
    split(image,ch);
    Mat result;
    std::stringstream stream;
    for(int j=0;j<arg.size();j++){
        for(int i=0;i<ch.size();i++){
            addGausNoise(ch[i],arg[i].first,arg[i].second);
        }   
        merge(ch,result);
        stream<<"GausNoise";
        stream<<j+1;
        stream<<".jpg";
        imwrite(stream.str(),result);
        stream.str(std::string());
        stream.clear();
//        imshow(stream.str(),result);
    }
    waitKey(0);
}

void task5(Mat& img){
    std::vector<Mat> gausNoiseImgs;
    std::vector<Mat> myLenaNoiseImgs;
    std::stringstream stream;
    for(int i=0;i<3;i++){
        stream<<"GausNoise";
        stream<<i+1;
        stream<<".jpg";
        Mat tmp = imread(stream.str());
        gausNoiseImgs.push_back(tmp);
        stream.str(std::string());
        stream.clear();
    }
    for(int i=0;i<3;i++){
        stream<<"MyNoiseLena";
        stream<<i+1;
        stream<<".jpg";
        Mat tmp = imread(stream.str());
        myLenaNoiseImgs.push_back(tmp);
        stream.str(std::string());
        stream.clear();
    }
    
    //blur
    std::vector<Mat> resultBlur;
    std::vector<Mat> resultMedianBlur;
    std::vector<Mat> resultGaussBlur;
    
//    for(int i=0;i<6;i++){
//        resultBlur.push_back(tmp);
//        resultMedianBlur.push_back(tmp);
//    }
    for(int j=0;j<3;j++){
        for(int i=0;i<3;i++){
            int ksize = 3+2*i;
            Mat tmp = Mat(img.rows, img.cols, CV_8UC1);
            cv::blur(myLenaNoiseImgs[j],tmp,cv::Size(ksize,ksize));
            resultBlur.push_back(tmp);
            tmp = Mat(img.rows, img.cols, CV_8UC1);
            cv::blur(gausNoiseImgs[j],tmp,cv::Size(ksize,ksize));
            resultBlur.push_back(tmp);
            
            tmp = Mat(img.rows, img.cols, CV_8UC1);
            cv::medianBlur(myLenaNoiseImgs[j],tmp,ksize);
            resultMedianBlur.push_back(tmp);
            tmp = Mat(img.rows, img.cols, CV_8UC1);
            cv::medianBlur(gausNoiseImgs[j],tmp,ksize);
            resultMedianBlur.push_back(tmp);
            
            tmp = Mat(img.rows, img.cols, CV_8UC1);
            cv::GaussianBlur(myLenaNoiseImgs[j],tmp,cv::Size(ksize,ksize),0);  
            resultGaussBlur.push_back(tmp);
            tmp = Mat(img.rows, img.cols, CV_8UC1);
            cv::GaussianBlur(gausNoiseImgs[j],tmp,cv::Size(ksize,ksize),0);  
            resultGaussBlur.push_back(tmp);
        }
    }
    const std::string filterMyNoise="FilterMyNoise";
    const std::string filterGausNoise="FilterGausNoise";
    for(int i=0;i<6;i++){
        std::vector<Mat> blurMerge;
        std::vector<Mat> medianMerge;
        std::vector<Mat> gausMerge;
        for(int j=0;j<3;j++){
            blurMerge.push_back(resultBlur[j*6+i]);
            medianMerge.push_back(resultMedianBlur[j*6+i]);
            gausMerge.push_back(resultGaussBlur[j*6+i]);
        }
        std::string nameFile = (i%2==0)?filterMyNoise:filterGausNoise;
        stream<<nameFile;
        stream<<i/2+1;
        stream<<".jpg";
        Mat allBlur = merge(blurMerge);
        Mat allMedian = merge(medianMerge);
        Mat allGaus = merge(gausMerge);
        std::vector<Mat> arg;
        arg.push_back(allBlur);
        arg.push_back(allGaus);
        arg.push_back(allMedian);
        Mat all = mergeVertical(arg);
        imshow(stream.str(),all);
        imwrite(stream.str(),all);
        stream.str(std::string());
        stream.clear();
    }
    waitKey(0);
}
int main(int argc, char** argv) {
    Mat src = imread("Lena.jpg");
    task5(src);
    
    
    return 0;
}

