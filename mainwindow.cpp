#include "mainwindow.h"
#include "ui_mainwindow.h"

static int R = 256;
static int C = 256;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    myProcessedIm(256,256)
{
    ui->setupUi(this);
    Ellipse el1(cv::Point(R/2,C/2),50,20,M_PI/4);
    myProcessedIm.addEllipse(el1);
    myProcessedIm.CV_Algorithm();
    myProcessedIm.showInputMat();
    cv::waitKey();


}

MainWindow::~MainWindow()
{
    delete ui;
}

