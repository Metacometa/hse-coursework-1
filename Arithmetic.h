#pragma once
#include "Compression.h"

#include <fstream>
#include <unordered_map>
#include <vector>

class Arithmetic : public Compression
{
	Q_OBJECT

private:

	void encode(const std::wstring& sourcePath, const std::wstring& destinationPath) override;
	void decode(const std::wstring& sourcePath, const std::wstring& destinationPath) override;

public:
	Arithmetic(const std::wstring& inputPath_, const std::wstring& outputPath_, QObject* parent = nullptr);

};


