#include"../algo/function.h"

Function::Function(){}

Function::~Function(){}
//打开图像
Mat Function::openImg(QString img_name){
    image = imread(img_name.toLocal8Bit().data());
    return image;
}
//高斯模糊
Mat Function::gaussianBlur(Mat image,Mat res){
    GaussianBlur(image, res, Size(29, 29), 0, 0);
    return res;
}
//灰度化
Mat Function::img2Gray(Mat image,Mat res){
    cvtColor(image, res, COLOR_BGR2GRAY);
    return res;
}
//边缘检测--canny算子
Mat Function::edgeDetect(Mat image,Mat gray){
    Canny(image, gray, 150, 100, 3);
    return gray;
}
//亮度调整
Mat Function::brightness_adj(Mat image, int brightness){
    // 创建一个和原图相同大小的Mat对象
    cv::Mat img_adj = image.clone();

    // 调整图像亮度
    for (int i = 0; i < img_adj.rows; i++) {
        for (int j = 0; j < img_adj.cols; j++) {
            // 访问每个像素，并增加亮度
            for (int c = 0; c < img_adj.channels(); c++) {
                img_adj.at<cv::Vec3b>(i, j)[c] = cv::saturate_cast<uchar>(img_adj.at<cv::Vec3b>(i, j)[c] + brightness);
            }
        }
    }
    return img_adj;
}
void Function::YOLO_Detect(){}
//保存图像
QImage Function::save_img(QPixmap res){
    // 将 QPixmap 转换为 QImage
    return res.toImage();
}
