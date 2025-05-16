#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <fstream>


Function f;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //未加载图像前 功能按钮禁止使用
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_4->setEnabled(false);
    ui->pushButton_5->setEnabled(false);
    ui->pushButton_6->setEnabled(false);
    ui->pushButton_7->setEnabled(false);
    ui->brightnessSlider->setEnabled(false);

    // 初始化线程
    m_Thread = new Thread(this);
    connect(m_Thread, &Thread::inferenceFinished, this, &MainWindow::handleInferenceFinished);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 打开图像
void MainWindow::on_pushButton_clicked()
{
    // 调用窗口打开文件
    ui->label->clear();
    ui->label_1->clear();

    QString filename = QFileDialog::getOpenFileName(this,tr("open image"),".",tr("Image file(*.png *.jpg *.bmp)"));
    image = f.openImg(filename); // 打开图像

    if (image.data) { // 加载图像后 启动功能按钮
        ui->pushButton_2->setEnabled(true);
        ui->pushButton_3->setEnabled(true);
        ui->pushButton_4->setEnabled(true);
        ui->pushButton_5->setEnabled(true);
        ui->pushButton_6->setEnabled(false);
        ui->pushButton_7->setEnabled(false);
        // 通过 lable 方式显示图片
        display_MatInQT(ui->label, image);
    } else {
        QMessageBox::information(this, tr("提示"), tr("未成功载入图片！"), QMessageBox::Ok);
    }
}

// 高斯模糊
void MainWindow::on_pushButton_2_clicked()
{
    ui->pushButton_6->setEnabled(true);
    ui->pushButton_7->setEnabled(true);
    ui->label_1->clear();
    if (image.data) {
        mat_Gaussian = f.gaussianBlur(image, mat_Gaussian);
        display_MatInQT(ui->label_1, mat_Gaussian);
    } else {
        QMessageBox::information(this, tr("提示"), tr("未成功载入图片！"), QMessageBox::Ok);
    }
}

// 灰度化
void MainWindow::on_pushButton_3_clicked()
{
    ui->pushButton_6->setEnabled(true);
    ui->pushButton_7->setEnabled(true);
    ui->label_1->clear();
    if (image.data) {
        // opencv灰度化函数
        gray = f.img2Gray(image, gray);
        display_MatInQT(ui->label_1, gray);
    } else {
        QMessageBox::information(this, tr("提示"), tr("未成功载入图片！"), QMessageBox::Ok);
    }
}

// 边缘检测--canny算子
void MainWindow::on_pushButton_4_clicked()
{
    ui->pushButton_6->setEnabled(true);
    ui->pushButton_7->setEnabled(true);
    ui->label_1->clear();
    if (image.data) {
        // opencv边缘检测函数
        imageCanny = f.edgeDetect(image, gray);
        display_MatInQT(ui->label_1, imageCanny);
    } else {
        QMessageBox::information(this, tr("提示"), tr("未成功载入图片！"), QMessageBox::Ok);
    }
}

// 亮度调整
void MainWindow::on_pushButton_5_clicked()
{
    ui->pushButton_6->setEnabled(true);
    ui->pushButton_7->setEnabled(true);
    // 清空
    ui->label_1->clear();
    if (image.data) {
        img_bri_adj = f.brightness_adj(image, brightness);
        display_MatInQT(ui->label_1, img_bri_adj);
        ui->brightnessSlider->setEnabled(true);
        ui->brightnessSlider->setRange(-100, 100);  // 范围
    } else {
        QMessageBox::information(this, tr("提示"), tr("未成功载入图片！"), QMessageBox::Ok);
    }
}

void MainWindow::on_brightnessSlider_valueChanged(int value)
{
    brightness = value;  // 更新亮度值
    on_pushButton_5_clicked();  // 调用更新图像的函数
}

// YOLO检测 设置标签和模型
void MainWindow::performInference(cv::Mat blob, cv::dnn::Net net, std::string weight_path, std::string label_path)
{
    m_Thread->setInput(blob, net, weight_path, label_path);
    m_Thread->start();
}

void MainWindow::on_pushButton_6_clicked()
{
    QPixmap currentPixmap = *(ui->label_1->pixmap());  // 假设你用 QLabel 来显示图像
    load_onnx();
    QImage mid = currentPixmap.toImage();
    for(auto v:class_names) cout<<v<<" ";
    cout<<endl;
    if (true) {
        // 图像预处理
        cv::Mat blob = preprocess(currentPixmap);
        // 启动推理线程
        performInference(blob, net, weight_path, label_path);
        // 移除 m_Thread->wait();
        preds = m_Thread->getOutput();
        postprocess(preds);

    }
}

// 图像保存
void MainWindow::on_pushButton_7_clicked()
{
    // 比如你有一个 QPixmap 成员变量，表示当前显示的图像
    QPixmap currentPixmap = *(ui->label_1->pixmap());  // 假设你用 QLabel 来显示图像
    // 将 QPixmap 转换为 QImage
    QImage img = f.save_img(currentPixmap);
    // 使用 QFileDialog 打开保存文件的对话框
    QString fileName = QFileDialog::getSaveFileName(this, tr("保存图像"), "", tr("JPEG Files (*.jpg);;PNG Files (*.png);;BMP Files (*.bmp)"));
    if (!fileName.isEmpty()) {
        // 保存图像为文件
        img.save(fileName);
    }
}

