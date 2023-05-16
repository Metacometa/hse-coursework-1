#pragma once
#include "Compression.h"

#include <fstream>
#include <unordered_map>
#include <vector>

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <set>
#include <sstream>

#include "gmpxx.h"

#define SIZE 256
#define BUFFER_SIZE 65536
#define PREC 600000
#define NUMS 175000

struct Segment {
	mpf_class left;
	mpf_class right;
	char character;
};

class Arithmetic : public Compression
{
	Q_OBJECT

private:

	void encode(const std::wstring& sourcePath, const std::wstring& destinationPath) override;
	void decode(const std::wstring& sourcePath, const std::wstring& destinationPath) override;

	std::vector<Segment> defineSegments(const std::vector<mpf_class>& freq);

public:
	Arithmetic(const std::wstring& inputPath_, const std::wstring& outputPath_, QObject* parent = nullptr);

};


