#include "Compression.h"

Compression::Compression(const std::wstring& inputPath_, const std::wstring& outputPath_, QObject* parent)
	: QObject(parent), inputPath(inputPath_), outputPath(outputPath_) {}

void Compression::reverseIsPaused() 
{
	this->isPaused = !this->isPaused;
}

void Compression::reverseCanBeUpdated()
{
	this->canBeUpdated = true;
}

void Compression::encodeSlot() {
	encode(inputPath, outputPath);
}

void Compression::decodeSlot() {
	decode(inputPath, outputPath);
}