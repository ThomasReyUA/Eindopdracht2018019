#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsScene>
#include <QFileDialog>
#include <QGraphicsView>

static int R = 256;
static int C = 256;

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
    myProcessedIm(R,C) //Initialize the image processing class dimensions
{

    ui->setupUi(this);
    //knop
    ui->draw->setDisabled(true);

    // setting the spinboxes
    ui->noiseSpinBox->setRange(0,15000);
    ui->minVal2aSpinBox->setRange(0, ui->graphicsView->width()/2);
    ui->minVal2aSpinBox->setValue(30);
    ui->minVal2bSpinBox->setRange(0, ui->graphicsView->width()/2);
    ui->minVal2bSpinBox->setValue(30);
    ui->tresholdSpinBox->setRange(0, 500);
    ui->tresholdSpinBox->setValue(50);

    // setting the checkboxes
    ui->input_check->setCheckable(true);
    ui->input_check->setChecked(true);
    ui->CV_check->setDisabled(true);
    ui->CV_check->setCheckable(true);
    ui->Xie_check->setDisabled(true);
    ui->Xie_check->setCheckable(true);
    ui->line_check->setCheckable(true);

    // setting the input table for the ellipses
    ui->parametersEllips->setColumnCount(5);
    ui->parametersEllips->setRowCount(1);

    for(int c=0; c<(ui->parametersEllips->columnCount());c++)
    {
        ui->parametersEllips->setColumnWidth(c,92);
        // also looping over the rows, so when the initial amount is changed it will still work.
        for(int r=0; r<ui->parametersEllips->rowCount(); r++)
        {
            ui->parametersEllips->setItem(r, c, new QTableWidgetItem("0"));

        }
    }

    QStringList HeadersEl;
    HeadersEl << "x" << "y" << "a" << "b" << "angle";
    ui->parametersEllips->setHorizontalHeaderLabels(HeadersEl);

    // setting the input table for the line
    ui->parametersLine->setColumnCount(3);
    ui->parametersLine->setRowCount(1);

    for(int c=0; c<(ui->parametersLine->columnCount());c++)
    {
        ui->parametersLine->setColumnWidth(c,76);
        // also looping over the rows, so when the initial amount is changed it will still work.
        for(int r=0; r<ui->parametersLine->rowCount(); r++)
        {
            ui->parametersLine->setItem(r, c, new QTableWidgetItem("0"));

        }
    }

    QStringList HeadersL;
    HeadersL << "x" << "y" << "angle";
    ui->parametersLine->setHorizontalHeaderLabels(HeadersL);
    ui->parametersLine->verticalHeader()->hide();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loadImage_clicked()
{
    QString imagePath = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "",
                tr("JPEG (*.jpg *.jpeg);;PNG (*.png)" )
                );
    imageObject = new QImage();

    imageObject->load(imagePath);
    if (imagePath.isNull() || imagePath.isEmpty()) return;

    QImage scaledImage ;
    scaledImage = imageObject->scaled(ui->graphicsView->width(), ui->graphicsView->height(), Qt::IgnoreAspectRatio);
    image = QPixmap::fromImage(scaledImage);

    scene = new QGraphicsScene(this);
    scene->addPixmap(image);
    scene->setSceneRect(image.rect());
    ui->graphicsView->setScene(scene);

    ui->input_check->setChecked(true);

    // setting the input mat

    input = cv::Mat(scaledImage.height(),scaledImage.width(),CV_8UC4, const_cast<uchar*>(scaledImage.bits()), static_cast<size_t>(scaledImage.bytesPerLine()));
    cvtColor(input, input, CV_RGB2GRAY);
    // BLUR
    blur(input,myProcessedIm.inputMat, cv::Size(3,3));
    Canny(myProcessedIm.inputMat,myProcessedIm.inputMat, 10, 30, 3);

    myProcessedIm.showInputMat();
    myProcessedIm.inputMade = true;

    ui->Input_widget->setDisabled(true);
    ui->lijn_widget->setDisabled(true);

    delete imageObject, scene;
}

void MainWindow::on_saveImage_clicked()
{
    QString imagePath = QFileDialog::getSaveFileName(
                this,
                tr("Save Image"),
                "",
                tr("PNG (*.png)" )
                );

    if (imagePath.isNull() || imagePath.isEmpty()) return;
    if (!imagePath.endsWith(".png"))
        imagePath += ".png";

    *imageObject = image.toImage();

    imageObject->save(imagePath);
}

