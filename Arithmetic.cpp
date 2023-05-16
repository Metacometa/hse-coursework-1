#include "Arithmetic.h"

Arithmetic::Arithmetic(const std::wstring& inputPath_, const std::wstring& outputPath_, QObject* parent)
	: Compression(inputPath_, outputPath_, parent) {}

void Arithmetic::encode(const std::wstring& sourcePath, const std::wstring& destinationPath)
{
	size_t namestart = sourcePath.length();
	while (sourcePath[namestart] != '\\') {
		namestart--;
	}


	std::wstring newname = sourcePath.substr(namestart, sourcePath.length());
	size_t it = newname.length();
	while (newname[it] != L'.') {
		it--;
	}


	std::ifstream fi;
	std::ofstream fo;
	fi.open(sourcePath, std::ios_base::in | std::ios_base::binary);
	fo.open(destinationPath + newname.substr(0, it) + arithmeticExtension, std::ios_base::out | std::ios_base::binary);

	size_t filelength = newname.length();
	for (size_t i = it; i < filelength; i++) {
		fo.put(newname[i]);
		//fo << newname[i];
	}
	fo << '*';

	fi.seekg(0, std::ios_base::end);
	size_t length = fi.tellg();
	fi.seekg(0, std::ios_base::beg);

	while (!fi.eof()) {
		char* buf = new char[BUFFER_SIZE] {0};
		fi.read(buf, BUFFER_SIZE);

		size_t currentpos = fi.tellg();
		//QThread pausing 
		while (this->isPaused) {}

		//updating QtProgressBar
		if (canBeUpdated)
		{
			emit updateProgressBar(currentpos * 100 / length);
			canBeUpdated = false;
		}

		std::vector<mpf_class> freq(SIZE, { 0,PREC });
		std::vector<long> count(SIZE, 0);
		for (int i = 0; i < BUFFER_SIZE; i++) {
			count[(unsigned char)buf[i]]++;
		}
		for (int i = 0; i < SIZE; i++) {
			freq[i] = count[i];
			freq[i] /= BUFFER_SIZE;
			fo << std::setprecision(NUMS) << freq[i] << '*';
		}
		std::vector<Segment> Segments = defineSegments(freq);
		mpf_class left = { 0,PREC };
		mpf_class right = { 1,PREC };
		for (int i = 0; i < BUFFER_SIZE; i++) {
			mpf_class newRight = left + (right - left) * Segments[(unsigned char)buf[i]].right;
			mpf_class newLeft = left + (right - left) * Segments[(unsigned char)buf[i]].left;
			left = newLeft;
			right = newRight;
		}
		mpf_class out = (left + right) / 2;
		fo << std::setprecision(NUMS) << out << '*';
		delete[] buf;
	}
	fi.close();
	fo.close();
	emit updateProgressBar(100);
	emit finished();
}

void Arithmetic::decode(const std::wstring& sourcePath, const std::wstring& destinationPath)
{
	size_t filelength = sourcePath.length();
	size_t namestart = filelength;
	while (sourcePath[namestart] != '\\') {
		namestart--;
	}


	std::wstring currext = arithmeticExtension;
	size_t extlength = currext.length();


	std::ifstream fi;
	std::ofstream fo;
	fi.open(sourcePath, std::ios_base::in | std::ios_base::binary);

	fi.seekg(0, std::ios_base::end);
	size_t length = fi.tellg();
	fi.seekg(0, std::ios_base::beg);

	std::wstring ext;
	char taken;
	fi >> taken;
	while (taken != '*') {
		ext.push_back(taken);
		fi >> taken;
	}

	fo.open(destinationPath + sourcePath.substr(namestart, filelength - extlength - namestart) + ext, std::ios_base::out | std::ios_base::binary);

	while (1) {
		std::vector<mpf_class> freq(SIZE, { 0,PREC });
		char ch;
		mpf_class code{ 0,PREC };
		for (int i = 0; i < SIZE; i++) {
			fi >> freq[i] >> ch;
		}

		size_t currentpos = fi.tellg();
		//QThread pausing 
		while (this->isPaused) {}

		//updating QtProgressBar
		if (canBeUpdated)
		{
			emit updateProgressBar(currentpos * 100 / length);
			canBeUpdated = false;
		}

		fi >> code >> ch;
		std::vector<Segment> Segments = defineSegments(freq);
		std::string out;
		for (size_t i = 0; i < BUFFER_SIZE; i++) {
			for (int j = 0; j < SIZE; j++) {
				if (code >= Segments[j].left && code < Segments[j].right) {
					out.push_back(Segments[j].character);
					code = (code - Segments[j].left) / (Segments[j].right - Segments[j].left);
					break;
				}
			}
		}
		fo << out;
		if (fi.peek() < 0) {
			break;
		}
	}
	fi.close();
	fo.close();
	emit updateProgressBar(100);
	emit finished();
}

std::vector<Segment> Arithmetic::defineSegments(const std::vector<mpf_class>& freq)
{
	std::vector<Segment> Segments(SIZE);
	std::set<mpf_class, std::greater<mpf_class>> sortedfreq;
	for (int i = 0; i < SIZE; i++) {
		sortedfreq.insert(freq[i]);
	}
	mpf_class l = { 0,PREC };
	for (auto& obj : sortedfreq) {
		if (obj == 0) break;
		for (int i = 0; i < SIZE; i++) {
			if (freq[i] == obj) {
				Segments[i].left = l;
				Segments[i].right = l + freq[i];
				Segments[i].character = i;
				l = Segments[i].right;
			}
		}
	}
	return Segments;
}
