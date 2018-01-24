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

FrameMatch PDM_Matches(vector<feature> fset1, vector<feature> fset2){
    double dist = 0;
    vector<feature> fret1(fset1.size()), fret2(fset2.size());

    for (int i = 0; i < fset1.size(); i++) {
        vector<double> dists(fset2.size());
        std::transform(fset2.begin(), fset2.end(),dists.begin() , [&](feature f){
            return descr_dist_sq(&f, &fset1[i]);
        });
        if(dists.size() == 0) {
            return FrameMatch(DBL_MAX, vector<feature>(0), vector<feature>(0));
        }else{
            int min_index = 0;
            for(int j = 1; j < dists.size(); j++){
                if(dists[j] < dists[min_index]){
                    min_index = j;
                }
            }
            fret1[i] = fset1[min_index];
            dist += dists[min_index];
        }
    }
    for (int i = 0; i < fset2.size(); i++) {
        vector<double> dists(fset1.size());
        std::transform(fset1.begin(), fset1.end(),dists.begin() , [&](feature f){
            return descr_dist_sq(&f, &fset2[i]);
        });
        if(dists.size() == 0){
            return FrameMatch(DBL_MAX, vector<feature>(0), vector<feature>(0));
        }else {
            int min_index = 0;
            for(int j = 1; j < dists.size(); j++){
                if(dists[j] < dists[min_index]){
                    min_index = j;
                }
            }
            fret2[i] = fset2[min_index];
            dist += dists[min_index];
        }
    }

    return FrameMatch(dist, fret1, fret2);
};

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

