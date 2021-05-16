/*
 #############################################################################
 #        Author: Alessio Xompero
 #         Email: a.xompero@qmul.ac.uk
 #
 #
 #  Created Date: 2017/10/13
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

#include <bitset>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <iomanip>
#include <iostream>

#include "Frame.h"
#include "morb.h"



// ORB extractor parameters
const float fScaleFactor = 1.2;
const int nLevels = 8;
const int fIniThFAST = 20;

Frame::Frame( )
    { frameID = -1; }

Frame::Frame( int _frameID)
    { frameID = _frameID; }


//
bool Frame::detectAndComputeLocalFeatures(std::string imageFilename,
    int nFeatures, int BlurSz)
{
    // Read the image
    cv::Mat img = cv::imread(imageFilename);

    if (img.empty())
    {
        std::cerr << "!!! Failed imread(): image not found !!!" << std::endl;
        return false;
    }

    // Convert RGB to gray image
    cv::Mat mImGray = img;
    if (mImGray.channels() == 3)
        { cvtColor(mImGray, mImGray, CV_RGB2GRAY); }
    
    // Initialise the ORB extractor of ORB-SLAM
    morbExtractor = new MORB(nFeatures, fScaleFactor, nLevels, fIniThFAST,
        MORB::HARRIS_SCORE, true, false);

    // Detect and extract ORB key-points
    (*morbExtractor)(mImGray, cv::Mat(), mvKeys, descriptor, dtScales,
        BlurSz, "empty.txt", frameID);
    
    return true;
}

//
bool Frame::computeLocalFeatures(std::string imageFilename,
    std::string fnKeyPoints, int nFeatures, int BlurSz)
{
    // Read the image
    cv::Mat img = cv::imread(imageFilename);

    if (img.empty())
    {
        std::cerr << "!!! Failed imread(): image not found !!!" << std::endl;
        return false;
    }

    // Convert RGB to gray image
    cv::Mat mImGray = img;
    if (mImGray.channels() == 3)
        { cvtColor(mImGray, mImGray, CV_RGB2GRAY); }
    
    // Initialise the ORB extractor of ORB-SLAM
    morbExtractor = new MORB(nFeatures, fScaleFactor, nLevels, fIniThFAST,
        MORB::HARRIS_SCORE, false, false);

    // Detect and extract ORB key-points
    (*morbExtractor)(mImGray, cv::Mat(), mvKeys, descriptor, dtScales,
        BlurSz, fnKeyPoints, frameID);
    
    return true;
}

//
void Frame::showKeypoints(cv::InputArray _img, std::vector<cv::KeyPoint> mvKeys,
    int flag)
{
    cv::Mat img = _img.getMat();
    cv::Mat imgKP = img; // new image where to embed the key-points

    cv::drawKeypoints(img, mvKeys, imgKP, cv::Scalar(0,0,255));

    cv::namedWindow( "ORB Keypoints", cv::WINDOW_AUTOSIZE );
    cv::imshow("ORB Keypoints", imgKP);

    if (flag == 1)
        cv::imwrite("keypoints.png", imgKP);

    cv::waitKey(0);
}

//
void Frame::saveDescriptors(std::vector<cv::KeyPoint> mvKeys,
    cv::InputArray _descriptor, std::string filename)
{
    cv::Mat descriptor = _descriptor.getMat();
    
    int N = descriptor.rows;
    int C = descriptor.cols;

    descriptor.convertTo(descriptor, CV_32SC1);

    // Save descriptors
    FILE* fout = fopen(filename.c_str(), "w");
    
    for (int n=0; n<N; ++n)
    {
        fprintf(fout, "%.7e %.7e %d %.7e %f\n", mvKeys[n].pt.x,
            mvKeys[n].pt.y, mvKeys[n].octave, mvKeys[n].angle,
            mvKeys[n].size);

        for (int s = 0; s < C; ++s )
        {
            std::string binary = std::bitset<8>(descriptor.at<int>(n,s)).to_string();
            fprintf(fout, "%c ", binary[7]);
            fprintf(fout, "%c ", binary[6]);
            fprintf(fout, "%c ", binary[5]);
            fprintf(fout, "%c ", binary[4]);
            fprintf(fout, "%c ", binary[3]);
            fprintf(fout, "%c ", binary[2]);
            fprintf(fout, "%c ", binary[1]);
            fprintf(fout, "%c ", binary[0]);
        }

        fprintf(fout, "\n");
    }
    
    fclose(fout);

    std::cout << "Saved key-points coordinates and descriptors!" << std::endl;
}


//
void Frame::saveDescriptorsAndIntensityDiff(std::vector<cv::KeyPoint> mvKeys,
    std::vector<cv::Mat> descriptor, std::string filename)
{
    int N = (int)mvKeys.size();

    // Save descriptors
    FILE* fout = fopen(filename.c_str(), "w");

    //fprintf(fout, "%d \n", N);  
    
    for (int n=0; n<N; ++n)
    {
        //fprintf(fout, "%d - ", n+1);
        fprintf(fout, "%.7e %.7e %d %.7e %f\n", mvKeys[n].pt.x,
            mvKeys[n].pt.y, mvKeys[n].octave, mvKeys[n].angle,
            mvKeys[n].size);
        
        for (int j=0;j<32; ++j)
            fprintf(fout, "%u ", descriptor[0].at<uchar>(n,j));

        fprintf(fout, "\n");

        for (int j=0;j<256; ++j)
            fprintf(fout, "%d ", descriptor[1].at<int>(n,j));

        fprintf(fout, "\n");

        for (int j=0;j<256; ++j)
            fprintf(fout, "%d ", descriptor[2].at<int>(n,j));

        fprintf(fout, "\n");
    }
    
    fclose(fout);

    std::cout << "Saved key-points coordinates, descriptors and intensity diff!" << std::endl;
}

 // Save descriptors
void Frame::saveMultiScaleDescriptor(std::string filename,
    std::vector<std::vector<cv::KeyPoint> > allKeypoints,
    cv::InputArray _sortedDescs, int nlevels)
{
    cv::Mat sortedDescs = _sortedDescs.getMat();
    std::vector<cv::KeyPoint>& keypoints = allKeypoints[0];

    int nkeypointsLevel = (int)allKeypoints[0].size();
    int nkeypoints = nkeypointsLevel * nlevels;
    
    // "multiscaleORB.txt"
    FILE* fout = fopen(filename.c_str(), "w");
    
    fprintf(fout, "%d \n", nkeypointsLevel);  

    int c;
    for (int n=0; n<nkeypoints; ++n)
    {
        c = floor(n / nlevels);
        fprintf(fout, "%.7e %.7e ", keypoints[c].pt.x, keypoints[c].pt.y);
        
        for (int j=0;j<32; ++j)
            fprintf(fout, "%u ", sortedDescs.at<uchar>(n,j));

        fprintf(fout, "\n");
    }
    
    fclose(fout);
}

