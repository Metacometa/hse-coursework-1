#pragma once

#include <QDialog>
#include "ui_CompressionDialog.h"
#include <string>

#include "Compression.h"

class CompressionDialog : public QDialog
{
	Q_OBJECT

public:
	CompressionDialog(bool mode, QString inputPath, QString outputPath, QString compressionAlgorithm, QWidget *parent = nullptr);
	~CompressionDialog();

	//Archiver functions
	void compress();
	void decompress();
private:
	Ui::CompressionDialogClass ui;
	Huffman huffman;
	QString algorithm;
	QString path;
	QString pathWhere;
	bool mode;
	bool isCompressing = false;
	int MAX_VALUE = 100;
	int progress;

	void updateBar();
private slots:
	void on_startButton_clicked();
};
