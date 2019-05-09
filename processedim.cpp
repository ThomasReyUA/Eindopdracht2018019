#include "processedim.h"
#include "compareFunctions.h"

void ProcessedIm::test()
{
    cv::Point a(100,200);
    cv::Point b(101,200);
    cv::Point c(99,200);
    cv::Point d(100,201);
    cv::Point e(100,199);
    std::vector<cv::Point> test;
    test.push_back(a);
    test.push_back(b);
    test.push_back(c);
    test.push_back(d);
    test.push_back(e);
    std::cout<<"Not sorted: "<<std::endl<<std::endl;
    for(std::vector<cv::Point>::iterator it=test.begin();it!=test.end();++it)
    {
        std::cout<<"x= "<<it->x<<",  y= "<<it->y<<std::endl;
    }
    std::cout<<std::endl<<"Sorted: "<<std::endl<<std::endl;
    std::sort(test.begin(),test.end(),comparePoints);
    for(std::vector<cv::Point>::iterator it=test.begin();it!=test.end();++it)
    {
        std::cout<<"x= "<<it->x<<",  y= "<<it->y<<std::endl;
    }
}


ProcessedIm::ProcessedIm(int R_,int C_):
    R(R_),
    C(C_)
{
    std::srand(time(NULL));
    reset();
}


