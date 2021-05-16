%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%
%        Author: Alessio Xompero
%         Email: a.xompero@qmul.ac.uk
% 
%  Created Date: 2017/10/17
% Modified Date: 2021/05/12
% 
% Centre for Intelligent Sensing, Queen Mary University of London, UK
% 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% 
%  The MIT License (MIT)
% 
%  Copyright © 2021 <copyright holders>
% 
%  Permission is hereby granted, free of charge, to any person obtaining a copy
%  of this software and associated documentation files (the “Software”), to deal
%  in the Software without restriction, including without limitation the rights
%  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
%  copies of the Software, and to permit persons to whom the Software is
%  furnished to do so, subject to the following conditions:
% 
%  The above copyright notice and this permission notice shall be included in
%  all copies or substantial portions of the Software.
% 
%  THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
%  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
%  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
%  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
%  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
%  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
%  SOFTWARE.
% 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%
%--------prologue
clear all; close all; clc;

% Settings
num_scales=8;
eta=2.5;  % threshold on validating a ground-truth correspondence after homography transformation
cam2_id = 7; % index of the second view

sequence='venice';
imgfmt='jpg';

method ='morb';

[NNAF, MS] = computeNearestNeighborAverageF1Score(sequence, cam2_id, ...
        num_scales, eta, method, imgfmt);

disp(['*** ' sequence ' 1 vs ' num2str(cam2_id) ' ***'])
disp(['Nearest Neighbour Average F1-score: ' num2str(NNAF, '%.3f')])
disp(['Matching score: ' num2str(MS, '%.3f')])
      
function [NNAF, MS] = computeNearestNeighborAverageF1Score(sequence, n, ...
  nLevels, d_th, filefmt, imgfmt)
%
% Parameters:
%   - n: index of the target image in the set
%   - nLevels: number of scales in the image pyramid
%   - d_th: threshold to determine a ground-truth correspondence
%   - filefmt: format of the file with the keypoints (e.g. img1.morb)
%   - imgfmt: format of the image (e.g. jpg)
%
% Output:
%   - NNAF: the nearest neighbour average F-1 score
%
% Example:
%   [NNAF MS] = computeNearestNeighborAverageF1Score(7, 8, 2.5, 'morb', 'jpg')
%
%    Creation Date: 2019/08/02
%    Modified Date: 2019/08/02
%           Author: Alessio Xompero
%            email: a.xompero@qmul.ac.uk
%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Parse data
imageFilename1 = 'img1';
imageFilename2 = ['img' num2str(n) ];

I1 = imread(fullfile([imageFilename1, '.' imgfmt]));
I2 = imread(fullfile([imageFilename2, '.' imgfmt]));

[I1h, I1w, ~] = size(I1);
[I2h, I2w, ~] = size(I2);

homographyFN = ['H1to' num2str(n) 'p'];
H = dlmread(homographyFN);

keypoints1FN = dlmread(fullfile(['img1.' filefmt]));
keypoints2FN = dlmread(fullfile(['img' num2str(n) '.' filefmt]));
% HD = dlmread('multiscaleHamming.txt');

kps1 = keypoints1FN(1:2:end,1:5);
kps2 = keypoints2FN(1:2:end,1:5);

L1 = size(kps1,1)/nLevels;
L2 = size(kps2,1)/nLevels;

kps1 = kps1(kps1(:,3) == 0,1:5);
kps2 = kps2(kps2(:,3) == 0,1:5);

kps1t = H * [kps1(1:L1,1:2)'; ones(1,L1)];
nFeats1 = sum(prod(kps1t(1:2,:) < repmat([I2w; I2h],1,size(kps1t,2)) ));

kps2t = inv(H) * [kps2(1:L2,1:2)'; ones(1,L2)];
nFeats2 = sum(prod(kps2t(1:2,:) < repmat([I1w; I1h],1, size(kps2t,2)) ));

% S = dlmread(fullfile(RESPATH,'scaledHamming.txt'));
matches = dlmread(fullfile(['matches_1vs' num2str(n) '.txt']));

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Compute groundtruth correspondences with brute force matching
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Compute the image distance between each transformed key-point and the
% key-points in the target image
S = ones(L1, L2) * 100000;
for k=1:nFeats1
  kp1 = H * [kps1(k,1:2) 1]';
  kp1 = kp1 ./ kp1(3);
  
  for m=1:L2
    kp2 = [kps2(m,1:2) 1]';
    S(k,m) = norm(kp1 - kp2);
  end
end

% Compute the correspondences as an assignment binary matrix
%disp('Compute the correspondences as an assignment binary matrix')
S1 = ones(L1, L2) * 100000;

minr = 0;
N = 0;
w = 0;
while minr < 10 && w < max(nFeats1,nFeats2)
  minr = 100000;
  
  [minr,minIdx] = min(S(:));
  [miny, minx] = ind2sub(size(S),minIdx);
  
  if minr < d_th
    S(miny,:) = 100000;
    S(:,minx) = 100000;
    
    S1(miny,minx) = minr;
    
    N = N + 1;
  end
  
  w = w  + 1;
end

% Compute the number of correspondences. A correspondence is valid if the image
% distance is lower than th pixels
G = double(sum(sum(S1 < d_th)));


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Similarity matching
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
a = 1;
res = zeros(129, 11);
for hd_th=0:128
  J = matches(matches(:,3) < hd_th,1) + 1;
   
  M = double(length(J));
  
  if M == 0
    TP = 0;
    FP = 0;
    P = 0;
    R = 0;
    F = 0;
    MS = 0;
  else
    K = matches(matches(:,3) < hd_th,2) + 1;
    
    mKPS1 = kps1(J,1:2) + 1;
    mKPS2 = kps2(K,1:2) + 1;
    
    mKPS1t = H * [mKPS1 ones(M,1)]';
    mKPS1t = [mKPS1t(1,:) ./ mKPS1t(3,:);mKPS1t(2,:) ./ mKPS1t(3,:)];
    mKPS1t = mKPS1t';
    
    TP_idx = sqrt(sum((mKPS1t - mKPS2).^2,2)) < d_th;
    FP_idx = sqrt(sum((mKPS1t - mKPS2).^2,2)) >= d_th;
    
    TP = sum(TP_idx);    % Number of correct matches (True Positives)
    FP = sum(FP_idx);    % Number of wrong matches (False Positives)
    
    P = TP / M;
    R = TP / G;
    F = 2 * P * R / (P + R);
    F(isnan(F)) = 0;
    MS = TP / min(nFeats1, L2);
  end
  
  res(a,:) = [hd_th, L1, L2, G, M, TP, FP, P, R, F, MS];
  a = a + 1;
end

NNAF = trapz(double(res(:,1)/128), double(res(:,10)));
MS = res(end,11);
end