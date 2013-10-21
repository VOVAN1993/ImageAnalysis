/* 
 * File:   task3.h
 * Author: vovan
 *
 * Created on 21 Октябрь 2013 г., 12:13
 */

#ifndef TASK3_H
#define	TASK3_H
#include "util.h"
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

#endif	/* TASK3_H */

