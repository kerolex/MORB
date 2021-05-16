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

#include <chrono>
#include "matcher.h"

//
void Matcher::multiScaleMatching(cv::InputArray _desc1,
                                 std::vector<int> dtScales1,
                                 cv::InputArray _desc2,
                                 std::vector<int> dtScales2, cv::OutputArray _H,
                                 cv::OutputArray _Hs, cv::OutputArray _S1,
                                 cv::OutputArray _S2, cv::OutputArray _matches,
                                 MatchMode mm) {
  cv::Mat D1 = _desc1.getMat();
  cv::Mat D2 = _desc2.getMat();

  using seco = std::chrono::seconds;
  auto startBFM = std::chrono::high_resolution_clock::now();

  // Multi-scale brute force matching
  std::cout << "Multi-scale brute force matching.." << std::endl;
  int L1 = D1.rows;
  int L2 = D2.rows;

  cv::Mat id1(L1, 1, CV_32SC1);
  for (int j = 0; j < L1; ++j) {
    id1.at<int>(j) = nLevels * (j % (L1 / nLevels)) + j / (L1 / nLevels);
  }

  cv::Mat id2(L2, 1, CV_32SC1);
  for (int n = 0; n < L2; ++n) {
    id2.at<int>(n) = nLevels * (n % (L2 / nLevels)) + n / (L2 / nLevels);
  }

  cv::Mat H(L1, L2, CV_32SC1);
  for (int j = 0; j < L1; ++j) {
    int iDx1 = id1.at<int>(j);
    for (int n = 0; n < L2; ++n) {
      int iDx2 = id2.at<int>(n);
      int hd = DescriptorDistance(D1.row(j), D2.row(n));
      H.at<int>(iDx1, iDx2) = hd;
    }
  }

  id1.release();
  id2.release();

  auto finishBFM = std::chrono::high_resolution_clock::now();
  std::cout << "MORB: brute force matching took "
            << std::chrono::duration_cast<seco>(finishBFM - startBFM).count()
            << " seconds\n";

  ////////////////////////////////////////////////////////////////////
  // Scaled matches
  std::cout << "Scaled matches.." << std::endl;
  cv::Mat S1(L1 / nLevels, L2 / nLevels, CV_32SC1);
  cv::Mat S2(L1 / nLevels, L2 / nLevels, CV_32SC1);
  cv::Mat Hs(L1 / nLevels, L2 / nLevels, CV_32SC1);

  //int s1 = 3;
  //int s2 = 5;

  std::vector<int> scores;
  int a = 0, b;
  for (int j = 0; j < L1; j += nLevels) {
    b = 0;
    for (int n = 0; n < L2; n += nLevels) {
      cv::Mat block = H.rowRange(j, j + nLevels).colRange(n, n + nLevels);

      switch (mm) {
        case MORB_RM:
          scaleMatchingMin(block, scores);
          break;
        case MORB_D:
          //std::cout << "Here" << std::endl;
          minHammingAndScale(block, scores);
          break;
        case MORB_C:
//          getDistanceAndScaleRatio(block, scores, dtScales1[a], dtScales2[b], W,
//                                   false, false, R);
          getDistanceAndScaleRatio(block, scores, dtScales1[a], dtScales2[b],
                                   false);
          break;
        case MORB_C_SEC:
//          getDistanceAndScaleRatio(block, scores, dtScales1[a], dtScales2[b], W,
//                                   true, false, R);
          getDistanceAndScaleRatio(block, scores, dtScales1[a], dtScales2[b],
                                   false);
          break;
        case MORB_wC:
//          getDistanceAndScaleRatio(block, scores, dtScales1[a], dtScales2[b], W,
//                                             false, true, R);
          getDistanceAndScaleRatio(block, scores, dtScales1[a], dtScales2[b],
                                   true);
          break;
        case MORB_wC_SEC:
//          getDistanceAndScaleRatio(block, scores, dtScales1[a], dtScales2[b], W,
//                                             true, true, R);
          getDistanceAndScaleRatio(block, scores, dtScales1[a], dtScales2[b],
                                   true);
          break;
        case cORB:
//          getDistanceAndScaleRatio(block, scores, dtScales1[a], dtScales2[b], 0,
//                                            false, false, -1);
          getDistanceAndScaleRatio(block, scores, dtScales1[a], dtScales2[b],
                                   false);
          break;
        default:
          scaleMatchingMin(block, scores);
      }

      Hs.at<int>(a, b) = scores[0];
      S1.at<int>(a, b) = scores[1];
      S2.at<int>(a, b) = scores[2];

//      if (j == 62 * nLevels && n == 0) {
//        std::cout << "Press a button..." << std::endl;
//        std::cin.get();
//      }

      ++b;
    }
    ++a;
  }

  auto finishSM = std::chrono::high_resolution_clock::now();
  std::cout
      << "MORB: scale-aware pair-wise descriptor distance and scale ratio estimation took "
      << std::chrono::duration_cast<seco>(finishSM - finishBFM).count()
      << " seconds\n";

  ////////////////////////////////////////////////////////////////////
  //Get matches
  std::cout << "Get matches.." << std::endl;
  cv::Mat dd(1, L2 / nLevels, CV_32SC1), idx1(1, L2 / nLevels, CV_32SC1), idx2;
  for (int j = 0; j < L2 / nLevels; ++j) {
    int minH = 255;
    int bestIdx = 0;
    for (int n = 0; n < L1 / nLevels; ++n) {
      if (Hs.at<int>(n, j) < minH) {
        minH = Hs.at<int>(n, j);
        bestIdx = n;
      }
    }
    dd.at<int>(j) = minH;
    idx1.at<int>(j) = bestIdx;
  }

  sortIdx(dd, idx2, CV_SORT_ASCENDING);

  cv::Mat V = cv::Mat::zeros(L1, 1, CV_32SC1);

  cv::Mat matches;
  for (int k = 0; k < idx2.cols; ++k) {
    int j = idx1.at<int>(idx2.at<int>(k));

    /**
     std::cout << k << ", " << j << ", " << idx2.at<int>(k) << ", " <<
     Hs.at<int>(j, idx2.at<int>(k)) << ", " << S1.at<int>(j, idx2.at<int>(k)) <<
     ", " << S2.at<int>(j, idx2.at<int>(k)) << ", " << V.at<int>(j) <<
     std::endl;
     */

    if (V.at<int>(j) == 1) {
      continue;
    }

    V.at<int>(j) = 1;

    cv::Mat tmp = (cv::Mat_<int>(1, 5) << j, idx2.at<int>(k), Hs.at<int>(
        j, idx2.at<int>(k)), S1.at<int>(j, idx2.at<int>(k)), S2.at<int>(
        j, idx2.at<int>(k)));

    matches.push_back(tmp);
  }

  auto finishNNM = std::chrono::high_resolution_clock::now();
  std::cout << "MORB: nearest neighbour matching took "
            << std::chrono::duration_cast<seco>(finishNNM - finishSM).count()
            << " seconds\n";

  std::cout << "MORB: scale-aware matching took "
            << std::chrono::duration_cast<seco>(finishNNM - startBFM).count()
            << " seconds\n";

  H.copyTo(_H);
  Hs.copyTo(_Hs);
  S1.copyTo(_S1);
  S2.copyTo(_S2);
  matches.copyTo(_matches);

}

