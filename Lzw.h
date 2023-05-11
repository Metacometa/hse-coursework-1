#pragma once
#include "Compression.h"

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>

typedef std::basic_string<unsigned char> ustring;

class Lzw : public Compression
{
	Q_OBJECT

private:
	void readFileToString(std::ifstream& file, ustring& s);
	void writeNumbers(std::ofstream& file, const int& one, const int& two);
	void getCodesFromBytes(std::vector<int>& codes, const std::vector<unsigned char>& bytes);
	void decoding(std::ofstream& file, const std::vector<int>& op);
	std::wstring parseExtension(const std::wstring& filePath);
	std::wstring parseFileName(const std::wstring& filePath);
	
	void encode(const std::wstring& sourcePath, const std::wstring& destinationPath) override;
	void decode(const std::wstring& sourcePath, const std::wstring& destinationPath) override;

public:
	Lzw(const std::wstring& inputPath_, const std::wstring& outputPath_, QObject* parent = nullptr);

};

