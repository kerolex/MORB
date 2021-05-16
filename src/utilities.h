/*
 #############################################################################
 #        Author: Alessio Xompero
 #         Email: a.xompero@qmul.ac.uk
 #
 #
 #  Created Date: 2017/11/14
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

#ifndef _UTILITIES_H_
#define _UTILITIES_H_

// STD Libraries
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>

// OpenCV Libraries
#include <opencv2/opencv.hpp>

void PrintHeading1(const std::string& heading);

void PrintHeading2(const std::string& heading);

void PrintClosing2(const std::string& heading);



enum distance_type{
    L1_NORM = 0,
    L2_NORM = 1,
    HAMMING = 2
};

/**
 *
 */
void readFeatures(std::string output, std::vector<cv::Point2f> &feats);

/**
 * @override
 */
void readFeatures(std::string output, std::vector<cv::Point2f> &KeyPointsLoc,
                  cv::OutputArray KeyPointsDesc);

/**
 *
 */
void saveFeatures(std::vector<cv::Point2f> KeyPointsLoc,
                  cv::InputArray KeyPointsDesc, std::string output);


/**
 *
 */
void saveFeatures(std::vector< cv::KeyPoint > KeyPointsLoc,
    cv::InputArray KeyPointsDesc, std::string output,
        distance_type dt, int nOctaves);

/**
 *
 */
bool writeMatches(cv::InputArray _matches, std::string matchesFileName);

/**
 *
 */
bool checkDuplicates(cv::InputArray _V);

/**
 *
 */
void saveKeyPoints(std::string output,
                   const std::vector<std::vector<cv::KeyPoint> >& allKeypoints);

void saveKeyPoints(std::string output,
                   const std::vector<std::vector<cv::KeyPoint> >& allKeypoints,
                   std::vector<int> dtScales);

/**
 *
 */
void readKeyPoints(std::string output, std::vector<cv::KeyPoint>& allKeypoints);

/**
 *
 */
void readHomography(std::string filename, cv::OutputArray _H);

#endif
