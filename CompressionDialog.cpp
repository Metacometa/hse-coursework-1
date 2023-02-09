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

	initCompressionProperties(mode, inputAlgorithm);
	initPaths(inputFile, outputPath);
	initTimer();
	setPathLabel();

	this->time = 0;
}

CompressionDialog::~CompressionDialog() {}

//Util functions void set
void CompressionDialog::initCompressionProperties(MODES mode, ALGORITHM inputAlgorithm)
{
	this->mode = MODES(mode);
	this->algorithm = ALGORITHM(inputAlgorithm);
	setCorrespondingWindowTitle();
}

void CompressionDialog::initPaths(QString inputFile, QString outputPath)
{
	this->inputFile = inputFile;
	this->outputPath = outputPath;
}

void CompressionDialog::initTimer()
{
	this->timer = new QTimer(this);
	this->timer->setInterval(1000);
}

void CompressionDialog::setCorrespondingWindowTitle()
{
	switch (this->algorithm)
	{
	case HUFFMAN:
		if (this->mode == COMPRESS)
		{
			this->setWindowTitle("Huffman compression");
		}
		else
		{
			this->setWindowTitle("Huffman decompression");
		}
		break;
	}
}

void CompressionDialog::setPathLabel()
{
	int temp = this->inputFile.length();
	if (this->inputFile.length() <= 2 * HALF_LENGHT_FILE_PATH_LABEL)
	{
		this->ui.filePathLabel->setText(this->inputFile);
	}
	else
	{
		QString newValueOfLabel = "...";
		QString left = "", right = "";
		for (int i = 0; i < HALF_LENGHT_FILE_PATH_LABEL; ++i)
		{
			left = left + this->inputFile[i];
			right = this->inputFile[this->inputFile.length() - 1 - i] + right;
		}
		newValueOfLabel = left + newValueOfLabel + right;
		this->ui.filePathLabel->setText(newValueOfLabel);
	}
}

void CompressionDialog::setTimerConnections()
{
	connect(this->timer, SIGNAL(timeout()), this, SLOT(timer_timeOut_event_slot()));
	connect(this->compressor, SIGNAL(finished()), this->timer, SLOT(stop()));
	this->timer->start();
}

void CompressionDialog::setAlgorithmConnection(QThread* thread)
{
	switch (this->algorithm)
	{
	case HUFFMAN:
	{
		if (this->mode == COMPRESS)
		{
			connect(thread, SIGNAL(started()), this->compressor, SLOT(huffmanCompression()));
		}
		else
		{
			connect(thread, SIGNAL(started()), this->compressor, SLOT(huffmanDecompression()));
		}
		break;
	}
	}
}

void CompressionDialog::setFinishedConnections(QThread* thread)
{
	connect(this->compressor, SIGNAL(finished()), thread, SLOT(quit()));
	connect(this->compressor, SIGNAL(finished()), this->compressor, SLOT(deleteLater()));
	connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
	connect(this->compressor, SIGNAL(finished()), this, SLOT(closeWindow()));
}

void CompressionDialog::createCompressorThread()
{
	QThread* compressorThread = new QThread();
	this->compressor = new Compressor(this->inputFile.toStdWString(), this->outputPath.toStdWString());
	this->compressor->moveToThread(compressorThread);
	setTimerConnections();
	setAlgorithmConnection(compressorThread);
	setFinishedConnections(compressorThread);
	connect(this->compressor, SIGNAL(updateProgressBar(int)), this->ui.progressBar, SLOT(setValue(int)));

	compressorThread->start();
}

//private slots
void CompressionDialog::on_startButton_clicked() 
{
	this->ui.startButton->setDisabled(true);
	createCompressorThread();
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