#pragma once
#include "Compression.h"

#include <chrono>
#include <iostream>
#include <fstream>
#include <cstddef>
#include <algorithm>

//#include <iostream>

//Constructors / Destructors
Compressor::Compressor(std::wstring inputPath, std::wstring outputPath) 
{
	this->inputPath = inputPath;
	this->outputPath = outputPath;
	this->isPaused = false;
	this->canBeUpdated = false;
}

void Compressor::huffmanCompression() 
{
	size_t namestart = this->inputPath.length();
	while (this->inputPath[namestart] != '\\') 
	{
		namestart--;
	}

	std::wstring newname = this->inputPath.substr(namestart, this->inputPath.length());
	size_t it = newname.length();
	while (newname[it] != '.') 
	{
		it--;
	}

	std::ifstream fr;

	fr.open(this->inputPath, std::ios_base::in | std::ios_base::binary);

	fr.seekg(0, std::ios_base::end);
	std::streamoff length = fr.tellg();
	fr.seekg(0, std::ios_base::beg);

	int freq[SIZE] = { 0 };

	for (std::streamoff i = 0; i < length; ++i)
	{
		freq[(unsigned char)fr.get()] ++;
	}
	fr.seekg(0, std::ios_base::beg);

	NODE* Prior = nullptr;
	for (int i = 0; i < SIZE; i++) 
	{
		if (freq[i] != 0) 
		{
			CELL Temp = { (unsigned char)i,(unsigned char)'1',(unsigned int)freq[i],nullptr,nullptr };
			Add2List(&Prior, Temp);
		}
	}

	char code[SIZE] = { 0 };
	char table[SIZE][SIZE] = { 0 };
	Prior = MakeTreeFromList(Prior);
	Simmetric(&(Prior->content), 0, code, table);

	std::ofstream coded;
	coded.open(outputPath + newname.substr(0, it) + huffmanExtension, std::ios_base::out | std::ios_base::binary);

	coded.put(' ');
	coded.put('*');
	size_t filelength = newname.length();

	for (size_t i = it; i < filelength; i++) 
	{
		coded.put(newname[i]);
	}
	coded.put('*');
	for (int i = 0; i < SIZE; i++) 
	{
		for (int j = 0; j < 4; j++) 
		{
			coded.put(freq[i] >> (8 * j));
		}
		coded.put('*');
	}

	BIT2CHAR buf;
	buf.symb = 0;
	int currbit = 0;
	for (std::streamoff i = 0; i < length; ++i) 
	{
		//pausing
		while (this->isPaused) {}

		int currsymb = (unsigned int)fr.get();
		int pointer = 0;

		//updating QtProgressBar
		if (canBeUpdated) {
			emit updateProgressBar(i * 100 / length);
			counter = 0;
			canBeUpdated = false;
		}

		while (table[currsymb][pointer] != 0) 
		{
			if (currbit == 8) 
			{
				coded.put(buf.symb);
				buf.symb = 0;
				currbit = 0;
			}
			if (table[currsymb][pointer] == '0') 
			{
				buf.symb &= ~((char)(1 << (currbit)));
			}
			else 
			{
				buf.symb ^= ((char)(1 << (currbit)));
			}
			pointer++;
			currbit++;
		}
	}

	coded.put(buf.symb);
	if (currbit % 8 == 0) 
	{
		currbit = 0;
	}

	coded.seekp(0, std::ios_base::beg);
	coded.put(currbit);

	fr.close();
	coded.close();

	//updating QtProgressBar and closing programm
	emit updateProgressBar(100);
	emit finished();
}

