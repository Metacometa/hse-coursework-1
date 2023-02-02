#pragma once

#include <QDialog>
#include "ui_CompressionDialog.h"
#include <string>
#include <QString>
#include <QTimer>

#include "Compression.h"

enum MODES { COMPRESS, DECOMPRESS };
enum ALGORITHM { HUFFMAN };

class CompressionDialog : public QDialog 
{
	Q_OBJECT

public:

	//Constructor / Destructor
	CompressionDialog(MODES mode, QString inputFile, QString outputPath, ALGORITHM inputAlgorithm, 
		QWidget *parent = nullptr);
	~CompressionDialog();


private:
	//Resources
	Ui::CompressionDialogClass ui;
	Compressor *compressor;

	//Variables
	QString inputFile;
	QString outputPath;
	QTimer* timer;
	short time;

	MODES mode;
	ALGORITHM algorithm;

private slots:
	void on_startButton_clicked();
	void timer_timeOut_event_slot();
	void closeWindow();
};
