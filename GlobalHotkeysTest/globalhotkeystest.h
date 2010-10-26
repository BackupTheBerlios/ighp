#ifndef GLOBALHOTKEYSTEST_H
#define GLOBALHOTKEYSTEST_H

#include <QtGui/QMainWindow>
#include "ui_globalhotkeystest.h"

class GlobalHotkeysTest : public QMainWindow
{
	Q_OBJECT

public:
	GlobalHotkeysTest(QWidget *parent = 0, Qt::WFlags flags = 0);
	~GlobalHotkeysTest();

private:
	Ui::GlobalHotkeysTestClass ui;
};

#endif // GLOBALHOTKEYSTEST_H
