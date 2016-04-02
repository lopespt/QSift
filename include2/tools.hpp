//
// Created by Guilherme Wachs on 16/03/16.
//

#ifndef SIMPLE_EXAMPLE_TOOLS_HPP
#define SIMPLE_EXAMPLE_TOOLS_HPP

#include <vector>
#include <sift.h>
#include <imgfeatures.h>


using namespace std;

double PDM(vector<feature> fset1, vector<feature> fset2);
double PDM_OpenCL(vector<feature> fset1, vector<feature> fset2);
double qGaussian(float x, float q, float b);


#endif //SIMPLE_EXAMPLE_TOOLS_HPP
