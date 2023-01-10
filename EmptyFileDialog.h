#pragma once

#include <QDialog>
#include "ui_EmptyFileDialog.h"

class EmptyFileDialog : public QDialog, public Ui::EmptyFileDialogClass
{
	Q_OBJECT

public:
	EmptyFileDialog(QWidget* parent = nullptr);
	~EmptyFileDialog();

private:

};