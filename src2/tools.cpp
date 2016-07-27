//
// Created by Guilherme Wachs on 16/03/16.
//

#include "tools.hpp"
#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/function.hpp>
//#include <cl.hpp>
#include <algorithm>

using namespace std;

double PDM(const vector<feature> fset1, const vector<feature> fset2) {
    double dist = 0;

    for (int i = 0; i < fset1.size(); i++) {
        vector<double> dists(fset2.size());
        std::transform(fset2.begin(), fset2.end(),dists.begin() , [&](feature f){
            return descr_dist_sq(&f, &fset1[i]);
        });
        if(dists.size() == 0) {
            return DBL_MAX;
        }else{
            dist += *std::min_element(dists.begin(), dists.end());
        }
    }
    for (int i = 0; i < fset2.size(); i++) {
        vector<double> dists(fset1.size());
        std::transform(fset1.begin(), fset1.end(),dists.begin() , [&](feature f){
            return descr_dist_sq(&f, &fset2[i]);
        });
        if(dists.size() == 0){
            return DBL_MAX;
        }else {
            dist += *std::min_element(dists.begin(), dists.end());
        }
    }
    return dist / (fset1.size() + fset2.size());
}


double PDM2(const vector<feature> fset1, const vector<feature> fset2) {
    double dist = 0;
    using namespace boost;
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
/*
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
  */  return 0;
}

