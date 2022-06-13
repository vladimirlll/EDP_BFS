#ifndef TESTWGT_H
#define TESTWGT_H

#include <QWidget>
#include <QFile>
#include <QRadioButton>
#include <QDebug>
#include "test.h"
#include "testresult.h"

namespace Ui {
class TestWgt;
}

class TestWgt : public QWidget
{
    Q_OBJECT

public:
    explicit TestWgt(QWidget *parent = nullptr);
    ~TestWgt();

signals:
    void BackClicked();

private slots:
    void on_btnToNextQ_clicked();
    void on_btnToPrevQ_clicked();
    void on_btnFinish_clicked();
    void on_btnOk_clicked();
    void OutCurrentQuestion();

private:
    Test *test;
    QRadioButton **rbtns;   //массив указателей на радио-баттоны формы
    Ui::TestWgt *ui;
};

#endif // TESTWGT_H
