#include "Huffman.h"

Huffman::Huffman(const std::wstring& inputPath_, const std::wstring& outputPath_, QObject* parent)
	: Compression(inputPath_, outputPath_, parent) {}

void Huffman::encode(const std::wstring& sourcePath, const std::wstring& destinationPath)
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
	size_t length = fr.tellg();
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
	for (size_t i = 0; i < length; ++i)
	{
		//pausing
		while (this->isPaused) {}

		int currsymb = (unsigned int)fr.get();
		int pointer = 0;

		//updating QtProgressBar
		if (canBeUpdated) {
			emit updateProgressBar(i * 100 / length);
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

void Huffman::decode(const std::wstring& sourcePath, const std::wstring& destinationPath)
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
	size_t length = coded.tellg();
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
