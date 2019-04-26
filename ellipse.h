#ifndef ELLIPSE_H
#define ELLIPSE_H
#include "shape.h"

class Ellipse : public Shape{
public:
    Ellipse(cv::Point M,int a,int b,double angle);

    void draw(cv::Mat);

protected:
    cv::Point M;
    int a,b;
    double angle;

private:
    double initialize_alpha(double) const;

};

#endif // ELLIPSE_H
