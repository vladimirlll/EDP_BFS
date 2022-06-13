#ifndef QUESTION_H
#define QUESTION_H

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonParseError>

class Question : public QObject
{
public:
    Question(QObject *parent = nullptr);

    Question(const QString& text,
                      const QList<QString>& answerVariants,
                      const QString correctAnswer,
                      const QList<QString>& themes,
                      const QString& type,
                      const QString& difLevel,
                      const QString& userAnswer,
                      QObject *parent = nullptr);

    Question(const Question& another);

    Question& operator=(const Question& another)
    {
        if(this == &another)
            return *this;
        this->text = another.GetText();
        this->answerVariants = another.GetAnswerVariants();
        this->correctAnswer = another.GetCorrectAnswer();
        this->themes = another.GetThemes();
        this->type = another.GetType();
        this->difficultyLevel = another.GetDifficultyLevel();
        return *this;
    }

    QString         GetText()           const;
    QList<QString>  GetAnswerVariants() const;
    QString         GetCorrectAnswer()  const;
    QList<QString>  GetThemes()         const;
    QString         GetType()           const;
    QString         GetDifficultyLevel()const;
    QString         GetUserAnswer()     const;
    void            SetUserAnswer(const QString&);
    void            FromJSON(const QJsonObject&);

    void            ShuffleAnswerVariants();

protected:
    QString text;
    QList<QString> answerVariants;
    QString correctAnswer;
    QString userAnswer;
    QList<QString> themes;
    QString type;
    QString difficultyLevel;

};

#endif // QUESTION_H
