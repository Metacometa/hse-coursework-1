#include "CompressionDialog.h"
#include <QMessageBox>
#include <QThread>
#include <iostream>

//Constructors / Destructors
CompressionDialog::CompressionDialog(MODES mode, QString inputPath, QString outputPath,
	ALGORITHMS inputAlgorithm, QWidget* parent) : QDialog(parent) 
{
	ui.setupUi(this);

	this->mode = MODES(mode);
	this->algorithm = ALGORITHMS(inputAlgorithm);
	this->inputPath = inputPath;
	this->outputPath = outputPath;

	ui.progressBar->setMinimum(0);
	ui.progressBar->setValue(0);
	ui.progressBar->setMaximum(100);

	ui.fileLabel->setText(inputPath);
	switch (this->mode) 
	{
		case COMPRESS: 
		{
			ui.processLabel->setText("Compression");
			break;
		}	
		case DECOMPRESS: 
		{
			ui.processLabel->setText("Decompression");
			break;
		}
	}
}

CompressionDialog::~CompressionDialog() {}

void CompressionDialog::on_startButton_clicked() 
{
	QThread* thread = new QThread();
	this->compressor = new Compressor(this->inputPath.toStdString(), this->outputPath.toStdString());
	this->compressor->moveToThread(thread);
	switch (this->algorithm) 
	{
		case HUFFMAN:  
		{
			if (this->mode == COMPRESS) connect(thread, SIGNAL(started()), compressor, SLOT(huffmanCompression()));
			else connect(thread, SIGNAL(started()), compressor, SLOT(huffmanDecompression()));
			break;
		}
	}

	connect(compressor, SIGNAL(finished()), thread, SLOT(quit()));
	connect(compressor, SIGNAL(finished()), compressor, SLOT(deleteLater()));
	connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
	connect(ui.startButton, SIGNAL(clicked()), compressor, SLOT(stop()));
	connect(compressor, SIGNAL(finished()), this, SLOT(closeWindow()));

	ui.startButton->setDisabled(true);

	thread->start();

	connect(compressor, SIGNAL(updateProgressBar(int)), ui.progressBar, SLOT(setValue(int)));
}

void CompressionDialog::closeWindow() 
{
	if (this->mode == COMPRESS) 
	{
		QMessageBox::information(this, "Done", "File is succesfully compressed!");
	} 
	else
	{
		QMessageBox::information(this, "Done", "File is succesfully decompressed!");
	}
	this->close();
}