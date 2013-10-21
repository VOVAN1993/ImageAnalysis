#include <opencv2/highgui/highgui_c.h>

#include "task1.h"
#include "task2.h"
#include "task3.h"
#include "task4.h"
#include "tools.h"
#include "task5.h"
#include "task3_1.h"
#include "task3_2.h"
#include "task3_3.h"
#include "task6.h"
#include "task3_6.h"

void concatImages(const cv::Mat& image1, const cv::Mat& image2, cv::Mat& dst) {
    CV_Assert(image1.rows == image2.rows);
    CV_Assert(image1.type() == image2.type());
    CV_Assert(image1.channels() == image2.channels());

    cv::Mat tmp(image1.rows, image1.cols + image2.cols, image1.type());
    cv::Mat tmp1 = tmp.colRange(0, image1.cols);
    cv::Mat tmp2 = tmp.colRange(image1.cols, tmp.cols);
    image1.copyTo(tmp1);
    image2.copyTo(tmp2);
    dst = tmp;
}


void rearrangeQuadrants(cv::Mat& image) {
    // rearrange the quadrants of Fourier image so that the origin is at the image center
    int cx = image.cols / 2;
    int cy = image.rows / 2;

    cv::Mat q0(image, cv::Rect(0, 0, cx, cy)); // Top-Left - Create a ROI per quadrant
    cv::Mat q1(image, cv::Rect(cx, 0, cx, cy)); // Top-Right
    cv::Mat q2(image, cv::Rect(0, cy, cx, cy)); // Bottom-Left
    cv::Mat q3(image, cv::Rect(cx, cy, cx, cy)); // Bottom-Right

    cv::Mat tmp;
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp); // swap quadrant (Top-Right with Bottom-Left)
    q2.copyTo(q1);
    tmp.copyTo(q2);

}

void lowPassFilter(cv::Mat& magnitude, int radius) {
    int cx = magnitude.cols / 2;
    int cy = magnitude.rows / 2;
    int curX, curY;
    int r2 = radius * radius;

    for (int i = 0; i < magnitude.rows; ++i) {
        curY = cy - i;
        for (int j = 0; j < magnitude.cols; ++j) {
            curX = cx - j;
            if (curY * curY + curX * curX >= r2) {
                magnitude.at<float>(i, j) = 0.0f;
            }
        }
    }
}

void highPassFilter(cv::Mat& magnitude, int radius) {
    int cx = magnitude.cols / 2;
    int cy = magnitude.rows / 2;
    int curX, curY;
    int r2 = radius * radius;

    for (int i = 0; i < magnitude.rows; ++i) {
        curY = cy - i;
        for (int j = 0; j < magnitude.cols; ++j) {
            curX = cx - j;
            if (curY * curY + curX * curX < r2) {
                magnitude.at<float>(i, j) = 0.0f;
            }
        }
    }
}

void visualization(const cv::Mat& magnitude, cv::Mat& res) {
    res = magnitude + cv::Scalar::all(1); // switch to logarithmic scale
    cv::log(res, res);
    rearrangeQuadrants(res);
    cv::normalize(res, res, 0, 1, CV_MINMAX);
    res *= 255;
    res.convertTo(res, CV_8U);
}

void performLowPass(const cv::Mat& image, cv::Mat& res, int rad) {
    cv::Mat grey, tmp;
    cv::cvtColor(image, grey, CV_BGR2GRAY);


    grey.convertTo(grey, CV_32F);
    grey.copyTo(res);
    res.convertTo(res, CV_8U);
    std::vector<cv::Mat> planes(2, cv::Mat());
    std::vector<cv::Mat> polar(2, cv::Mat());

    cv::dft(grey, tmp, cv::DFT_COMPLEX_OUTPUT);
    cv::split(tmp, planes);
    cv::cartToPolar(planes[0], planes[1], polar[0], polar[1]);
    visualization(polar[0], tmp);
    concatImages(res, tmp, res);

    rearrangeQuadrants(polar[0]);
    lowPassFilter(polar[0], rad);
    rearrangeQuadrants(polar[0]);

    visualization(polar[0], tmp);
    tmp.convertTo(tmp, res.type());
    concatImages(res, tmp, res);

    cv::polarToCart(polar[0], polar[1], planes[0], planes[1]);
    cv::merge(planes, tmp);
   // idft(tmp, tmp);
    cv::dft(tmp, tmp, cv::DFT_SCALE | cv::DFT_INVERSE | cv::DFT_REAL_OUTPUT);
    tmp.convertTo(tmp, CV_8U);
    concatImages(res, tmp, res);
}

