#include "QtArchiver.h"

//Constructors / Destructors
QtArchiver::QtArchiver(QWidget* parent)
    : QWidget(parent) {
    this->ui.setupUi(this);
}

QtArchiver::~QtArchiver() {}

//private
//utils
ALGORITHM QtArchiver::defineMode()
{
    if (this->ui.compressionAlgorithms->currentText() == "Huffman")
    {
        return ALGORITHM::HUFFMAN;
    }
    else if (this->ui.compressionAlgorithms->currentText() == "LZW")
    {
        return ALGORITHM::LZW;
    }
    else if (this->ui.compressionAlgorithms->currentText() == "Arithmetic") {
        return ALGORITHM::ARITHMETIC;
    }
    return ALGORITHM();
}

std::wstring QtArchiver::getExtension(const ALGORITHM &algorithm)
{
    switch (algorithm)
    {
    case HUFFMAN:
        return huffmanExtension;
    case LZW:
        return lzwExtension;
    case ARITHMETIC:
        return arithmeticExtension;
    default:
        return L"";
    }
}

bool QtArchiver::isFileAndPathCorrect()
{
    /*
        @isFileAndPathcorrect check, if user enter path and file or not
    */
    if (!QFile::exists(this->ui.fileLine->text()) and !QFile::exists(this->ui.pathLine->text()))
    {
        QMessageBox::warning(this, warningTitle, wrongFileAndPathMessage);
        return false;
    }
    else if (!QFile::exists(this->ui.fileLine->text()))
    {
        QMessageBox::warning(this, warningTitle, wrongFileMessage);
        return false;
    }
    else if (!QFile::exists(this->ui.pathLine->text()))
    {
        QMessageBox::warning(this, warningTitle, wrongPathMessage);
        return false;
    }
    QFile testOfEmptiness(ui.fileLine->text());
    testOfEmptiness.open(QIODevice::ReadOnly);

    bool isEmpty = testOfEmptiness.size();
    if (!isEmpty)
    {
        QMessageBox::warning(this, warningTitle, emptyFileMessage);
    }
    testOfEmptiness.close();
    return isEmpty;
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

void QtArchiver::loadPath(QWidget *parent, QLineEdit *line, QFileDialog::FileMode mode)
{
    QFileDialog loadFile(parent);
    loadFile.setFileMode(mode);
    loadFile.exec();
    QStringList selectedFiles = loadFile.selectedFiles();

    if (!selectedFiles.isEmpty())
    {
        QString path = selectedFiles.at(0);
        line->setText(path.replace("/", "\\"));
    }
}

//private slots
void QtArchiver::on_editFileButton_clicked()
{
    loadPath(this, this->ui.fileLine, QFileDialog::ExistingFile);
}

void QtArchiver::on_editPathButton_clicked() 
{
    loadPath(this, this->ui.pathLine, QFileDialog::Directory);
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
    std::wstring appropriateExtension = getExtension(algorithm);

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
