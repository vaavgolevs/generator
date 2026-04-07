#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_generator.h"

class generator : public QMainWindow
{
    Q_OBJECT

public:
    generator(QWidget *parent = nullptr);
    ~generator();

private:
    Ui::generatorClass ui;
};

