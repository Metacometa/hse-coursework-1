#pragma once

#include "QtArchiver.h"

#include <QFileDialog.h>
#include <QMessageBox.h>
#include "CompressionDialog.h"

//Constructors / Destructors
QtArchiver::QtArchiver(QWidget* parent)
    : QWidget(parent) {
    this->ui.setupUi(this);
}

QtArchiver::~QtArchiver() {}

//private
//utils
QString QtArchiver::replaceSymbols(const QString& path, const char& replaced, const char& replacedBy)
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
        return huffmanExtension;
        break;
    }
    return L"";
}

bool QtArchiver::isFileAndPathCorrect()
{
    /*
        @isFileAndPathcorrect check, if user enter path and file or not
    */
    if (!QFile::exists(this->ui.fileLine->text()) and !QFile::exists(this->ui.pathLine->text()))
    {
        QMessageBox::warning(this, warningTitle, messageWrongFileAndPath);
        return false;
    }
    else if (!QFile::exists(this->ui.fileLine->text()))
    {
        QMessageBox::warning(this, warningTitle, messageWrongFile);
        return false;
    }
    else if (!QFile::exists(this->ui.pathLine->text()))
    {
        QMessageBox::warning(this, warningTitle, messageWrongPath);
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
    QString fileDir = this->ui.fileLine->text();
    for (qsizetype i = fileDir.length() - 1; i >= 0 and fileDir[i] != '.'; --i)
    {
        inputExtension = fileDir[i] + inputExtension;
    }
    inputExtension = "." + inputExtension;
    return inputExtension;
}

//private slots
void QtArchiver::on_editFileButton_clicked()
{
    QFileDialog loadFile(this);
    loadFile.setFileMode(QFileDialog::ExistingFile); //set picking of only ONE file
    loadFile.exec();
    QStringList selectedFiles = loadFile.selectedFiles();

    if (!selectedFiles.isEmpty())
    {
        this->ui.fileLine->setText(replaceSymbols(selectedFiles.at(0), '/', '\\'));
    }
}

void QtArchiver::on_editPathButton_clicked() {
    QFileDialog loadFile(this);
    loadFile.setFileMode(QFileDialog::Directory); //set picking of only ONE directory
    loadFile.exec();
    QStringList selectedFiles = loadFile.selectedFiles();

    if (!selectedFiles.isEmpty())
    {
        this->ui.pathLine->setText(replaceSymbols(selectedFiles.at(0), '/', '\\'));
    }
}

void QtArchiver::on_compressButton_clicked()
{
    ALGORITHM algorithm = defineMode();

    if (isFileAndPathCorrect()) //If path and file chosen successfully 
    {
        CompressionDialog dialog(COMPRESS, this->ui.fileLine->text(), this->ui.pathLine->text(), algorithm);
        dialog.exec();
    }
}

void QtArchiver::on_decompressButton_clicked()
{
    /*
        @Decompressor can decompress only appropriate files
        For example, Huffman can decompress only '.huffman' files
    */
    ALGORITHM algorithm = defineMode();
    std::wstring appropriateExtension = getAppropriateExtension(algorithm);

    if (isFileAndPathCorrect()) //If path and file chosen successfully 
    {
        QString inputFileExtension = getInputFileExtension();
        if (inputFileExtension.toStdWString() == appropriateExtension) //check, if it is appropriate file extension
        {
            //load compressor dialog with decompress params
            CompressionDialog dialog(DECOMPRESS, this->ui.fileLine->text(), this->ui.pathLine->text(), algorithm);
            dialog.exec();
        }
        else
        {
            QMessageBox::warning(this, warningTitle, "Can't decompress '" + inputFileExtension + "' by " +
                this->ui.compressionAlgorithms->currentText() + " algorithm.");
        }
    }
}
