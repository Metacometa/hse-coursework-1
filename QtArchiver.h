#pragma once

#include <QtWidgets/QWidget>
#include "ui_QtArchiver.h"
#include "CompressionDialog.h"

class QtArchiver : public QWidget
{
    Q_OBJECT

public:
    //Constructors / Destructors
    QtArchiver(QWidget* parent = nullptr);
    ~QtArchiver();

private:
    Ui::QtArchiverClass ui;
    QString inputPath;
    QString outputPath;

private slots:
    void on_openFileButton_clicked();
    void on_whereButton_clicked();
    void on_clearPathButton_clicked();
    void on_clear2PathButton_clicked();
    void on_compressButton_clicked();
    void on_decompressButton_clicked();
};