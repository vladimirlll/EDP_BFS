#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Theory/theorywidget.h"
#include "Demo/demo.h"
#include "Testing/testing.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    TheoryWidget *ptheory;
    Demo* pdemo;
    Testing *ptesting;
};
#endif // MAINWINDOW_H
