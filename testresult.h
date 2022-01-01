#ifndef TESTRESULT_H
#define TESTRESULT_H

#include <QObject>
#include <QDataStream>

struct TestResult
{
    uint userScore;     //баллы, набранные пользователем
    uint totalScore;    //максимальное количество баллов, которое можно набрать
    char name[50];      //имя тестируемого
};

QDataStream& operator <<(QDataStream &out, const TestResult &result);
QDataStream& operator >>(QDataStream &in, TestResult &result);

#endif // TESTRESULT_H
