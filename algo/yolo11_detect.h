#pragma once
#include "../mainwindow.h"
#include "ui_mainwindow.h"

class YOLO11_Detect : public MainWindow{
public:

    void set_config_path() ;
    string set_weight_path();



};
