#ifndef TESTWGT_H
#define TESTWGT_H

#include <QWidget>
#include <QFile>
#include <QRadioButton>
#include <QDebug>
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

private:
    void GetQueAndAnsFromFile(const QString &filePath);
    void OutQueAndAns(const uint&);
    void MixQuesFromFile();
    QVector<QVector<QString>> QA;   //двумерный вектор, содержащий вопросы, выводящиеся пользователю,
                                            //ответы на них и их уровни сложности
                                            //QA[0] - тексты вопросов
                                            //QA[1] - правильные ответ на вопросы
                                            //QA[2] - уровни сложности вопросов
                                            //QA[3] - виды вопросов
    QVector<QVector<QString>> QueVariants;   //вектор вариантов для вопросов
    QVector<QVector<QString>> QueThemes;     //вектор тем вопросов
    QVector<QString> usersAnswers;  //вектор, содержащий ответы пользователя
    QString testTheme; //тема тестирования
    uint index;     //индекс текущего вопроса
    QRadioButton **rbtns;   //массив указателей на радио-баттоны формы
    Ui::TestWgt *ui;
};

#endif // TESTWGT_H
