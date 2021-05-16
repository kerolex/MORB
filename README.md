# MORB: A multi-scale binary descriptor

This is the official repository of MORB, a multi-scale binary descriptor based
on ORB that improves the accuracy of feature matching under scale changes. 
MORB describes an image patch at different scales using a properly oriented 
sampling pattern of intensity comparisons in predefined pixel pairs. 
For this multi-scale descriptor, we also propose a matching strategy that 
estimates the cross-scale match between MORB descriptors in different views.

<p style="text-align: center;" markdown="1">
<img src="https://kerolex.github.io/images/MORB.png" width="400"/>
</p
<p style="text-align: center;" markdown="1">
[<a href="https://kerolex.github.io/projects/icip2018/2018_ICIP_MORBaMultiScaleBinaryDescriptor_Xompero_Lanz_Cavallaro.pdf" target="_blank">Paper</a>] 
[<a href="https://kerolex.github.io/projects/icip2018/2018_ICIP_poster_Xompero_Lanz_Cavallaro.pdf" TARGET = "_BLANK">Poster</a>]
</p>

The current software can be modified to use different local features than ORB.

Note that results may slighlty differ from those in the paper after cleaning and
releasing this code.

## Installation

### Requirements
* C++ (7.9) 
* OpenCV (3.4.1)
* MATLAB 2019 (for evaluation)
* CMake (2.8.9)
* Boost library (1.65.1)

The software has been tested on a Linux machine with Ubuntu 18.04 LTS OS. Note 
the version of the software tested in bracket.

### Instructions

The software can be compiled and installed on a Linux machine, e.g. running
Ubuntu 18.04, using the provided Bash script [build.sh](build.sh). The script
uses CMake to compile the [CMakelist](CMakeList.txt) file. The file should 
be appropriately changed based on the local machine configuration. For example,
you might want to change the path where OpenCV is installed. 

1. Open terminal (e.g., CTRL+ALT+T)
2. Run ``` source build.sh ```

The installation generates two executable files: [morb] and [detector]. 

The installation can be clean by running on the terminal:
``` source clean.sh```.

The script will remove the directory build, and the two executable files, [morb] 
and [detector].

### Demo

You can run a demo example to verify the installation and reproduce the results
for some images.  

1. Open terminal (e.g., CTRL+ALT+T)
2. Run

```
cd demo/
source run_demo.sh
```

You can play and change the settings within the bash script file to run MORB
on different image pairs of the _venice_ set. The reference image is always
the first one in the set and numbered as 1. The target image is one among the 
other images in the set (for _venice_, images are numbered from 2 to 7).
You can change the value of the variable _n_ to choose a difference target image. 

See Data format for input and output format of the files. 
Note that the current demo does not generate outputs showing the detected 
features and the matched features between the two images.  

After running the bash script, you can can compute the performance measures, 
Nearest Neighbour Average F-1 Score (NN-AF) and Matching Score (MS), between the two images by 
running the script ``evaluation_scores.m`` in MATLAB. 

For reproducibility, here are the results as provided in our paper:

| Image pair | NN-AF | MS   |
| -----------| ----- | ---- |
| venice 1-2 | 0.73  | 0.51 |
| venice 1-3 | 0.69  | 0.44 |
| venice 1-4 | 0.56  | 0.25 |
| venice 1-5 | 0.38  | 0.14 |
| venice 1-6 | 0.14  | 0.05 |
| venice 1-7 | 0.09  | 0.03 | 


## Running arguments
### detector

* imagepath: path and filename of the image to use for detecting the ORB features using OpenCV

**Options**
* -F: maximum number of desired features to detect in the input image (read through)

Example:

```
./detector img1.jpg -F 1000
```

### morb