// 格式转换
QImage MainWindow::MatToQImage(const cv::Mat& mat)
{
    // 8-bits unsigned, NO. OF CHANNELS = 1  单通道--灰度图像
    if (mat.type() == CV_8UC1) {
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
        // Set the color table (used to translate colour indexes to qRgb values)
        image.setColorCount(256);
        for (int i = 0; i < 256; i++) { // opencv 读入图像为BGR通道
            image.setColor(i, qRgb(i, i, i));
        }
        // Copy input Mat
        uchar* pSrc = mat.data;
        for (int row = 0; row < mat.rows; row++) {
            uchar* pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return image;
    }
    // 8-bits unsigned, NO. OF CHANNELS = 3 三通道--彩色图像
    else if (mat.type() == CV_8UC3) {
        // Copy input Mat
        const uchar* pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, (int)mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    } else if (mat.type() == CV_8UC4) {
        //qDebug() << "CV_8UC4";
        // Copy input Mat
        const uchar* pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, (int)mat.step, QImage::Format_ARGB32);
        return image.copy();
    } else {
        //qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}

// 显示功能
void MainWindow::display_MatInQT(QLabel* label, cv::Mat mat)
{
    label->setPixmap(QPixmap::fromImage(MatToQImage(mat)).scaled(label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

// 设置模型与标签路径
std::string MainWindow::set_weight_path()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("选择onnx模型"), "", tr("*.onnx"));
    weight_path = fileName.toStdString();
    set_config_path();
    qDebug() << "Selected weight file: " << fileName; // 打印选中的文件路径进行调试
    return weight_path;
}

void MainWindow::set_config_path()
{
    auto fileName = QFileDialog::getOpenFileName(this, "选择标签文件", "", tr("(*.txt)"));
    label_path = fileName.toStdString();
    qDebug() << "Selected labels file: " << fileName; // 打印选中的文件路径进行调试
    // 标签文件
    std::ifstream fp(label_path);
    if (!fp.is_open()) {
        printf("could not open file...\n");
        exit(-1);
    }
    std::string name;
    while (!fp.eof()) {
        std::getline(fp, name);
        if (name.length())
            class_names.push_back(name);
    }
    fp.close();
}

// 加载模型
void MainWindow::load_onnx()
{
    if (weight_path != "")
    {
        net = cv::dnn::readNetFromONNX(weight_path);

    }
    else {
        std::string w = set_weight_path();
        net = cv::dnn::readNetFromONNX(w);
    }
}

// 前处理
cv::Mat MainWindow::preprocess(QPixmap currentPixmap)
{
    QImage img = currentPixmap.toImage();  // 将 QPixmap 转换为 QImage
    img = img.convertToFormat(QImage::Format_RGB888); // 转换为 RGB 格式，确保没有透明度通道
    cv::Mat mat = cv::Mat(img.height(), img.width(), CV_8UC3, const_cast<uchar*>(img.bits()), img.bytesPerLine());  // 转换为 cv::Mat
    res = mat;  // 转换为 Mat 类型
    int w = res.cols;
    int h = res.rows;
    int _max = std::max(h, w);
    cv::Mat image = cv::Mat::zeros(cv::Size(_max, _max), CV_8UC3);
    cv::Rect roi(0, 0, w, h);
    res.convertTo(image, CV_8UC3);  // 将res转换为3通道图像
    x_factor = image.cols / 640.0f;
    y_factor = image.rows / 640.0f;
    // 图像预处理
    cv::Mat blob = cv::dnn::blobFromImage(image, 1 / 255.0, cv::Size(640, 640), cv::Scalar(0, 0, 0), true, false);
    return blob;
}

// 后处理
//void MainWindow::postprocess(cv::Mat preds)
//{
//    // 初始化置信度阈值
//    float confidence_threshold = 0.25f;

//    qDebug() << "开始后处理...";

//    cv::Mat detOut(preds.size[1], preds.size[2], CV_32F, preds.ptr<float>());
//    cout<<"接收输出"<<endl;
//    cv::Mat det_output = detOut.t();

//    qDebug() << "检测框数量:" << det_output.rows;

//    for (int i = 0; i < det_output.rows; i++) {
//        cv::Mat classes_scores = det_output.row(i).colRange(4, preds.size[1]);
//        cv::Point classIdPoint;
//        double score;
//        cv::minMaxLoc(classes_scores, 0, &score, 0, &classIdPoint);

//        // 输出每个检测框的置信度
//        qDebug() << "检测框" << i << "最大置信度:" << score;

//        // 使用初始化后的置信度阈值
//        if (score > confidence_threshold) {
//            float cx = det_output.at<float>(i, 0);
//            float cy = det_output.at<float>(i, 1);
//            float ow = det_output.at<float>(i, 2);
//            float oh = det_output.at<float>(i, 3);
//            int x = static_cast<int>((cx - 0.5 * ow) * x_factor);
//            int y = static_cast<int>((cy - 0.5 * oh) * y_factor);
//            int width = static_cast<int>(ow * x_factor);
//            int height = static_cast<int>(oh * y_factor);
//            cv::Rect box;
//            box.x = x;  // 左上x
//            box.y = y;  // 左上y
//            box.width = width;
//            box.height = height;

//            boxes.push_back(box);
//            classIds.push_back(classIdPoint.x);
//            confidences.push_back(score);
//        }
//    }

//    qDebug() << "通过置信度阈值的检测框数量:" << boxes.size();

//    // NMS
//    cv::dnn::NMSBoxes(boxes, confidences, confidence_threshold, 0.50, indexes);
//    qDebug() << "NMS后的检测框数量:" << indexes.size();

//    draw_boxes();
//}
void MainWindow::postprocess(cv::Mat preds)
{
    // 初始化置信度阈值
    float confidence_threshold = 0.25f;

    qDebug() << "开始后处理...";

//    // 检查 preds 的维度
//    if (preds.dims < 3) {
//        cout << "2222"<<endl;
//        return;
//    }

    cv::Mat detOut(preds.size[1], preds.size[2], CV_32F, preds.ptr<float>());
    cout<<"接收输出"<<endl;
    cv::Mat det_output = detOut.t();

    qDebug() << "检测框数量:" << det_output.rows;

    for (int i = 0; i < det_output.rows; i++) {
        cv::Mat classes_scores = det_output.row(i).colRange(4, preds.size[1]);
        cv::Point classIdPoint;
        double score;
        cv::minMaxLoc(classes_scores, 0, &score, 0, &classIdPoint);

        // 输出每个检测框的置信度
        qDebug() << "检测框" << i << "最大置信度:" << score;

        // 使用初始化后的置信度阈值
        if (score > confidence_threshold) {
            float cx = det_output.at<float>(i, 0);
            float cy = det_output.at<float>(i, 1);
            float ow = det_output.at<float>(i, 2);
            float oh = det_output.at<float>(i, 3);
            int x = static_cast<int>((cx - 0.5 * ow) * x_factor);
            int y = static_cast<int>((cy - 0.5 * oh) * y_factor);
            int width = static_cast<int>(ow * x_factor);
            int height = static_cast<int>(oh * y_factor);
            cv::Rect box;
            box.x = x;  // 左上x
            box.y = y;  // 左上y
            box.width = width;
            box.height = height;

            boxes.push_back(box);
            classIds.push_back(classIdPoint.x);
            confidences.push_back(score);
        }
    }

    qDebug() << "通过置信度阈值的检测框数量:" << boxes.size();

    // NMS
    cv::dnn::NMSBoxes(boxes, confidences, confidence_threshold, 0.50, indexes);
    qDebug() << "NMS后的检测框数量:" << indexes.size();

    draw_boxes();
}
// 绘框
void MainWindow::draw_boxes()
{
    int w = res.cols;
    int h = res.rows;
    int _max = std::max(h, w);
    float scale = static_cast<float>(_max) / 640.0f;
    int thickness = std::max(1, static_cast<int>(2 * scale));
    double fontScale = 0.4 * scale;
    int fontThickness = std::max(1, static_cast<int>(1 * scale));

    result_boxes.clear();// 清除过往数据

    // 输出检测到的对象数量
    qDebug() << "检测到" << indexes.size() << "个对象：";

    for (size_t i = 0; i < indexes.size(); i++) {
        int index = indexes[i];
        int idx = classIds[index];
        float conf = confidences[index];

        ODResultBox result_box;  // 结果保存
        result_box.box = boxes[index];
        result_box.score = conf;
        result_box.name = class_names[idx];
        result_boxes.push_back(result_box);

        // 输出每个检测到的对象信息
        qDebug() << "对象" << i + 1 << ":";
        qDebug() << "  类别:" << QString::fromStdString(class_names[idx]);
        qDebug() << "  置信度:" << conf;
        qDebug() << "  位置: x=" << boxes[index].x << " y=" << boxes[index].y
                 << " width=" << boxes[index].width << " height=" << boxes[index].height;

        // 绘制矩形框
        cv::rectangle(res, boxes[index], cv::Scalar(0, 255, 0), thickness, cv::LINE_8);

        // 计算文本位置，确保在图像内
        cv::Point text_org = boxes[index].tl();
        text_org.y = std::max(text_org.y - 5, 20); // 确保文本不会超出图像顶部

        std::string label = class_names[idx] + cv::format(" %.2f", conf);
        cv::putText(res, label, text_org,
                    cv::FONT_HERSHEY_SIMPLEX, fontScale, cv::Scalar(0, 255, 0), fontThickness, cv::LINE_AA);
    }

    // 如果没有检测到任何对象
    if (indexes.size() == 0) {
        qDebug() << "未检测到任何对象";
    }
    display_MatInQT(ui->label_1, res);
}

void MainWindow::handleInferenceFinished(cv::Mat preds)
{
    // 后处理
    postprocess(preds);
    // 显示
    display_MatInQT(ui->label_1, res);
}
