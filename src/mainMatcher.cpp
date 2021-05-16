/*
 #############################################################################
 #        Author: Alessio Xompero
 #         Email: a.xompero@qmul.ac.uk
 #
 #
 #  Created Date: 2017/10/17
 # Modified Date: 2021/04/10
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

#include <chrono>
#include <cstdio>
#include <cstdlib>

#include <fstream>
#include <iostream>
#include <map>

#include "morb.h"
#include "Frame.h"
#include "matcher.h"
#include "utilities.h"
#include "timer.h"

#include "boost/filesystem/path.hpp"

using namespace std;

const int nLevels = 8;

/**
 *
 */
void printReadme() {
  std::cout << "Use ./MORB <filepath/refImageName> "
            << "<filepath/targetImageName> <filepath/refKeypointsName>  "
            << "<filepath/tarKeypointsName>  <output refFilename> "
            << "<output tarFilename> "
            << "<number of features> <Gaussian kernel size[0|3|5|7]> "
            << "<matching strategy>" << std::endl;
}

// Multi-scale ORB: main
int main(int argc, char** argv) {
  PrintHeading1("MORB: A multi-scale binary descriptor based on ORB");

  if (argc != 12) {
    std::cerr << "The number of arguments is: " << argc << std::endl;
    printReadme();
    return EXIT_FAILURE;
  }

  Timer t;

  // Parse command line arguments
  std::string image1 = argv[1];  // image filename (+ path)
  std::string image2 = argv[2];  // image filename (+ path)
  std::string fnKps1 = argv[3];  // image filename (+ path)
  std::string fnKps2 = argv[4];  // image filename (+ path)
  int nFeatures = atoi(argv[7]);  // number of key-points to detect
  int BlurSz = atoi(argv[8]);  // Gaussian kernel size for the smoothing

  int matchingStrategy = atoi(argv[9]);
  int W = atoi(argv[10]);  // Size of the kernel for the convolution
  int R = atoi(argv[11]);

  ////////////////////////////////////////////////////////////////////
  // First image
  std::cout << "First image.." << std::endl;
  boost::filesystem::path p1(image1);

  Frame f1(1);

  Timer t_unit;
  std::cout << "Detecting and extracting ORB features.." << std::endl;
  if (!f1.computeLocalFeatures(image1, fnKps1, nFeatures, BlurSz)) {
    return EXIT_FAILURE;
  }
  t_unit.print("processing image 1");

  std::cout << "Saving ORB features.." << std::endl;
  f1.saveDescriptors(f1.mvKeys, f1.descriptor, argv[5]);

  t_unit.reset();

  ////////////////////////////////////////////////////////////////////
  // Second image

  std::cout << "Second image.." << std::endl;
  boost::filesystem::path p2(image2);

  Frame f2(2);

  std::cout << "Detecting and extracting ORB features.." << std::endl;
  if (!f2.computeLocalFeatures(image2, fnKps2, nFeatures, BlurSz)) {
    return EXIT_FAILURE;
  }
  t_unit.print("processing image 2");

  std::cout << "Saving MORB features.." << std::endl;
  f2.saveDescriptors(f2.mvKeys, f2.descriptor, argv[6]);

  t_unit.reset();
  ////////////////////////////////////////////////////////////////////
  //
  // Multi-scale Matching
  PrintHeading2("Multi-scale matching");

  Matcher morb_matcher(W, R, nLevels, matchingStrategy);

  cv::Mat H, Hs, S1, S2, M;
  morb_matcher.multiScaleMatching(
      f1.descriptor, f1.dtScales, f2.descriptor, f2.dtScales, H, Hs, S1, S2, M,
      static_cast<Matcher::MatchMode>(matchingStrategy));

  t.print("MORB detection, extraction and matching");

  PrintHeading2("Saving multi-scale matching results");
  //saveMatching(H, "multiscaleHamming.txt");
  morb_matcher.saveMatching(Hs, "scaledHamming.txt");
  morb_matcher.saveMatching(S1, "scales1.txt");
  morb_matcher.saveMatching(S2, "scales2.txt");
  morb_matcher.saveMatching(M, "matches.txt");
  t.print("MORB");

  ////////////////////////////////////////////////////////////////////
  // Finished!
  PrintClosing2("MORB extraction and matching finished!");

  return EXIT_SUCCESS;
}

