#pragma once
#include "Compression.h"

#include <chrono>
#include <iostream>
#include <fstream>

void printTime(QLabel* label, const std::chrono::steady_clock::time_point& start) {
	auto end = std::chrono::steady_clock::now();
	int duration = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
	label->setText(QString::fromStdString(std::to_string(duration / 3600) + ":" + std::to_string(duration / 60) + ":" + std::to_string(duration % 60)));
}

void Huffman::Compression(QProgressBar* pbar, QLabel* label, std::string filename, std::string outputname) {
	label->setText("1");
	size_t namestart = filename.length();
	while (filename[namestart] != '\\') {
		namestart--;
	}
	//chrono
	auto start = std::chrono::steady_clock::now();

	std::string newname = filename.substr(namestart, filename.length());
	size_t it = newname.length();
	while (newname[it] != '.') {
		it--;
	}

	std::ifstream fr;
	fr.open(filename, std::ios_base::in | std::ios_base::binary);

	fr.seekg(0, std::ios_base::end);
	std::streamoff length = fr.tellg();
	fr.seekg(0, std::ios_base::beg);

	int freq[SIZE] = { 0 };

	label->setText("2");
	for (std::streamoff i = 0; i < length; ++i)
	{

		freq[(unsigned char)fr.get()] ++;
	}
	fr.seekg(0, std::ios_base::beg);

	NODE* Prior = nullptr;
	label->setText("3");
	for (int i = 0; i < SIZE; i++) {
		if (freq[i] != 0) {
			CELL Temp = { (unsigned char)i,(unsigned char)'1',(unsigned int)freq[i],nullptr,nullptr };
			Add2List(&Prior, Temp);
		}
	}

	char code[SIZE] = { 0 };
	char table[SIZE][SIZE] = { 0 };
	Prior = MakeTreeFromList(Prior);
	Simmetric(&(Prior->content), 0, code, table);

	std::ofstream coded;
	coded.open(outputname + newname.substr(0, it) + outputExtension, std::ios_base::out | std::ios_base::binary);

	coded.put(' ');
	coded.put('*');
	size_t filelength = newname.length();

	label->setText("4");
	for (size_t i = it; i < filelength; i++) {
		coded.put(newname[i]);
	}
	coded.put('*');
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < 4; j++) {
			coded.put(freq[i] >> (8 * j));
		}
		coded.put('*');
	}



	BIT2CHAR buf;
	buf.symb = 0;
	int currbit = 0;
	for (std::streamoff i = 0; i < length; ++i) {
		int currsymb = (unsigned int)fr.get();
		int pointer = 0;

		//Сюда добавь что-то, отображающее прогресс архивации (i/length)*100%
		pbar->setValue(i * 100 / length);

		while (table[currsymb][pointer] != 0) {
			//printTime(label, start);
			if (currbit == 8) {
				coded.put(buf.symb);
				buf.symb = 0;
				currbit = 0;
			}
			if (table[currsymb][pointer] == '0') {
				buf.symb &= ~((char)(1 << (currbit)));
			}
			else {
				buf.symb ^= ((char)(1 << (currbit)));
			}
			pointer++;
			currbit++;
		}
		printTime(label, start);
	}

	coded.put(buf.symb);
	if (currbit % 8 == 0) {
		currbit = 0;
	}

	coded.seekp(0, std::ios_base::beg);
	coded.put(currbit);

	fr.close();
	coded.close();
}

void Huffman::Decompression(QProgressBar* pbar, QLabel* label,	std::string filename, std::string outputname) {
	size_t filelength = filename.length();
	size_t namestart = filelength;
	while (filename[namestart] != '\\') {
		namestart--;
	}

	std::string currext = outputExtension;
	size_t extlength = currext.length();

	std::ifstream coded;
	coded.open(filename, std::ios_base::in | std::ios_base::binary);

	coded.seekg(0, std::ios_base::end);
	std::streamoff length = coded.tellg();
	coded.seekg(0, std::ios_base::beg);

	//set maximum for PROGRESS BAR
	pbar->setMaximum(100);

	int bit = coded.get();
	coded.get();

	std::string ext;
	char taken = coded.get();
	while (taken != '*') {
		ext.push_back(taken);
		taken = coded.get();
	}

	int freq[SIZE] = { 0 };
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < 4; j++) {
			int temp = coded.get();
			freq[i] ^= (temp << (8 * j));
		}
		coded.get();
	}

	NODE* Prior = nullptr;
	for (int i = 0; i < SIZE; i++) {
		if (freq[i] != 0) {
			CELL Temp = { (unsigned char)i,(unsigned char)'1',(unsigned int)freq[i],nullptr,nullptr };
			Add2List(&Prior, Temp);
		}
	}

	char code[SIZE] = { 0 };
	char table[SIZE][SIZE] = { 0 };
	Prior = MakeTreeFromList(Prior);
	Simmetric(&(Prior->content), 0, code, table);

	std::ofstream decoded;
	decoded.open(outputname + filename.substr(namestart, filelength - extlength - namestart) + ext, std::ios_base::out | std::ios_base::binary);

	BIT2CHAR buf;
	BIT2CHAR bufnext;
	buf.symb = coded.get();
	CELL* temp = &(Prior->content);
	while (1) {
		bufnext.symb = coded.get();

		//Сюда добавь что-то, отображающее прогресс декодирования (coded.tellg()/length)*100%
		pbar->setValue(coded.tellg() * 100 / length);
		//std::streamoff = long long;
		//std::streamoff coded.tellg();
		//std::streamoff length;

		if (coded.eof()) {
			for (int i = 0; i < bit; i++) {
				if (buf.mbit.b1 == 0) {
					if (temp->left) {
						temp = temp->left;
					}
				}
				else {
					if (temp->right) {
						temp = temp->right;
					}
				}
				if (temp->isSymb == '1') {
					decoded.put(temp->symb);
					temp = &(Prior->content);
				}
				buf.symb = buf.symb >> 1;
			}
			break;
		}
		for (int i = 0; i < 8; i++) {
			if (buf.mbit.b1 == 0) {
				if (temp->left) {
					temp = temp->left;
				}
			}
			else {
				if (temp->right) {
					temp = temp->right;
				}
			}
			if (temp->isSymb == '1') {
				decoded.put(temp->symb);
				temp = &(Prior->content);
			}
			buf.symb = buf.symb >> 1;
		}
		buf = bufnext;
	}

	coded.close();
	decoded.close();
}
