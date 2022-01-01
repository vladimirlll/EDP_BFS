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
