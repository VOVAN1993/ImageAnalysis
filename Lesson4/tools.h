#ifndef TOOLS_H
#define	TOOLS_H

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
#include <algorithm>
#include <map>
#include <vector>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace cv;
using namespace std;
typedef std::vector<std::vector<Point> > VVP;
typedef std::vector<cv::Point> VP;
#define ALL(v) v.begin(), v.end()
typedef unsigned long long ull;
typedef long long ll;
const string PATH_TO_COREL="br";
const string PATH_TO_OUT = "out/";

#endif	/* TOOLS_H */

