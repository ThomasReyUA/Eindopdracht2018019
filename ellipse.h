#ifndef ELLIPSE_H
#define ELLIPSE_H
#include "shape.h"

class Ellipse : public Shape{
public:
    Ellipse(cv::Point M,int a,int b,double angle);  //Constructor

    void draw(cv::Mat,int thickness=1);             //Draws this ellipse on a Mat
    void remove(cv::Mat,int thickness=1);           //Removes this ellipse from a Mat
protected:
    cv::Point M;                            //Center point of ellipse
    int a,b;                                //Lengths of a and b
    double angle;                           //Angle between a and x-axis (degrees)

private:
    double initialize_alpha(double) const;  //Check input angle for stupid values
};

#endif // ELLIPSE_H
