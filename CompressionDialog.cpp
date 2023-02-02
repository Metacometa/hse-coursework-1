#include "CompressionDialog.h"
#include <QMessageBox>
#include <QThread>
#include <QTimer>
#include <iostream>

//Constructors / Destructors
CompressionDialog::CompressionDialog(MODES mode, QString inputFile, QString outputPath,
	ALGORITHM inputAlgorithm, QWidget* parent) : QDialog(parent) 
{
	this->ui.setupUi(this);

	this->mode = MODES(mode);
	this->algorithm = ALGORITHM(inputAlgorithm);
	this->inputFile = inputFile;
	this->outputPath = outputPath;

	this->timer = new QTimer(this);
	this->timer->setInterval(1000);

	this->ui.progressBar->setMinimum(0);
	this->ui.progressBar->setValue(0);
	this->ui.progressBar->setMaximum(100);

	this->time = 0;

	this->ui.fileLabel->setText(inputFile);
	switch (this->mode) 
	{
		case COMPRESS: 
		{
			this->ui.processLabel->setText("Compression");
			break;
		}	
		case DECOMPRESS: 
		{
			this->ui.processLabel->setText("Decompression");
			break;
		}
	}	
}

CompressionDialog::~CompressionDialog() {}

void CompressionDialog::on_startButton_clicked() 
{
	QThread* compressorThread = new QThread();
	this->compressor = new Compressor(this->inputFile.toStdWString(), this->outputPath.toStdWString());
	this->compressor->moveToThread(compressorThread);

	connect(this->timer, SIGNAL(timeout()), this, SLOT(timer_timeOut_event_slot()));
	connect(this->compressor, SIGNAL(finished()), this->timer, SLOT(stop()));

	switch (this->algorithm) 
	{
		case HUFFMAN:  
		{
			if (this->mode == COMPRESS)
			{
				connect(compressorThread, SIGNAL(started()), this->compressor, SLOT(huffmanCompression()));
			}
			else
			{
				connect(compressorThread, SIGNAL(started()), this->compressor, SLOT(huffmanDecompression()));
			}
			break;
		}
	}

	connect(this->compressor, SIGNAL(finished()), compressorThread, SLOT(quit()));
	connect(this->compressor, SIGNAL(finished()), this->compressor, SLOT(deleteLater()));
	connect(compressorThread, SIGNAL(finished()), compressorThread, SLOT(deleteLater()));
	connect(this->compressor, SIGNAL(finished()), this, SLOT(closeWindow()));

	this->ui.startButton->setDisabled(true);

	//start timer ticking
	this->timer->start();
	
	compressorThread->start();

	connect(this->compressor, SIGNAL(updateProgressBar(int)), this->ui.progressBar, SLOT(setValue(int)));
}

void CompressionDialog::timer_timeOut_event_slot()
{
	std::string hours = "", minutes = "", seconds = "";
	//seconds calculating
	if (this->time % 60 < 10)
	{
		seconds = "0" + std::to_string(this->time % 60);
	}
	else
	{
		seconds = std::to_string(time % 60);
	}

	//minutes calculating
	if ((this->time / 60 % 60) < 10)
	{
		minutes = "0" + std::to_string(this->time / 60 % 60);
	}
	else
	{
		minutes = std::to_string(this->time / 60 % 60);
	}

	//hours calculating
	if (this->time / 3600 < 10)
	{
		hours = "0" + std::to_string(this->time / 3600);
	}
	else
	{
		hours = std::to_string(this->time / 3600);
	}

	this->ui.timerLabel->setText(QString::fromStdString(hours) + ":" +
	QString::fromStdString(minutes) + ":" +
	QString::fromStdString(seconds));
	this->time++;
}

void CompressionDialog::closeWindow()
{
	if (this->mode == COMPRESS) 
	{
		QMessageBox::information(this, informationTitle, messageCompressionDone + ui.timerLabel->text());
	} 
	else
	{
		QMessageBox::information(this, informationTitle, messageDecompressionDone + ui.timerLabel->text());
	}
	this->close();
}