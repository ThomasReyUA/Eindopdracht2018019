#include "mainwindow.h"
#include "ui_mainwindow.h"

//I suggest setting these values in Mainwindow constructor from the dimensions of the labels,
//which should be 256*256 instead of using (ugly) static integers
static int R = 256;//Temporary
static int C = 256;//Temporary

//INTENDED USAGE OF THESE CLASSES:
// When new image is to be processed use ProcessedIm memberfunctions
// like 'reset()', 'AddNoise(int)' and 'addEllipse(Ellipse)' to generate a new InputMat
// and then calculate with 'CV_Algorithm()' or 'Xie_Algorithm'.
//
// The results will be stored in the vectors 'XieRecognizedEllipses' and
// 'CVRecognizedEllipses' as Ellipse-objects, which can easily be drawn on a Mat
// using the Ellipse-memberfuntion 'draw(Mat)'.
// The Signal-to-Noise ratio can be calculated from the ProcessedIm-datamembers 'N' and 'M'

//TO DO PROGRAMMEERWERK:
// - Xie-Algoritme
// - Rectangle- en Line-objects
// - setInputMat-functie om ingeladen afbeeldingen te kunnen processen
// - Onderscheid Public - Protected - Private en Const opkuisen

// - Optimalisatie CV_Algoritme/getBestContour-functie om ipv
//   telkens opnieuw de beste gewoon alle goeie te returnen
//   -> geprobeerd, werkt beduidend minder goed en niet merkbaar sneller
//


//MainWindow constructor
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    myProcessedIm(R,C)//Initialize the image processing class dimensions
{

    ui->setupUi(this);

    //Define 2 ellipses
    Ellipse el1(cv::Point(64,64),50,20,-100);
    Ellipse el2(cv::Point(80,128),80,20,60);

    //Add the 2 ellipses to the image processing
    myProcessedIm.addEllipse(el1);
    myProcessedIm.addEllipse(el2);

    //Add some noise
    myProcessedIm.addNoise(50);

    /*
    //Ellipse detection with cv::fitEllipse()
    myProcessedIm.CV_Algorithm();

    //Temporary visualisation of results
    myProcessedIm.showInputMat();
    myProcessedIm.showResultMat();
    */

    //Ellipse detection with ProcessedIm::Xie_Algorithm()
    myProcessedIm.Xie_Algorithm(20,20,100);
    std::cout<<"test2"<<std::endl;

    //Temporary visualisation of results
    myProcessedIm.showInputMat();
    myProcessedIm.showResultMat();

}

MainWindow::~MainWindow()
{
    delete ui;
}




