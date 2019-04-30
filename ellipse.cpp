#include "ellipse.h"

Ellipse::Ellipse(cv::Point M_,int a_,int b_,double angle_):
    M(cv::Point(M_.x,M_.y)),
    a(a_),
    b(b_),
    angle(initialize_alpha(angle_))
{

}

double Ellipse::initialize_alpha(double angle) const{
    while(angle > 90){
        angle -= 180;
    }
    while(angle < 90){
        angle += 180;
    }
    return angle;
}

void Ellipse::draw(cv::Mat mat,int thickness){
    double alpha = -angle;
    cv::ellipse(mat,M,cv::Size(a,b),alpha ,0,360,255,thickness);
}


void Ellipse::remove(cv::Mat mat,int thickness){
    double alpha = -angle;
    cv::ellipse(mat,M,cv::Size(a,b),alpha ,0,360,0,thickness);
}