void ProcessedIm::reset(){
    inputMat = blankIm();
    Result = blankIm();
    XieRecognizedEllipses.clear();
    CVRecognizedEllipses.clear();
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

void ProcessedIm::CV_Algorithm(size_t thresh){

    cv::Mat im = inputMat.clone();
    std::vector<cv::Point> currentContour = getBestContour(im,thresh);
    while(currentContour.size() > 0) {
        cv::RotatedRect box = cv::fitEllipse(currentContour);
        Ellipse res(cv::Point(int(box.center.x),int(box.center.y)),int(box.size.height/2),int(box.size.width/2),90-double(box.angle));
        CVRecognizedEllipses.push_back(res);
        res.remove(im,4);
        currentContour = getBestContour(im,thresh);
        //std::cout << CVRecognizedEllipses.size() << std::endl;
    }
    for(std::vector<Ellipse>::iterator it=CVRecognizedEllipses.begin(); it!=CVRecognizedEllipses.end();it++){
        (*it).draw(Result);
    }

    //Temporarily Printing results
    int x,y,a,b;
    double angle;
    printf("# of ellipses: %lu\n",CVRecognizedEllipses.size());
    for(std::vector<Ellipse>::iterator it=CVRecognizedEllipses.begin(); it!=CVRecognizedEllipses.end();it++){
        x=(*it).M.x;
        y=(*it).M.y;
        a=(*it).a;
        b=(*it).b;
        angle=(*it).angle;
        printf("M = [%3d %3d], a = %3d, b = %3d, angle = %3.0f°\n", x,y,a,b,angle);
    }

}
void ProcessedIm::Xie_Algorithm2(double reqLeastDistance1, double reqLeastDistance2, double reqLeastVotesb) //input=binary mat file with the contourpoints, all points are stored in a vector of Points
{
    reqLeastDistance1=reqLeastDistance1*2;
    cv::Mat im =inputMat.clone();

    std::vector<cv::Point> contourPoints;
    /*
    cv::Mat canny_output;
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;

    cv::Canny(im, canny_output, 100, 200, 3 );

    findContours( canny_output, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );
    findNonZero(canny_output,contourPoints);
    */
    findNonZero(im,contourPoints);
    std::vector<int> vectorVotesb(148);
    cv::Point2i center(0,0);
    double a=0;
    double alpha=0;
    double a2=0;
    double d2=0;
    double f2=0;
    double b2=0;
    double cost=0;
    double d=0;
    double b=0;
    double numerator=0;
    double denominator=0;
    std::vector<cv::Point> testPoints=contourPoints;
    startfunction:
    for(std::vector<cv::Point>::iterator it1=contourPoints.begin();it1!=contourPoints.end();++it1)
    {
        for(std::vector<cv::Point>::iterator it2=it1;it2!=contourPoints.end();++it2)
        {
            if(cv::norm(*it1-*it2)>reqLeastDistance1)
            {
                    test:
                   center=(*it1+*it2)*0.5;
                   a=cv::norm(*it1-*it2)*0.5;
                   alpha= atan2(it2->y-it1->y,it2->x-it1->x);
                   for(std::vector<cv::Point>::iterator it3=testPoints.begin();it3!=testPoints.end();++it3)
                   //for(std::vector<cv::Point>::iterator it3=contourPoints.begin();it3!=contourPoints.end();++it3)
                   {
                       if(it3!=it2 && it3!=it1 && cv::norm(*it3-center)>reqLeastDistance2)
                       {
                           d=cv::norm(*it3-center);
                           a2=pow(a,2);
                           d2=pow(d,2);
                           f2=pow(std::min(cv::norm(*it3-*it1),cv::norm(*it3-*it2)),2);
                           cost=(a2+d2-f2)*pow(2*a*d,-1);
                           numerator=a2*d2*(1-pow(cost,2));
                           denominator=a2-(d2*pow(cost,2));

                           b2=numerator*pow(denominator,-1);
                           if(b2>0&&b2<16384)
                           {
                                 b=pow(b2,0.5);

                                 vectorVotesb.at((int)round(b))++;
                           }



                       }
                   }

                   std::vector<int>::iterator itMaxEl=std::max_element(vectorVotesb.begin(),vectorVotesb.end());
                   if ((*itMaxEl)>reqLeastVotesb)
                   {
                        std::cout<<*itMaxEl<<", "<<reqLeastVotesb<<std::endl;
                        //std::cout<<" test3" <<std::endl;
                        b=(itMaxEl-vectorVotesb.begin());
                        Ellipse res(center,a,b,-alpha*180/CV_PI);
                        XieRecognizedEllipses.push_back(res);
                        res.remove(im,6);
                        if(cv::countNonZero(im)<1)
                        {
                            std::cout<<"test"<<std::endl;
                           goto endfunction;

                        }
                        else
                        {
                            findNonZero(im,testPoints);
                            //findNonZero(im,contourPoints);
                            std::cout<<contourPoints.size()<<std::endl;
                            vectorVotesb.assign(148,0);
                            goto test;
                            //goto startfunction;
                        }

                    }
                   vectorVotesb.assign(148,0);
                   //std::cout<<" test"<<std::endl;
            }
        }
    }
    endfunction:
    //temp drawing found ellipses
    for(std::vector<Ellipse>::iterator it=XieRecognizedEllipses.begin(); it!=XieRecognizedEllipses.end();it++)
    {
        (*it).draw(Result);

    }
}


void ProcessedIm::Xie_Algorithm(double reqLeastDistance1, double reqLeastDistance2, double reqLeastVotesb) //input=binary mat file with the contourpoints, all points are stored in a vector of Points
{
    reqLeastDistance1=reqLeastDistance1*2;
    cv::Mat im =inputMat.clone();

    std::vector<cv::Point> contourPoints;
    std::vector<cv::Point> newContourPoints;
    findNonZero(im,contourPoints);

    std::sort(contourPoints.begin(),contourPoints.end(),comparePoints); //The vector contourPoints is sorted using our own rules, so we don't get unexpected behavior.
    std::vector<int> vectorVotesb(148);

    cv::Point2i center(0,0);
    double a=0;
    double alpha=0;
    double a2=0;
    double d2=0;
    double f2=0;
    double b2=0;
    double cost=0;
    double d=0;
    double b=0;
    double numerator=0;
    double denominator=0;


    std::vector<cv::Point>::iterator it1=contourPoints.begin();
    std::vector<cv::Point>::iterator it2=contourPoints.begin();
    std::vector<cv::Point>::iterator it3=contourPoints.begin();
    std::vector<cv::Point>::iterator itend=contourPoints.end();
    while(it1!=itend)
    {
        while(it2!=itend)
        {
            if(cv::norm(*it1-*it2)>reqLeastDistance1)
            {

                center=(*it1+*it2)*0.5;
                a=cv::norm(*it1-*it2)*0.5;
                alpha= atan2(it2->y-it1->y,it2->x-it1->x);
                while(it3!=itend)
                {
                    if(it3!=it2 && it3!=it1 && cv::norm(*it3-center)>reqLeastDistance2)
                    {
                        d=cv::norm(*it3-center);
                        a2=pow(a,2);
                        d2=pow(d,2);
                        f2=pow(std::min(cv::norm(*it3-*it1),cv::norm(*it3-*it2)),2);
                        cost=(a2+d2-f2)*pow(2*a*d,-1);
                        numerator=a2*d2*(1-pow(cost,2));
                        denominator=a2-(d2*pow(cost,2));
                        b2=numerator*pow(denominator,-1);
                        if(b2>0&&b2<16384)
                        {
                              b=pow(b2,0.5);
                              ++vectorVotesb.at((int)round(b));

                        }
                    }
                    ++it3;
                }

                std::vector<int>::iterator itMaxEl=std::max_element(vectorVotesb.begin(),vectorVotesb.end());
                if ((*itMaxEl)>reqLeastVotesb)
                {
                     b=(itMaxEl-vectorVotesb.begin());
                     Ellipse res(center,a,b,-alpha*180/CV_PI);
                     XieRecognizedEllipses.push_back(res);
                     res.remove(im,4);
                     if(countNonZero(im)<1)
                     {
                         goto endfunction;
                     }
                     else
                     {
                         findNonZero(im,newContourPoints);
                         std::sort(newContourPoints.begin(),newContourPoints.end(),comparePoints);
                         it1=std::find_first_of(newContourPoints.begin(),newContourPoints.end(),it1,itend);
                         it2=std::find_first_of(newContourPoints.begin(),newContourPoints.end(),it2,itend);
                         contourPoints.clear();
                         contourPoints=newContourPoints;
                         //std::sort(contourPoints.begin(),contourPoints.end(),comparePoints);
                         it1=find(contourPoints.begin(),contourPoints.end(),*it1);
                         it2=find(contourPoints.begin(),contourPoints.end(),*it2);
                         it3=contourPoints.begin();
                         itend=contourPoints.end();

                     }


                }
                vectorVotesb.assign(148,0);
                //std::cout<<" test"<<std::endl;
            }
            it3=contourPoints.begin();
            ++it2;
        }
        it2=contourPoints.begin();
        ++it1;
    }
    endfunction:
    //temp drawing found ellipses
    for(std::vector<Ellipse>::iterator it=XieRecognizedEllipses.begin(); it!=XieRecognizedEllipses.end();it++)
    {
        (*it).draw(Result);

    }
}


std::vector<cv::Point>  ProcessedIm::getBestContour(cv::Mat im ,size_t thresh){
    cv::Mat canny_output;
        std::vector<std::vector<cv::Point> > contours;
        std::vector<cv::Vec4i> hierarchy;

        cv::Canny(im, canny_output, 100, 200, 3 );

        findContours( canny_output, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );

        std::vector<cv::Point> bestContour = contours.at(0);
        size_t currentSize = bestContour.size();
        for(std::vector< std::vector<cv::Point> >::iterator it=contours.begin();it!=contours.end();++it){
            if(it->size()>currentSize){
                bestContour = *it;
                currentSize = it->size();
            }
        }
        if(currentSize > thresh) return bestContour;
        else return {};
}

std::vector<cv::Point> ProcessedIm::allSignalPixels(){
    std::vector<cv::Point> pixelList;
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
