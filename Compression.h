#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "List.h"

class Compression {
public:
	int Huffman(std::string filename, int mode);
};
