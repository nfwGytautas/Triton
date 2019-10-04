#include "TritonEditor/TritonEditor.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	TritonEditor w;
	w.show();
	return a.exec();
}
