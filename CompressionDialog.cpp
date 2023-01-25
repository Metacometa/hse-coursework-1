#include "CompressionDialog.h"
#include <QMessageBox>
#include <QThread>
#include <QTimer>
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

	this->timer = new QTimer(this);
	this->timer->setInterval(1000);

	ui.progressBar->setMinimum(0);
	ui.progressBar->setValue(0);
	ui.progressBar->setMaximum(100);

	this->time = 0;

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
	QThread* compressorThread = new QThread();
	this->compressor = new Compressor(this->inputPath.toStdString(), this->outputPath.toStdString());
	this->compressor->moveToThread(compressorThread);

	connect(this->timer, SIGNAL(timeout()), this, SLOT(timer_timeOut_event_slot()));
	connect(compressor, SIGNAL(finished()), timer, SLOT(stop()));

	switch (this->algorithm) 
	{
		case HUFFMAN:  
		{
			if (this->mode == COMPRESS)
			{
				connect(compressorThread, SIGNAL(started()), compressor, SLOT(huffmanCompression()));
			}
			else
			{
				connect(compressorThread, SIGNAL(started()), compressor, SLOT(huffmanDecompression()));
			}
			break;
		}
	}

	connect(compressor, SIGNAL(finished()), compressorThread, SLOT(quit()));
	connect(compressor, SIGNAL(finished()), compressor, SLOT(deleteLater()));
	connect(compressorThread, SIGNAL(finished()), compressorThread, SLOT(deleteLater()));
	connect(compressor, SIGNAL(finished()), this, SLOT(closeWindow()));

	ui.startButton->setDisabled(true);

	//start timer ticking
	timer->start();
	
	compressorThread->start();

	connect(compressor, SIGNAL(updateProgressBar(int)), ui.progressBar, SLOT(setValue(int)));
}

void CompressionDialog::timer_timeOut_event_slot()
{
	std::string hours = "", minutes = "", seconds = "";
	//seconds calculating
	if (time % 60 < 10)
	{
		seconds = "0" + std::to_string(time % 60);
	}
	else
	{
		seconds = std::to_string(time % 60);
	}

	//minutes calculating
	if ((time / 60 % 60) < 10)
	{
		minutes = "0" + std::to_string(time / 60 % 60);
	}
	else
	{
		minutes = std::to_string(time / 60 % 60);
	}

	//hours calculating
	if (time / 3600 < 10)
	{
		hours = "0" + std::to_string(time / 3600);
	}
	else
	{
		hours = std::to_string(time / 3600);
	}

	ui.timerLabel->setText(QString::fromStdString(hours) + ":" +
	QString::fromStdString(minutes) + ":" +
	QString::fromStdString(seconds));
	time++;
}

void CompressionDialog::closeWindow()
{
	if (this->mode == COMPRESS) 
	{
		QMessageBox::information(this, "Done", "File is succesfully compressed!\nTotal time: " + ui.timerLabel->text());
	} 
	else
	{
		QMessageBox::information(this, "Done", "File is succesfully decompressed!\nTotal time: " + ui.timerLabel->text());
	}
	this->close();
}