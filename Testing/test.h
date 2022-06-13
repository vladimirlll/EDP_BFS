#ifndef TEST_H
#define TEST_H

#include <QObject>
#include "question.h"

class Test : public QObject
{
    Q_OBJECT
public:
    enum Mode
    {
        allQuestions,
        onlyThemeQuestions
    };

    explicit Test(const QString& testFilePath, const Mode &mode, const uint& askingQueCount = 5, QObject *parent = nullptr);

    Test(const Test& another)
    {
        this->operator=(another);
    }

    Test& operator=(const Test& another)
    {
        if(this == &another) return *this;

        questions = another.questions;
        currentQuestion = another.currentQuestion;
        theme = another.theme;
        passingUserName = another.passingUserName;
        askingQuestionsCount = another.askingQuestionsCount;
        _mode = another._mode;

        return *this;
    }

    QList<Question> Questions()         const;
    void            SetCurrentQuestion(const uint&);
    Question*       CurrentQuestion()   const;
    QString         Theme()             const;
    void            ToNextQuestion()         ;
    void            ToPrevQuestion()         ;
    void            SetPassingUserName  (const QString &);
    QString         PassingUserName()   const;
    void            Shuffle()                ;

    uint            GetTotalScore()     const;
    uint            GetUserScore()      const;
    void            SaveTestResultInFile(const QString&) const;

    uint            GetAskingQuestionCount() const;
    void            SetAskingQuestionCount(const uint&);

    Mode            mode()              const;

signals:
    void            CurrentQuestionChanged();


private:
    QList<Question> questions;
    Question*       currentQuestion;
    uint            currentQuestionNumInList;
    QString         theme;
    QString         passingUserName;
    uint            askingQuestionsCount;
    Mode            _mode;


    void            LoadFromJSON(const QString &testFilePath);
};

#endif // TEST_H
