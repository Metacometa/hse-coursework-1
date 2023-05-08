#pragma once
#include <QObject>

class Compression : public QObject
{
	Q_OBJECT

protected:
	//удали counter
	bool isPaused = false;
	bool canBeUpdated = true;
	std::wstring inputPath;
	std::wstring outputPath;

	virtual void encode(const std::wstring& sourcePath, const std::wstring& destinationPath) = 0;
	virtual void decode(const std::wstring& sourcePath, const std::wstring& destinationPath) = 0;

public:
	Compression(const std::wstring& inputPath_, const std::wstring& outputPath_, QObject* parent = nullptr);

signals:
	void updateProgressBar(int value);
	void finished();

private slots:
	void encodeSlot();
	void decodeSlot();

	void reverseIsPaused();
	void reverseCanBeUpdated();
};
