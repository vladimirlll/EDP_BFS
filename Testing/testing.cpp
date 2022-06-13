#include "testing.h"
#include "ui_testing.h"
#include <stdexcept>
#include <QMessageBox>

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
    QString filePath = QApplication::applicationDirPath() + "/testResults.dat";
    try
    {
        vec = TestResult::ReadAllEntriesInFile(filePath);
    }
    catch (std::runtime_error &ex)
    {
        QMessageBox msg(QMessageBox::Icon::Critical, "Ошибка чтения файла", ex.what());
    }
    return vec;
}

Testing::~Testing()
{
    delete ui;
}


