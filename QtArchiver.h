#pragma once

#include <QtWidgets/QWidget>
#include "ui_QtArchiver.h"

class QtArchiver : public QWidget
{
    Q_OBJECT

public:
    QtArchiver(QWidget* parent = nullptr);
    ~QtArchiver();

private:
    Ui::QtArchiverClass ui;
    QString path;

private slots:
    void on_openFileButton_clicked();
    void on_clearPathButton_clicked();
    void on_compressButton_clicked();
    void on_decompressButton_clicked();
};