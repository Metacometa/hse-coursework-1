#include "CompressionDialog.h"

//Constructors / Destructors
CompressionDialog::CompressionDialog(const MODE &mode, const QString& inputPath, const QString& outputPath,
	const ALGORITHM &algorithm, QWidget* parent) : QDialog(parent) 
{
	switch (algorithm) {
		case HUFFMAN:
			this->compression = new Huffman(inputPath.toStdWString(), outputPath.toStdWString());
			break;
		case LZW:
			this->compression = new Lzw(inputPath.toStdWString(), outputPath.toStdWString());
			break;
	}

	if (mode == COMPRESS) {
		closeMessage = &compressionDoneMessage;
	}
	else if (mode == DECOMPRESS) {
		closeMessage = &decompressionDoneMessage;
	}

	this->ui.setupUi(this);

	changeWindowTitle(mode, algorithm);
	initTimers();
	setPathLabel(inputPath);
	createCompressionThread(mode);

	this->time = 0;
}

CompressionDialog::~CompressionDialog() {}

void CompressionDialog::initTimers()
{
	this->timer = new QTimer(this);
	this->timer->setInterval(1000);

	this->progressBarUpdatingTimer = new QTimer(this);
	this->progressBarUpdatingTimer->setInterval(160);
}

void CompressionDialog::changeWindowTitle(const MODE&mode, const ALGORITHM&algorithm)
{
	switch (algorithm)
	{
	case HUFFMAN:
		if (mode == COMPRESS)
		{
			this->setWindowTitle(huffmanTitle + " " + compressionTitle);
		}
		else
		{
			this->setWindowTitle(huffmanTitle + " " + decompressionTitle);
		}
		break;
	case LZW:
		if (mode == COMPRESS)
		{
			this->setWindowTitle(lzwTitle + " " + compressionTitle);
		}
		else
		{
			this->setWindowTitle(lzwTitle + " " + decompressionTitle);
		}
	}

}

void CompressionDialog::setPathLabel(const QString &inputPath)
{
	int temp = inputPath.length();
	if (inputPath.length() <= 2 * HALF_LENGHT_FILE_PATH_LABEL)
	{
		this->ui.filePathLabel->setText(inputPath);
	}
	else
	{
		QString newValueOfLabel = "...";
		QString left = "", right = "";
		for (int i = 0; i < HALF_LENGHT_FILE_PATH_LABEL; ++i)
		{
			left = left + inputPath[i];
			right = inputPath[inputPath.length() - 1 - i] + right;
		}
		newValueOfLabel = left + newValueOfLabel + right;
		this->ui.filePathLabel->setText(newValueOfLabel);
	}
}

void CompressionDialog::setTimerConnections()
{
	connect(this->timer, SIGNAL(timeout()), this, SLOT(timer_timeOut_event_slot()));
	connect(this->compression, SIGNAL(finished()), this->timer, SLOT(stop()));
}

void CompressionDialog::setAlgorithmConnection(QThread* thread, const MODE& mode)
{
	if (mode == COMPRESS)
	{
		connect(thread, SIGNAL(started()), this->compression, SLOT(encodeSlot()));
	}
	else
	{
		connect(thread, SIGNAL(started()), this->compression, SLOT(decodeSlot()));
	}
}

void CompressionDialog::setFinishedConnections(QThread* thread)
{
	connect(this->compression, SIGNAL(finished()), thread, SLOT(quit()));
	connect(this->compression, SIGNAL(finished()), this->compression, SLOT(deleteLater()));
	connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
	connect(this->compression, SIGNAL(finished()), this, SLOT(closeWindow()));
}

void CompressionDialog::createCompressionThread(const MODE& mode)
{
	QThread* compressionThread = new QThread();

	this->compression->moveToThread(compressionThread);
	setTimerConnections();
	setAlgorithmConnection(compressionThread, mode);
	setFinishedConnections(compressionThread);
	connect(this->compression, SIGNAL(updateProgressBar(int)), this->ui.progressBar, SLOT(setValue(int)));
	connect(this, SIGNAL(pauseIsClicked()), this->compression, SLOT(reverseIsPaused()), Qt::DirectConnection);
	connect(this->progressBarUpdatingTimer, SIGNAL(timeout()), this->compression, SLOT(reverseCanBeUpdated()), Qt::DirectConnection);
	
	this->timer->start();
	this->progressBarUpdatingTimer->start();

	compressionThread->start();
}

//private slots
void CompressionDialog::on_pauseButton_clicked()
{
	if (this->timer->isActive())
	{
		this->ui.pauseButton->setText(pauseButtonPausedText);
		this->timer->stop();
	}
	else
	{
		this->ui.pauseButton->setText(pauseButtonUnpausedText);
		this->timer->start();
	}

	emit pauseIsClicked();
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
	QMessageBox::information(this, informationTitle, *closeMessage + ui.timerLabel->text());
	this->close();
}