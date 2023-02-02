#pragma once

#include <QtWidgets/QWidget>
#include "ui_QtArchiver.h"

class QtArchiver : public QWidget
{
    Q_OBJECT

public:
    //Constructors / Destructors
    QtArchiver(QWidget* parent = nullptr);
    ~QtArchiver();

private:
    Ui::QtArchiverClass ui;

    QString inputFile;
    QString outputPath;

    //Utils
    void clearPath(QLabel* label, QString& path);
    QString replaceSymbols(const QString& path, const char& replaced, const char& replacedBy);
    enum ALGORITHM defineMode();
    bool isFileAndPathCorrect();
    QString getInputFileExtension();
    std::wstring getAppropriateExtension(ALGORITHM algorithm);

private slots:
    void on_editFileButton_clicked();
    void on_editPathButton_clicked();

    void on_compressButton_clicked();
    void on_decompressButton_clicked();
};