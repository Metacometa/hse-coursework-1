#include "QtArchiver.h"
#include "emptyfiledialog.h"

#include <QFileDialog.h>


QtArchiver::QtArchiver(QWidget* parent)
    : QWidget(parent) {
    //ts.setEncoding(bUtf8 ? QStringConverter::Utf8 : QStringConverter::Latin1);
    ui.setupUi(this);
    path = "<No file chosen>";
    ui.filePathLabel->setText(this->path);
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

void QtArchiver::on_clearPathButton_clicked() {
    path = "<No file chosen>";
    ui.filePathLabel->setText(this->path);
}

void QtArchiver::on_compressButton_clicked() {
    if (path == "<No file chosen>") {
        EmptyFileDialog dialog;
        dialog.exec();
    }
    else if (path == "")
        path = "<No file chosen";
    else {
        compression.Huffman(path.toStdString(), 1);
        on_clearPathButton_clicked();
    }
}

void QtArchiver::on_decompressButton_clicked() {
    if (path == "<No file chosen>") {
        EmptyFileDialog dialog;
        dialog.exec();
    }
    else if (path == "")
        path = "<No file chosen";
    else {
        compression.Huffman(path.toStdString(), 2);
    }
}