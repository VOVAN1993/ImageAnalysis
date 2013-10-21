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
#include "task6_new.h"



Mat myupdateResult(Mat complex) {
    Mat work;
    idft(complex, work);
    Mat planes[] = {Mat::zeros(complex.size(), CV_32F), Mat::zeros(complex.size(), CV_32F)};
    split(work, planes); // planes[0] = Re(DFT(I)), planes[1] = Im(DFT(I))

    magnitude(planes[0], planes[1], work); // === sqrt(Re(DFT(I))^2 + Im(DFT(I))^2)
    normalize(work, work, 0, 1, NORM_MINMAX);
    return work;
}


Mat myupdateMag(Mat complex) {

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

Mat mycomputeDFT(Mat image) {
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



Mat mycreateMask(Size size, int R, int type, int mx, int my,int flag) {
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


void task3_5(Mat& src,const Mat& dst){
    
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
    imshow("qwe",res);
            
    res.convertTo(res,CV_32FC1,255.0);
    res.convertTo(res,CV_8UC1);
    
    Mat t;
    cv::matchTemplate(dst, res, t, CV_TM_SQDIFF);
    std::cout << "RMSE: " << t / (dst.rows * dst.cols) << std::endl;
}
Mat rotateImage(const Mat& source, double angle) {
    Point2f src_center(source.cols / 2.0F, source.rows / 2.0F);
    Mat rot_mat = getRotationMatrix2D(src_center, angle, 1.0);
    Mat dst;
    warpAffine(source, dst, rot_mat, source.size());
    return dst;
}
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
void f(const Mat& src,const Mat& dst){
    Mat local_img,tmp;
    src.convertTo(local_img,CV_32FC1);
    std::vector<Mat> plan,polar;
    cv::dft(local_img, tmp, DFT_COMPLEX_OUTPUT);
    split(tmp,plan);
    polar.resize(plan.size());
    cv::cartToPolar(plan[0], plan[1], polar[0], polar[1]);
    Mat t = updateMag(polar[0]);
    t.convertTo(t,CV_8UC1);
    Mat thresh;
    threshold( t, thresh, 160, 255, CV_THRESH_BINARY );
    blur(thresh,thresh,Size(3,3));
//    for(int i=0;i<thresh.rows;i++){
//        uchar *Mi = thresh.ptr(i);
//        for(int j=0;j<thresh.cols;j++){
//            std::cout<<(int)Mi[j]<<" \n";
//        }
//    }
    thresh = mythreshold(thresh,60);
    int x1,x2;
    bool b=false;
    for(int i=0;i<thresh.rows&&!b;i++){
        uchar *Mi = thresh.ptr(i);
        for(int j=0;j<thresh.cols && !b;j++){
            if(Mi[j]!=0){
                int h=j+1;
                for(;h<thresh.cols && Mi[h]!=0;h++){
                }
                x1=j;x2=h;
                b=true;
            }
        }
    }
    thresh = rotateImage(thresh,-90);
    int y1,y2;
    b=false;
    for(int i=0;i<thresh.rows&&!b;i++){
        uchar *Mi = thresh.ptr(i);
        for(int j=0;j<thresh.cols&&!b;j++){
            if(Mi[j]!=0){
                int h=j+1;
                for(;h<thresh.cols&& Mi[h]!=0;h++){
                }
                y1=h;y2=j;
                b=true;
            }
        }
    }
    thresh = rotateImage(thresh,90);
     std::cout<<x1<<" "<<x2<<" "<<y1<<" "<<y2;
     y1=thresh.rows-y1;
     y2=thresh.rows-y2;
     std::cout<<x1<<" "<<x2<<" "<<y1<<" "<<y2;
    cv::Mat noizeCol = t.colRange(x1 - 1, x2 + 1);
    
    
    cv::Mat noizeRow = t.rowRange(y1 - 1, y2 + 1);
    
    cv::Mat blurCol = t.colRange(x1 - 15, x2 -10);
    
    cv::Mat blurRow = t.rowRange(y1 - 15, y2 - 10);
//    for(int i=0;i<blurRow.rows;i++){
//        uchar *Mi = blurRow.ptr(i);
//        for(int j=0;j<blurRow.cols;j++){
//            Mi[j]=0;
//        }
//    }
    std::cout<<noizeCol.size()<<blurCol.size();
    blurCol.copyTo(noizeCol);
    blurRow.copyTo(noizeRow);
//     t = updateMag(polar[0]);
//    t.convertTo(t,CV_8UC1);
    imshow("qwe",t);
    
    
//    cv::Mat result;
//    cv::matchTemplate(dst, tmp, result, CV_TM_SQDIFF);
//    std::cout << "RMSE Task 3.5: " << result / (dst.cols * dst.rows) << std::endl;
//
//    concatImages(local_img, tmp, local_img);
//    cv::absdiff(tmp, dst, tmp);
//    concatImages(local_img, tmp, local_img);
//
//    cv::absdiff(image, dst, tmp);
//    concatImages(local_img, tmp, local_img);
    
   
//    //threshold( t, thresh, 100.0, 255.0, CV_THRESH_BINARY );
//    imshow("qwe",thresh);
}


int main(int argc, char** argv) {
    //Mat mask = createGausFilterMask(Size(512,512), 256, 256, 510, true, true);
//    Mat src = imread("lena_gray_512.jpg",CV_LOAD_IMAGE_GRAYSCALE);
//    Mat src = imread("lena.jpg");
//    Mat src = imread("lena_color_512-noise.tif");
    Mat src = imread("lena_diagonal.jpg",CV_LOAD_IMAGE_GRAYSCALE);
    Mat good = imread("lena_gray_512.tif",CV_LOAD_IMAGE_GRAYSCALE);
// 
    f(src,good);
//    task6_new(src);
//    task5(src);
    

    waitKey(0);
    return 0;
}
