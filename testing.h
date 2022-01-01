#ifndef TESTING_H
#define TESTING_H

#include <QWidget>
#include <QFile>
#include <QRadioButton>
#include <QDebug>
#include "testresult.h"

class TestWgt;

namespace Ui {
class Testing;
}

class Testing : public QWidget
{
    Q_OBJECT

public:
    explicit Testing(QWidget *parent = nullptr);
    ~Testing();

private slots:
    void on_test_clicked();
    void on_seeResults_clicked();
    void ShowMenu();

private:
    Ui::Testing *ui;
    TestWgt *testWgt;
    QVector<TestResult> GetTestResults();
};

#endif // TESTING_H