// Compute the minimum normalised Hamming distance among all alignment
// of the multi-scale ORB descriptors.
void Matcher::minHammingAndScale(cv::InputArray _block,
                                 std::vector<int>& scores, float hamm,
                                 int scaleDiff) {
  scores.clear();
  scores.resize(3);

  cv::Mat bb = _block.getMat();

  assert(bb.rows == bb.cols);

  int N = bb.rows;  // Number of scale levels

  std::map<int, std::vector<int>> dist;

  for (int j = 0; j < N; ++j) {
    for (int n = 0; n < N; ++n) {
      std::vector<int> &step = dist[j - n];
      if (step.empty()) {
        step.resize(2);
        step[0] = bb.at<int>(j, n);
        step[1] = 1;
      } else {
        step[0] += bb.at<int>(j, n);
        step[1] += 1;
      }
    }
  }

  std::map<int, std::vector<int>>::iterator iti, ite;
  iti = dist.begin();
  ite = dist.end();

  for (; iti != ite; ++iti) {
    //std::cout << iti->first << " -> [" << iti->second[0] << ", "
    //          << iti->second[1] << "]" << std::endl;
    int score = (int) ceil((float) iti->second[0] / iti->second[1]);
    if (score < hamm) {
      hamm = score;
      scaleDiff = iti->first;
    }
  }

  scores[0] = (int) hamm;
  if (scaleDiff < 0) {
    scores[1] = 0;
    scores[2] = (int) abs(scaleDiff);
  } else {
    scores[1] = (int) abs(scaleDiff);
    scores[2] = 0;
  }
}

