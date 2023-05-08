#pragma once
#include "Compression.h"
#include "List.h"
#include "Config.h"

#include <fstream>

class Huffman : public Compression
{
	Q_OBJECT

public:
	Huffman(const std::wstring& inputPath_, const std::wstring& outputPath_, QObject* parent = nullptr);

private:
	void encode(const std::wstring& sourcePath, const std::wstring& destinationPath) override;
	void decode(const std::wstring& sourcePath, const std::wstring& destinationPath) override;

};

