#pragma once

#include <QObject>

#include "List.h"
#include "Config.h"

class Compressor : public QObject 
{
	Q_OBJECT

public:
	bool isPaused;
	bool canBeUpdated;
	long long counter = 0;
	std::wstring filename;
	std::wstring outputname;
	Compressor(std::wstring filename, std::wstring outputname);

signals:
	void updateProgressBar(int value);
	void finished();

private slots:
	void huffmanCompression();
	void huffmanDecompression();
	void reverseIsPaused();
	void reverseCanBeUpdated();

};
