#include "compareFunctions.h"


bool comparePoints(cv::Point a,cv::Point b)  //return true if point a < point b
{
    if(a.y<b.y)
    {
        return true;
    }
    else if(a.y==b.y)
    {
        if(a.x<b.x)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool compareContours(std::vector<cv::Point> a,std::vector<cv::Point>b) //best contour is contour with the most points
{
    if(a.size()<b.size())
    {
        return true;
    }
    else
    {
        return false;
    }
}
