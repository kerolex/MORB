/*
 #############################################################################
 #        Author: Alessio Xompero
 #         Email: a.xompero@qmul.ac.uk
 #
 #
 #  Created Date: 2019/08/03
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

#ifndef SRC_MORB_MATCHER_H_
#define SRC_MORB_MATCHER_H_

// STL Libraries
#include <string>
#include <vector>

// OpenCV Libraries
#include <opencv2/opencv.hpp>

class Matcher {
 public:
  enum MatchMode {
    MORB_RM = 0,  // Cross-scale global minimum or set2setmindist
    MORB_D = 1,
    MORB_C = 2,  // Finds the minimum after filtering to use the consistency check + considering the detection scales
    MORB_C_SEC = 3,
    MORB_wC = 4,
    MORB_wC_SEC = 5,
    cORB = 6
  };

  Matcher()
      : W(0),
        R(0),
        maxCond(0),
        southEastCheck(0),
        nLevels(8) {

  }

  Matcher(int _W, int _R, int _nLevels, int matchingStrategy) {
    W = _W;
    R = _R;
    maxCond = 0;
    southEastCheck = false;
    nLevels = _nLevels;

    switch (matchingStrategy) {
      case Matcher::MatchMode::MORB_RM:
        maxCond = 0;
        southEastCheck = false;
        break;
      case Matcher::MatchMode::MORB_D:
        maxCond = 0;
        southEastCheck = false;
        break;
      case Matcher::MatchMode::MORB_C:
        maxCond = 0;
        southEastCheck = false;
        break;
      case Matcher::MatchMode::MORB_C_SEC:
        maxCond = 0;
        southEastCheck = true;
        break;
      case Matcher::MatchMode::MORB_wC:
        maxCond = 0;
        southEastCheck = false;
        break;
      case Matcher::MatchMode::MORB_wC_SEC:
        maxCond = 0;
        southEastCheck = true;
        break;
      case Matcher::MatchMode::cORB:
        maxCond = 0;
        southEastCheck = false;
        W = 0;
        R = -1;
        break;
      default:
        maxCond = 0;
        southEastCheck = false;
    }

  }

  /**
   *
   */
  void multiScaleMatching(cv::InputArray _desc1, std::vector<int> dtScales1,
                          cv::InputArray _desc2, std::vector<int> dtScales2,
                          cv::OutputArray _H, cv::OutputArray _Hs,
                          cv::OutputArray _S1, cv::OutputArray _S2,
                          cv::OutputArray _matches, MatchMode _ = MORB_RM);

  /**
   *
   */
  void saveMatching(cv::InputArray _M, std::string filename);

 private:
  /**
   *
   */
  int DescriptorDistance(const cv::Mat &a, const cv::Mat &b);

  /**
   * @fn minHammingAndScale
   *
   * @brief Compute the minimum normalised Hamming distance among all alignment
   *        of the multi-scale ORB descriptors.
   *
   * @param[in] block A NxN matrix containing all the Hamming distances
   *  between the multi-scale ORB descriptors at all scale levels.
   *  N is the number of scale levels.
   * @param[in,out] hamm The minimum normalised Hamming distance at a
   *  specific scale level difference.
   * @param[in,out] scaleDiff The scale level difference of the minimum
   *  normalised Hamming distance.
   */
  void minHammingAndScale(cv::InputArray _block, std::vector<int>& scores,
                          float hamm = 255, int scaleDiff = 0);

  /**
   *
   *
   */
  void scaleMatchingMin(cv::InputArray _block, std::vector<int>& scores);

  /**
   * @fn  getDistanceAndScaleRatio()
   * @brief Compute the distance and offset of two multi-scale descriptors.
   *
   * @param s1  Detection scale of the feature in the first image.
   * @param s2  Detection scale of the feature in the second image.
   * @param WMA Boolean variable to determine if also computing weights.
   * @param SEC Boolean variable to determine if computing only the single scale
   *            descriptor distances at resolution layers higher than the detection
   *            scale (i.e. coarser levels).
   * @param W   Half-length of the diagonal for the cross-correlation based distance.
   * @param R   Variable that denotes if to compute only the value at the detection
   *            scale (R=-1), if only at lower scales than the detection scale of
   *            either one of the two features (R=0), or to find the minimum in
   *            all possible cross scales between features. When R
   */
  void getDistanceAndScaleRatio(cv::InputArray _block, std::vector<int>& scores,
                                int s1, int s2, bool WMA);

 public:
  int W;  //
  int R;  //
  int maxCond;
  bool southEastCheck;

  int nLevels;
};

#endif /* SRC_MORB_MATCHER_H_ */
