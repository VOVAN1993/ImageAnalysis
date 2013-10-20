
#include <opencv2/core/cvdef.h>

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
#include <opencv2/highgui/highgui_c.h>       
using namespace cv;


void scaleImg(const Mat& src,Mat& dst,double alpha,double beta){
    dst = src*alpha+Scalar(beta,beta,beta);
}
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
    for (uint i = 1; i < imgs.size(); i++) {
        if (imgs[i].size() != imgs[i - 1].size() || imgs[i].size()!=imgs[i-1].size()) {
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
Mat concatCh(const Mat& src){
    std::vector<Mat> channels;
    split(src,channels);
    Mat res = merge(channels);
    return res;
}

void task1(const Mat &image) {
    std::vector<Mat> dsts,channels;
    Mat diff;
    for (int i = 0; i < 5; i++) {
        dsts.push_back(Mat::zeros(image.size(), image.type()));
    }
    
    Mat testCvt;
    image.convertTo(testCvt, -1, 2, 100);
    Mat testMyCvt = Mat::zeros(image.size(), image.type());
    scaleImg(image,testMyCvt,2,100);
    absdiff(testCvt,testMyCvt,diff);
    split(diff,channels);
    for(int i=0;i<channels.size();i++){
        int mmax = *(std::max_element(channels[i].begin<uchar>(), channels[i].end<uchar>()));
        std::cout<<mmax<<" ";
    }
    
    std::vector<std::pair<double, int> > arrParam;
    arrParam.push_back(std::make_pair(0.5, 0));
    arrParam.push_back(std::make_pair(1.0, -125));
    arrParam.push_back(std::make_pair(2.0, 0));
    arrParam.push_back(std::make_pair(2.0, -255));
    arrParam.push_back(std::make_pair(1.0, 125));

    for (int i = 0; i < arrParam.size(); i++) {
        scaleImg(image,dsts[i],arrParam[i].first,arrParam[i].second);
    }
    std::vector<Mat> param;
    param.push_back(concatCh(dsts[0]));
    param.push_back(concatCh(dsts[1]));
    Mat res1 = mergeVertical(param);
    param.clear();
    imwrite("Task1Img1+2.jpg",res1);
    param.push_back(concatCh(dsts[2]));
    param.push_back(concatCh(dsts[3]));
    param.push_back(concatCh(dsts[4]));
    Mat res2 = mergeVertical(param);
    imwrite("Task1Img3+4+5.jpg",res2);
    waitKey(0);
}


void task2(const Mat& image) {
    Mat gray,dst_eq,temp,diff,dst_norm;
    cvtColor(image, gray, CV_BGR2GRAY);

    cv::equalizeHist(gray, dst_eq);

    normalize(gray, dst_norm, 0, 255, NORM_MINMAX, CV_8UC1);
    absdiff(dst_norm, dst_eq, diff);
    std::vector<Mat> v;
    v.push_back(dst_norm);
    v.push_back(dst_eq);
    v.push_back(diff);
    Mat allImgs = merge(v);

    imwrite("Task2.jpg", allImgs);
    imshow("all", allImgs);

    waitKey(0);
}

void task3(Mat& image) {
    std::vector<std::pair<double, double> > arg;
    arg.push_back(std::make_pair(0.05, 0.05));
    arg.push_back(std::make_pair(0.1, 0.1));
    arg.push_back(std::make_pair(0.15, 0.15));

    std::vector<Mat> chan;
    split(image, chan);
    std::stringstream stream;
    for (int j = 0; j < arg.size(); j++) {
        for (int i = 0; i < chan.size(); i++) {
            addNoise(chan[i], arg[j].first, arg[j].second);
        }
        Mat img_result;
        cv::merge(chan, img_result);

        stream << "Task3_img_";
        stream << j + 1;
        stream << ".jpg";

        imwrite(stream.str(), img_result);
        stream.str(std::string());
        stream.clear();
    }
}

void addGausNoise(const Mat& src, Mat& dst, double mean, double stddev) {
    dst = src.clone();
    cv::Mat noise(dst.rows, dst.cols, dst.type(), cv::Scalar(0));
    cv::randn(noise, mean, stddev);
    dst += noise;
}

void task4(Mat& image) {
    std::vector<std::pair<double, double> > arg;
    arg.push_back(std::make_pair(0.0, 30.0));
    arg.push_back(std::make_pair(0.0, 60.0));
    arg.push_back(std::make_pair(50.0, 30.0));

    std::vector<Mat> ch;
    split(image, ch);
    Mat result;
    std::stringstream stream;
    for (int j = 0; j < arg.size(); j++) {
        std::vector<cv::Mat> newChan;
        for(int i=0;i<ch.size();i++){
            Mat tmp;
            addGausNoise(ch[i], tmp, arg[j].first,arg[j].second);
            newChan.push_back(tmp);
        }
        Mat result;
        merge(newChan, result);
 
        stream << "Task4img";
        stream << j + 1;
        stream << ".jpg";
        std::cout<<result.channels();
         cv::imwrite(stream.str(), result);
        stream.str(std::string());
        stream.clear();
    }
    waitKey(0);
}

void task5(Mat& img) {
    std::vector<Mat> gausNoiseImgs;
    std::vector<Mat> myLenaNoiseImgs;
    std::stringstream stream;
    for (int i = 0; i < 3; i++) {
        stream << "Task4img";
        stream << i + 1;
        stream << ".jpg";
        Mat tmp = imread(stream.str());
        gausNoiseImgs.push_back(tmp);
        stream.str(std::string());
        stream.clear();
    }
    for (int i = 0; i < 3; i++) {
        stream << "Task3_img_";
        stream << i + 1;
        stream << ".jpg";
        Mat tmp = imread(stream.str());
        myLenaNoiseImgs.push_back(tmp);
        stream.str(std::string());
        stream.clear();
    }

    //blur
    std::vector<Mat> resultBlur;
    std::vector<Mat> resultMedianBlur;
    std::vector<Mat> resultGaussBlur;

   
    for (int j = 0; j < 3; j++) {
        for (int i = 0; i < 3; i++) {
            int ksize = 3 + 2 * i;
            Mat tmp = Mat(img.rows, img.cols, CV_8UC1);
            cv::blur(myLenaNoiseImgs[j], tmp, cv::Size(ksize, ksize));
            resultBlur.push_back(tmp);
            tmp = Mat(img.rows, img.cols, CV_8UC1);
            cv::blur(gausNoiseImgs[j], tmp, cv::Size(ksize, ksize));
            resultBlur.push_back(tmp);

            tmp = Mat(img.rows, img.cols, CV_8UC1);
            cv::medianBlur(myLenaNoiseImgs[j], tmp, ksize);
            resultMedianBlur.push_back(tmp);
            tmp = Mat(img.rows, img.cols, CV_8UC1);
            cv::medianBlur(gausNoiseImgs[j], tmp, ksize);
            resultMedianBlur.push_back(tmp);

            tmp = Mat(img.rows, img.cols, CV_8UC1);
            cv::GaussianBlur(myLenaNoiseImgs[j], tmp, cv::Size(ksize, ksize), 0);
            resultGaussBlur.push_back(tmp);
            tmp = Mat(img.rows, img.cols, CV_8UC1);
            cv::GaussianBlur(gausNoiseImgs[j], tmp, cv::Size(ksize, ksize), 0);
            resultGaussBlur.push_back(tmp);
        }
    }
    const std::string filterMyNoise = "FilterMyNoise";
    const std::string filterGausNoise = "FilterGausNoise";
    for (int i = 0; i < 6; i++) {
        std::vector<Mat> blurMerge;
        std::vector<Mat> medianMerge;
        std::vector<Mat> gausMerge;
        for (int j = 0; j < 3; j++) {
            blurMerge.push_back(resultBlur[j * 6 + i]);
            medianMerge.push_back(resultMedianBlur[j * 6 + i]);
            gausMerge.push_back(resultGaussBlur[j * 6 + i]);
        }
        std::string nameFile = (i % 2 == 0) ? filterMyNoise : filterGausNoise;
        stream << nameFile;
        stream << i / 2 + 1;
        stream << ".jpg";
        Mat allBlur = merge(blurMerge);
        Mat allMedian = merge(medianMerge);
        Mat allGaus = merge(gausMerge);
        std::vector<Mat> arg;
        arg.push_back(allBlur);
        arg.push_back(allGaus);
        arg.push_back(allMedian);
        Mat all = mergeVertical(arg);
        imshow(stream.str(), all);
        imwrite(stream.str(), all);
        stream.str(std::string());
        stream.clear();
    }
    waitKey(0);
}

void task3_1(const Mat& src) {
    Mat floatImg;
    src.convertTo(floatImg, CV_32FC1, 1.0 / 255.0);
    
//    for (uint i = 0; i < src.rows; i++) {
//        const uchar* Mi = src.ptr(i);
//        float* Ni = floatImg.ptr<float>(i);
//        for (uint j = 0; j < src.cols; j++) {
//            Ni[j] = log(1 + Ni[j]);
//        }
//    }
    Mat result=Mat::zeros(floatImg.size(),CV_8UC1);
    //floatImg+=Scalar::all(1.0);
    
    cv::log(floatImg,floatImg);
    for(int i=0;i<floatImg.rows;i++){
        float* Mi = floatImg.ptr<float>(i);
        for(int j=0;j<floatImg.cols;j++){
            std::cout<<Mi[j]<<" \n";
        }
    }
//    floatImg.convertTo(floatImg, CV_32FC1, 255.0);
//    result.convertTo(result,CV_8UC1);
//    std::cout<<result.channels();
////
    imshow("LenaTask3_1.jpg", floatImg);
}

Mat powImg(const Mat& src, const int arg) {
    Mat floatImg,result;
    src.convertTo(floatImg, CV_32FC1, 1.0 / 255.0);
    for (uint i = 0; i < src.rows; i++) {
        const uchar* Mi = src.ptr(i);
        float* Ni = floatImg.ptr<float>(i);
        for (uint j = 0; j < src.cols; j++) {
            Ni[j] = std::pow(Ni[j], arg);
        }
    }
    floatImg.convertTo(result, CV_8UC1, 255.0);
    return result;
}

double calculationMSE(const Mat& img1, const Mat& img2) {
    double ans = 0.0;
    for (int i = 0; i < img1.rows; i++) {
        const uchar *Mi = img1.ptr(i);
        const uchar *Ni = img2.ptr(i);
        for (int j = 0; j < img1.cols; j++) {
            ans += pow((int) Mi[j]-(int) Ni[j], 2);
        }
    }
    ans /= img1.cols * img1.rows;
    ans = pow(ans, 2);
    return ans;
}

void task3_2(const Mat& src) {
    std::vector<int> arg;
    arg.push_back(2);
    arg.push_back(4);
    arg.push_back(6);
    std::stringstream stream;
    for (int i = 0; i < arg.size(); i++) {
        Mat res = powImg(src, arg[i]);
        stream << "LenaTask3_2_n=";
        stream << arg[i];
        stream << ".jpg";

        imshow(stream.str(), res);
        imwrite(stream.str(), res);
        stream.str(std::string());
        stream.clear();
    }
    //    waitKey(0);
}

Mat mythreshold(const Mat& src, const int arg) {
    Mat dst = Mat::zeros(src.size(), src.type());
    for (int i = 0; i < src.rows; i++) {
        const uchar *Mi = src.ptr(i);
        uchar *Ni = dst.ptr(i);
        for (int j = 0; j < src.cols; j++) {
            Ni[j] = Mi[j] <= arg ? 0 : 255;
        }
    }
    return dst;
}

void task3_3(const Mat& src) {
    std::vector<int> args;
    args.push_back(20);
    args.push_back(30);
    args.push_back(40);
    args.push_back(100);
    std::vector<Mat> result;
    for (int i = 0; i < args.size(); i++) {
        Mat tmp = mythreshold(src, args[i]);
        result.push_back(tmp);
    }
    for (int i = 0; i < args.size(); i++) {
        Mat tmp;
        threshold(src, tmp, args[i], 255, CV_THRESH_BINARY);
        cv::Mat diff = tmp != result[i];
        // Equal if no elements disagree
        bool eq = cv::countNonZero(diff) == 0;
        if (!eq) {
            throw std::invalid_argument("Error");
        }
    }
    Mat mergeResult = merge(result);
    imshow("mergeThreshold.jpg", mergeResult);
    waitKey(0);
}

Mat updateResult(Mat complex) {
    Mat work;
    idft(complex, work);
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
    dft(complex, complex, DFT_COMPLEX_OUTPUT); // furier transform
    return complex;
}

Mat rotateImage(const Mat& source, double angle) {
    Point2f src_center(source.cols / 2.0F, source.rows / 2.0F);
    Mat rot_mat = getRotationMatrix2D(src_center, angle, 1.0);
    Mat dst;
    warpAffine(source, dst, rot_mat, source.size());
    return dst;
}

Mat createMask(Size size, int R, int type, int mx, int my,int flag) {
    Mat mask = Mat::zeros(size, type);
    for (int i = 0; i < mask.rows; i++) {
        float *Mi = mask.ptr<float>(i);
        for (int j = 0; j < mask.cols; j++) {
            if(flag){
                if (pow(abs(mx - j), 2) + pow(abs(my - i), 2) <= R * R) {
                    Mi[j] = 1;
                }
            }else{
                if (pow(abs(mx - j), 2) + pow(abs(my - i), 2) >= R * R) {
                    Mi[j] = 1;
                }
            }
        }
    }
    return mask;
}
Mat implementFiltering(const Mat &src,int R,Size size,int flag){
    Mat local_src = src.clone();
    Mat mask = createMask(size, R, 5, size.height / 2, size.width / 2,flag);
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
void task6(Mat& src){
    std::vector<int> arg;
    arg.push_back(5);
    arg.push_back(15);
    arg.push_back(30);
    arg.push_back(255);
    arg.push_back(380);
    Mat complex = computeDFT(src);
    Mat spec = updateMag(complex); // show spectrum
    std::cout<<spec.type()<<" "<<CV_32FC1<<" \n";
    spec.convertTo(spec,CV_32FC1,255.0);
    spec.convertTo(spec,CV_8UC1);
    std::vector<Mat> src_merge_img_vec;
    src_merge_img_vec.push_back(src);
    src_merge_img_vec.push_back(spec);
    Mat src_merge_img = merge(src_merge_img_vec);
    Size src_size = src.size();
    std::stringstream stream;
    int flag=0;
    for(int i=0;i<arg.size();i++){
        for(int j=0;j<=1;j++){
            stream<<"Result_R=";
            stream<<arg[i];
            stream<<"j=";
            stream<<j;
            stream<<".jpg";
            Mat res = implementFiltering(complex,arg[i],src_size,j);
            res.convertTo(res,CV_32FC1,255.0);
            res.convertTo(res,CV_8UC1);
            std::vector<Mat> vert_merge;
            vert_merge.push_back(src_merge_img);
            vert_merge.push_back(res);
            Mat show_img = mergeVertical(vert_merge);
            imwrite(stream.str(),show_img);
            stream.str(std::string());
            stream.clear();
        }
    }
    
}

int main(int argc, char** argv) {
    //Mat mask = createGausFilterMask(Size(512,512), 256, 256, 510, true, true);
    Mat src = imread("lena_gray_512.tif",CV_LOAD_IMAGE_GRAYSCALE);
//    Mat src = imread("lena.jpg",CV_LOAD_IMAGE_GRAYSCALE);
    task3_1(src);
    waitKey(0);
    return 0;
}
