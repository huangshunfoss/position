#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Point2f getCrossPoint(Vec4i LineA, Vec4i LineB);

int getBackCorner(int num);