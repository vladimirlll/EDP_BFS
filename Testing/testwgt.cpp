#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonParseError>
#include <QRandomGenerator>
#include "testwgt.h"
#include <stdexcept>
#include <QMessageBox>
#include "ui_testwgt.h"

TestWgt::TestWgt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestWgt)
{
    ui->setupUi(this);
    connect(ui->btnBack, SIGNAL(clicked()), SIGNAL(BackClicked()));
    rbtns = new QRadioButton*[9];
    rbtns[0] = ui->rBtn_1; rbtns[1] = ui->rBtn_2; rbtns[2] = ui->rBtn_3;
    rbtns[3] = ui->rBtn_4; rbtns[4] = ui->rBtn_5; rbtns[5] = ui->rBtn_6;
    rbtns[6] = ui->rBtn_7; rbtns[7] = ui->rBtn_8; rbtns[8] = ui->rBtn_9;
    ui->lEdName->hide(); ui->btnOk->hide();
    QString filePath = QApplication::applicationDirPath() + "/testing.json";
    test = new Test(filePath, Test::onlyThemeQuestions);
    test->Shuffle();
    connect(test, SIGNAL(CurrentQuestionChanged()), SLOT(OutCurrentQuestion()));
    OutCurrentQuestion();
}

TestWgt::~TestWgt()
{
    delete ui;
}

void TestWgt::on_btnToNextQ_clicked()
{
    if(test->CurrentQuestion()->GetType() == "Закрытый")
        test->CurrentQuestion()->SetUserAnswer(ui->rbtnsGroup->checkedButton()->text());
    else
        test->CurrentQuestion()->SetUserAnswer(ui->lEdUserAnswer->text());
    test->ToNextQuestion();
}


void TestWgt::on_btnToPrevQ_clicked()
{
    if(test->CurrentQuestion()->GetType() == "Закрытый")
        test->CurrentQuestion()->SetUserAnswer(ui->rbtnsGroup->checkedButton()->text());
    else
        test->CurrentQuestion()->SetUserAnswer(ui->lEdUserAnswer->text());
    test->ToPrevQuestion();
}

void TestWgt::on_btnOk_clicked()
{
    ui->lblQueLvl->hide();
    ui->lEdName->hide();
    ui->btnOk->hide();
    ui->tEditQues->show();
    QString userName = ui->lEdName->text();
    test->SetPassingUserName(userName);
    QString filePath = QApplication::applicationDirPath() + "/testResults.dat";
    try
    {
        test->SaveTestResultInFile(filePath);
        QString testFinishingText = "Тестирование завершено.\nВы получили " + QString::number(test->GetUserScore()) +
                " баллов из " + QString::number(test->GetTotalScore()) + " возможных";
        ui->tEditQues->setText(testFinishingText);
    }
    catch(std::runtime_error &ex)
    {
        QMessageBox msg(QMessageBox::Icon::Critical, "Ошибка чтения файла", ex.what());
        msg.exec();
    }
}

void TestWgt::on_btnFinish_clicked()
{
    //в случае, если пользователь нажимает "Закончить тестирование" на последнем вопросе, то ответ на этот вопрос
    if(test->CurrentQuestion()->GetType() == "Закрытый")
        test->CurrentQuestion()->SetUserAnswer(ui->rbtnsGroup->checkedButton()->text());
    else
        test->CurrentQuestion()->SetUserAnswer(ui->lEdUserAnswer->text());
    ui->btnToNextQ->hide();
    ui->btnToPrevQ->hide();
    ui->btnFinish->hide();
    ui->lblQueLvl->setText("Введите свое имя");
    ui->tEditQues->hide();
    for (uint i = 0; i < 9; ++i) rbtns[i]->hide();
    ui->lEdUserAnswer->hide();
    ui->lEdName->show(); ui->btnOk->show();
}

void TestWgt::OutCurrentQuestion()
{
    Question *currentQ = test->CurrentQuestion();
    ui->lblQueLvl->setText("Уровень сложности: " + currentQ->GetDifficultyLevel());
    ui->tEditQues->setText(currentQ->GetText());
    for (uint i = 0; i < 9; ++i) rbtns[i]->hide();
    ui->lEdUserAnswer->hide();
    if(currentQ->GetType() == "Закрытый") //если вопрос закрытый, то выводим варианты ответов
    {
        QList<QString> answerVariants = currentQ->GetAnswerVariants();
        uint variantsCount = answerVariants.size();
        for(uint j = 0; j < variantsCount; ++j)
        {
            rbtns[j]->setText(answerVariants[j]);
            rbtns[j]->show();
        }
        QString userAnswer = currentQ->GetUserAnswer();
        if(!userAnswer.isEmpty()) //если пользователь отвечал уже на этот вопрос, то отметить его ответ
        {
            uint i = 0;
            while(i < 9 && rbtns[i]->text() != userAnswer) ++i;
            if(i < 9) rbtns[i]->setChecked(true);
        }
    }
    else
    {
        ui->lEdUserAnswer->show();
        ui->lEdUserAnswer->setPlaceholderText("Введите ответ");
    }
}
