#!/bin/bash

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

now=$(date +"%Y.%m.%d")


###### SETTINGS #####
MORB=../morb

N=1000 	# Number of desired features
W=7			# Gaussian kernel size
R=-1
dataset=venice
n=7    # Target image index


DATAPATH=.
#../datasets/$dataset


# Multi-scale matching mode:
MSmatch=0

################################################################################
echo "*** ${dataset} ***"
echo ">>>>>>> Image 1 vs Image " $n

## Methods
# 1. ORB
fmt=orb

# Detect ORB features
../detector $DATAPATH/img1.jpg    -F $N
../detector $DATAPATH/img${n}.jpg -F $N


echo "*** Running MORB ***"

# Output file extension and destination folder based on the matching mode
fmt2=morb


$MORB $DATAPATH/img1.jpg $DATAPATH/img$n.jpg img1.$fmt img${n}.$fmt \
  "img1."$fmt2 "img${n}."$fmt2 $N $W $MSmatch -1 $R
mv keypoints2.dat keypoints$n.dat	
mv cullpoints2.dat cullpointss$n.dat	
mv matches.txt matches_1vs$n.txt

echo "Done!"
