#ifndef COMPAREFUNCTIONS_H
#define COMPAREFUNCTIONS_H
#include "opencv2/opencv.hpp"

bool comparePoints(cv::Point,cv::Point);
bool compareContours(std::vector<cv::Point>,std::vector<cv::Point>);
#endif // COMPAREFUNCTIONS_H
