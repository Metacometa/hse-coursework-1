#pragma once

#include <QObject>

#include "List.h"
#include "Config.h"

class Compressor : public QObject 
{
	Q_OBJECT

public:
	std::string filename;
	std::string outputname;
	Compressor(std::string filename, std::string outputname);



signals:
	void updateProgressBar(int value);
	void finished();
private slots:
	void huffmanCompression();
	void huffmanDecompression();
};
