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
