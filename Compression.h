#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "List.h"

#include <QProgressBar>

class Compression {
public:
	int Huffman(QProgressBar *pbar, std::string filename, int mode);
};
