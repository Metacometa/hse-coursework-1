#include "QtArchiver.h"
#include "CompressionDialog.h"

#include <QFileDialog.h>
#include <QMessageBox.h>

QtArchiver::QtArchiver(QWidget* parent)
    : QWidget(parent) {
    ui.setupUi(this);
    path = "<No file chosen>";
    pathWhere = "<No file chosen";
    ui.filePathLabel->setText(this->path);
    ui.filePathToLabel->setText(this->path);
}

QtArchiver::~QtArchiver() {}

void QtArchiver::on_openFileButton_clicked() {
    QFileDialog loadFile(this);
    loadFile.setFileMode(QFileDialog::ExistingFile); //set picking of ONLY ONE file
    loadFile.exec();
    QStringList selectedFiles = loadFile.selectedFiles();

    if (!selectedFiles.isEmpty()) {
        path = selectedFiles.at(0);
        std::u16string stdpath = path.toStdU16String();
        QString temp = "";

        for (int i = 0; i < stdpath.length(); ++i) {
            if (stdpath[i] == '/')
                temp += "\\";
            else
                temp += stdpath[i];
        }

        this->path = temp;
        ui.filePathLabel->setText(this->path);
    }
}

void QtArchiver::on_whereButton_clicked() {
    QFileDialog loadFile(this);
    loadFile.setFileMode(QFileDialog::Directory); //set picking of ONLY ONE file
    loadFile.exec();
    QStringList selectedFiles = loadFile.selectedFiles();

    if (!selectedFiles.isEmpty()) {
        pathWhere = selectedFiles.at(0);
        std::u16string stdpath = pathWhere.toStdU16String();
        QString temp = "";

        for (int i = 0; i < stdpath.length(); ++i) {
            if (stdpath[i] == '/')
                temp += "\\";
            else
                temp += stdpath[i];
        }

        this->pathWhere = temp;
        ui.filePathToLabel->setText(this->pathWhere);
    }
}

void QtArchiver::on_clearPathButton_clicked() {
    path = "<No file chosen>";
    ui.filePathLabel->setText(this->path);
}

void QtArchiver::on_clear2PathButton_clicked() {
    pathWhere = "<No file chosen>";
    ui.filePathToLabel->setText(this->pathWhere);
}

void QtArchiver::on_copyPathFromToButton_clicked() {
    pathWhere = path;
    ui.filePathToLabel->setText(this->pathWhere);
}

void QtArchiver::on_compressButton_clicked() {
    if (path == "<No file chosen>") {
        QMessageBox::warning(this, "Warning", "No file chosen");
    }
    else {
        CompressionDialog dialog(1, this->path, this->pathWhere, ui.compressionAlgorithms->currentText());
        dialog.exec();
        on_clearPathButton_clicked();
        on_clear2PathButton_clicked();
    }
}

void QtArchiver::on_decompressButton_clicked() {
    if (path == "<No file chosen>") {
        QMessageBox::warning(this, "Warning", "No file chosen");
    }
    else {
        CompressionDialog dialog(0, this->path, this->pathWhere, ui.compressionAlgorithms->currentText());
        dialog.exec();

        on_clearPathButton_clicked();
    }
}