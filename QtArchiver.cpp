
#pragma once

#include "QtArchiver.h"

#include <QFileDialog.h>
#include <QMessageBox.h>
#include "CompressionDialog.h"

//Constructors / Destructors
QtArchiver::QtArchiver(QWidget* parent)
    : QWidget(parent) {
    this->ui.setupUi(this);
    this->inputFile = emptyFile;
    this->outputPath = emptyPath;
    this->ui.fileNameLabel->setText(this->inputFile);
    this->ui.pathNameLabel->setText(this->outputPath);
}

QtArchiver::~QtArchiver() {}

//private
//utils
void QtArchiver::clearPath(QLabel* label, QString& path)
{
    path = emptyPath;
    label->setText(path);
}

QString QtArchiver::replaceSymbols(const QString &path, const char &replaced, const char &replacedBy) 
{
    /*
        @replaceSymbols replace all chars in string on chosen char
    */
    QString temp = "";

    for (auto& i : path) 
    {
        if (i == replaced)
        {
            temp += replacedBy;
        }
        else
        {
            temp += i;
        }
    }

    return temp;
}

ALGORITHM QtArchiver::defineMode()
{
    if (this->ui.compressionAlgorithms->currentText() == "Huffman")
    {
        return ALGORITHM::HUFFMAN;
    }
    return ALGORITHM();
}

std::wstring QtArchiver::getAppropriateExtension(ALGORITHM algorithm)
{
    switch (algorithm)
    {
        case HUFFMAN:
        {
            return huffmanExtension;
            break;
        }
    }
    return L"";
}

bool QtArchiver::isFileAndPathCorrect()
{
    /*
        @isFileAndPathcorrect check, if user enter path and file or not
    */
    if (this->inputFile == emptyFile and this->outputPath == emptyPath)
    {
        QMessageBox::warning(this, warningTitle, messageNoFileAndPath);
        return false;
    }
    else if (this->inputFile == emptyFile)
    {
        QMessageBox::warning(this, warningTitle, messageNoFile);
        return false;
    }
    else if (this->outputPath == emptyPath)
    {
        QMessageBox::warning(this, warningTitle, messageNoPath);
        return false;
    }
    else 
    {
        return true;
    }
}

QString QtArchiver::getInputFileExtension()
{
    QString inputExtension = "";
    for (qsizetype i = this->inputFile.length() - 1; i >= 0 and this->inputFile[i] != '.'; --i)
    {
        inputExtension = inputFile[i] + inputExtension;
    }
    inputExtension = "." + inputExtension;
    return inputExtension;
}

//private slots
void QtArchiver::on_editFileButton_clicked() 
{
    QFileDialog loadFile(this);
    loadFile.setFileMode(QFileDialog::ExistingFile); //set picking of ONLY ONE file
    loadFile.exec();
    QStringList selectedFiles = loadFile.selectedFiles();

    if (!selectedFiles.isEmpty()) 
    {
        this->inputFile = replaceSymbols(selectedFiles.at(0), '/', '\\');
        this->ui.fileNameLabel->setText(this->inputFile);
    }
}

void QtArchiver::on_editPathButton_clicked() {
    QFileDialog loadFile(this);
    loadFile.setFileMode(QFileDialog::Directory); //set picking of ONLY ONE directory
    loadFile.exec();
    QStringList selectedFiles = loadFile.selectedFiles();

    if (!selectedFiles.isEmpty()) 
    {
        this->outputPath = replaceSymbols(selectedFiles.at(0), '/', '\\');
        this->ui.pathNameLabel->setText(this->outputPath);
    }
}

void QtArchiver::on_compressButton_clicked() 
{
    ALGORITHM algorithm = defineMode();

    if (isFileAndPathCorrect()) //If path and file chosen successfully 
    {
        CompressionDialog dialog(COMPRESS, this->inputFile, this->outputPath, algorithm);
        dialog.exec();
    }
}

void QtArchiver::on_decompressButton_clicked() //If path and file chosen successfully 
{
    /*
        @Decompressor can decompress only appropriate files
        For example, Huffman can decompress only '.huffman' files
    */
    ALGORITHM algorithm = defineMode();
    std::wstring appropriateExtension = getAppropriateExtension(algorithm);

    if (isFileAndPathCorrect())
    {
        QString inputFileExtension = getInputFileExtension();
        if (inputFileExtension.toStdWString() == appropriateExtension) //check, if it is appropriate file extension
        {
            //load compressor dialog with decompress params
            CompressionDialog dialog(DECOMPRESS, this->inputFile, this->outputPath, algorithm);
            dialog.exec();
        }
        else
        {
            QMessageBox::warning(this, warningTitle, "Can't decompress '" + inputFileExtension + "' by " + ui.compressionAlgorithms->currentText() +
                " algorithm.");
        }
    }
}