//
void Matcher::scaleMatchingMin(cv::InputArray _block,
                               std::vector<int>& scores) {
  scores.clear();
  scores.resize(3);

  cv::Mat block = _block.getMat();

  assert(block.rows == block.cols);

  double minHblock;
  cv::Point idx;
  cv::minMaxLoc(block, &minHblock, 0, &idx);

  switch (maxCond) {
    case 1:
      if (idx.y == 0 || idx.x == 0) {
        scores[0] = (int) minHblock;
        scores[1] = (int) idx.y;
        scores[2] = (int) idx.x;
      } else {
        scores[0] = (int) 10000;
        scores[1] = (int) idx.y;
        scores[2] = (int) idx.x;
      }
      break;
    case 2:
      if (idx.y == 0 || idx.x == 0 || idx.y == 1 || idx.x == 1) {
        scores[0] = (int) minHblock;
        scores[1] = (int) idx.y;
        scores[2] = (int) idx.x;
      } else {
        scores[0] = (int) 10000;
        scores[1] = (int) idx.y;
        scores[2] = (int) idx.x;
      }
      break;
    default:
      scores[0] = (int) minHblock;
      scores[1] = (int) idx.y;
      scores[2] = (int) idx.x;
  }

  if (southEastCheck) {
    int N = block.rows;  // Number of scale levels

    int j = idx.y + 1, n = idx.x + 1;
    int hammDistSE = (int) minHblock;
    int counterCells = 1;

    while (j < N && n < N) {
      hammDistSE += block.at<int>(j, n);

      ++j;
      ++n;
      ++counterCells;
    }

    scores[0] = (int) hammDistSE / counterCells;
    scores[1] = (int) idx.y;
    scores[2] = (int) idx.x;
  }
}

