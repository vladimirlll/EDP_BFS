#include <QFile>
#include <QTextStream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonParseError>
#include <QRandomGenerator>
#include <QDebug>
#include "test.h"
#include "testresult.h"

Test::Test(const QString& testFilePath, const Mode &mode,  const uint& askingQueCount, QObject *parent) :  QObject(parent)
{
    _mode = mode;
    askingQuestionsCount = askingQueCount;
    LoadFromJSON(testFilePath);
    if(questions.length() > askingQuestionsCount) askingQuestionsCount = questions.length();
    passingUserName = "unknown";
    currentQuestion = new Question();
    SetCurrentQuestion(0);
}

QList<Question> Test::Questions()         const
{
    return questions;
}

void Test::SetCurrentQuestion(const uint& questionIndexInList)
{
    if(questionIndexInList < questions.length())
    {
        currentQuestion = &questions[questionIndexInList];
        currentQuestionNumInList = questionIndexInList;
        emit CurrentQuestionChanged();
    }
}

Question* Test::CurrentQuestion()   const
{
    return currentQuestion;
}

QString Test::Theme()             const
{
    return theme;
}

void Test::LoadFromJSON(const QString &testFilePath)
{
    QFile file(testFilePath);
    if(file.exists())
    {
        if(file.open(QIODevice::ReadOnly))
        {
            QTextStream strm(&file);
            strm.setCodec("UTF-8");
            QString text = strm.readAll();
            file.close();

            QJsonParseError error;
            QJsonDocument doc = QJsonDocument::fromJson(text.toUtf8(), &error);
            qDebug() << "Error: " << error.errorString() << error.offset << error.error;

            if (doc.isObject())
            {

                QJsonObject json = doc.object();
                theme = json["testTheme"].toString();
                QJsonArray questions = json["questions"].toArray();
                foreach (const QJsonValue & value, questions) // - итерирование по вопросам
                {
                    if (value.isObject()) // - каждый вопрос объект
                    {
                        Question queFromJSON;
                        queFromJSON.FromJSON(value.toObject());
                        if(_mode == Mode::onlyThemeQuestions)
                        {
                            if(queFromJSON.GetThemes().contains(theme))
                                this->questions.push_back(queFromJSON);
                        }
                        else
                            this->questions.push_back(queFromJSON);
                    }
                }
            }

        }
        else
        {
            file.close();
        }
    }
    else
    {
        file.close();
    }
}

void Test::ToNextQuestion()
{
    SetCurrentQuestion(currentQuestionNumInList + 1);
}

void Test::ToPrevQuestion()
{
    SetCurrentQuestion(currentQuestionNumInList - 1);
}

void Test::SetPassingUserName(const QString& userName)
{
    passingUserName = userName;
}

QString Test::PassingUserName() const
{
    return passingUserName;
}

void Test::Shuffle()
{
    int qCount = questions.length(); //количество всех вопросов из базы вопросов
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(questions.begin(), questions.end(), g);
    for(int i = 0; i < qCount; ++i)
    {
        questions[i].ShuffleAnswerVariants();
    }
    SetCurrentQuestion(currentQuestionNumInList);
}


uint Test::GetTotalScore() const
{
    uint quesCount = questions.length();
    uint totalScore = 0;
    for(uint i = 0; i < quesCount; ++i)
    {
        totalScore += questions[i].GetDifficultyLevel().toUInt();
    }
    return totalScore;
}

uint Test::GetUserScore() const
{
    uint quesCount = questions.length();
    uint userScore = 0;
    for(uint i = 0; i < quesCount; ++i)
    {
        if(questions[i].GetCorrectAnswer() == questions[i].GetUserAnswer())
            userScore += questions[i].GetDifficultyLevel().toUInt();
    }
    return userScore;
}

void Test::SaveTestResultInFile(const QString& filePath) const
{
    TestResult result;
    strcpy(result.name, passingUserName.toStdString().c_str());
    result.totalScore = GetTotalScore();
    result.userScore = GetUserScore();
    result.SaveInFile(filePath);
}

uint Test::GetAskingQuestionCount() const
{
    return askingQuestionsCount;
}

void Test::SetAskingQuestionCount(const uint& askingQuesCount)
{
    askingQuestionsCount = askingQuesCount;
}

Test::Mode Test::mode() const
{
    return _mode;
}