1. image1: path and filename of the reference image (example: img1.jpg)
2. image2: path and filename of the target image (example: img1.jpg)
3. fnKps1: path and filename of the keypoints extracted with detector from the referencd image (example: img1.orb)
4. fnKps2: path and filename of the keypoints extracted with detector from the target image (example: img2.orb)
5. MorbKps1: path and filename of the output morb features from the referencd image (example: img1.morb)
6. MorbKps2: path and filename of the output morb features from the target image (example: img2.morb)
7. N: maximum number of desired features to detect in the input image (default value: 1000)  
8. W: Gaussian kernel size for the smoothing (default value: 7)
9. MSmatch: matching strategy (default value: 0). Note that we provide only the matching strategy described in the paper. 
10. W: Size of the kernel for the convolution (default value: -1). This was used for a different strategy not available now.
11. R: Size of the kernel for the convolution (default value: -1). This was used for a different strategy not available now.  

Example:

```
./morb img1.jpg img2.jpg img1.orb img2.orb img1.morb img2.morb 1000 7 0 -1 -1 
```


## Data format

We describe here the files outputted by each executable and their format. We also
use the image pair venice 1-7 as example to illustrate the content of the different
files. All files are saved in either *.txt* or *.dat* extension. 

### detector

The executable *detector* generates an output file with the list of all detected
ORB features with the following format. 

The first two rows contain the information about the rest of the file:
* D: descriptor dimensionality (256 for ORB);
* N: number of detected features.

For each row from the third one, the file provides the information for each ORB feature:
* x: image coordinate of the detected corner point on the x-axis;
* y: image coordinate of the detected corner point on the y-axis;
* octave: level of the ORB image pyramid (0 is the original resolution);
* angle: orientation angle as estimated by the intensity centroid method;
* size: length of the square patch (fixed to 31 for ORB);
* response: estimated Harris response;
* binary descriptor: D-dimensionality vector of binary (0,1) values. 

```
D
N
x y octave angle size response [binary descriptor]
...
...
...
x y octave angle size response [binary descriptor]
```

Example for img1.orb

```
256
1000
726 86 0 242.882 31 0.000489941 1 1 1 0 0 0 1 0 1 1 1 1 1 0 1 0 1 0 1 1 0 1 1 1 1 0 1 0 0 0 1 1 0 1 0 1 0 1 0 1 0 1 0 0 1 0 0 0 1 1 1 1 0 1 0 1 1 1 0 1 1 1 0 1 0 1 1 1 0 1 0 1 0 1 0 1 1 1 1 1 0 1 1 1 1 0 0 0 0 0 1 0 0 1 1 1 1 0 1 1 1 0 1 1 0 1 1 0 0 1 1 0 1 1 1 1 1 0 1 1 1 1 1 0 0 1 1 0 1 1 1 0 1 1 0 0 1 0 1 0 0 0 1 0 0 1 1 1 1 1 1 0 1 1 1 1 0 1 1 1 1 0 0 0 0 0 1 1 1 0 0 0 0 0 1 1 1 1 1 1 0 1 0 1 0 0 0 1 0 0 1 0 0 0 1 1 0 1 1 0 0 1 0 1 0 1 0 0 1 1 1 1 0 1 1 1 0 0 1 0 0 0 0 1 0 0 1 1 0 0 0 0 0 1 0 0 1 1 1 1 1 0 0 0 0 1 0 1 1 0 1 0 1 1 1 0 
...
...
...
426.399 462.23 7 333.623 111.079 0.000338766 0 1 0 0 1 0 0 1 1 1 0 1 1 1 1 0 1 1 0 0 0 0 0 1 1 0 1 0 1 1 0 1 0 1 1 0 0 1 0 0 0 1 0 1 0 0 1 0 1 0 1 1 0 1 0 1 0 1 1 0 1 1 1 1 0 0 0 0 1 1 0 0 0 1 0 0 0 1 1 1 0 1 0 0 1 0 1 0 1 1 1 0 0 1 1 1 1 0 1 0 1 1 0 1 1 0 1 0 0 1 0 1 1 0 0 0 1 0 0 1 1 1 0 0 0 0 0 0 0 1 1 1 0 1 0 0 1 0 1 1 0 1 1 0 1 0 1 0 1 0 0 0 1 0 1 1 0 0 1 1 1 0 0 1 0 1 1 1 0 0 1 1 1 0 1 1 0 0 1 0 0 0 0 1 0 0 0 1 0 1 0 0 0 0 1 0 0 1 0 1 0 0 0 0 0 1 0 0 1 1 1 1 0 1 1 1 0 0 0 0 0 0 0 1 1 0 1 1 0 1 0 0 0 0 0 0 1 1 1 0 1 1 0 0 1 1 0 0 1 0 0 1 1 1 1 1 
```

