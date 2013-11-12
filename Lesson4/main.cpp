//#define CV_PIXEL(type,img,x,y) (((type*)(img->imageData+(y)*img->widthStep))+(x)*img->nChannels)
//
//#include <opencv2/core/cvdef.h>
//
//#include <opencv/cv.hpp>
//#include <opencv2/highgui.hpp>
//#include <opencv2/stitching.hpp>
//#include <stdio.h>
//#include <algorithm>
//#include <iostream>
//#include <vector>
//#include <utility>
//#include <stdexcept>
//#include <time.h>
//#include <opencv2/highgui/highgui_c.h>   
//#include <algorithm>
//#include <map>
//#include <math.h>
//#include <algorithm>
//#include "opencv2/opencv.hpp"
//#include "opencv2/core/core.hpp"
//#include "opencv2/features2d/features2d.hpp"
//
//
//#include <sys/types.h>
//#include <dirent.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <iostream>
////контейнер для храниения имени и растояния.
//template<class T>
//class Image {
//private:
//	Image(const Image& i);
//
//public:
//	T distance;
//	char *name;
//	Image(T dist, const char* _name) :
//			distance(dist) {
//		name = new char[256];
//		strcpy(name, _name);
//	}
//	~Image() {
//		delete[] name;
//	}
//};
//
//
///**
// * функция постоения цветовой гистограммы изоб. в hsv.
// * kolX Y Z количество промеутков для каждой оси прострамства
// * histogram - масиив куда будет записанна гистограмма, размер должен быть kolX*kolY*kolZ
// *
// */
//void buildHistogram(IplImage* src, const uchar kolX, const uchar kolY,
//		const uchar kolZ, int* histogram) {
//	const int len = kolX * kolY * kolZ;
//	const int lenX = 256 / kolX;
//	const int lenY = 256 / kolY;
//	const int lenZ = 256 / kolZ;
//
//	for (int i = 0; i < len; ++i) {
//		histogram[i] = 0;
//	}
//
//	for (int x = 0; x < src->width; ++x) {
//		for (int y = 0; y < src->height; ++y) {
//			uchar* pixel = CV_PIXEL(uchar, src, x, y);
//			int h = pixel[2] / lenX;
//			int s = pixel[1] / lenY;
//			int v = pixel[0] / lenZ;
//			int pos = v * kolX * kolY + s * kolX + h;
//			++histogram[pos];
//		}
//	}
//        
//        for(int i=0;i<len;i++){
//            std::cout<<histogram[i]<<" ";
//        }
//
//}
//
///**
// * сравнивает 2 гистограммы по L1.
// * len - длина  гистограмы
// *
// *
// */
//double compareByL1(double * histogram1, double *histogram2, int len) {
//	 double res = 0;
//	for (int i = 0; i < len; ++i) {
//		res +=std::min(histogram1[i],histogram2[i]);
//	}
//	return 1 - res;
//}
///**
// * сравнивает 2 гистограммы по X2.
// * len - длина  гистограмы
// *
// */
//double compareByX2(double * histogram1, double *histogram2,int len) {
//	double res = 0;
//	for (int i = 0; i < len; ++i) {
//		if (histogram1[i] + histogram2[i] != 0) {
//			res += pow(histogram1[i] - histogram2[i], 2)
//					/ (histogram1[i] + histogram2[i]);
//		}
//	}
//	return res*0.5;
//}
//
//void normalizeHist(double* nHist, int *hist, int len, int div) {
//    std::cout<<div<<"\n";
//	for (int i = 0; i < len; ++i) {
//		double tmp = hist[i];
//		nHist[i] =  tmp/ div;
//	}
//}
//
//bool compatator(Image<double>* i1, Image<double> *i2) {
//	return i1->distance < i2->distance;
//}
//
//const char*PATH = "Corel";
//const char*NAME_SAMPLE_IMAGE = "Corel/TN_28008.JPG";
//const char* OUT_FILE1 = "L1.txt";
//const char* OUT_FILE2 = "X2.txt";
//const int K_X = 8; //количество промежутков по X,Y,Z
//const int K_Y = 8;
//const int K_Z = 8;
//int main() {
//	const int len = K_X * K_Y * K_Z;
//	std::vector<Image<double>*> imagesL1; //вектор содержащий растояния по L1
//	std::vector<Image<double>*> imagesX2; //вектор содержащий растояния по X2
//	IplImage* imageBGR = cvLoadImage(NAME_SAMPLE_IMAGE, CV_LOAD_IMAGE_COLOR);
//	if (imageBGR != 0) {
//		double* nHistSample = new double[len]; // нормированная гистограмма изображения от которого считаем растояния
//		double* nHist = new double[len];
//		int* hist = new int[len]; //гистограмма
//		IplImage* imageHSV = cvCreateImage(cvGetSize(imageBGR), imageBGR->depth,
//				imageBGR->nChannels);
//		cvCvtColor(imageBGR, imageHSV, CV_BGR2HSV);
//		buildHistogram(imageHSV, K_X, K_Y, K_Y, hist); //заполняем гистограмму изображения от которого мы будем считать растояния
//		std::vector<int> v1;
//                for(int i=0;i<len;++i){
//                    v1.push_back(hist[i]);
//                }
//                std::cout<<"\n";
//                normalizeHist(nHistSample,hist,len,imageHSV->height*imageHSV->width);//нормируем
//                std::vector<int> v2;
////                for(int i=0;i<len;++i){
////                    v2.push_back(nHistSample[i]);
////                }
////                for(int i=0;i<len;++i){
////                    std::cout<<v1[i]-v2[i]<<"\n";
////                }
//		cvReleaseImage(&imageBGR);
//		cvReleaseImage(&imageHSV);
//
//
//		DIR *dir = opendir(PATH);
//		if (dir) {
//			struct dirent *ent;
//			while ((ent = readdir(dir)) != 0) { //итерируемя по всем файлм директории
//				char buf[256];
//				if (ent->d_type != 4) {
//					sprintf(buf, "%s/%s", PATH, ent->d_name);
//					imageBGR = cvLoadImage(buf, CV_LOAD_IMAGE_COLOR);
//					imageHSV = cvCreateImage(cvGetSize(imageBGR),
//							imageBGR->depth, imageBGR->nChannels);
//					cvCvtColor(imageBGR, imageHSV, CV_BGR2HSV);
//					buildHistogram(imageHSV, K_X, K_Y, K_Y, hist);
//					normalizeHist(nHist,hist,len,imageHSV->height*imageHSV->width);
//
//					imagesL1.push_back( //добавляем (растояние, имя файла)
//							new Image<double>(compareByL1(nHistSample, nHist, len),
//									ent->d_name));
//
//					imagesX2.push_back(
//							new Image<double>(compareByX2(nHistSample, nHist, len),
//									ent->d_name));
//
//					cvReleaseImage(&imageBGR);
//					cvReleaseImage(&imageHSV);
//				}
//			}
//		}
//		//сортируем
//		std::sort(imagesL1.begin(), imagesL1.end(), compatator);
//
//		std::sort(imagesX2.begin(), imagesX2.end(), compatator);
//		FILE * out1 = fopen(OUT_FILE1, "w");
//		FILE * out2 = fopen(OUT_FILE2, "w");
//		for (int i = 0; i < imagesL1.size(); ++i) {
//			fprintf(out1, "%s  \n", imagesL1[i]->name);
//			delete imagesL1[i];
//			fprintf(out2, "%s \n", imagesX2[i]->name);
//			delete imagesX2[i];
//		}
//		fclose(out1);
//		fclose(out2);
//		delete[] hist;
//		delete[] nHist;
//		delete[] nHistSample ;
//
//
//	}
//
//}
#include "task1.h"
int main(){
    task1(8,8,8);
    return 0;
}