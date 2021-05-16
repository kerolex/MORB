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

#ifndef _MORB_H_
#define _MORB_H_

#include <vector>
#include <list>
#include <map>

#include "opencv/cv.h"

#include <opencv2/opencv.hpp>


class ExtractorNode {
 public:
  ExtractorNode()
      : bNoMore(false) {
  }

  void DivideNode(ExtractorNode &n1, ExtractorNode &n2, ExtractorNode &n3,
                  ExtractorNode &n4);

  std::vector<cv::KeyPoint> vKeys;
  cv::Point2i UL, UR, BL, BR;
  std::list<ExtractorNode>::iterator lit;
  bool bNoMore;
};

class MORB {
 public:
  MORB(int nfeatures, float scaleFactor, int nlevels, int fastThreshold,
      int scoreType, bool detectKPs, bool griDetector);

  ~MORB() {
  }

  // Compute the ORB features and descriptors on an image.
  // ORB are dispersed on the image using an octree.
  // Mask is ignored in the current implementation.
  void operator()(cv::InputArray _image, cv::InputArray _mask,
                  std::vector<cv::KeyPoint>& _keypoints,
                  cv::OutputArray _descriptors, std::vector<int>& dtScales,
                  int BlurSz, std::string fnKeyPoints, int imgID = -1);

  int inline GetLevels() {
    return nlevels;
  }

  float inline GetScaleFactor() {
    return scaleFactor;
  }

  std::vector<float> inline GetScaleFactors() {
    return mvScaleFactor;
  }

  std::vector<float> inline GetInverseScaleFactors() {
    return mvInvScaleFactor;
  }

  std::vector<float> inline GetScaleSigmaSquares() {
    return mvLevelSigma2;
  }

  std::vector<float> inline GetInverseScaleSigmaSquares() {
    return mvInvLevelSigma2;
  }

 protected:
  /**
   * @fn
   *
   * @brief Select best features using the Harris cornerness
   *  (better scoring than FAST).
   *
   * @param [in,out] allKeypoint Vector of vector containing the
   * key-points for each scale level.
   * @param counters
   */
  void RetainBestKeyPoints(
      std::vector<std::vector<cv::KeyPoint> >& allKeypoints,
      const std::vector<int> counters);

  /**
   *
   *
   */
  void HarrisResponses(std::vector<std::vector<cv::KeyPoint> >& pts,
                       int blockSize);

  /**
   * @fn cullKeypoints
   *
   * @brief Remove key-points too close at the border in the lowest scale level.
   *
   * Discard key-points that at lowest scale level have coordinates
   * too close to the boundaries of the Image at that level, that means
   * we cannot compute the descriptor as the patch does not fit the
   * image.
   *
   * @param [in,out] allKeypoint Vector of vector containing the
   * key-points for each scale level.
   */
  void cullKeypoints(std::vector<std::vector<cv::KeyPoint> >& allKeypoints);

  /**
   *
   */
  void scaleKeyPoints(std::vector<std::vector<cv::KeyPoint> > &allKeypoints);

  /**
   *
   */
  void readKeyPointsFromFile(
      std::string filename,
      std::vector<std::vector<cv::KeyPoint> >& allKeypoints);

  /**
   *
   */
  void removeDuplicates(std::vector<std::vector<cv::KeyPoint>> allKeypoints,
                        std::vector<std::vector<cv::KeyPoint>> &newKeypoints,
                        std::vector<int> &dtScales);
 public:
  enum {
    kBytes = 32,
    HARRIS_SCORE = 0,
    FAST_SCORE = 1
  };

  std::vector<cv::Mat> mvImagePyramid;
  std::vector<cv::Mat> mvMaskPyramid;

 protected:

  void ComputePyramid(cv::Mat image);

  void computeKeypoints(std::vector<std::vector<cv::KeyPoint> >& allKeypoints);

  ////////////////////////////////////////////
  // Grid-based pyramid for SLAM

  /*
   *
   */
  void DetectKeyPointPyramid(
      std::vector<std::vector<cv::KeyPoint> >& allKeypoints);

  /*
   *
   */
  void ComputeKeyPointsOctTree(
      std::vector<std::vector<cv::KeyPoint> >& allKeypoints, int &level);

  /*
   *
   */
  std::vector<cv::KeyPoint> DistributeOctTree(
      const std::vector<cv::KeyPoint>& vToDistributeKeys, const int &minX,
      const int &maxX, const int &minY, const int &maxY, const int &nFeatures,
      const int &level);

  ////////////////////////////////////////////
  std::vector<cv::Point> pattern;

  int nfeatures;
  double scaleFactor;
  int nlevels;
  int fastThreshold;

  std::vector<int> mnFeaturesPerLevel;
  std::vector<cv::Rect> mnLayerInfo;

  std::vector<int> umax;

  std::vector<float> mvScaleFactor;
  std::vector<float> mvInvScaleFactor;
  std::vector<float> mvLevelSigma2;
  std::vector<float> mvInvLevelSigma2;

  int scoreType;

  bool detectKPs;
  bool griDetector;
};

#endif
