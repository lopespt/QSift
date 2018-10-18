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
#include <string>
#include <sstream>

using namespace std;

Mat VideoASift::readFrame(const int &frameNum){
    Mat m;
    stringstream s;
    s <<  basename << frameNum << ".png";
    m = imread(s.str());
    return m;
}

VideoASift::VideoASift(string basename, const unsigned int& from,const unsigned int& to) : basename(basename), from(from), to(to) {
    default_sift_parameters(siftparameters);
}


Mat VideoASift::extractASiftFeatures(const unsigned int &frameNum, const int &maxPoints,
                            const unsigned int &h){

        ///// Compute ASIFT keypoints
	// number N of tilts to simulate t = 1, \sqrt{2}, (\sqrt{2})^2, ..., {\sqrt{2}}^(N-1)
	int num_of_tilts1 = 7;
	int num_of_tilts2 = 7;
//	int num_of_tilts1 = 1;
//	int num_of_tilts2 = 1;
	int verb = 0;
	// Define the SIFT parameters

	vector< vector< keypointslist > > keys1;		
	
	
	int num_keys1=0, num_keys2=0;
	
	
	cout << "Computing keypoints on the two images..." << endl;
	
    Mat frame = readFrame(frameNum);
    
    vector<float> img;
    img.resize(frame.rows*frame.cols);
    cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
    namedWindow("frame");
    imshow("frame", frame);
    waitKey(0);
    
    return frame;

	/*num_keys1 = compute_asift_keypoints(ipixels1_zoom, wS1, hS1, num_of_tilts1, verb, keys1, siftparameters);
	
	tend = time(0);
	cout << "Keypoints computation accomplished in " << difftime(tend, tstart) << " seconds." << endl;
    */
}
