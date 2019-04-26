#include "ellipse.h"

Ellipse::Ellipse(cv::Point M_,int a_,int b_,double angle_):
    M(cv::Point(M_.x,M_.y)),
    a(a_),
    b(b_),
    angle(initialize_alpha(angle_))
{

}

double Ellipse::initialize_alpha(double angle) const{
    while(angle > M_PI/2){
        angle -= M_PI;
    }
    while(angle < -M_PI/2){
        angle += M_PI;
    }
    return angle;
}

void Ellipse::draw(cv::Mat mat){
    double alpha = -angle*180/M_PI;
    cv::ellipse(mat,M,cv::Size(a,b),alpha ,0,360,255);
}
