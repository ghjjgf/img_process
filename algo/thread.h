#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <opencv2/opencv.hpp>
#include<opencv2/core/mat.hpp>

class Thread : public QThread
{
    Q_OBJECT
public:
    explicit Thread(QObject *parent = nullptr);
    void setInput(cv::Mat blob, cv::dnn::Net net, std::string weight_path, std::string label_path);
    cv::Mat getOutput();

signals:
    void inferenceFinished(cv::Mat preds);

protected:
    void run() override;

private:
    cv::Mat m_blob;
    cv::dnn::Net m_net;
    std::string m_weight_path;
    std::string m_label_path;
    cv::Mat m_preds;
};

#endif // THREAD_H
