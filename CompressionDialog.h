#pragma once

#include <QDialog>
#include "ui_CompressionDialog.h"
#include <string>
#include <QString>
#include <QTimer>

#include "Compression.h"

enum MODES { COMPRESS, DECOMPRESS };
enum ALGORITHMS { HUFFMAN };

class CompressionDialog : public QDialog 
{
	Q_OBJECT

public:

	//Constructor / Destructor
	CompressionDialog(MODES mode, QString inputPath, QString outputPath, ALGORITHMS inputAlgorithm, 
		QWidget *parent = nullptr);
	~CompressionDialog();


private:
	//Resources
	Ui::CompressionDialogClass ui;
	Compressor *compressor;

	//Variables
	QString inputPath;
	QString outputPath;

	MODES mode;
	ALGORITHMS algorithm;

private slots:
	void on_startButton_clicked();
	void closeWindow();
};
