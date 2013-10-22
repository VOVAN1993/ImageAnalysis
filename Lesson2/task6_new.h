/* 
 * File:   task6_new.h
 * Author: vovan
 *
 * Created on 21 Октябрь 2013 г., 16:12
 */

#ifndef TASK6_NEW_H
#define	TASK6_NEW_H
#include "tools.h"
#include "util.h"
#include "for_dft.h"


void highOrLowPass(cv::Mat& src, int r,int flag) {
//1=>high
//2=>low
    //центр
    int mx = src.cols / 2;
    int my = src.rows / 2;
    int tmpx, tmpy;
    int R2 = r*r;

    for (int i = 0; i < src.rows; ++i) {
        float *Mi = src.ptr<float>(i);
        tmpy = my - i;
        for (int j = 0; j < src.cols; ++j) {
            tmpx = mx - j;
            //зануляем в зависимости от фильтра
            if (flag) {
                if (tmpy * tmpy + tmpx * tmpx < R2) 
                    Mi[j] = 0.0;
            } else {
                if (tmpy * tmpy + tmpx * tmpx >= R2) 
                    Mi[j] = 0.0;
            }
        }
    }

}

std::vector<Mat> transformSpectrum(const Mat& image, int R, int flag) {
    //flag для выбора нужного фильтра
    std::vector<Mat> result;//все нужные изображения

    Mat local_src = image.clone(), temp;
    result.push_back(local_src);
    local_src.convertTo(local_src, CV_32FC1);
    std::vector<Mat> planes(2, Mat());//для dft
    std::vector<Mat> polar(2, Mat());//для полярной системы

    cv::dft(local_src, temp, cv::DFT_COMPLEX_OUTPUT);
    cv::split(temp, planes);
    cv::cartToPolar(planes[0], planes[1], polar[0], polar[1]);//перевод в полярные системы координат
    temp = updateMag(polar[0]);//наглядное представление
    result.push_back(temp);

    shift(polar[0]);//меняем (1 и 3) & (2 и 4) квадранты местами, чтобы было как в temp
    highOrLowPass(polar[0],R,(int)(flag==1));//применяем фильтр
    shift(polar[0]);//меняем (1 и 3) & (2 и 4) квадранты местами, чтобы было как в temp

    temp = updateMag(polar[0]);//наглядное представление получившегося спектра

    result.push_back(temp);

    cv::polarToCart(polar[0], polar[1], planes[0], planes[1]);//переход из полярной системы координат
    cv::merge(planes, temp);//соединение
    idft(temp, temp, cv::DFT_SCALE | cv::DFT_REAL_OUTPUT);//обратное преобразование
    temp.convertTo(temp, CV_8U);//to uchar

    result.push_back(temp);
    return result;
}

void task6_new(Mat& src) {
    //перевод в полутон
    cv::cvtColor(src, src, CV_BGR2GRAY);
    std::vector<int> arg;
    //входные аргументы
    arg.push_back(5);
    arg.push_back(15);
    arg.push_back(30);
    arg.push_back(255);
    arg.push_back(380);
    std::stringstream stream;
    for (int i = 0; i < arg.size(); i++) {
        for (int j = 0; j <= 1; j++) {
            std::vector<Mat> result = transformSpectrum(src, arg[i], j);
            //сохранение
            stream << "out/Task6_R=";
            stream << arg[i];
            if (!j)
                stream << "LowPass";
            else
                stream << "HighPass";
            stream << ".jpg";
            imwrite(stream.str(), merge(result));
            stream.str(std::string());
            stream.clear();
        }

    }
}
#endif	/* TASK6_NEW_H */

