/*
 #############################################################################
 #        Author: Alessio Xompero
 #         Email: a.xompero@qmul.ac.uk
 #
 #
 #  Created Date: 2017/10/17
 # Modified Date: 2021/05/12
 #
 # Centre for Intelligent Sensing, Queen Mary University of London, UK
 #
 ###############################################################################
 #
 # The MIT License (MIT)
 #
 # Copyright © 2021 Alessio Xompero
 #
 # Permission is hereby granted, free of charge, to any person obtaining a copy
 # of this software and associated documentation files (the “Software”), to deal
 # in the Software without restriction, including without limitation the rights
 # to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 # copies of the Software, and to permit persons to whom the Software is
 # furnished to do so, subject to the following conditions:
 #
 # The above copyright notice and this permission notice shall be included in
 # all copies or substantial portions of the Software.
 #
 # THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 # IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 # FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 # AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 # LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 # OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 # SOFTWARE.
 #
 ###############################################################################
 */

// STD Libraries
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>
#include <map>

// OpenCV Libraries
#include <opencv2/opencv.hpp>

//#include "opencv2/xfeatures2d.hpp"

// My libraries
#include "utilities.h"

using namespace std;

enum Params {
  F = 1,  // number of features
  S = 2,  // number of levels
  s = 3,  // scale factor
  G = 4,  // Patch size
  H = 5,
  t = 6
};

/**
 *
 */
void ORBextractor(cv::InputArray _img, std::vector<cv::KeyPoint>& feats,
                  cv::OutputArray _D1, int nFeats, int nLevels, float sf,
                  int PATCH_SIZE, int fast_threshold) {
  cout << "ORB Feature Detection and Extraction..." << endl;

  cv::Mat image = _img.getMat();
  cv::Mat D1;
  feats.clear();

  cv::Ptr<cv::ORB> ORBdetector = cv::ORB::create(nFeats, sf, nLevels,
                                                 PATCH_SIZE, 0, 2,
                                                 cv::ORB::HARRIS_SCORE,
                                                 PATCH_SIZE, fast_threshold);

  ORBdetector->detectAndCompute(image, cv::Mat(), feats, D1);

  D1.copyTo(_D1);
  cout << "Done..." << endl;
};

//
int main(int argc, char** argv) {

  if (argc < 2) {
    cerr
        << "Wrong number of inputs. Please use ORB <image filename> [<features file> <keyframe>]"
        << endl;
    return -1;
  }

  int nLevels = 8;
  float sf = 1.2f;
  int PATCH_SIZE = 31;
  int HALF_PATCH_SIZE = 15;
  int fast_threshold = 20;

  std::map<std::string, int> mParams;
  mParams["-F"] = 1;
  mParams["-S"] = 2;
  mParams["-s"] = 3;
  mParams["-G"] = 4;
  mParams["-H"] = 5;
  mParams["-t"] = 6;

  std::vector<cv::KeyPoint> feats;
  cv::Mat D1;

  // Parse input parameters
  std::string filename(argv[1]);
  std::cout << filename << std::endl;

  int binD, nFeats;

  int j = 2;
  while (j < argc) {
    switch (mParams[argv[j]]) {
      case Params::F:
        nFeats = atoi(argv[j + 1]);
        break;
      case Params::S:
        nLevels = atoi(argv[j + 1]);
        break;
      case Params::s:
        sf = atof(argv[j + 1]);
        break;
      case Params::G:
        PATCH_SIZE = atoi(argv[j + 1]);
        break;
      case Params::H:
        HALF_PATCH_SIZE = atoi(argv[j + 1]);
        break;
      case Params::t:
        fast_threshold = atoi(argv[j + 1]);
        break;
    }

    j += 2;
  }

  cv::Mat image = cv::imread(filename, 0);

  std::string output;

  ORBextractor(image, feats, D1, nFeats, nLevels, sf, PATCH_SIZE,
               fast_threshold);
  if (nLevels == 1) {
    output = filename.substr(0, filename.size() - 3) + "orb1";
  } else {
    output = filename.substr(0, filename.size() - 3) + "orb";
  }

  //
  saveFeatures(feats, D1, output, distance_type::HAMMING, 8);

  return 0;
}


