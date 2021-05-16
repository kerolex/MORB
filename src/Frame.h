/*
 #############################################################################
 #        Author: Alessio Xompero
 #         Email: a.xompero@qmul.ac.uk
 #
 #
 #  Created Date: 2021/04/10
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

#ifndef _FRAME_H_
#define _FRAME_H_

// STL Libraries
#include <vector>

//OpenCV Libraries
#include <opencv2/opencv.hpp>

//
#include "morb.h"

class Frame {
 public:
  Frame();

  Frame(int _frameID);

  /**
   *
   */
  bool detectAndComputeLocalFeatures(std::string imageFilename, int nFeatures,
                                     int BlurSz);

  /**
   *
   */
  bool computeLocalFeatures(std::string imageFilename, std::string fnKeyPoints,
                            int nFeatures, int BlurSz);

  /**
   *
   */
  void showKeypoints(cv::InputArray _img, std::vector<cv::KeyPoint> mvKeys,
                     int flag = 0);

  /**
   *
   */
  void saveDescriptors(std::vector<cv::KeyPoint> mvKeys,
                       cv::InputArray _descriptor, std::string filename);

  /**
   *
   */
  void saveDescriptorsAndIntensityDiff(std::vector<cv::KeyPoint> mvKeys,
                                       std::vector<cv::Mat> descriptor,
                                       std::string filename);

  /**
   *
   */
  void saveMultiScaleDescriptor(
      std::string filename,
      std::vector<std::vector<cv::KeyPoint> > allKeypoints,
      cv::InputArray _sortedDescs, int nlevels);

  /**
   *
   */
  void saveKeyPoints(
      std::string output,
      const std::vector<std::vector<cv::KeyPoint> > allKeypoints);

 public:

  std::vector<cv::KeyPoint> mvKeys;
  cv::Mat descriptor;
  std::vector<int> dtScales;
  std::vector<cv::Mat> orbID;

  //protected:
  MORB *morbExtractor;
  int frameID;
};

#endif