void MainWindow::on_input_clicked()
{
    ui->loadImage_widget->setDisabled(true);
    ui->draw->setDisabled(true);

    if (countNonZero(myProcessedIm.inputMat) == 0 || myProcessedIm.inputMat.cv::Mat::empty() || !myProcessedIm.inputMade)
        //the inputMat should only be calculated when there is no data or only zeros or xhen a change is made
    {
        myProcessedIm.inputMat = cv::Mat(cv::Size(ui->graphicsView->width(), ui->graphicsView->height()), CV_8UC1, cv::Scalar(0));
        bool ok;
        for(int r=0; r < ui->parametersEllips->rowCount(); r++)
        {
            // getting the parameters from the table.
            int x = ui->parametersEllips->item(r,0)->text().toInt(&ok, 10);
            int y = ui->parametersEllips->item(r,1)->text().toInt(&ok, 10);
            int a = ui->parametersEllips->item(r,2)->text().toInt(&ok, 10);
            int b = ui->parametersEllips->item(r,3)->text().toInt(&ok, 10);
            int alpha = ui->parametersEllips->item(r,4)->text().toInt(&ok, 10);

            if(a < b) // the long axis should be longer
            {
            int b2 = a;
            a = b;
            b = b2;
            ui->parametersEllips->item(r,2)->setText(QString::number(a));
            ui->parametersEllips->item(r,3)->setText(QString::number(b));
            }

            Ellipse el(cv::Point(x,y), a, b, alpha); // constructing an ellipse.
            myProcessedIm.addEllipse(el); // adding the ellips to the inputMat
            myProcessedIm.InputEllipses.push_back(el);
        }
        myProcessedIm.addNoise(ui->noiseSpinBox->value()); // adding the noise to the inputMat
        // ADDLINE --> checkbox: line_check
        cvtColor(myProcessedIm.inputMat, input, CV_GRAY2BGR);
        myProcessedIm.inputMade = true;
    }

    QImage img = QImage((unsigned char*) myProcessedIm.inputMat.data, myProcessedIm.inputMat.cols, myProcessedIm.inputMat.rows, QImage::Format_Indexed8);
    image = QPixmap::fromImage(img);
    scene = new QGraphicsScene(this);
    scene->addPixmap(image);
    scene->setSceneRect(image.rect());
    ui->graphicsView->setScene(scene);

    ui->input_check->setChecked(true);
}

void MainWindow::on_addLineButton_clicked()
{
    ui->parametersEllips->insertRow(ui->parametersEllips->rowCount()); // adding a new row
    for(int c=0; c<(ui->parametersEllips->columnCount());c++)
        // setting al the elements to zero
    {
        ui->parametersEllips->setItem(ui->parametersEllips->rowCount()-1, c, new QTableWidgetItem("0"));
    }
}

bool MainWindow::isNumber(std::string s)
{
    std::string::const_iterator it = s.begin();
    if(*it == '-') // checking if the first character is '-'
    {
            it++;
    }
    while (it != s.end() && std::isdigit(*it))  ++it;
    return !s.empty() && it == s.end();
}

void MainWindow::on_parametersEllips_cellChanged(int row, int column)
{
    myProcessedIm.inputMade = false; //indicating that the inputMat isn't up to date anymore

    QString qStr = ui->parametersEllips->item(row, column)->text(); // getting the QString from the QTableWidgetItem
    std::string str = qStr.toStdString(); // converting the QString to a std::string

    if(!isNumber(str)) // the string needs to be a number
    {
            ui->parametersEllips->item(row, column)->setText("0");
    }

    //checking the range
    bool ok;
    if(column == 0) // x needs to be in inside the graphicsview
    {
        int i = qStr.toInt(&ok, 10); // converting the QString to a decimal integer
        if(i < 0 || i > ui->graphicsView->width())
        {
            ui->parametersEllips->item(row, column)->setText("0");
        }
    }

    if(column == 1) // y needs to be in inside the graphicsview
    {
        int i = qStr.toInt(&ok, 10); // converting the QString to a decimal integer
        if(i < 0 || i > ui->graphicsView->height())
        {
            ui->parametersEllips->item(row, column)->setText("0");
        }
    }

    if(column == 2 || column == 3) // a and b need to be in [0, half the window]
    {
        int i = qStr.toInt(&ok, 10); // converting the QString to a decimal integer
        if(i < 0 || i > ui->graphicsView->width()/2)
        {
            ui->parametersEllips->item(row, column)->setText("0");
        }
    }

    if(column == 4) // alpha needs to be in [-90,90]
    {
        int alpha = qStr.toInt(&ok, 10); // converting the QString to a decimal integer
        if(alpha < -90 || alpha > 90)
        {
            ui->parametersEllips->item(row, column)->setText("0");
        }
    }
}

