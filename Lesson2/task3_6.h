/* 
 * File:   task3_6.h
 * Author: vovan
 *
 * Created on 21 Октябрь 2013 г., 12:36
 */

#ifndef TASK3_6_H
#define	TASK3_6_H
#include "util.h"
void task3_6(){    
    Mat src = imread("lena_color_512.tif");
    Mat noise = imread("lena_color_512-noise.tif");
    Mat detail = noise.clone();

std::vector<Mat> planes;
    // Разбиваем исходное изображение на каналы
    split(noise, planes);
    
    // Сглаживаем по отдельности каждый канал
    // медианным фильтром
    medianBlur(planes[0], planes[0], 3);
    medianBlur(planes[1], planes[1], 3);
    medianBlur(planes[2], planes[2], 3);
    
    // Объединяем каналы
    merge(planes, noise);
    
    // Применяем фильтр Гаусса с ядром 9
    GaussianBlur(noise, noise, Size(9, 9), 0, 2);

    Mat tmp;
    cv::matchTemplate(src, noise, tmp, CV_TM_SQDIFF);
    std::cout << "RMSE: " << tmp / (src.rows * src.cols * src.channels()) << std::endl;
    // Расчитываем MSE
//    std::cout << "MSE " << mse(src, noise) << std::endl;
}


#endif	/* TASK3_6_H */

