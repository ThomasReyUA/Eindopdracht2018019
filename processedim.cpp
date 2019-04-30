#include "processedim.h"

ProcessedIm::ProcessedIm(int R_,int C_):
    R(R_),
    C(C_)
{
    thresh = 100;
    std::srand(time(nullptr));
    reset();
}

void ProcessedIm::reset(){
    inputMat = blankIm();
    Result = blankIm();
    XieRecognizedEllipses = {};
    CVRecognizedEllipses = {};
    N=0;
    M=0;
}

cv::Mat ProcessedIm::blankIm(){
    cv::Mat im = cv::Mat(cv::Size(R, C), CV_8UC1, cv::Scalar(0));
    return im;
}

void ProcessedIm::addEllipse(Ellipse el1){
    int Tot1 = cv::countNonZero(inputMat);
    //    el1.draw(EllipseMat);
    el1.draw(inputMat);
    int Tot2 = cv::countNonZero(inputMat);
    N+= Tot2 - Tot1;
}

void ProcessedIm::showInputMat(){
    cv::imshow("inputMat",inputMat);
}

void ProcessedIm::showResultMat(){
    cv::imshow("Result",Result);
}

void ProcessedIm::CV_Algorithm(){

    cv::Mat im = inputMat.clone();
    std::vector<cv::Point> currentContour = getBestContour(im);
    while(currentContour.size() > 0) {
        cv::RotatedRect box = cv::fitEllipse(currentContour);
        Ellipse res(cv::Point(int(box.center.x),int(box.center.y)),int(box.size.height/2),int(box.size.width/2),90-double(box.angle));
        CVRecognizedEllipses.push_back(res);
        res.remove(im,4);
        currentContour = getBestContour(im);
        //std::cout << CVRecognizedEllipses.size() << std::endl;
    }
    for(size_t i=0;i<CVRecognizedEllipses.size();++i){
        CVRecognizedEllipses.at(i).draw(Result);
    }

    //    cv::RotatedRect box = cv::fitEllipse(pixelList);
    //    std::cout << "Center " << box.center << std::endl;
    //    std::cout << " a b " << int(box.size.height/2) << " " << int(box.size.width/2) << std::endl;
    //    std::cout << " angle "<< 90-box.angle<< std::endl;
    //    Ellipse res(cv::Point(int(box.center.x),int(box.center.y)),int(box.size.height/2),int(box.size.width/2),90-double(box.angle));
    //    CVRecognizedEllipses.push_back(res);
    //    res.draw(Result);
    //    cv::imshow("Result",Result);
}


std::vector<cv::Point>  ProcessedIm::getBestContour(cv::Mat im ){
    cv::Mat canny_output;
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;

    cv::Canny(im, canny_output, 100, 200, 3 );

    findContours( canny_output, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );

    std::vector<cv::Point> bestContour = contours.at(0);
    size_t currentSize = bestContour.size();
    for(size_t i = 0;i<contours.size();++i){
        if(contours.at(i).size()>currentSize){
            bestContour = contours.at(i);
            currentSize = bestContour.size();
        }
    }
    if(currentSize > thresh) return bestContour;
    else return {};
}

std::vector<cv::Point> ProcessedIm::allSignalPixels(){
    std::vector<cv::Point> pixelList = {};
    for(int x=0;x<C;++x){
        for (int y=0;y<R;++y) {
            cv::Point current(x,y);
            if(inputMat.at<uchar>(current)!=0) pixelList.push_back(current);
        }
    }
    return pixelList;
}

void ProcessedIm::addNoise(int Noise){
    //Adds N random white pixels to image
    if(Noise < R*C*0.98){ //prevents looking for zero-pixels when there are too few
        int x=0;
        int y=0;
        for (int i=0; i<Noise; ++i) {
            bool isZero=false;
            while (!isZero) {
                x = rand()%R;
                y = rand()%C;
                isZero=(inputMat.at<uchar>(cv::Point(x,y))!=255);
            }
            inputMat.at<uchar>(cv::Point(x,y))=255;
        }
        M+=Noise;
    }

}
