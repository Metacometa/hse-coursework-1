#pragma once

#include <QObject>

#include "List.h"
#include "Config.h"
#include <vector>
#include <unordered_map>
#include <fstream>
#include <cstddef>
#include <algorithm>

typedef std::basic_string<unsigned char> ustring;

class Compressor : public QObject 
{
	Q_OBJECT

private:
	bool isPaused;
	bool canBeUpdated;
	long long counter = 0;
	std::wstring inputPath;
	std::wstring outputPath;

	//lzw utilities
	void readFileToString(std::ifstream& file, ustring& s);
	void writeNumbers(std::ofstream& file, const int& one, const int& two);
	void getCodesFromBytes(std::vector<int>& codes, const std::vector<unsigned char>& bytes);
	void decoding(std::ofstream& file, const std::vector<int>& op);
	std::wstring parseExtension(const std::wstring& filePath);
	std::wstring parseFileName(const std::wstring& filePath);

public:
	Compressor(std::wstring inputPath, std::wstring outputPath);

signals:
	void updateProgressBar(int value);
	void finished();

private slots:
	void huffmanCompression();
	void huffmanDecompression();
	void reverseIsPaused();
	void reverseCanBeUpdated();
	void lzwCompression();
	void lzwDecompression();
};
