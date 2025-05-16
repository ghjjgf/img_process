#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QFileDialog>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/dnn/dnn.hpp>
#include<QScreen>
#include<stack>
#include<string>
#include <vector>
#include <fstream>
#include <QDebug>
#include "../algo/thread.h"
#include "../algo/function.h"

#include<opencv2/core/mat.hpp>
using namespace std;
using namespace cv;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct ODResultBox {
    cv::Rect box;
    float score;
    std::string name;
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_brightnessSlider_valueChanged(int value);
    void on_pushButton_6_clicked();
    void on_pushButton_7_clicked();
    void handleInferenceFinished(cv::Mat preds);

private:
    Ui::MainWindow *ui;
    cv::Mat image;
    cv::Mat mat_Gaussian;
    cv::Mat gray;
    cv::Mat imageCanny;
    cv::Mat img_bri_adj;
    cv::Mat res;
    cv::dnn::Net net;
    std::string weight_path;
    std::string label_path;
    std::vector<std::string> class_names;
    std::vector<cv::Rect> boxes;
    std::vector<int> classIds;
    std::vector<float> confidences;
    std::vector<int> indexes;
    std::vector<ODResultBox> result_boxes;
    float brightness;
    float x_factor;
    float y_factor;
    Thread *m_Thread;
    Mat preds;

    void performInference(cv::Mat blob, cv::dnn::Net net, std::string weight_path, std::string label_path);
    QImage MatToQImage(const cv::Mat& mat);
    void display_MatInQT(QLabel* label, cv::Mat mat);
    std::string set_weight_path();
    void set_config_path();
    void load_onnx();
    cv::Mat preprocess(QPixmap currentPixmap);
    void postprocess(cv::Mat preds);
    void draw_boxes();
};
#endif // MAINWINDOW_H









