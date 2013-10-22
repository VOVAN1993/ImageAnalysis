/* 
 * File:   task3_5.h
 * Author: vovan
 *
 * Created on 21 Октябрь 2013 г., 22:49
 */

#ifndef TASK3_5_H
#define	TASK3_5_H
#include "tools.h"
#include "for_dft.h"
Mat rotateImage(const Mat& source, double angle) {
    //поворот изображения
    Point2f src_center(source.cols / 2.0F, source.rows / 2.0F);
    Mat rot_mat = getRotationMatrix2D(src_center, angle, 1.0);
    Mat dst;
    warpAffine(source, dst, rot_mat, source.size());
    return dst;
}
void task3_5(const Mat& src, const Mat& dst){
    Mat local_img, tmp;
    src.convertTo(local_img, CV_32FC1);
    std::vector<Mat> plan, polar;
    //dft
    cv::dft(local_img, tmp, DFT_COMPLEX_OUTPUT);
    split(tmp, plan);
    polar.resize(plan.size());
    cv::cartToPolar(plan[0], plan[1], polar[0], polar[1]);//перевод в полярные координаты
    Mat t = updateMag(polar[0]);//для визуализации спектра
    t.convertTo(t, CV_8UC1);
    Mat thresh;
    //делаем бинаризацию для выявления горизонтальных и вертикальных линий, образованных в 
    //результате наложения диагональных полос
    threshold(t, thresh, 160, 255, CV_THRESH_BINARY);
    blur(thresh, thresh, Size(3, 3));
    thresh = mythreshold(thresh, 60);
    //нахождение горизонтальной полосы
    int x1, x2;
    bool b = false;
    for (int i = 0; i < thresh.rows && !b; i++) {
        uchar *Mi = thresh.ptr(i);
        for (int j = 0; j < thresh.cols && !b; j++) {
            if (Mi[j] != 0) {
                int h = j + 1;
                for (; h < thresh.cols && Mi[h] != 0; h++) {
                }
                x1 = j;
                x2 = h;
                b = true;
            }
        }
    }
    thresh = rotateImage(thresh, -90);//повернем для удобства поиска
    int y1, y2;
    b = false;
    //находим вертикальную линию
    for (int i = 0; i < thresh.rows && !b; i++) {
        uchar *Mi = thresh.ptr(i);
        for (int j = 0; j < thresh.cols && !b; j++) {
            if (Mi[j] != 0) {
                int h = j + 1;
                for (; h < thresh.cols && Mi[h] != 0; h++) {
                }
                y1 = h;
                y2 = j;
                b = true;
            }
        }
    }
    thresh = rotateImage(thresh, 90);//поворот 
    y1 = thresh.rows - y1;
    y2 = thresh.rows - y2;
    shift(polar[0]);
    // выделяем нужные прямоугольные участки для замены(noizeCol,noizeRow)
    // и для их замещения (blurCol blurRow)
    Mat noizeCol = polar[0].colRange(x1 - 1, x2 + 1);
    Mat noizeRow = polar[0].rowRange(y1 - 1, y2 + 1);
    Mat blurCol = polar[0].colRange(x1 - 15, x2 - 13);
    Mat blurRow = polar[0].rowRange(y1 - 15, y2 - 13);

    
    blurCol.copyTo(noizeCol);
    blurRow.copyTo(noizeRow);

    //проводим аналогичную операцию для нижней части изображения 
    Mat noizeColB = polar[0].colRange(t.cols - x2 - 1, t.cols - x1 + 1);
    Mat noizeRowB = polar[0].rowRange(t.rows - y2, t.rows - y1 - 1);
//заменяем плохие участки
    blurCol.copyTo(noizeColB);
    blurRow.copyTo(noizeRowB);
    //меняем (1 и 3) & (2 и 4) квадранты местами
    shift(polar[0]);
    
    t = updateMag(polar[0]);
    t.convertTo(t, CV_8UC1);
    polarToCart(polar[0], polar[1], plan[0], plan[1]);//перевод из полярных координат
    merge(plan, tmp);
    dft(tmp, tmp, cv::DFT_SCALE | cv::DFT_INVERSE | cv::DFT_REAL_OUTPUT);
    tmp.convertTo(tmp, CV_8U);
    tmp -= 20;//уменьшение яркости =)
    imwrite("out/Task3_5result.jpg", tmp);
    Mat rm;//подсчет mse
    matchTemplate(dst, tmp, rm, CV_TM_SQDIFF);
    std::cout << "RMSE: " << rm / (src.rows * src.cols * src.channels()) << std::endl;
}

#endif	/* TASK3_5_H */

