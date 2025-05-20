项目名：图像处理系统 (Image Processing System)

开发者：王修

软件使用方法：
1. 运行程序后，点击"打开图像"按钮选择要处理的图片
2. 选择图片后，可以使用以下功能按钮进行处理：
   - 高斯模糊：对图像进行高斯模糊处理
   - 灰度化：将图像转换为灰度图
   - 边缘检测：使用Canny算子进行边缘检测
   - 亮度调整：通过滑块调节图像亮度
   - YOLO检测：使用YOLO模型进行目标检测（待完善）
   - 保存图像：将处理后的图像保存到本地

功能介绍：
1. 基础图像处理
   - 图像打开和保存
   - 高斯模糊处理
   - 图像灰度化
   - 边缘检测
   - 亮度调节

2. 高级功能
   - YOLO目标检测
   - 多格式图像支持（支持PNG、JPG、BMP格式）

注意事项：
1. 使用前请确保已正确安装OpenCV 4.8.0
2. 进行YOLO检测前需要选择正确的模型文件(.onnx)和标签文件(.txt)
3. 图像处理前必须先打开图像文件
4. 保存图像时请选择正确的保存格式
5. 建议使用分辨率适中的图片，过大的图片可能会影响处理速度

软件开发环境和工具：
1. 开发环境
   - 操作系统：Windows 11
   - IDE：Qt Creator
   - 编译器：MSVC 2017 64bit
   - Qt版本：Qt 5.12.12

2. 依赖库
   - OpenCV 4.8.0
   - Qt 5.12.12
   - MSVC 2017 64bit

3. 支撑环境
   - OpenCV库路径：G:/enlishpath/opencv-4.8.0/opencv/build
   - 需要安装的DLL：
     * opencv_world480.dll (Release版本)
     * opencv_world480d.dll (Debug版本)

4. 项目结构
   - mainwindow.cpp/h：主窗口实现
   - algo/：算法实现目录
     * function.cpp/h：图像处理函数
     * config.cpp/h：配置相关
     * thread.cpp/h：线程处理
   - main.cpp：程序入口
   - mainwindow.ui：界面设计文件

  5. 项目ui界面展示：

     ![image](https://github.com/user-attachments/assets/9293bb08-76fc-4223-90ae-6f67ff6c7738)

