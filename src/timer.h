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

#ifndef INCLUDE_TIMER_H_
#define INCLUDE_TIMER_H_

#include <cmath>
#include <iostream>
#include <chrono>
#include <string>

class Timer {
 private:
  // Type aliases to make accessing nested type easier
  using clock_t = std::chrono::high_resolution_clock;
  using second_t = std::chrono::duration<double, std::ratio<1> >;

  std::chrono::time_point<clock_t> m_beg;

 public:
  Timer()
      : m_beg(clock_t::now()) {
  }

  void reset() {
    m_beg = clock_t::now();
  }

  double elapsed() const {
    return std::chrono::duration_cast<second_t>(clock_t::now() - m_beg).count();
  }

  void print(const std::string& heading) {
    std::cout << "***Running time for " << heading << ": ";
    if (elapsed() < 1) {
      std::cout << elapsed() * pow(10, 3) << " milliseconds\n";
    } else if (elapsed() < 60) {
      std::cout << elapsed() << " seconds\n";
    } else {
      std::cout << elapsed() / 60 << " minutes\n";
    }

  }

  /**
   * modes:
   *  1. Local feature extraction
   *  2. Feature tracking
   *  3. Re-detection
   *  4. Frame processing
   *  5. VPR_Processing
   */
  void savetofile(const std::string& filename, const int& mode) {
    FILE* fout = fopen(filename.c_str(), "a");
    fprintf(fout, "%d %.6f\n", mode, elapsed() * pow(10, 3));
    fclose(fout);
  }

};


#endif /* INCLUDE_TIMER_H_ */
