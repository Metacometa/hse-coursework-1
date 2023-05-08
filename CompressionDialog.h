#pragma once

#include "ui_CompressionDialog.h"
#include <QDialog>
#include <QMessageBox>
#include <QString>
#include <QTimer>
#include <QThread>

#include <string>
#include <iostream>

#include "Compression.h"
#include "Config.h"
#include "Huffman.h"
#include "LZW.h"

enum MODE { COMPRESS, DECOMPRESS };
enum ALGORITHM { HUFFMAN, LZW };

class CompressionDialog : public QDialog 
{
	Q_OBJECT

public:
	//Constructor / Destructor
	CompressionDialog(const MODE& mode, const QString &inputPath, const QString &outputPath, const ALGORITHM &algorithm,
		QWidget *parent = nullptr);
	~CompressionDialog();

private:
	//Resources
	Ui::CompressionDialogClass ui;
	Compression *compression;

	//Variables
	QTimer* timer;
	QTimer* progressBarUpdatingTimer;
	short time;

	const QString* closeMessage;

	//Util functions void set
	void initTimers();
	void changeWindowTitle(const MODE& mode, const ALGORITHM& algorithm);
	void setPathLabel(const QString &inputPath); 
	void setTimerConnections();
	void setAlgorithmConnection(QThread* thread, const MODE& mode);
	void setFinishedConnections(QThread* thread);
	void createCompressionThread(const MODE& mode);

signals:
	void pauseIsClicked();

private slots:
	void on_pauseButton_clicked();
	void timer_timeOut_event_slot();
	void closeWindow();
};
