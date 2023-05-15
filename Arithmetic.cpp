#include "Arithmetic.h"

Arithmetic::Arithmetic(const std::wstring& inputPath_, const std::wstring& outputPath_, QObject* parent)
	: Compression(inputPath_, outputPath_, parent) {}

void Arithmetic::encode(const std::wstring& sourcePath, const std::wstring& destinationPath)
{
	emit updateProgressBar(100);
	emit finished();
}

void Arithmetic::decode(const std::wstring& sourcePath, const std::wstring& destinationPath)
{
	emit updateProgressBar(100);
	emit finished();
}