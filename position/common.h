#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

/// common and shared functions are placed here
Point2f getCrossPoint(Vec4i LineA, Vec4i LineB);