#pragma once

#include "QtArchiver.h"

#include <QFileDialog.h>
#include <QMessageBox.h>

QString emptyPath = "<No file chosen>";

QtArchiver::QtArchiver(QWidget* parent)
    : QWidget(parent) {
    ui.setupUi(this);
    inputPath = emptyPath;
    outputPath = emptyPath;
    ui.filePathLabel->setText(this->inputPath);
    ui.filePathToLabel->setText(this->inputPath);
}

QtArchiver::~QtArchiver() {}

void QtArchiver::on_openFileButton_clicked() {
    QFileDialog loadFile(this);
    loadFile.setFileMode(QFileDialog::ExistingFile); //set picking of ONLY ONE file
    loadFile.exec();
    QStringList selectedFiles = loadFile.selectedFiles();

    if (!selectedFiles.isEmpty()) {
        inputPath = selectedFiles.at(0);
        std::u16string stdpath = inputPath.toStdU16String();
        QString temp = "";

        for (int i = 0; i < stdpath.length(); ++i) {
            if (stdpath[i] == '/')
                temp += "\\";
            else
                temp += stdpath[i];
        }

        this->inputPath = temp;

        ui.filePathLabel->setText(this->inputPath);
    }


}

void QtArchiver::on_whereButton_clicked() {
    QFileDialog loadFile(this);
    loadFile.setFileMode(QFileDialog::Directory); //set picking of ONLY ONE directory
    loadFile.exec();
    QStringList selectedFiles = loadFile.selectedFiles();

    if (!selectedFiles.isEmpty()) {
        outputPath = selectedFiles.at(0);
        std::u16string stdpath = outputPath.toStdU16String();
        QString temp = "";

        for (int i = 0; i < stdpath.length(); ++i) {
            if (stdpath[i] == '/')
                temp += "\\";
            else
                temp += stdpath[i];
        }

        this->outputPath = temp;
        ui.filePathToLabel->setText(this->outputPath);
    }
}

void QtArchiver::on_clearPathButton_clicked() {
    inputPath = emptyPath;
    ui.filePathLabel->setText(this->inputPath);
}

void QtArchiver::on_clear2PathButton_clicked() {
    outputPath = emptyPath;
    ui.filePathToLabel->setText(this->outputPath);
}

void QtArchiver::on_compressButton_clicked() {
    if (inputPath == emptyPath and outputPath == emptyPath)
        QMessageBox::warning(this, "Warning", "No file and directory chosen");
    else if (inputPath == emptyPath)
        QMessageBox::warning(this, "Warning", "No file chosen");
    else if (outputPath == emptyPath)
        QMessageBox::warning(this, "Warning", "No directory chosen");
    else {

        QString temp = "";

        for (int i = inputPath.size() - 1; i >= 0 and inputPath[i] != '.'; --i)
            temp = inputPath[i] + temp;
        temp = "." + temp;

        if (temp.toStdString() != extension) {
            CompressionDialog dialog(1, this->inputPath, this->outputPath, ui.compressionAlgorithms->currentText());
            dialog.exec();
            on_clearPathButton_clicked();
            on_clear2PathButton_clicked();
        }
        else
            QMessageBox::warning(this, "Warning", QString::fromStdString("Can't compress '" + extension + "' files"));

    }
}

void QtArchiver::on_decompressButton_clicked() {
    if (inputPath == emptyPath and outputPath == emptyPath)
        QMessageBox::warning(this, "Warning", "No file and directory chosen");
    else if (inputPath == emptyPath)
        QMessageBox::warning(this, "Warning", "No file chosen");
    else if (outputPath == emptyPath)
        QMessageBox::warning(this, "Warning", "No directory chosen");
    else {
        CompressionDialog dialog(0, this->inputPath, this->outputPath, ui.compressionAlgorithms->currentText());
        dialog.exec();

        on_clearPathButton_clicked();
        on_clear2PathButton_clicked();
    }
}