void MainWindow::on_clear_clicked()
{
    myProcessedIm.reset();

    //graphicsView leeg
    ui->graphicsView->setScene(0);
    ui->graphicsView->viewport()->update();

    //gevonden ellipsen leeg
    ui->CV_text->clear();
    ui->Xie_text->clear();

    //checkboxen
    ui->CV_check->setChecked(false);
    ui->CV_check->setDisabled(true);
    ui->Xie_check->setChecked(false);
    ui->Xie_check->setDisabled(true);

    //parametersspinboxen
    ui->minVal2aSpinBox->setValue(30);
    ui->minVal2bSpinBox->setValue(30);
    ui->tresholdSpinBox->setValue(50);

    ui->noiseSpinBox->setValue(0);

    //tabel ellipsen
    while(ui->parametersEllips->rowCount() > 1) ui->parametersEllips->removeRow(0);
    ui->parametersEllips->clear();
    for(int c=0; c<(ui->parametersEllips->columnCount());c++)
    {
        ui->parametersEllips->setColumnWidth(c,92);
        // also looping over the rows, so when the initial amount is changed it will still work.
        for(int r=0; r<ui->parametersEllips->rowCount(); r++)
        {
            ui->parametersEllips->setItem(r, c, new QTableWidgetItem("0"));

        }
    }

    QStringList HeadersEl;
    HeadersEl << "x" << "y" << "a" << "b" << "angle";
    ui->parametersEllips->setHorizontalHeaderLabels(HeadersEl);

    //tabel lijn
    ui->parametersLine->clear();
    for(int c=0; c<(ui->parametersLine->columnCount());c++)
    {
        ui->parametersLine->setColumnWidth(c,76);
        // also looping over the rows, so when the initial amount is changed it will still work.
        for(int r=0; r<ui->parametersLine->rowCount(); r++)
        {
            ui->parametersLine->setItem(r, c, new QTableWidgetItem("0"));

        }
    }

    //knoppen ed
    ui->draw->setDisabled(true);

    ui->Input_widget->setEnabled(true);
    ui->loadImage_widget->setEnabled(true);
    ui->lijn_widget->setEnabled(true);

    QStringList HeadersL;
    HeadersL << "x" << "y" << "angle";
    ui->parametersLine->setHorizontalHeaderLabels(HeadersL);
    ui->parametersLine->verticalHeader()->hide();

}

void MainWindow::on_parametersLine_cellChanged(int row, int column)
{
    myProcessedIm.inputMade = false; //indicating that the inputMat isn't up to date anymore

    QString qStr = ui->parametersEllips->item(row, column)->text(); // getting the QString from the QTableWidgetItem
    std::string str = qStr.toStdString(); // converting the QString to a std::string

    if(!isNumber(str)) // the string needs to be a number
    {
            ui->parametersEllips->item(row, column)->setText("0");
    }

    //checking the range
    bool ok;
    if(column == 0) // x needs to be in inside the graphicsview
    {
        int i = qStr.toInt(&ok, 10); // converting the QString to a decimal integer
        if(i < 0 || i > ui->graphicsView->width())
        {
            ui->parametersEllips->item(row, column)->setText("0");
        }
    }

    if(column == 1) // y needs to be in inside the graphicsview
    {
        int i = qStr.toInt(&ok, 10); // converting the QString to a decimal integer
        if(i < 0 || i > ui->graphicsView->height())
        {
            ui->parametersEllips->item(row, column)->setText("0");
        }
    }

    if(column == 2) // angle needs to be in [-90,90]
    {
        int angle = qStr.toInt(&ok, 10); // converting the QString to a decimal integer
        if(angle < -90 || angle > 90)
        {
            ui->parametersEllips->item(row, column)->setText("0");
        }
    }
}

