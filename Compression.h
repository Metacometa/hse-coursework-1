#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <QProgressBar>
#include <QDateTime>
#include <QLabel>
#include <QObject>

#include "List.h"
#include "Config.h"

class Huffman : public QObject {
	Q_OBJECT

public:
	int huffman(QProgressBar *pbar,std::string filename, int mode);
	void Compression(QProgressBar* pbar, QLabel* label, std::string filename, std::string outputname);
	void Decompression(QProgressBar* pbar, QLabel* label, std::string filename, std::string outputname);

signals:
	void updateProgressBar(int new_value);

private slots:
};
