#pragma once
#include "Compression.h"


int Compression::Huffman(std::string s_filepath, int mode) {
	//file name getting
	std::string temp = "";
	int index = s_filepath.length();
	for (int i = s_filepath.length() - 1; i >= 0; --i) {
		if (s_filepath[i] != '\\')
			temp = s_filepath[i] + temp;
		else
			break;
	}
	std::string::size_type n = s_filepath.find(temp);
	if (n != std::string::npos)
		s_filepath.erase(s_filepath.begin() + n, s_filepath.end());

	char filename[CODE_SIZE] = { 0 };
	char newfilename[CODE_SIZE] = "Coded";
	char decodedfilename[CODE_SIZE] = "Decoded";
	for (int i = 0; i < temp.length(); ++i)
		filename[i] = temp[i];

	//path name getting
	char pathname[CODE_SIZE] = { 0 };
	char newpathname[CODE_SIZE] = { 0 };
	char decodedpathname[CODE_SIZE] = { 0 };
	for (int i = 0; i < s_filepath.length(); ++i)
		decodedpathname[i] = newpathname[i] = pathname[i] = s_filepath[i];

	strcat(newfilename, filename);
	strcat(newpathname, newfilename);
	strcpy(newfilename, newpathname);

	strcat(decodedfilename, filename);
	strcat(decodedpathname, decodedfilename);
	strcpy(decodedfilename, decodedpathname);

	strcat(pathname, filename);
	strcpy(filename, pathname);

	if (mode == 1) {
		char tempname[CODE_SIZE] = { 0 };
		FILE* fr = fopen(filename, "rb");
		int freq[SIZE] = { 0 };
		if (!fr)
			return 0;
		fseek(fr, 0L, SEEK_END);
		long length = ftell(fr);
		fseek(fr, 0, SEEK_SET);
		for (int i = 0; i < length; ++i)
		{
			freq[(unsigned char)fgetc(fr)] ++;
		}
		fseek(fr, 0, SEEK_SET);
		NODE* Prior = NULL;
		for (int i = 0; i < SIZE; i++) {
			if (freq[i] != 0) {
				CELL Temp = { (unsigned char)i,(unsigned char)'1',(unsigned int)freq[i],NULL,NULL };
				Add2List(&Prior, Temp);
			}
		}
		char code[CODE_SIZE] = { 0 };
		char table[SIZE][CODE_SIZE] = { 0 };
		Prior = MakeTreeFromList(Prior);
		Simmetric(&(Prior->content), 0, code, table);
		FILE* coded = fopen(newfilename, "wb");
		if (!coded)
			return 0;
		fputc(' ', coded);
		fputc('*', coded);
		for (int i = 0; i < SIZE; i++) {
			for (int j = 0; j < 4; j++) {
				fputc(freq[i] >> (8 * j), coded);
			}
			fputc('*', coded);
		}
		BIT2CHAR buf;
		buf.symb = 0;
		int currbit = 0;
		for (int i = 0; i < length; ++i)
		{
			int currsymb = (unsigned int)fgetc(fr);
			int pointer = 0;
			while (table[currsymb][pointer] != 0) {
				if (currbit == 8) {
					fputc(buf.symb, coded);
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
		}
		fputc(buf.symb, coded);
		if (currbit % 8 == 0) {
			currbit = 0;
		}
		fseek(coded, 0, SEEK_SET);
		fputc(currbit, coded);
		fclose(fr);
		fclose(coded);
	}
	else if (mode == 2) {
		FILE* coded = fopen(filename, "rb");
		int bit = fgetc(coded);
		fgetc(coded);
		int freq[SIZE] = { 0 };
		for (int i = 0; i < SIZE; i++) {
			for (int j = 0; j < 4; j++) {
				int temp = fgetc(coded);
				freq[i] ^= (temp << (8 * j));
			}
			fgetc(coded);
		}
		NODE* Prior = NULL;
		for (int i = 0; i < SIZE; i++) {
			if (freq[i] != 0) {
				CELL Temp = { (unsigned char)i,(unsigned char)'1',(unsigned int)freq[i],NULL,NULL };
				Add2List(&Prior, Temp);
			}
		}
		char code[CODE_SIZE] = { 0 };
		char table[SIZE][CODE_SIZE] = { 0 };
		Prior = MakeTreeFromList(Prior);
		Simmetric(&(Prior->content), 0, code, table);

		BIT2CHAR buf;
		BIT2CHAR bufnext;
		FILE* decoded = fopen(decodedfilename, "wb");
		buf.symb = fgetc(coded);
		CELL* temp = &(Prior->content);
		int currbit = 0;
		while (1) {
			bufnext.symb = fgetc(coded);
			if (feof(coded)) {
				for (int i = 0; i < bit; i++) {
					if (buf.mbit.b1 == 0) {
						temp = temp->left;
					}
					else {
						temp = temp->right;
					}
					if (temp->isSymb == '1') {
						fputc(temp->symb, decoded);
						temp = &(Prior->content);
					}
					buf.symb = buf.symb >> 1;
				}
				break;
			}
			for (int i = 0; i < 8; i++) {
				if (buf.mbit.b1 == 0) {
					temp = temp->left;
				}
				else {
					temp = temp->right;
				}
				if (temp->isSymb == '1') {
					fputc(temp->symb, decoded);
					temp = &(Prior->content);
				}
				buf.symb = buf.symb >> 1;
			}
			buf = bufnext;
		}
		fclose(decoded);
		fclose(coded);
	}
	else {
		printf("%s", "Error!");
	}
	return 0;
}