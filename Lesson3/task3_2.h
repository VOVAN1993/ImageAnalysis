/* 
 * File:   task3_2.h
 * Author: vovan
 *
 * Created on 27 Октябрь 2013 г., 19:42
 */

#ifndef TASK3_2_H
#define	TASK3_2_H
#include "tools.h"
#include <limits.h>

Mat task3_2(const Mat& src,int background){
    background*=255;
    CV_Assert(src.type()==CV_8UC1);
    Mat binary;
    threshold(src,binary,128,255,CV_THRESH_BINARY);
    int c=0;
    //номер компоненты для каждого пикселя
    vector<vector<int> > mat(src.rows,vector<int>(src.cols,0));
    //для первого прохода.
    vector<int> bestNum(1,0);
    
    for(int i=0;i<binary.rows;i++){
        uchar *Mi=binary.ptr(i);
        for(int j=0;j<binary.cols;j++){
            bool b=false;
            vector<int> neib;
            if(Mi[j]==background)continue;
            for(int ii=max(i-1,0);ii<=min(i+1,binary.rows-1);ii++){
                for(int jj=max(j-1,0);jj<=min(j+1,binary.cols-1);jj++){
                    //пробегаем всех соседей
                    uchar val = binary.at<uchar>(ii,jj);
                    if(val!=Mi[j] || mat[ii][jj]==0)continue;
                    b=true;
                    neib.push_back(mat[ii][jj]);
                }
            }
            if(!b){
              //если не обнаружили уже помеченных соседей нужного цвета
              //,то новая компонента
              c++;
              mat[i][j]=c;
              bestNum.push_back(c);
            }else{
                sort(neib.begin(),neib.end());
                mat[i][j]=bestNum[neib[0]];
                for(int ii=0;ii<neib.size();ii++){
                    bestNum[neib[ii]]=mat[i][j];
                }
            }
        }
    }
    //второй проход, соединяем соседей
    for(int i=0;i<mat.size();i++){
        for(int j=0;j<mat[i].size();j++){
            mat[i][j]=bestNum[mat[i][j]];
        }
    }
    //окрашиваем
    map<int,Scalar> map;
    RNG rng(12345);
    for(int i=0;i<c;i++){
        int r=rng.next()%256;
        int g=rng.next()%256;
        int b=rng.next()%256;
        
        Scalar tmp(b,g,r);
        map[i]=tmp;
    }
    Mat result=Mat::zeros(binary.size(),CV_8UC3);
    for(int i=0;i<mat.size();i++){
        uchar *Mi=result.ptr(i);
        for(int j=0;j<mat[i].size();j++){
            if(!mat[i][j]){
                Mi[3*j]=Mi[3*j+1]=Mi[3*j+2]=background;
            }else{
                Mi[3*j]=(uchar)map[mat[i][j]-1][0];
                Mi[3*j+1]=(uchar)map[mat[i][j]-1][1];
                Mi[3*j+2]=(uchar)map[mat[i][j]-1][2];
            }
        }
    }

    return result;
}


#endif	/* TASK3_2_H */

