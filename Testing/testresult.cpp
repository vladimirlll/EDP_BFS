#include <QFile>
#include <QVector>
#include <stdexcept>
#include "testresult.h"

QDataStream& operator <<(QDataStream &out, const TestResult &result)
{
    out << result.userScore;
    out << result.totalScore;
    out.writeRawData(result.name,sizeof(result.name));
    return out;
}

QDataStream& operator >>(QDataStream &out, TestResult &result)
{
    out >> result.userScore;
    out >> result.totalScore;
    out.readRawData(result.name,sizeof(result.name));
    return out;
}

void TestResult::SaveInFile(const QString& filePath)
{
    QFile file(filePath);
    if(file.open(QIODevice::Append))
    {
        QDataStream strm(&file);
        strm.setVersion(QDataStream::Qt_5_9);
        strm << *this;
        if(strm.status() != QDataStream::Ok)
            throw std::runtime_error("Произошла ошибка записи результатов тестирования в файл");
    }
    file.close();
}

void TestResult::LoadFromFile(const QString& filePath)
{
    QFile file(filePath);
    if(file.open(QIODevice::ReadOnly))
    {
        QDataStream strm(&file);
        strm.setVersion(QDataStream::Qt_5_9);
        TestResult someResult;
        strm >> someResult;
        if(strm.status() != QDataStream::Ok)
        {
            throw std::runtime_error("Произошла ошибка записи результатов тестирования в файл");
        }
        else *this = someResult;
    }
    file.close();
}

QVector<TestResult> TestResult::ReadAllEntriesInFile(const QString &filePath)
{
    QVector<TestResult> entries;
    QFile file(filePath);
    if(file.open(QIODevice::ReadOnly))
    {
        QDataStream strm(&file);
        strm.setVersion(QDataStream::Qt_5_9);
        TestResult someResult;
        while(!strm.atEnd())
        {
            strm >> someResult;
            if(strm.status() != QDataStream::Ok)
            {
                throw std::runtime_error("Произошла ошибка записи результатов тестирования в файл");
            }
            else entries.push_back(someResult);
        }
    }
    file.close();
    return entries;
}
