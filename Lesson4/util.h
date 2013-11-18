// -*- C++ -*-
/* 
 * File:   util.h
 * Author: vovan
 *
 * Created on 12 Ноябрь 2013 г., 15:40
 */

#ifndef UTIL_H
#define	UTIL_H
#include "tools.h"
class Image{

public:
    double value;
    string name;
    Image(double _value,string _name){
        name=_name;
        value=_value;
    }
};
void print_vector_to_file(string name,vector<Image> &vec){
    freopen(name.c_str(), "w", stdout);
    for (int i = 0; i < vec.size(); ++i) {
        cout<<vec[i].name.c_str()<<"\n";
    }
    fclose(stdout);
}
bool cmpImage(Image i1, Image i2) {
    return i1.value < i2.value;
}


#endif	/* UTIL_H */

