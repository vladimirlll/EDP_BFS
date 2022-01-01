#include "testing.h"
#include "ui_testing.h"

Testing::Testing(QWidget *p) :
    QWidget(p),
    ui(new Ui::Testing)
{
    ui->setupUi(this);
    connect(ui->testwgt, &TestWgt::BackClicked, this, &Testing::ShowMenu);
    ShowMenu();
}

void Testing::on_test_clicked()
{
    ui->testwgt->show();
    ui->testResults->hide();
    ui->seeResults->hide();
    ui->test->hide();
}

void Testing::on_seeResults_clicked()
{
    ui->testwgt->hide();
    ui->testResults->show();
    ui->seeResults->show();
    ui->test->show();
    QVector<TestResult> testResultsVec = GetTestResults();
    if(testResultsVec.empty()) ui->testResults->setText("Пока что никто не тестировался...");
    else if(testResultsVec.size() == 1 && testResultsVec[0].totalScore == 0 && testResultsVec[0].userScore == 1)
        ui->testResults->setText("При чтении результатов тестирования что-то пошло не так...\n"
                                 "Попробуйте удалить файл с результатами тестирования testResults.dat");
    else
    {
        uint resultsCount = testResultsVec.size();
        for(uint i = 0; i < resultsCount; ++i)
        {
            ui->testResults->append(QString::fromStdString(testResultsVec[i].name) + " набрал " +
                                    QString::number(testResultsVec[i].userScore) + " баллов из " +
                                    QString::number(testResultsVec[i].totalScore) + " возможных.\n");
        }
    }

}

void Testing::ShowMenu()
{
    ui->testResults->hide();
    ui->testwgt->hide();
    ui->seeResults->show();
    ui->test->show();
}

QVector<TestResult> Testing::GetTestResults()
{
    QVector<TestResult> vec;
    QFile file(QApplication::applicationDirPath() + "/testResults.dat");
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
                TestResult badStatusRes = {1, 0, "a"};
                QVector<TestResult> badStatusVec(1);
                badStatusVec.push_back(badStatusRes);
                return badStatusVec;
            }
            else vec.push_back(someResult);
        }
    }
    file.close();
    return vec;
}

Testing::~Testing()
{
    delete ui;
}


