#include "processedim.h"

ProcessedIm::ProcessedIm(int R_,int C_):
    R(R_),
    C(C_)
{
    reset();
}

void ProcessedIm::reset(){
    InputMat = blankIm();
    NoiseMat = blankIm();
    EllipseMat = blankIm();
    XieRecognizedEllipses = {};
    CVRecognizedEllipses = {};
}

cv::Mat ProcessedIm::blankIm(){
    cv::Mat im = cv::Mat(cv::Size(R, C), CV_8UC1, cv::Scalar(0));
    return im;
}

void ProcessedIm::addEllipse(Ellipse el1){
    el1.draw(EllipseMat);
    el1.draw(InputMat);
}

void ProcessedIm::showInputMat(){
    cv::imshow("InputMat",InputMat);
}

void ProcessedIm::CV_Algorithm(){
    std::vector<cv::Point> pixelList = allSignalPixels();
    cv::RotatedRect box = cv::fitEllipse(pixelList);
    std::cout << box.center << " " << int(box.size.height/2) << " " << int(box.size.width/2) << " "<< double(box.angle) << std::endl;
    Ellipse res(cv::Point(int(box.center.x),int(box.center.y)),int(box.size.height/2),int(box.size.width/2),double(box.angle));
    res.draw(Result);
}

std::vector<cv::Point> ProcessedIm::allSignalPixels(){
    std::vector<cv::Point> pixelList = {};
    for(int x=0;x<C;++x){
        for (int y=0;y<R;++y) {
            cv::Point current(x,y);
            if(InputMat.at<uchar>(current)!=0) pixelList.push_back(current);
        }
    }
    return pixelList;
}
