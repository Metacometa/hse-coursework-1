#pragma once

#include <QDialog>
#include "ui_CompressionDialog.h"
#include <string>
#include <QTimer>

#include "Compression.h"

class CompressionDialog : public QDialog {
	Q_OBJECT

public:
	//Constructor / Destructor
	CompressionDialog(bool mode, QString inputPath, QString outputPath, QString inputAlgorithm, 
		QWidget *parent = nullptr);
	~CompressionDialog();

	//Archiver functions
	void compress();
	void decompress();

private:
	//Resources
	Ui::CompressionDialogClass ui;
	Huffman huffman;

	//Variables
	QString algorithm;
	QString inputPath;
	QString outputPath;

	bool mode;

private slots:
	void on_startButton_clicked();
};
