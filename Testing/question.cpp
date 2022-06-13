#include "question.h"
#include <QRandomGenerator>

//Question::Type Question::_type = Type::close;

Question::Question(QObject *parent) : QObject(parent)
{
    this->text = "";
    this->answerVariants = {};
    this->themes = {};
    this->correctAnswer = "";
    this->type = "Открытый";
    this->difficultyLevel = "0";
    this->userAnswer = "";
}

Question::Question(const QString& text,
                   const QList<QString>& answerVariants,
                   const QString correctAnswer,
                   const QList<QString>& themes,
                   const QString& type,
                   const QString& difLevel,
                   const QString& userAnswer,
                   QObject *parent) : QObject(parent)
{
    this->text = text;
    this->answerVariants = answerVariants;
    this->correctAnswer = correctAnswer;
    this->themes = themes;
    this->type = type;
    this->difficultyLevel = difLevel;
    this->userAnswer = userAnswer;
}

Question::Question(const Question& another)
{
    this->operator=(another);
}

QString Question::GetType() const
{
    return type;
}

QString Question::GetText() const
{
    return text;
}

QList<QString> Question::GetAnswerVariants() const
{
    return answerVariants;
}

QString Question::GetCorrectAnswer() const
{
    return correctAnswer;
}

QList<QString> Question::GetThemes() const
{
    return themes;
}

QString Question::GetDifficultyLevel() const
{
    return difficultyLevel;
}

void Question::FromJSON(const QJsonObject &obj)
{
    // - парсинг тем вопроса
    QJsonArray questionThemes = obj["themes"].toArray();
    foreach(const QJsonValue & value, questionThemes)
    {

        if(value.isString()) themes << value.toString();
    }
    // - парсинг вида вопроса
    if(obj["type"].isString()) type = obj["type"].toString();
    // - парсинг текста вопроса
    if(obj["text"].isString()) text = obj["text"].toString();
    // - парсинг вариантов ответов
    answerVariants.clear();
    QJsonArray variantsArray;
    if(obj["variants"].isArray())
    {
        variantsArray = obj["variants"].toArray();
        foreach(const QJsonValue & value, variantsArray)
        {
            if(value.isString()) answerVariants.push_back(value.toString());
        }
    }
    // - парсинг правильного ответа
    if(obj["correctAnswer"].isString()) correctAnswer = obj["correctAnswer"].toString();
    // - парсинг уровня сложности вопроса
    if(obj["lvl"].isString()) difficultyLevel = obj["lvl"].toString();
}

QString Question::GetUserAnswer() const
{
    return userAnswer;
}

void Question::SetUserAnswer(const QString& userAnswer)
{
    this->userAnswer = userAnswer;
}

void Question::ShuffleAnswerVariants()
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(answerVariants.begin(), answerVariants.end(), g);
}
