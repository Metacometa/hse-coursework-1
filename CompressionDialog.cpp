#include "CompressionDialog.h"
#include <QMessageBox>

//Constructors / Destructors
CompressionDialog::CompressionDialog(bool mode, QString inputPath, QString outputPath, 
	QString inputAlgorithm, QWidget* parent)
	: QDialog(parent) {
	ui.setupUi(this);

	if (mode)
		ui.processLabel->setText("Compression");
	else
		ui.processLabel->setText("Decompression");

	this->inputPath = inputPath;
	this->outputPath = outputPath;
	this->algorithm = inputAlgorithm;
	this->mode = mode;


	ui.progressBar->setMinimum(0);
	ui.progressBar->setValue(0);
	ui.progressBar->setMaximum(100);
}

CompressionDialog::~CompressionDialog() {}

//Archiver functions
void CompressionDialog::compress() {
	if (this->algorithm == "Huffman") {
		huffman.Compression(ui.progressBar, inputPath.toStdString(), outputPath.toStdString());
		QMessageBox::information(this, "Done", "File is succesfully compressed!");
	}

	this->close();
}

void CompressionDialog::decompress() {
	if (this->algorithm == "Huffman") {
		huffman.Decompression(ui.progressBar, inputPath.toStdString(), outputPath.toStdString());
		QMessageBox::information(this, "Done", "File is succesfully decompressed!");
	}

	this->close();
}

void CompressionDialog::on_startButton_clicked() {
	if (mode)
		compress();
	else
		decompress();
}