void Compressor::huffmanDecompression() 
{
	size_t filelength = this->inputPath.length();
	size_t namestart = filelength;
	while (this->inputPath[namestart] != '\\')
	{
		namestart--;
	}

	std::wstring currext = huffmanExtension;
	size_t extlength = currext.length();

	std::ifstream coded;
	coded.open(this->inputPath, std::ios_base::in | std::ios_base::binary);

	coded.seekg(0, std::ios_base::end);
	std::streamoff length = coded.tellg();
	coded.seekg(0, std::ios_base::beg);

	int bit = coded.get();
	coded.get();

	std::wstring ext;
	char taken = coded.get();
	while (taken != '*') 
	{
		ext.push_back(taken);
		taken = coded.get();
	}

	int freq[SIZE] = { 0 };
	for (int i = 0; i < SIZE; i++) 
	{
		for (int j = 0; j < 4; j++) 
		{
			int temp = coded.get();
			freq[i] ^= (temp << (8 * j));
		}
		coded.get();
	}

	NODE* Prior = nullptr;
	for (int i = 0; i < SIZE; i++) 
	{
		if (freq[i] != 0) 
		{
			CELL Temp = { (unsigned char)i,(unsigned char)'1',(unsigned int)freq[i],nullptr,nullptr };
			Add2List(&Prior, Temp);
		}
	}

	char code[SIZE] = { 0 };
	char table[SIZE][SIZE] = { 0 };
	Prior = MakeTreeFromList(Prior);
	Simmetric(&(Prior->content), 0, code, table);

	std::ofstream decoded;
	decoded.open(this->outputPath + this->inputPath.substr(namestart, filelength - extlength - namestart) + ext, std::ios_base::out | std::ios_base::binary);

	BIT2CHAR buf;
	BIT2CHAR bufnext;
	buf.symb = coded.get();
	CELL* temp = &(Prior->content);
	while (true) 
	{
		//pausing
		if (!this->isPaused) 
		{
			bufnext.symb = coded.get();

			//updating QtProgressBar
			if (canBeUpdated) {
				emit updateProgressBar(coded.tellg() * 100 / length);
				counter = 0;
				canBeUpdated = false;
			}

			if (coded.eof())
			{
				for (int i = 0; i < bit; i++)
				{
					if (buf.mbit.b1 == 0)
					{
						if (temp->left)
						{
							temp = temp->left;
						}
					}
					else
					{
						if (temp->right)
						{
							temp = temp->right;
						}
					}
					if (temp->isSymb == '1')
					{
						decoded.put(temp->symb);
						temp = &(Prior->content);
					}
					buf.symb = buf.symb >> 1;
				}
				break;
			}
			for (int i = 0; i < 8; i++)
			{
				if (buf.mbit.b1 == 0)
				{
					if (temp->left)
					{
						temp = temp->left;
					}
				}
				else
				{
					if (temp->right)
					{
						temp = temp->right;
					}
				}
				if (temp->isSymb == '1')
				{
					decoded.put(temp->symb);
					temp = &(Prior->content);
				}
				buf.symb = buf.symb >> 1;
			}
			buf = bufnext;
		}
	}

	coded.close();
	decoded.close();

	//updating QtProgressBar
	emit updateProgressBar(100);
	emit finished();
}

void Compressor::reverseIsPaused()
{
	this->isPaused = !this->isPaused;
}

void Compressor::reverseCanBeUpdated()
{
	canBeUpdated = true;
}

void Compressor::lzwCompression()
{
	std::unordered_map<ustring, int> table;
	for (int i = 0; i <= 255; i++) {
		ustring ch;
		ch += static_cast<unsigned char>(i);
		table[ch] = i;
	}
	std::ifstream fileForReading(inputPath, std::ios::binary);
	std::ofstream fileForWritting(outputPath + L"\\" + parseFileName(inputPath) + L".lzw", std::ios::binary);

	
	fileForWritting.put('*');
	fileForWritting.put('.');
	for (auto& i : parseExtension(inputPath)) 
	{
		fileForWritting.put(i);
	}
	fileForWritting.put('*');
	

	ustring bytes;
	readFileToString(fileForReading, bytes);

	ustring p, c;
	std::vector<int> inputNumbers;

	p += bytes[0];

	for (size_t i = 0; i < bytes.length(); ++i) 
	{
		if (i != bytes.length() - 1) 
		{
			c += bytes[i + 1];
		}

		if (table.find(p + c) != table.end())
		{
			p = p + c;
		}
		else 
		{
			inputNumbers.emplace_back(table[p]);

			if (inputNumbers.size() == 2) 
			{
				writeNumbers(fileForWritting, inputNumbers[0], inputNumbers[1]);
				inputNumbers.clear();
			}

			if (table.size() < 4096)
			{
				table[p + c] = table.size();
			}

			p = c;
		}
		c = reinterpret_cast<const unsigned char*>("");
	}
	inputNumbers.emplace_back(table[p]);
	if (inputNumbers.size() == 2) 
	{
		writeNumbers(fileForWritting, inputNumbers[0], inputNumbers[1]);

	}
	else if (inputNumbers.size() == 1) 
	{
		fileForWritting << static_cast<unsigned char>(inputNumbers[0] >> 4) << static_cast<unsigned char>(inputNumbers[0] << 4);
	}

	fileForReading.close();
	fileForWritting.close();

	emit finished();
}

