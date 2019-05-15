#ifndef PROCESSEDIM_H
#define PROCESSEDIM_H
#include "opencv2/opencv.hpp"
#include "ellipse.h"
#include "compareFunctions.h"
#include <vector>

#include <time.h>
#include <stdio.h>
#include <algorithm>

class ProcessedIm
{
    friend class MainWindow;
public:
    ProcessedIm(int R_,int C_);     //constructor
    void test();
protected:
    void Xie_Algorithm(double reqLeastDistance1, double reqLeastDistance2, double reqLeastVotesb);
    void Xie_Algorithm2(double reqLeastDistance1, double reqLeastDistance2, double reqLeastVotesb);
    //Our own implementation of the algorithm, outputs Ellipses into 'XieRecognizedEllipses'
    void CV_Algorithm(size_t thresh=100);            //Implementation of cv function 'fitEllipse', outputs Ellipses into 'CVRecognizedEllipses'

    void reset();                   //Resets all Mat-obj to zero's, all counters to 0 and all containers to empty
    void addEllipse(Ellipse el1);   //Draws Ellipse on InputMat
    //void addLine(Line l1);          //Draws Line on InputMat
    void setInputMat(cv::Mat newIm);//Sets InputMat to 256x256 grayscale-reduction of 'newIm'
    void addNoise(int Noise);       //Adds Noise random white pixels on InputMat

    std::vector<Ellipse> XieRecognizedEllipses;     //container for the Ellipses found with Xie-algorithm
    std::vector<Ellipse> CVRecognizedEllipses;      //container for the Ellipses found with CV-algorithm
    std::vector<Ellipse> InputEllipses;             //container for the Ellipses from the input

    cv::Mat Result;                 //Temporary Mat to draw results on

    void showInputMat();            //Temporary function to show input
    void showResultMat();           //Temporary function to show results

    int N,M;                        //Keep track of SignalPoints and WhitePoints


private:
    const int R,C;                  //#rows,#columns respectively of mat-objects

    cv::Mat inputMat;               //Container for input

    cv::Mat blankIm();              //Function that makes a black R*C-Mat

    bool inputMade = false;         //Bool to see if there is already an up to date inputMat

    std::vector<cv::Point>  getBestContour(cv::Mat,size_t thresh);    //Function returns the largest Contour (vector of points) greater that thresh

    std::vector<cv::Point> allSignalPixels();   //Function to list all white pixels... no longer needed I think



   };

#endif // PROCESSEDIM_H