### morb

The executable *morb* generates six output types of files:
* *cullpointsN.dat*: list of all ORB features from imgN.orb excluded those that are too close at the borders of the image at the coarsest level.
Example for cullpoints1.dat (976 rows/features):

```
726 86 0 242.882 31 0.000489941
492 278 0 279.115 31 0.00140223
...
...
...
170 134 7 285.236 111.079 0.000294549
119 129 7 333.623 111.079 0.000338766
```
* *keypointsN.dat*: list of all ORB features after scaling across all levels of the image pyramid and removing duplicates. For example, keyponts1.dat contains 622 rows (features).
* *imgN.morb*: list of all features (from *keypointsN.dat*) for each scale of the image pyramid with their corresponding ORB descriptor.
Example for img1.morb (622 * 8 features):

```
7.2600000e+02 8.6000000e+01 0 2.4288200e+02 31.000000
1 1 1 0 0 0 1 0 1 1 1 1 1 0 1 0 1 0 1 1 0 1 1 1 1 0 1 0 0 0 1 1 0 1 0 1 0 1 0 1 0 1 0 0 1 0 0 0 1 1 1 1 0 1 0 1 1 1 1 1 1 1 0 1 0 1 1 1 0 1 0 1 0 1 0 1 1 1 1 1 0 1 1 1 1 0 0 0 0 0 1 0 0 1 1 1 1 0 1 1 1 0 1 1 0 1 1 0 0 1 1 0 1 1 1 1 1 0 1 1 1 1 1 0 0 1 1 0 1 1 1 0 1 1 0 0 1 0 1 0 0 0 1 0 0 1 1 1 1 1 1 0 1 1 1 1 0 1 1 1 1 0 0 0 0 0 1 1 1 0 0 0 0 0 1 1 1 1 1 1 0 1 0 1 0 0 0 1 0 0 1 0 0 0 1 1 0 1 1 0 0 1 0 1 0 1 0 0 1 1 1 1 0 1 1 1 0 0 1 0 0 0 0 1 0 0 1 1 0 1 0 0 0 1 0 0 1 1 1 1 1 0 0 0 0 1 0 1 1 0 1 0 1 1 1 0 
4.9200000e+02 2.7800000e+02 0 2.7911499e+02 31.000000
1 0 0 1 1 0 1 1 1 0 1 1 0 0 1 0 0 1 1 0 0 0 1 0 0 0 1 1 1 1 0 1 1 0 1 0 1 0 1 0 0 1 0 0 1 0 1 1 0 1 1 1 0 1 1 0 0 1 0 0 0 1 1 0 1 0 1 0 1 1 0 1 0 1 1 0 0 0 0 1 0 1 1 0 0 1 1 1 0 1 1 0 0 0 0 0 1 0 0 1 0 1 0 0 0 1 0 0 0 1 1 1 1 1 0 0 0 1 0 0 1 1 1 0 1 0 1 1 0 0 0 1 0 1 1 0 0 0 1 0 0 1 1 1 0 1 0 0 0 1 0 1 0 1 0 1 0 0 1 1 0 0 1 1 0 1 1 0 1 1 0 1 1 0 0 1 0 1 1 0 1 1 0 0 1 0 1 1 0 1 1 1 0 1 0 0 1 1 0 1 1 1 0 0 0 1 1 1 0 1 0 0 0 0 1 1 1 0 0 0 0 0 0 0 1 1 0 1 0 1 0 0 0 0 1 0 1 1 1 1 0 0 0 1 0 1 1 0 0 0 1 1 0 0 1 1 
9.0200000e+02 4.4500000e+02 0 6.1943298e+01 31.000000
0 0 0 1 1 1 0 0 1 0 1 1 0 0 0 1 0 1 1 1 0 1 1 0 0 0 1 1 0 1 1 1 0 1 1 1 1 0 1 1 1 1 0 0 0 0 0 1 1 1 1 1 0 0 1 0 1 1 1 0 1 1 1 0 0 0 1 0 0 0 1 1 0 0 1 0 0 0 0 0 0 1 1 1 1 1 1 1 0 1 0 0 0 0 0 0 1 1 0 0 1 1 1 0 1 1 0 0 0 1 1 1 0 0 0 0 1 1 0 1 0 0 0 0 0 1 1 0 0 0 0 1 1 1 1 0 0 1 0 0 1 1 0 1 0 1 1 1 1 0 0 0 0 0 0 1 0 1 1 1 0 0 0 1 1 1 1 1 1 1 0 1 0 1 0 1 0 0 1 0 0 1 0 0 1 0 1 1 1 1 0 1 1 1 0 1 1 1 1 0 1 0 1 1 0 1 0 1 0 1 1 0 0 1 0 1 0 0 0 1 0 0 0 1 1 1 1 0 1 1 0 0 0 1 0 1 0 1 0 1 0 0 1 0 1 1 1 1 0 0 0 1 0 1 0 0 
4.9800000e+02 2.7800000e+02 0 8.4801102e+00 31.000000
1 0 0 1 0 0 1 0 0 0 0 0 0 0 0 0 1 1 0 1 1 0 1 1 1 0 1 0 0 1 1 1 0 0 1 0 0 1 1 0 0 1 0 1 0 1 1 0 1 0 1 0 0 1 1 0 1 1 1 0 1 0 0 0 1 0 0 0 0 1 1 0 1 1 0 1 0 0 1 0 1 1 1 1 1 1 1 0 0 0 0 0 0 0 1 0 1 1 1 1 1 0 0 1 1 1 0 0 1 1 0 0 1 0 0 1 0 0 0 1 0 0 0 1 1 1 1 0 0 1 0 0 1 1 1 0 0 1 1 1 0 1 0 0 0 0 1 1 0 1 0 0 0 0 0 1 0 0 0 1 1 0 0 0 1 0 0 0 0 0 0 0 1 1 0 1 1 0 1 0 0 1 0 1 0 0 0 0 0 0 1 0 0 0 1 0 0 0 1 1 1 0 1 1 0 1 0 1 0 1 0 0 1 1 0 0 0 0 0 0 0 0 0 0 0 1 0 1 0 1 0 1 0 1 0 0 1 1 1 1 1 0 0 0 0 1 1 1 1 0 0 0 0 1 0 0 
4.1700000e+02 4.4600000e+02 0 2.8752200e+02 31.000000
0 1 0 0 0 1 0 0 0 0 0 0 1 1 0 1 0 1 0 1 1 1 1 0 1 0 0 0 0 0 1 0 1 1 0 0 0 1 1 0 1 1 1 1 1 0 1 1 0 0 0 0 1 1 1 1 1 1 1 0 0 1 1 0 1 0 0 1 0 1 1 0 1 0 1 1 0 1 0 0 0 0 1 1 1 1 1 0 0 0 0 1 1 1 0 0 0 1 0 1 0 0 0 0 0 0 1 1 1 1 0 1 0 1 1 0 0 0 0 1 0 0 1 0 1 1 0 1 0 1 1 1 0 1 0 1 1 1 1 1 1 0 0 1 0 1 1 0 1 0 1 1 0 0 0 0 1 0 0 0 0 1 1 0 1 0 0 0 0 1 0 1 1 1 0 1 1 0 0 0 0 1 1 1 1 1 1 0 0 0 1 1 1 1 1 1 1 0 0 0 1 0 1 1 1 0 1 1 0 0 0 0 1 0 0 0 1 1 0 0 0 1 1 1 1 1 0 0 0 1 1 1 0 0 1 0 0 0 0 1 0 1 1 1 0 1 1 0 1 1 1 1 1 1 1 1 
4.7300000e+02 4.4300000e+02 0 7.2886703e+01 31.000000
1 1 0 0 0 0 1 1 0 1 0 1 1 1 0 1 0 1 0 1 1 0 0 1 0 1 0 1 1 0 0 1 0 1 0 1 0 0 0 1 1 0 0 0 0 1 0 1 0 0 0 0 0 0 0 0 1 1 1 1 0 0 1 1 0 1 0 1 1 1 0 1 1 1 0 1 1 0 0 0 0 0 0 0 0 1 0 1 0 0 1 1 1 1 1 0 0 0 1 0 0 1 0 1 0 1 0 0 0 1 1 1 0 0 0 0 0 1 1 0 0 1 0 1 1 0 1 0 1 0 1 0 0 1 0 1 1 1 0 1 0 1 0 0 0 1 0 1 0 1 1 0 0 0 0 0 1 0 0 1 1 1 1 0 1 0 0 0 1 1 1 1 0 0 1 0 0 1 0 1 0 0 0 1 1 1 0 0 1 1 1 0 0 0 0 1 1 0 0 1 0 0 1 0 1 0 0 0 1 1 1 1 1 0 0 0 0 1 0 1 1 1 1 0 0 0 0 1 1 0 0 1 0 0 0 0 0 0 1 0 1 1 0 1 0 1 0 1 1 0 1 1 0 0 0 1 
6.9300000e+02 9.6000000e+01 0 2.5113800e+02 31.000000
0 1 0 1 0 1 0 0 0 0 0 0 0 1 0 1 0 1 0 0 0 1 1 1 1 0 0 0 1 0 0 1 1 1 1 1 0 0 1 0 1 0 0 1 1 1 1 1 0 0 0 0 1 1 1 1 1 1 0 0 0 0 1 0 1 1 1 1 1 0 1 1 1 0 1 0 1 1 1 0 1 0 1 0 1 1 1 1 1 1 0 1 1 0 0 0 0 0 0 1 0 0 0 0 0 0 1 1 0 0 0 1 0 1 1 0 1 1 1 0 0 0 1 1 0 0 0 1 1 0 0 1 0 1 1 1 1 1 0 1 0 0 0 1 1 1 0 0 0 0 0 1 1 0 0 0 1 0 0 1 0 1 1 0 1 0 0 0 1 1 1 1 1 1 1 0 0 1 1 0 1 1 0 0 1 0 0 0 1 0 1 1 1 1 1 1 1 0 0 0 1 0 0 1 1 0 1 0 0 1 0 1 1 0 0 1 1 1 1 0 0 1 1 0 1 1 0 0 1 0 1 1 0 0 1 1 0 0 0 0 0 1 1 1 0 0 1 1 0 1 1 0 0 0 1 1 
8.0700000e+02 4.4000000e+02 0 4.3021702e+01 31.000000
1 0 0 0 1 0 0 0 1 1 1 1 1 0 0 0 1 1 0 1 0 1 1 0 1 0 1 0 0 1 1 0 0 1 1 0 1 1 0 1 0 1 0 0 0 0 0 0 1 1 1 1 0 1 1 0 1 1 1 0 1 1 0 0 0 0 1 0 1 1 1 0 0 0 0 1 0 0 1 1 0 1 1 1 1 0 1 0 0 1 1 0 0 1 1 1 1 0 0 0 1 0 1 0 1 1 1 0 0 1 0 0 0 0 0 0 1 0 0 1 0 0 0 0 0 1 1 0 0 1 1 0 1 1 0 0 0 1 1 1 0 1 1 1 1 0 1 0 1 0 0 0 1 0 1 1 0 1 0 1 1 0 0 0 0 1 1 1 0 0 0 1 1 0 0 1 1 0 1 0 0 1 0 0 0 1 1 1 0 0 1 0 0 0 1 0 0 1 1 1 1 1 1 1 0 1 0 0 0 1 1 1 0 1 0 1 0 0 0 0 0 0 0 1 1 0 1 1 0 1 0 0 0 0 0 0 0 1 1 1 1 0 0 0 1 1 0 1 1 0 0 1 1 1 1 0
...
...
...
3.2000053e+01 1.4999992e+02 7 4.3157001e+01 111.079002
1 0 0 0 0 1 1 1 1 1 0 0 0 1 1 0 0 1 0 1 1 1 1 0 1 1 1 0 0 0 1 0 0 0 1 0 0 1 0 1 0 1 1 1 1 0 0 0 1 1 1 0 0 0 1 0 1 0 1 0 1 0 0 1 1 0 0 0 0 1 1 0 0 0 1 1 0 0 0 0 0 1 1 0 1 1 1 1 0 0 1 1 0 0 0 0 1 1 0 0 1 1 1 1 1 0 0 0 0 0 1 0 0 1 0 1 1 0 0 1 0 0 1 0 1 0 0 0 0 0 0 1 1 0 0 0 0 0 1 1 0 0 1 1 0 0 1 1 0 0 1 0 0 1 1 1 0 1 1 0 0 0 0 1 0 0 1 1 0 0 1 0 1 1 0 1 1 0 1 0 0 1 1 1 0 0 0 1 0 0 0 0 0 1 1 0 0 1 1 1 1 0 0 1 1 1 1 1 0 0 1 0 0 1 0 0 0 0 0 0 1 0 0 0 0 1 0 0 0 1 1 0 0 1 0 1 1 0 0 1 0 1 0 0 0 0 1 1 0 0 0 1 0 1 0 0 
3.3999950e+01 1.4999992e+02 7 5.7576099e+01 111.079002
1 0 1 1 0 1 1 1 0 1 0 1 0 1 1 0 0 1 1 1 0 1 1 0 0 1 0 0 1 1 1 0 0 0 1 1 0 1 1 0 1 0 1 1 1 1 0 1 0 1 0 0 1 0 1 0 0 0 0 0 1 0 0 1 1 0 0 0 0 1 1 1 0 0 1 1 0 1 0 1 0 1 1 0 1 1 0 1 1 0 1 1 0 0 0 0 1 1 0 0 1 0 1 1 1 0 0 1 0 0 0 0 1 1 1 0 1 0 1 0 1 1 0 1 1 0 0 0 0 0 0 1 0 0 0 1 0 0 1 0 0 1 1 1 0 0 1 1 0 1 1 0 0 1 1 1 1 0 1 0 0 0 1 1 0 1 0 1 0 1 1 0 1 1 1 1 0 0 0 0 0 0 1 1 0 0 0 1 0 1 0 0 0 1 1 0 0 0 1 0 1 0 0 1 1 0 1 1 1 0 0 0 0 1 1 0 1 0 0 0 1 1 0 0 0 0 0 1 0 0 1 1 1 1 0 1 1 1 0 1 0 1 0 1 1 0 1 0 0 1 0 0 0 0 0 0 
2.1099992e+02 3.3000004e+01 7 1.7797200e+02 111.079002
0 0 0 1 0 1 1 0 1 1 0 1 1 0 1 0 0 0 1 1 1 0 1 1 0 1 1 1 1 0 1 1 0 1 1 1 1 1 1 0 1 1 0 1 1 0 0 1 1 1 0 0 1 0 0 1 0 0 0 0 1 1 0 0 0 1 0 1 1 0 0 1 0 0 1 1 0 1 0 1 1 0 1 0 1 1 0 1 0 0 1 1 0 1 1 1 0 1 1 0 0 1 1 1 1 0 0 0 0 1 1 1 1 1 0 0 0 1 0 1 0 0 1 0 0 0 0 0 0 1 1 1 1 0 1 0 0 0 0 0 1 1 1 0 0 1 1 0 0 1 0 1 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 1 1 0 1 0 1 1 0 0 1 0 1 0 1 1 0 1 0 1 1 1 1 0 1 0 1 0 1 0 0 1 1 1 1 1 0 1 0 1 1 1 0 0 0 1 1 1 0 1 1 0 1 1 1 0 1 0 1 1 0 1 0 1 0 1 1 0 1 0 1 0 1 1 0 1 1 1 0 0 0 1 1 0 1 0 1 0 1 
8.9999901e+01 1.4899998e+02 7 2.3372099e+02 111.079002
0 1 0 0 0 0 1 1 0 0 0 0 0 1 0 1 0 0 1 1 0 1 0 1 1 0 0 0 1 0 1 1 0 0 1 1 0 0 0 0 1 0 0 1 0 1 1 1 1 0 0 0 0 1 0 1 1 1 0 0 0 0 0 1 0 1 1 1 0 1 1 1 1 1 0 1 1 1 1 0 0 1 1 0 0 0 0 1 0 1 0 0 1 0 0 0 0 0 0 0 1 0 0 1 1 0 0 1 1 0 0 1 0 0 1 0 1 0 1 0 1 1 0 1 0 1 1 0 1 0 0 1 0 1 1 0 0 1 1 0 0 0 0 0 1 1 0 1 1 1 1 0 1 0 0 1 1 0 0 1 0 1 0 0 1 0 0 0 1 1 1 1 0 0 1 0 1 1 0 0 1 0 1 1 1 0 0 0 1 0 1 0 1 0 1 1 1 0 1 0 0 0 0 1 0 1 0 0 1 1 0 1 1 1 1 1 1 0 1 1 1 1 1 1 0 0 1 0 0 0 0 1 0 0 0 0 0 0 0 0 1 0 1 0 1 0 1 0 0 1 0 0 0 1 0 0 
2.2500005e+02 1.2000005e+02 7 2.4617300e+01 111.079002
1 1 0 0 0 1 0 0 1 0 0 1 0 0 1 0 1 1 0 0 0 1 1 0 0 1 0 1 0 0 1 0 1 0 1 1 1 0 1 1 1 1 1 0 0 0 1 1 1 1 1 1 0 0 1 1 1 0 1 1 0 1 1 1 1 0 1 0 0 0 1 0 0 0 0 0 0 0 1 1 0 0 0 1 0 1 1 1 0 1 0 0 0 0 1 0 0 0 0 0 1 1 0 0 0 0 1 1 1 0 1 0 1 0 1 0 1 1 0 0 0 0 1 0 0 1 1 1 0 0 0 1 1 0 1 1 0 0 0 1 0 1 0 0 0 1 0 0 1 0 0 0 1 1 1 1 1 1 1 1 1 1 0 1 0 0 1 1 0 1 0 1 0 1 0 1 0 1 1 1 1 1 1 0 0 1 1 1 1 1 1 1 1 0 1 0 0 1 0 0 0 0 1 1 1 0 0 0 1 1 1 0 0 0 0 1 0 0 0 1 0 0 0 0 1 0 1 1 1 0 1 0 0 1 1 1 1 0 0 0 0 0 0 0 1 1 1 0 0 0 1 1 0 1 1 1 
1.2699998e+02 5.5999954e+01 7 2.2391199e+01 111.079002
0 1 0 0 0 0 0 0 0 0 0 0 1 0 1 1 0 1 1 1 1 0 0 1 1 1 0 1 1 0 0 1 0 1 1 0 0 0 1 0 0 0 1 0 0 0 1 1 0 0 0 1 1 1 0 1 0 0 0 0 0 0 1 0 1 1 1 1 1 0 0 0 1 0 1 0 1 0 0 0 1 1 0 0 0 0 1 0 1 1 0 0 0 0 0 1 0 0 1 0 0 0 0 1 0 1 1 1 1 0 0 1 0 1 1 1 0 1 0 0 1 0 1 1 0 0 1 0 0 1 1 0 0 1 1 1 1 0 0 1 1 1 0 0 1 1 0 0 1 1 0 0 1 0 0 0 0 0 0 1 0 1 1 0 1 0 0 0 0 0 0 1 1 0 1 1 0 1 0 0 1 0 0 0 1 0 0 0 1 1 1 1 1 0 1 0 0 0 0 0 0 1 0 0 1 1 0 0 0 1 0 1 1 0 1 1 1 1 0 0 0 0 1 0 0 0 1 0 1 0 0 1 1 0 0 1 0 0 0 0 1 0 1 0 0 1 0 0 0 1 1 0 1 0 1 1 
1.7000003e+02 1.3399991e+02 7 2.8523599e+02 111.079002
0 0 0 1 1 1 1 0 1 0 0 0 0 1 0 0 0 0 0 1 0 1 1 1 1 0 1 0 0 1 0 0 0 0 0 1 0 1 0 0 0 1 1 0 0 0 1 0 1 1 0 1 1 0 1 0 0 0 0 0 1 0 0 0 1 0 0 0 0 1 0 0 0 0 1 1 0 1 1 1 0 1 1 1 0 0 1 1 1 0 1 1 0 0 0 1 1 1 0 0 1 1 1 1 1 1 1 0 1 1 0 0 1 1 0 1 0 0 1 1 0 1 0 1 1 0 1 0 0 0 1 0 1 0 1 0 0 0 1 0 0 1 1 1 1 0 1 1 1 1 1 0 0 0 1 1 1 1 1 1 0 0 0 1 0 0 0 0 0 1 0 0 0 0 1 1 1 0 1 0 0 1 1 1 0 0 0 1 0 0 0 0 1 1 0 1 1 0 1 1 1 0 1 0 0 1 1 1 0 0 1 0 0 1 1 1 1 0 0 0 1 0 0 1 0 1 0 0 0 1 0 1 1 1 0 0 0 1 0 1 0 1 1 0 1 0 0 0 1 1 0 0 1 0 1 0 
1.1900011e+02 1.2899988e+02 7 3.3362299e+02 111.079002
0 1 0 0 1 0 0 1 1 1 0 1 1 1 1 0 1 1 0 0 0 0 0 1 1 0 1 0 1 1 0 1 0 1 1 0 0 1 0 0 0 1 0 1 0 0 1 0 1 0 1 1 0 1 0 1 0 1 1 0 1 1 1 1 0 0 0 0 1 1 0 0 0 1 0 0 0 1 1 1 0 1 0 0 1 0 1 0 1 1 1 0 0 1 1 1 1 0 1 0 1 1 0 1 1 0 1 0 0 1 0 1 1 0 0 0 1 0 0 1 1 1 0 0 0 0 0 0 0 1 1 1 0 1 1 0 1 0 1 1 0 1 1 0 1 0 1 0 1 1 0 0 1 0 1 1 0 0 1 1 1 0 0 1 0 1 1 1 0 0 1 1 1 0 1 1 0 0 1 0 0 0 0 1 0 0 0 1 0 1 0 0 0 0 1 0 0 1 0 1 0 1 0 0 0 1 0 0 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 1 1 0 1 1 0 1 0 0 0 0 0 0 1 1 1 0 1 1 0 0 1 1 0 0 1 0 0 1 1 1 1 1 

```

