#pragma once
#include <QString>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QImage>
#include <QPixmap>

using namespace std;
using namespace cv;

class Function{
private:
    Mat image;
public:
    Function();
    ~Function();
    Mat openImg(QString img_name);
    Mat img2Gray(Mat image, Mat dst);
    Mat gaussianBlur(Mat image, Mat dst);
    Mat edgeDetect(Mat image, Mat dst);
    Mat brightness_adj(Mat image, int brightness);
    void YOLO_Detect();
    QImage save_img(QPixmap res);
};