// Get the distance and offset of the two points
void Matcher::getDistanceAndScaleRatio(cv::InputArray _block,
                                       std::vector<int>& scores, int s1, int s2,
                                       bool WMA) {
  int v = 0;  // Visualisation of the matrices

  scores.clear();
  scores.resize(3);

  cv::Mat block = _block.getMat();

  cv::Mat K = cv::Mat::eye(W * 2 + 1, W * 2 + 1, block.type());  // Diagonal kernel
  K.convertTo(K, CV_32FC1);

  if (southEastCheck && WMA) {
    cv::Mat tmp = K.rowRange(0, W).colRange(0, W);
    tmp.setTo(0);

    double a, tot = 0;
    ;
    std::vector<float> q;
    for (int j = 0; j < W + 1; ++j) {
      //std::cout << j/ (float)(L + 1) << std::endl;
      //std::cout << pow(j/ (float) (L + 1), 2) << std::endl;
      //std::cout << 1 - pow(j/ (float)(L + 1), 2) << std::endl;
      a = pow(1 - pow(j / (float) (W + 1), 2), 2);
      //std::cout << a << std::endl;
      q.push_back(a);
      tot += a;
    }

    for (int j = W; j < 2 * W + 1; ++j) {
      //std::cout << q[j-L] << " / " << tot << std::endl;
      K.at<float>(j, j) = q[j - W] / tot;
    }
  } else if (southEastCheck) {
    cv::Mat tmp = K.rowRange(0, W).colRange(0, W);
    tmp.setTo(0);
  } else if (WMA) {
    double a, tot = 0;
    ;
    std::vector<float> q;
    for (int j = -W; j < W + 1; ++j) {
      a = pow(1 - pow(j / (float) (W + 1), 2), 2);
      q.push_back(a);
      tot += a;
    }

    for (int j = 0; j < 2 * W + 1; ++j) {
      K.at<float>(j, j) = q[j] / tot;
    }
  }

  cv::Mat dummyOnes = cv::Mat::ones(block.rows, block.cols, block.type());

  cv::Mat fBlock, normFact;

  block.convertTo(block, CV_32FC1);
  dummyOnes.convertTo(dummyOnes, CV_32FC1);

  cv::filter2D(block, fBlock, -1, K, cv::Point(-1, -1), 0, cv::BORDER_ISOLATED);  // Convolution of the Hamming distance matrix with the diagonal kernel
  cv::filter2D(dummyOnes, normFact, -1, K, cv::Point(-1, -1), 0,
               cv::BORDER_ISOLATED);  // Convolution of the Hamming distance matrix with the diagonal kernel

  if (v == 1) {
    std::cout << block << std::endl;
    std::cout << "Kernel:" << std::endl;
    std::cout << K << std::endl;

    std::cout << "Block after conv:" << std::endl;
    std::cout << fBlock << std::endl;

    std::cout << "Normalisation factor:" << std::endl;
    std::cout << normFact << std::endl;
  }

  cv::divide(fBlock, normFact, fBlock);

  dummyOnes *= 256;
  if (R == -1) {
    dummyOnes.at<float>(s1, s2) = 0;
  } else if (R == 0) {
    //cv::Mat row0 = dummyOnes.row(s1).colRange(s2, dummyOnes.cols);
    cv::Mat row0 = dummyOnes.row(s1).colRange(0, dummyOnes.cols);
    row0.setTo(0);

    //cv::Mat col0 = dummyOnes.col(s2).rowRange(s1, dummyOnes.rows);
    cv::Mat col0 = dummyOnes.col(s2).rowRange(0, dummyOnes.rows);
    col0.setTo(0);
  } else {
    cv::Mat area = dummyOnes.rowRange(s1, dummyOnes.rows).colRange(
        s2, dummyOnes.cols);
    area.setTo(0);
  }

  cv::Mat roi = fBlock + dummyOnes;

  double minHblock;
  cv::Point idx;
  cv::minMaxLoc(roi, &minHblock, 0, &idx);

  scores[0] = (int) round(minHblock);  // It would be better ceil as done for MORB-D
  scores[1] = (int) idx.y;
  scores[2] = (int) idx.x;

  if (v == 1) {
    std::cout << "Block after conv and normalisation:" << std::endl;
    std::cout << fBlock << std::endl;
    std::cout << "Mask:" << std::endl;
    std::cout << dummyOnes << std::endl;
    std::cout << "Masked block:" << std::endl;
    std::cout << roi << std::endl;

    std::cout << scores[0] << ", " << scores[1] << ", " << scores[2]
              << std::endl;

    std::cout << "Press button to continue..." << std::endl;
    std::cin.get();
  }
}

// Bit set count operation from
// http://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
int Matcher::DescriptorDistance(const cv::Mat &a, const cv::Mat &b) {
  const int *pa = a.ptr<int32_t>();
  const int *pb = b.ptr<int32_t>();

  int dist = 0;

  for (int i = 0; i < 8; i++, pa++, pb++) {
    unsigned int v = *pa ^ *pb;
    v = v - ((v >> 1) & 0x55555555);
    v = (v & 0x33333333) + ((v >> 2) & 0x33333333);
    dist += (((v + (v >> 4)) & 0xF0F0F0F) * 0x1010101) >> 24;
  }

  return dist;
}

void Matcher::saveMatching(cv::InputArray _M, std::string filename) {
  cv::Mat M = _M.getMat();
  int r = M.rows;
  int c = M.cols;

  // Save descriptors
  FILE* fout = fopen(filename.c_str(), "w");

  for (int j = 0; j < r; ++j) {
    for (int n = 0; n < c; ++n) {
      fprintf(fout, "%d ", M.at<int>(j, n));
    }
    fprintf(fout, "\n");
  }
  fclose(fout);
}

