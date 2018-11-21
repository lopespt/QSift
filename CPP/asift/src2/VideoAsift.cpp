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

#include <VideoAsift.hpp>
#include <flimage.h>
#include <io_png/io_png.h>
#include <json.hpp>
#include <opencv2/opencv.hpp>
#include <sstream>
#include <string>
#include <json.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <fstream>

using namespace boost::serialization;
using namespace nlohmann;
using namespace std;
using namespace cv;




template<class Archive>
void serialize(Archive & ar, keypoint & g, const unsigned int version)
{
    ar & g.angle;
    ar & g.scale;
    ar & g.vec;
    ar & g.x;
    ar & g.y;
}

template<class Archive>
void serialize(Archive & ar, FrameInfo & g, const unsigned int version)
{
    ar & g.h;
    ar & g.w;
    ar & g.keypoints;
}

template<class Archive>
void deserialize(Archive & ar, keypoint & g, const unsigned int version)
{
    ar & g.angle;
    ar & g.scale;
    ar & g.vec;
    ar & g.x;
    ar & g.y;
}

template<class Archive>
void deserialize(Archive & ar, FrameInfo & g, const unsigned int version)
{
    ar & g.h;
    ar & g.w;
    ar & g.keypoints;
}

template <typename T> ostream &operator<<(ostream &out, const vector<T> &x) {
  for (int i = 0; i < x.size(); i++) {
    out << x[i] << "\t";
  }
  return out;
}

Mat VideoASift::readFrame(const int &frameNum) {
  Mat actualFrame;
  stringstream ss;
  ss << basename << frameNum << ".png";
  actualFrame = imread(ss.str());
  return actualFrame;
}

VideoASift::VideoASift(string basename, const unsigned int &from,
                       const unsigned int &to)
    : basename(basename), from(from), to(to) {
  default_sift_parameters(siftparameters);
}


FrameInfo VideoASift::extractASiftFeaturesMemoized(const unsigned int &frameNum){
  stringstream ss;
  ss << basename << frameNum << ".asift";
  
  FrameInfo ret;
  ifstream ifs(ss.str(), ios_base::binary);
  try{
    if (ifs){
      boost::archive::binary_iarchive entrada(ifs);
      entrada >> ret; 
      return ret;
    }
  }catch(exception e){
  }
  return extractASiftFeatures(frameNum);

}

FrameInfo VideoASift::extractASiftFeatures(const unsigned int &frameNum) {
  ///// Compute ASIFT keypoints
  // number N of tilts to simulate t = 1, \sqrt{2}, (\sqrt{2})^2, ...,
  // {\sqrt{2}}^(N-1)
  int num_of_tilts1 = 5;
  int num_of_tilts2 = 5;
  int verb = 0;
  // Define the SIFT parameters
  FrameInfo ret;

  int num_keys1 = 0, num_keys2 = 0;

  // cout << "Computing keypoints on the two images..." << endl;

  Mat frame = readFrame(frameNum);

  flimage image;
  cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
  frame.convertTo(frame, CV_32FC1);
  image.create(frame.cols, frame.rows, (float *)frame.data);

  vector<float> fimage((float *)frame.data, (float *)frame.dataend);
  ret.h = frame.rows;
  ret.w = frame.cols;

  num_keys1 =
      compute_asift_keypoints(fimage, frame.cols, frame.rows, num_of_tilts1,
                              verb, ret.keypoints, siftparameters);


  //Memoiza os keypoints
  stringstream ss;
  ss << basename << frameNum << ".asift";
  ofstream ofs(ss.str(), ios_base::binary);
  boost::archive::binary_oarchive saida(ofs);
  saida << ret;

  return ret;
}
