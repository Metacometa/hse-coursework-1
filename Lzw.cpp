#include "LZW.h"

Lzw::Lzw(const std::wstring& inputPath_, const std::wstring& outputPath_, QObject* parent)
	: Compression(inputPath_, outputPath_, parent) {}

void Lzw::encode(const std::wstring& sourcePath, const std::wstring& destinationPath) 
{
	std::unordered_map<ustring, int> table;
	for (int i = 0; i <= 255; i++) {
		ustring ch;
		ch += static_cast<unsigned char>(i);
		table[ch] = i;
	}
	std::ifstream fileForReading(sourcePath, std::ios::binary);
	std::ofstream fileForWritting(destinationPath + L"\\" + parseFileName(sourcePath) + L".lzw", std::ios::binary);

	fileForWritting.put('*');
	fileForWritting.put('.');
	for (auto& i : parseExtension(sourcePath))
	{
		fileForWritting.put(i);		
	}
	fileForWritting.put('*');

	ustring bytes;
	readFileToString(fileForReading, bytes);

	ustring subsequence, nextSymbol;
	std::vector<int> inputNumbers;

	subsequence += bytes[0];

	for (size_t i = 0; i < bytes.length(); ++i)
	{
		//QThread pausing 
		while (this->isPaused) {}

		//updating QtProgressBar
		if (canBeUpdated) {
			emit updateProgressBar(i * 100 / bytes.length());
			canBeUpdated = false;
		}

		if (i != bytes.length() - 1)
		{
			nextSymbol += bytes[i + 1];
		}

		if (table.find(subsequence + nextSymbol) != table.end())
		{
			subsequence = subsequence + nextSymbol;
		}
		else
		{
			inputNumbers.emplace_back(table[subsequence]);

			if (inputNumbers.size() == 2)
			{
				writeNumbers(fileForWritting, inputNumbers[0], inputNumbers[1]);
				inputNumbers.clear();
			}

			if (table.size() < 4096)
			{
				table[subsequence + nextSymbol] = table.size();
			}

			subsequence = nextSymbol;
		}
		nextSymbol = reinterpret_cast<const unsigned char*>("");
	}

	inputNumbers.emplace_back(table[subsequence]);
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

	//updating QtProgressBar and closing programm
	emit updateProgressBar(100);
	emit finished();
}

void Lzw::decode(const std::wstring& sourcePath, const std::wstring& destinationPath) 
{
	ustring source;

	std::ifstream fileForReading(sourcePath, std::ios::binary);

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

	//this construction is trash and shoul be replaced by something adequate
	if (extension.size() == 0) {
		fileForReading.close();
		return;
	}
	else if (extension[0] != L'.') {
		fileForReading.close();
		return;
	}

	std::wcout << L"Extension: " << extension << std::endl;

	std::ofstream fileForWritting(destinationPath + L"\\" + parseFileName(sourcePath) + extension, std::ios::binary);

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

	//updating QtProgressBar and closing programm
	emit updateProgressBar(100);
	emit finished();
}

void Lzw::readFileToString(std::ifstream& file, ustring& s)
{
	while (file.good()) {
		s += file.get();
	}
	s.erase(s.end() - 1, s.end());
}

void Lzw::writeNumbers(std::ofstream& file, const int& one, const int& two)
{
	unsigned char secondByte = one & 15;
	secondByte = secondByte << 4;
	secondByte = secondByte | (two >> 8);
	file << static_cast<unsigned char>(one >> 4);
	file << secondByte;
	file << static_cast<unsigned char>(two);
}

void Lzw::getCodesFromBytes(std::vector<int>& codes, const std::vector<unsigned char>& bytes)
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

void Lzw::decoding(std::ofstream& file, const std::vector<int>& codes)
{
	std::unordered_map<int, ustring> table;
	for (int i = 0; i <= 255; i++) {
		ustring ch;
		ch += static_cast<unsigned char>(i);
		table[i] = ch;
	}
	int oldCode = codes[0];
	int n;

	ustring s;
	s += table[oldCode];

	ustring c;
	c += s[0];

	for (auto& j : s) {
		file << j;
	}

	//int count = 256;
	for (int i = 0; i < codes.size() - 1; i++) {
		//QThread pausing
		while (this->isPaused) {}

		//updating QtProgressBar
		if (canBeUpdated) {
			emit updateProgressBar(i * 100 / codes.size());
			canBeUpdated = false;
		}

		n = codes[long(i + 1)];
		if (table.find(n) == table.end()) {
			s = table[oldCode];
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
		table[table.size()] = table[oldCode] + c;
		//count++;
		oldCode = n;
	}
}

std::wstring Lzw::parseExtension(const std::wstring& filePath)
{
	std::wstring extension;

	for (int i = filePath.size() - 1; i >= 0 and filePath[i] != '.'; --i) {
		extension = filePath[i] + extension;
	}

	return extension;
}

std::wstring Lzw::parseFileName(const std::wstring& filePath)
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
