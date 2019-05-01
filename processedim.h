#ifndef PROCESSEDIM_H
#define PROCESSEDIM_H
#include "opencv2/opencv.hpp"
#include "ellipse.h"
#include <vector>
#include <time.h>

class ProcessedIm
{
    friend class MainWindow;
public:
    ProcessedIm(int R_,int C_);     //constructor


protected:
    void Xie_Algorithm();           //Our own implementation of the algorithm, outputs Ellipses into 'XieRecognizedEllipses'
    void CV_Algorithm(size_t thresh=100);            //Implementation of cv function 'fitEllipse', outputs Ellipses into 'CVRecognizedEllipses'

    void reset();                   //Resets all Mat-obj to zero's, all counters to 0 and all containers to empty
    void addEllipse(Ellipse el1);   //Draws Ellipse on InputMat
    //void addLine(Line l1);          //Draws Line on InputMat
    void setInputMat(cv::Mat newIm);//Sets InputMat to 256x256 grayscale-reduction of 'newIm'
    void addNoise(int Noise);       //Adds Noise random white pixels on InputMat

    std::vector<Ellipse> XieRecognizedEllipses;     //container for the Ellipses found with Xie-algorithm
    std::vector<Ellipse> CVRecognizedEllipses;      //container for the Ellipses found with CV-algorithm

    cv::Mat Result;                 //Temporary Mat to draw results on

    void showInputMat();            //Temporary function to show input
    void showResultMat();           //Temporary function to show results

    int N,M;                        //Keep track of SignalPoints and WhitePoints


private:
    const int R,C;                  //#rows,#columns respectively of mat-objects

    cv::Mat inputMat;               //Container for input

    cv::Mat blankIm();              //Function that makes a black R*C-Mat

    std::vector<cv::Point>  getBestContour(cv::Mat,size_t thresh);    //Function returns the largest Contour (vector of points) greater that thresh

    std::vector<cv::Point> allSignalPixels();   //Function to list all white pixels... no longer needed I think

};

#endif // PROCESSEDIM_H
