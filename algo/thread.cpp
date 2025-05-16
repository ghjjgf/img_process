#include "../algo/thread.h"

Thread::Thread(QObject *parent) : QThread(parent)
{
}

void Thread::setInput(cv::Mat blob, cv::dnn::Net net, std::string weight_path, std::string label_path)
{
    m_blob = blob;
    m_net = net;
    m_weight_path = weight_path;
    m_label_path = label_path;
}

cv::Mat Thread::getOutput()
{
    return m_preds;
}

void Thread::run()
{
    m_net.setInput(m_blob);
    m_preds = m_net.forward();
    emit inferenceFinished(m_preds);
}
