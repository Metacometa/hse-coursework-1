#include "List.h"

void Add2List(NODE** pphead, CELL temp)
{
	while (*pphead)
	{
		if (((*pphead)->content).freq > temp.freq)
			break;
		pphead = &((*pphead)->next);
	}
	NODE* pnew = (NODE*)malloc(sizeof(NODE));
	pnew->content = temp;
	pnew->next = *pphead;
	*pphead = pnew;
}

NODE* DeleteList(NODE* phead)
{
	if (phead)
	{
		DeleteList(phead->next);
		free(phead);
	}
	return NULL;
}

void PrintList(const NODE* phead)
{
	while (phead)
	{
		printf("%5d", (phead->content).freq);
		phead = phead->next;
	}
	printf("\n");
}

NODE* MakeTreeFromList(NODE* head)
{
	while (head && head->next)
	{
		CELL* left = &(head->content);
		CELL* right = &((head->next)->content);
		Add2List(&(head->next->next), *(MakeNodeFromNode(left, right)));
		head = head->next->next;
	}
	return head;
}

CELL* MakeNodeFromNode(CELL* left, CELL* right)
{
	CELL* res = (CELL*)malloc(sizeof(CELL));
	res->freq = left->freq + right->freq;
	res->isSymb = 0;
	res->symb = 0;
	res->left = left;
	res->right = right;
	return res;
}
void Simmetric(const CELL* root, int pos, char code[], char table[][CODE_SIZE])
{
	if (root->left)
	{
		code[pos] = '0';
		Simmetric(root->left, pos + 1, code, table);
		code[pos] = 0;
	}
	if (root->right) {
		code[pos] = '1';
		Simmetric(root->right, pos + 1, code, table);
		code[pos] = 0;
	}
	if (root->isSymb) {
		strcpy(table[root->symb], code);
	}
}

void Check(int* deshpoint, char* deshiph, FILE* decoded, char table[][CODE_SIZE]) {
	for (int i = 0; i < SIZE; i++) {
		if (strcmp(deshiph, table[i]) == 0) {
			fputc(i, decoded);
			for (int g = 0; g < *deshpoint; g++) {
				deshiph[g] = 0;
			}
			*deshpoint = 0;
			break;
		}
	}
}