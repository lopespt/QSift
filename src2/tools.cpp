//
// Created by Guilherme Wachs on 16/03/16.
//

#include "tools.hpp"
#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/function.hpp>
#include <cl.hpp>
#include <fstream>

using namespace boost;

double PDM(const vector<feature> fset1, const vector<feature> fset2) {
    double dist = 0;

    for (int i = 0; i < fset1.size(); i++) {
        dist += *boost::range::min_element(
                adaptors::transform(fset2, boost::function<double(feature)>([&fset1, &i](feature f2) {
                    return descr_dist_sq(&f2, &fset1[i]);
                }))
        );
    }
    for (int i = 0; i < fset2.size(); i++) {
        dist += *boost::range::min_element(
                adaptors::transform(fset1, boost::function<double(feature)>([&fset2, &i](feature f1) {
                    return descr_dist_sq(&f1, &fset2[i]);
                }))
        );
    }

    return dist / (fset1.size() + fset2.size());
}


double PDM_OpenCL(vector<feature> fset1, vector<feature> fset2) {

    vector<float> v{1, 2, 3, 4};
    using namespace cl;
    using namespace std;

    ifstream ifs("/Users/wachs/Dropbox/SIFT/CPP/OpenSift/src2/pdm_opencl.c", fstream::in);
    std::string str((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());

    Platform plat = Platform::getDefault();
    vector<Device> devs;
    plat.getDevices(CL_DEVICE_TYPE_GPU, &devs);
    cl_context_properties props[] = {
            CL_CONTEXT_PLATFORM, (cl_context_properties) (plat()), 0
    };

//    Context ctx = Context(devs,props, );
    //  Program prog = Program(ctx,str.c_str());
    //  cl_int cc = prog.build(devs);
    // printf("%d\n", cc);
}


double qGaussian(float x, float q, float b) {
    assert(q < 3);
    double cq;
    double eq;
    if (q < 1) {
        cq = (2 * sqrt(M_PI) * tgamma(1. / (1 - q)));
        cq /= (3 - q) * sqrt(1 - q) * tgamma((3 - q) / (2. - 2. * q));
        eq = pow(1 + ((1 - q) * ((-b) * x * x)), 1. / (1. - q));
    } else if (q > 1) {
        cq = sqrt(M_PI) * tgamma((3 - q) / (2.*q - 2.));
        cq /= sqrt(q - 1) * tgamma(1. / (q - 1));
        eq = pow(1 + ((1 - q) * ((-b) * x * x)), 1. / (1. - q));
    } else {
        //q == 1;
        cq = sqrt(M_PI);
        eq = exp(-b * x * x);
    }
    double res = sqrt(b) / cq * eq;
    printf("cq = %.2f    eq = %.2f    res = %.2f\n",cq,eq, res);
    return res < 0 ? 0 : res;
}

