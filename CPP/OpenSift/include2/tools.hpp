//
// Created by Guilherme Wachs on 16/03/16.
//

#ifndef SIMPLE_EXAMPLE_TOOLS_HPP
#define SIMPLE_EXAMPLE_TOOLS_HPP

#include <vector>
#include <tuple>
#include <sift.h>
#include <imgfeatures.h>
#include <exception>

using namespace std;

typedef tuple<double, vector<feature>, vector<feature> > FrameMatch;

double PDM(vector<feature> fset1, vector<feature> fset2);
FrameMatch PDM_Matches(vector<feature> fset1, vector<feature> fset2);
double PDM_OpenCL(vector<feature> fset1, vector<feature> fset2);
double qGaussian(float x, float q, float b);
CvMat * createQGaussianKernel();


#endif //SIMPLE_EXAMPLE_TOOLS_HPP
