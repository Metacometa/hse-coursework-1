#include "CompressionDialog.h"

CompressionDialog::CompressionDialog(bool mode, QString inputPath, QString chosenAlgorithm, QWidget* parent)
	: path(inputPath), algorithm(chosenAlgorithm), QDialog(parent) {
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
		compression.Huffman(ui.progressBar, path.toStdString(), 1);
	}
	//this->close();
}

void CompressionDialog::decompress() {
	if (this->algorithm == "Huffman") {
		//compression.Huffman(path.toStdString(), 2);
	}
	//this->close();
}

void CompressionDialog::updateBar() {
	progress++;
	if (progress >= 100)
		progress = 0;
	ui.progressBar->setValue(progress);
}

void CompressionDialog::on_startButton_clicked() {
	if (!isCompressing) {
		if (mode)
			compress();
		else
			decompress();
		isCompressing = !isCompressing;
	}
	else
		updateBar();
}

