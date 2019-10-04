#include "TritonEditor.h"

#include "Triton2/Triton.h"

TritonEditor::TritonEditor(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	Triton::EngineSettings settings;
	settings.in_customInputLoop = true;

	Triton::Engine::getInstance().init(settings);

	keyboard = settings.out_keyboard;
	mouse = settings.out_mouse;
}

TritonEditor::~TritonEditor()
{
	Triton::Engine::getInstance().shutdown();
}

void TritonEditor::keyPressEvent(QKeyEvent* ev)
{
	// Needs key mapping can currently crash
	keyboard->setKey((Triton::IO::Key)ev->key(), true);
}

void TritonEditor::keyReleaseEvent(QKeyEvent* ev)
{
	keyboard->setKey((Triton::IO::Key)ev->key(), false);
}
