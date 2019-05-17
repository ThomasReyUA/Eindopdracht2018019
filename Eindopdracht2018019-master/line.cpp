#include "line.h"

Line::Line(cv::Point M_,double angle_):
    M(cv::Point(M_.x,M_.y)),
    angle(initialize_alpha(angle_))
{

}

double Line::initialize_alpha(double angle_) const{
    double a=fmod(angle_,180);
    if(a>90){return (a-180);}
    else if(a<=-90){return (a+180);}
    else {return a;}
}

void Line::draw(cv::Mat mat,int thickness){

    if(abs(angle)<89.9)
    {
        double m=tan(angle*3.1415926535/180);
        double b=-M.y-m*M.x;
        int x2=0;
        int x3=mat.cols+1;
        int y2=-(int)(b);
        int y3=-(int)(m*x3+b);
        cv::line(mat,cv::Point(x2,y2),cv::Point(x3,y3),255,thickness,8,0);
    }
    else
    {
        int y2=0;
        int y3=mat.rows+1;
        int x2=M.x;
        int x3=M.x;
        cv::line(mat,cv::Point(x2,y2),cv::Point(x3,y3),255,thickness,8,0);
    }
}


void Line::remove(cv::Mat mat,int thickness){
    double alpha = angle;
    if(abs(angle)<89.9)
    {
        double m=tan(angle);
        double b=M.y-m*M.x;
        int x2=0;
        int x3=mat.cols+1;
        int y2=(int)(b);
        int y3=(int)(m*x3+b);
        cv::line(mat,cv::Point(x2,y2),cv::Point(x3,y3),0,thickness,8,0);
    }
    else
    {
        int y2=0;
        int y3=mat.rows+1;
        int x2=M.x;
        int x3=M.x;
        cv::line(mat,cv::Point(x2,y2),cv::Point(x3,y3),0,thickness,8,0);
    }
}