void performHighPass(const cv::Mat& image, cv::Mat& res, int rad) {
    cv::Mat grey, tmp;
    cv::cvtColor(image, grey, CV_BGR2GRAY);


    grey.convertTo(grey, CV_32F);
    grey.copyTo(res);
    res.convertTo(res, CV_8U);
    std::vector<cv::Mat> planes(2, cv::Mat());
    std::vector<cv::Mat> polar(2, cv::Mat());

    cv::dft(grey, tmp, cv::DFT_COMPLEX_OUTPUT);
    cv::split(tmp, planes);
    cv::cartToPolar(planes[0], planes[1], polar[0], polar[1]);
    visualization(polar[0], tmp);
    concatImages(res, tmp, res);

    rearrangeQuadrants(polar[0]);
    highPassFilter(polar[0], rad);
    rearrangeQuadrants(polar[0]);

    visualization(polar[0], tmp);
    tmp.convertTo(tmp, res.type());
    concatImages(res, tmp, res);

    cv::polarToCart(polar[0], polar[1], planes[0], planes[1]);
    cv::merge(planes, tmp);
    cv::dft(tmp, tmp, cv::DFT_SCALE | cv::DFT_INVERSE | cv::DFT_REAL_OUTPUT);
    tmp.convertTo(tmp, CV_8U);
    concatImages(res, tmp, res);
}

bool task66(const cv::Mat& image) {
    int suff = 1;
    bool res = true;

        cv::Mat tmp1, tmp2;
        performLowPass(image, tmp1, 6);
        imshow("stas",tmp1);
//        res &= cv::imwrite(PATH + generateName("Task6-", suff++), tmp1);
//        performHighPass(image, tmp2, 60);
//        imshow("stas",tmp2);
//        res &= cv::imwrite(PATH + generateName("Task6-", suff++), tmp2);
    return res;
}

void task3_5(Mat& src, Mat& dst){
    
    Mat complex = computeDFT(src);
    Mat spec = updateMag(complex); // show spectrum
    std::cout<<spec.type()<<" "<<CV_32FC1<<" \n";
    spec.convertTo(spec,CV_32FC1,255.0);
    spec.convertTo(spec,CV_8UC1);
    
    Size src_size = src.size();
  
    Mat local_src = src.clone();
    Mat mask = createMask(src_size, 60, 5, src_size.height/2, src_size.width/2,1);
    imshow("asd",mask);
    Mat kernel_spec;
    std::vector<Mat> planes(2);
    shift(mask);
    planes[0] = mask; // real
    planes[1] = mask; // imaginar
    merge(planes, kernel_spec);
    std::cout<<kernel_spec.type()<<" ";
    mulSpectrums(complex, kernel_spec, local_src, DFT_ROWS);
    Mat mag = updateMag(local_src);
    Mat res = updateResult(local_src);
            
    res.convertTo(res,CV_8UC1,255);
    
    
}




int main(int argc, char** argv) {
    //Mat mask = createGausFilterMask(Size(512,512), 256, 256, 510, true, true);
    Mat src = imread("lena_gray_512.jpg");
//    Mat src = imread("lena.jpg");
//    Mat src = imread("lena_color_512-noise.tif");
//    Mat src = imread("lena_diagonal.jpg",CV_LOAD_IMAGE_GRAYSCALE);
//    Mat good = imread("lena_gray_512.tif",CV_LOAD_IMAGE_GRAYSCALE);
//    task3_5(src,good);
    task6(src);
    

    waitKey(0);
    return 0;
}
