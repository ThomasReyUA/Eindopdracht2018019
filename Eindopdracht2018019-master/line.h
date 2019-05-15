#ifndef LINE_H
#define LINE_H

#include "shape.h"

class Line : public Shape{
    friend class MainWindow;
    friend class ProcessedIm;
public:
    Line(cv::Point M,double angle);  //Constructor

    void draw(cv::Mat,int thickness=1);             //Draws this ellipse on a Mat
    void remove(cv::Mat,int thickness=1);           //Removes this ellipse from a Mat
protected:
    cv::Point M;                            //Center point of ellipse
    double angle;                           //Angle between a and x-axis (degrees)

private:
    double initialize_alpha(double) const;  //Check input angle for stupid values
};
#endif // LINE_H
