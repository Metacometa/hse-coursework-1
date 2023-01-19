#include "CompressionDialog.h"
#include <QMessageBox>

CompressionDialog::CompressionDialog(bool mode, QString inputPath, QString outputPath, QString chosenAlgorithm, QWidget* parent)
	: path(inputPath), pathWhere(outputPath), algorithm(chosenAlgorithm), QDialog(parent) {
	ui.setupUi(this);

	progress = 0;
	ui.progressBar->setValue(0);
	ui.progressBar->setMaximum(100000);
	ui.progressBar->setMinimum(0);

	this->mode = mode;
}

CompressionDialog::~CompressionDialog() {}

//Archiver functions
void CompressionDialog::compress() {
	if (this->algorithm == "Huffman") {
		//compression.Huffman(ui.progressBar, path.toStdString(), 1);
		huffman.Compression(ui.progressBar, path.toStdString(), pathWhere.toStdString());
	}
	QMessageBox::information(this, "Done", "File is succesfully compressed!");
	this->close();
}

void CompressionDialog::decompress() {
	if (this->algorithm == "Huffman") {
		//compression.Huffman(ui.progressBar, path.toStdString(), 2);
		huffman.Decompression(ui.progressBar, path.toStdString(), pathWhere.toStdString());
	}
	QMessageBox::information(this, "Done", "File is succesfully compressed!");
	this->close();
}

void CompressionDialog::updateBar() {
	progress++;
	if (progress >= 100)
		progress = 0;
	ui.progressBar->setValue(progress);
}

void CompressionDialog::on_startButton_clicked() {
	if (mode)
		compress();
	else
		decompress();
}

