#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <opencv2/opencv.hpp>
#include "processedim.h"
#include "shape.h"
#include "ellipse.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_loadImage_clicked();

    void on_saveImage_clicked();

    void on_input_clicked();

    void on_addLineButton_clicked();

    void on_parametersEllips_cellChanged(int row, int column);

    void on_clear_clicked();

    void on_parametersLine_cellChanged(int row, int column);

    void on_calculate_clicked();

    void on_draw_clicked();

private:
    Ui::MainWindow *ui;
    ProcessedIm myProcessedIm;  //This is where the magic happens
    QPixmap image;
    QImage  *imageObject;
    QGraphicsScene *scene;

    bool isNumber(std::string); // checks if the string is a number (- is allowed)
};

#endif // MAINWINDOW_H
