/* 
 * File:   task4.h
 * Author: vovan
 *
 * Created on 21 Октябрь 2013 г., 12:16
 */

#ifndef TASK4_H
#define	TASK4_H
#include "util.h"
void task4(Mat& image) {
    //входные параметры
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
            //добавление гуссового шума
            addGausNoise(ch[i], tmp, arg[j].first,arg[j].second);
            newChan.push_back(tmp);
        }
        Mat result;
        merge(newChan, result);
 
        stream << "out/Task4img";
        stream << j + 1;
        stream << ".jpg";
        cv::imwrite(stream.str(), result);
        stream.str(std::string());
        stream.clear();
    }
}


#endif	/* TASK4_H */

