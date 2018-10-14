// ------------------------------------------------------------------------
// Project: q-SIFT
// Authors:  Guilherme Wachs-Lopes <gwachs@fei.edu.br>
//           Miller Horvath <mhorvath@fei.edu.br>
//
// This is an open source code created for research and academic purposes.
// If you have used this code in you research, please cite:
//
//   P. S. Rodrigues, G. A. Wachs-Lopes, G. Antonio Giraldi and M. Horvath,
//   in press. q-SIFT: A Strategy Based on Non-Extensive Statistic to
//   Improve SIFT Algorithm Under Severe Conditions. Pattern Recognition.
//
// ------------------------------------------------------------------------


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
