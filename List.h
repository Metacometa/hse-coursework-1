#pragma once

#include <string>
#include <array>

#define SIZE 256

typedef union bit2char {
	char symb;
	struct bit {
		unsigned b1 : 1;
		unsigned b2 : 1;
		unsigned b3 : 1;
		unsigned b4 : 1;
		unsigned b5 : 1;
		unsigned b6 : 1;
		unsigned b7 : 1;
		unsigned b8 : 1;
	}mbit;
}BIT2CHAR;
typedef struct cell {
	unsigned char symb;
	unsigned char isSymb;
	unsigned int freq;
	struct cell* left, * right;
}CELL;

typedef struct Node {
	CELL content;
	struct Node* next;
}NODE;

void Add2List(NODE** pphead, CELL temp);
NODE* DeleteList(NODE* phead);
NODE* MakeTreeFromList(NODE* head);
CELL* MakeNodeFromNode(CELL* left, CELL* right);
void Simmetric(const CELL* root, int pos, char code[], char table[][SIZE]);