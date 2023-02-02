#pragma once

#include "QtArchiver.h"

#include <QFileDialog.h>
#include <QMessageBox.h>
#include "CompressionDialog.h"

QString emptyPath = "<No file chosen>";

QtArchiver::QtArchiver(QWidget* parent)
    : QWidget(parent) {
    ui.setupUi(this);
    inputPath = emptyPath;
    outputPath = emptyPath;
    ui.fileNameLabel->setText(this->inputPath);
    ui.pathNameLabel->setText(this->inputPath);
}

QtArchiver::~QtArchiver() {}

void QtArchiver::on_editFileButton_clicked() {
    QFileDialog loadFile(this);
    loadFile.setFileMode(QFileDialog::ExistingFile); //set picking of ONLY ONE file
    loadFile.exec();
    QStringList selectedFiles = loadFile.selectedFiles();

    if (!selectedFiles.isEmpty()) {
        inputPath = selectedFiles.at(0);
        std::wstring stdpath = inputPath.toStdWString();
        QString temp = "";

        for (int i = 0; i < stdpath.length(); ++i) {
            if (stdpath[i] == '/') temp += "\\";
            else temp += stdpath[i];
        }

        this->inputPath = temp;

        ui.fileNameLabel->setText(this->inputPath);
    }
}

void QtArchiver::on_editPathButton_clicked() {
    QFileDialog loadFile(this);
    loadFile.setFileMode(QFileDialog::Directory); //set picking of ONLY ONE directory
    loadFile.exec();
    QStringList selectedFiles = loadFile.selectedFiles();

    if (!selectedFiles.isEmpty()) {
        outputPath = selectedFiles.at(0);
        std::u16string stdpath = outputPath.toStdU16String();
        QString temp = "";

        for (int i = 0; i < stdpath.length(); ++i) {
            if (stdpath[i] == '/') temp += "\\";
            else temp += stdpath[i];
        }

        this->outputPath = temp;
        ui.pathNameLabel->setText(this->outputPath);
    }
}

void QtArchiver::clearPath(QLabel* label, QString& path) 
{
    path = emptyPath;
    label->setText(path);
}

void QtArchiver::on_compressButton_clicked() 
{
    ALGORITHMS mode;
    if (ui.compressionAlgorithms->currentText() == "Huffman")
    {
        mode = HUFFMAN;
    }

    if (inputPath == emptyPath and outputPath == emptyPath)
    {
        QMessageBox::warning(this, "Warning", "No file and directory chosen");
    }
    else if (inputPath == emptyPath)
    {
        QMessageBox::warning(this, "Warning", "No file chosen");
    }
    else if (outputPath == emptyPath)
    {
        QMessageBox::warning(this, "Warning", "No directory chosen");
    }
    else //If paths chosen successfully 
    {
        QString inputExtension = "";
        for (int i = inputPath.size() - 1; i >= 0 and inputPath[i] != '.'; --i) inputExtension = inputPath[i] + inputExtension;
        inputExtension = "." + inputExtension;

        if (inputExtension.toStdWString() != huffmanExtension) 
        {
            CompressionDialog dialog(COMPRESS, this->inputPath, this->outputPath, mode);
            dialog.exec();
            clearPath(ui.fileNameLabel, inputPath);
            clearPath(ui.pathNameLabel, inputPath);
        }
        else
        {
            QMessageBox::warning(this, "Warning", QString::fromStdWString(L"Can't compress '" + huffmanExtension + L"' files"));
        }
    }
}

void QtArchiver::on_decompressButton_clicked() 
{
    ALGORITHMS mode;
    if (ui.compressionAlgorithms->currentText() == "Huffman")
    {
        mode = HUFFMAN;
    }

    if (inputPath == emptyPath and outputPath == emptyPath)
    {
        QMessageBox::warning(this, "Warning", "No file and directory chosen");
    }
    else if (inputPath == emptyPath)
    {
        QMessageBox::warning(this, "Warning", "No file chosen");
    }
    else if (outputPath == emptyPath)
    {
        QMessageBox::warning(this, "Warning", "No directory chosen");
    }
    else
    {
        CompressionDialog dialog(DECOMPRESS, this->inputPath, this->outputPath, mode);
        dialog.exec();
        clearPath(ui.fileNameLabel, inputPath);
        clearPath(ui.pathNameLabel, inputPath);
    }
}