void MainWindow::on_calculate_clicked()
{
    ui->CV_check->setEnabled(true);
    ui->Xie_check->setEnabled(true);
    ui->draw->setEnabled(true);

    ui->CV_text->clear();
    ui->Xie_text->clear();

    myProcessedIm.CVRecognizedEllipses.clear();
    myProcessedIm.XieRecognizedEllipses.clear();
    myProcessedIm.CV_Algorithm();
    std::cout << "CV DONE" << std::endl;

    QString strCV1;
    strCV1.QString::setNum(myProcessedIm.CVRecognizedEllipses.size());
    strCV1.append(" ellipses found by the CV algorithm: \n");
    ui->CV_text->append(strCV1);
    if(!myProcessedIm.CVRecognizedEllipses.empty())
    {
        int i = 1;
        for(std::vector<Ellipse>::iterator it = myProcessedIm.CVRecognizedEllipses.begin(); it < myProcessedIm.CVRecognizedEllipses.end(); it++)
        {
            QString strCV, s2, s3, s4, s5, s6, s7;
            strCV.append("Ellipse: ");
            strCV.append(s2.QString::setNum(i));
            i++;
            strCV.append("\n   center x = ");
            strCV.append(s3.QString::setNum(it->M.x));
            strCV.append("\n   center y = ");
            strCV.append(s4.QString::setNum(it->M.y));
            strCV.append("\n   a = ");
            strCV.append(s5.QString::setNum(it->a));
            strCV.append("\n   b = ");
            strCV.append(s6.QString::setNum(it->b));
            strCV.append("\n   angle = ");
            strCV.append(s7.QString::setNum(it->angle));
            strCV.append("\n");
            ui->CV_text->append(strCV); //writing the data of the found ellips
        }
    }
    std::cout << "starting XIE" << std::endl;
    myProcessedIm.Xie_Algorithm(ui->minVal2aSpinBox->value(), ui->minVal2bSpinBox->value(), ui->tresholdSpinBox->value());
    std::cout << "Xie DONE" << std::endl;

    QString strXie1;
    strXie1.QString::setNum(myProcessedIm.XieRecognizedEllipses.size());
    strXie1.append(" ellipses found by the Xie algorithm: \n");
    ui->Xie_text->append(strXie1);
    if(!myProcessedIm.XieRecognizedEllipses.empty())
    {
        int i = 1;
        for(std::vector<Ellipse>::iterator it = myProcessedIm.XieRecognizedEllipses.begin(); it < myProcessedIm.XieRecognizedEllipses.end(); it++)
        {
            QString strXie, s2, s3, s4, s5, s6, s7;
            strXie.append("Ellipse: ");
            strXie.append(s2.QString::setNum(i));
            i++;
            strXie.append("\n   center x = ");
            strXie.append(s3.QString::setNum(it->M.x));
            strXie.append("\n   center y = ");
            strXie.append(s4.QString::setNum(it->M.y));
            strXie.append("\n   a = ");
            strXie.append(s5.QString::setNum(it->a));
            strXie.append("\n   b = ");
            strXie.append(s6.QString::setNum(it->b));
            strXie.append("\n   angle = ");
            strXie.append(s7.QString::setNum(it->angle));
            strXie.append("\n");
            ui->Xie_text->append(strXie); //writing the data of the found ellips
        }
    }

}

void MainWindow::on_draw_clicked()
{
    cv::Mat Result = cv::Mat(cv::Size(ui->graphicsView->width(), ui->graphicsView->height()), CV_8UC3, cv::Scalar(0,0,0));

    if(ui->input_check->isChecked())
    {
        Result = input.clone();
    }

    if(ui->CV_check->isChecked())
    {
        for(std::vector<Ellipse>::iterator it = myProcessedIm.CVRecognizedEllipses.begin(); it < myProcessedIm.CVRecognizedEllipses.end(); it++)
        {
            cv::ellipse(Result,(*it).M,cv::Size((*it).a,(*it).b),-(*it).angle ,0,360,cv::Scalar(0,255,0),1);
        }
    }

    if(ui->Xie_check->isChecked())
    {
        for(std::vector<Ellipse>::iterator it = myProcessedIm.XieRecognizedEllipses.begin(); it < myProcessedIm.XieRecognizedEllipses.end(); it++)
        {
            cv::ellipse(Result,(*it).M,cv::Size((*it).a,(*it).b),-(*it).angle ,0,360,cv::Scalar(0,0,255),1);
        }
    }

    QImage img = QImage((unsigned char*) Result.data, Result.cols, Result.rows, QImage::Format_RGB888);
    image = QPixmap::fromImage(img);
    scene = new QGraphicsScene(this);
    scene->addPixmap(image);
    scene->setSceneRect(image.rect());
    ui->graphicsView->setScene(scene);
}
