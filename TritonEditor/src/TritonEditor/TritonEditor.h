#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_TritonEditor.h"

class TritonEditor : public QMainWindow
{
	Q_OBJECT

public:
	TritonEditor(QWidget *parent = Q_NULLPTR);
	~TritonEditor();

	void keyPressEvent(QKeyEvent* ev);

	void keyReleaseEvent(QKeyEvent* ev);

private:
	Ui::TritonEditorClass ui;

	Triton::relay_ptr<Triton::IO::Keyboard> keyboard;
	Triton::relay_ptr<Triton::IO::Mouse> mouse;
};
