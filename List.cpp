#include "List.h"
void Add2List(NODE** pphead, CELL temp)
{
	while (*pphead)
	{
		if (((*pphead)->content).freq > temp.freq)
			break;
		pphead = &((*pphead)->next);
	}
	NODE* pnew = new NODE;
	pnew->content = temp;
	pnew->next = *pphead;
	*pphead = pnew;
}

NODE* DeleteList(NODE* phead)
{
	if (phead)
	{
		DeleteList(phead->next);
		delete phead;
	}
	return nullptr;
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
	CELL* res = new CELL;
	res->freq = left->freq + right->freq;
	res->isSymb = 0;
	res->symb = 0;
	res->left = left;
	res->right = right;
	return res;
}

void Simmetric(const CELL* root, int pos, char code[], char table[][SIZE])
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
		if (pos == 0) {
			table[root->symb][0] = '0';
		}
		else {
			for (size_t i = 0; i <= pos; i++) {
				table[root->symb][i] = code[i];
			}
		}
	}
}