* *matches_NvsM.txt*: list of all matched MORB features between two images ordered 
by their Hamming distance (from smaller to higher).
For each row (match), the file provides:
  - Index of the MORB feature in the reference image;
  - Index of the MORB feature in the target image;
  - Hamming distance between the two features;
  - Matched scale of the MORB feature in the reference image;
  - Matched scale of the MORB feature in the target image.
Example for matches_1vs7.txt:

```
233 33 8 0 7 
...
...
...
113 109 72 3 3
```

* *scaledHamming.txt*: a NxM matrix with the cross-scale Hamming distances for each pair of MORB features, where N and M are here the number of MORB features for each image, respectively.  
* *scalesN.txt*: a NxM matrix with the matched scale in the reference (target) image for each cross-scale Hamming distance in *scaledHamming.txt*. 

## Enquiries, Question and Comments

If you have any further enquiries, question, or comments, please contact <email>a.xompero@qmul.ac.uk</email>. If you would like to file a bug report or 
a feature request, use the Github issue tracker. 

## Citation

If you use MORB in your research, please use the following BibTeX entry.

```
@INPROCEEDINGS{Xompero2018ICIP_MORB,
  title = {{MORB: A multi-scale binary descriptor}},
  author = {Xompero, Alessio and Lanz, Oswald and Cavallaro, Andrea},
  booktitle = {Proceedings of the IEEE International Conference on Image Processing},
  address = {Athens, Greece},
  month = "7--10~" # oct,
  year = {2018}
}
```

## Credits

This software was built upon the implementation of ORB available as opensource 
in [OpenCV](https://github.com/opencv/opencv/blob/master/modules/features2d/src/orb.cpp) 
and the modified implementation of ORB available as opensource within
[ORB-SLAM 2](https://github.com/raulmur/ORB_SLAM2/blob/master/src/ORBextractor.cc).

## Licence

This work is licensed under the MIT License.  

MIT License

Copyright (c) 2021 Alessio

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