void Compressor::lzwDecompression()
{
	ustring source;

	std::ifstream fileForReading(inputPath, std::ios::binary);

	readFileToString(fileForReading, source);
	std::wstring extension;
	
	int fileBeginning = 0;
	for (auto& i : source)
	{
		fileBeginning++;
		if (i == '*' and fileBeginning > 1)
		{
			break;
		}
		else if (i != '*')
		{
			extension += i;
		}
	}

	std::ofstream fileForWritting(outputPath + L"\\" + parseFileName(inputPath) + L".txt", std::ios::binary);

	std::vector<int> codes;
	std::vector<unsigned char> bytes;

	
	for (int i = fileBeginning; i < source.length(); ++i)
	{
		bytes.push_back(source[i]);

		if (bytes.size() == 3) 
		{
			getCodesFromBytes(codes, bytes);
			bytes.clear();
		}

	}

	if (bytes.size() == 2) 
	{
		getCodesFromBytes(codes, bytes);
	}

	decoding(fileForWritting, codes);

	fileForReading.close();
	fileForWritting.close();

	emit finished();
}

void Compressor::readFileToString(std::ifstream& file, ustring& s) 
{
	while (file.good()) {
		s += file.get();
	}
	s.erase(s.end() - 1, s.end());
}

void Compressor::writeNumbers(std::ofstream& file, const int& one, const int& two) 
{
	unsigned char secondByte = one & 15;
	secondByte = secondByte << 4;
	secondByte = secondByte | (two >> 8);
	file << static_cast<unsigned char>(one >> 4);
	file << secondByte;
	file << static_cast<unsigned char>(two);
}

void Compressor::getCodesFromBytes(std::vector<int>& codes, const std::vector<unsigned char>& bytes)
{
	if (bytes.size() < 2 or bytes.size() > 3) {
		return;
	}
	int code = bytes[0];
	code = code << 4;
	code = code | (static_cast<unsigned char>(bytes[1] >> 4));
	codes.emplace_back(code);

	if (bytes.size() == 3) {
		code = 0;
		code = code | static_cast<unsigned char>(bytes[1] << 4);
		code = code << 4;
		code = code | bytes[2];
		codes.emplace_back(code);
	}
}

void Compressor::decoding(std::ofstream& file, const std::vector<int>& op)
{
	std::unordered_map<int, ustring> table;
	for (int i = 0; i <= 255; i++) {
		ustring ch;
		ch += static_cast<unsigned char>(i);
		table[i] = ch;
	}
	int old = op[0];
	int n;
	ustring s;
	s += table[old];
	ustring c;
	c += s[0];
	for (auto& j : s) {
		file << j;
	}

	int count = 256;
	for (int i = 0; i < op.size() - 1; i++) {
		n = op[long(i + 1)];
		if (table.find(n) == table.end()) {
			s = table[old];
			s = s + c;
		}
		else {
			s = table[n];
		}
		for (auto& j : s) {
			file << j;
		}

		c = reinterpret_cast<const unsigned char*>("");
		c += s[0];
		table[count] = table[old] + c;
		count++;
		old = n;
	}
}

std::wstring Compressor::parseExtension(const std::wstring& filePath)
{
	std::wstring extension;

	for (int i = filePath.size() - 1; i >= 0 and filePath[i] != '.'; --i) {
		extension = filePath[i] + extension;
	}

	return extension;
}

std::wstring Compressor::parseFileName(const std::wstring& filePath)
{
	std::wstring fileName;
	bool isFileName = false;

	for (int i = filePath.size() - 1; i >= 0 and filePath[i] != '\\'; --i) {
		if (isFileName) {
			fileName = filePath[i] + fileName;
		}

		if (filePath[i] == '.') {
			isFileName = true;
		}
	}
	return fileName;
}
