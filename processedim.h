#ifndef PROCESSEDIM_H
#define PROCESSEDIM_H
#include "opencv2/opencv.hpp"
#include "ellipse.h"
#include <vector>

class ProcessedIm
{
    friend class MainWindow;
public:
    ProcessedIm(int R_,int C_);
    void Xie_Algorithm();
    void CV_Algorithm();

    void reset();
    void addEllipse(Ellipse el1);
    //void addLine(Line l1);
    void setInputMat(cv::Mat newIm);
    void addNoise(int N);

    std::vector<Ellipse> XieRecognizedEllipses;
    std::vector<Ellipse> CVRecognizedEllipses;

    cv::Mat Result;

protected:
    void showInputMat();
    void showResultMat();
private:
    const int R,C;
    cv::Mat InputMat;
    cv::Mat NoiseMat;
    cv::Mat EllipseMat;

    cv::Mat blankIm();

    std::vector<cv::Point> allSignalPixels();

};

#endif // PROCESSEDIM_H
