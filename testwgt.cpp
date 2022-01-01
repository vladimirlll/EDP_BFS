#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonParseError>
#include <QRandomGenerator>
#include "testwgt.h"
#include "ui_testwgt.h"

void TestWgt::MixQuesFromFile()
{
    int qCount = QA[0].size(); //количество всех вопросов из базы вопросов
    QVector<int> indexes(qCount);
    for(int i = 0; i < qCount; ++i) indexes[i] = i;
    std::random_device rd; std::mt19937 g(rd());
    std::shuffle(indexes.begin(), indexes.end(), g);
    //Выбор случайных 5 вопросов и ответов
    QVector<QVector<QString>> QAMixed(4, QVector<QString>(5)); //перемешанный Testing::QA
    QVector<QVector<QString>> QueVariantsMixed(5);   //перемешанный вектор вариантов для вопросов
    QVector<QVector<QString>> QueThemesMixed(5);     //перемешанный вектор тем вопросов
    int askingQCount = qCount < 5 ? qCount : 5; //количество задаваемых вопросов на тесте
    for(int i = 0; i < askingQCount; ++i)  //добавление только 5 случайных вопросов на тест
    {
        QAMixed[0][i] = QA[0][indexes[i]];
        QAMixed[1][i] = QA[1][indexes[i]];
        QAMixed[2][i] = QA[2][indexes[i]];
        QAMixed[3][i] = QA[3][indexes[i]];
        QueVariantsMixed[i] = QueVariants[indexes[i]];
        QueThemesMixed[i] = QueThemes[indexes[i]];
    }
    QA = QAMixed; QueVariants = QueVariantsMixed; QueThemes = QueThemesMixed;
    for(int i = 0; i < askingQCount; ++i)
        std::shuffle(QueVariants[i].begin(), QueVariants[i].end(), g);  //перемешивание вариантов ответов для каждого вопроса
}

TestWgt::TestWgt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestWgt)
{
    ui->setupUi(this);
    connect(ui->btnBack, SIGNAL(clicked()), SIGNAL(BackClicked()));
    QA.resize(4);
    rbtns = new QRadioButton*[9];
    rbtns[0] = ui->rBtn_1; rbtns[1] = ui->rBtn_2; rbtns[2] = ui->rBtn_3;
    rbtns[3] = ui->rBtn_4; rbtns[4] = ui->rBtn_5; rbtns[5] = ui->rBtn_6;
    rbtns[6] = ui->rBtn_7; rbtns[7] = ui->rBtn_8; rbtns[8] = ui->rBtn_9;
    ui->lEdName->hide(); ui->btnOk->hide();
    QString filePath = QApplication::applicationDirPath() + "/testing.json";
    QFile file(filePath);
    if(file.exists())
    {
        if(file.open(QIODevice::ReadOnly))
        {
            GetQueAndAnsFromFile(filePath);
            MixQuesFromFile();
            qDebug() << "количество вопросов: " << QA[0].size() << "\n\n";
            if(!QA[0].empty())
            {
                index = 0; //обнуляем индекс текущего вопроса
                usersAnswers.resize(QA[0].size());
                OutQueAndAns(index); //выводим вопрос по текущему индексу (первый вопрос)
            }
        }
        else file.close();
    }
    else file.close();
}

TestWgt::~TestWgt()
{
    delete ui;
}

void TestWgt::on_btnToNextQ_clicked()
{
    if(QA[3][index] == "Закрытый") usersAnswers[index] = ui->rbtnsGroup->checkedButton()->text();
    else usersAnswers[index] = ui->lEdUserAnswer->text();
    if((index+1) < QA[0].size()) OutQueAndAns(++index);
}


void TestWgt::on_btnToPrevQ_clicked()
{
    if(QA[3][index] == "Закрытый") usersAnswers[index] = ui->rbtnsGroup->checkedButton()->text();
    else usersAnswers[index] = ui->lEdUserAnswer->text();
    if(index != 0) OutQueAndAns(--index);
}

void TestWgt::on_btnOk_clicked()
{
    ui->lblQueLvl->hide();
    ui->lEdName->hide();
    ui->btnOk->hide();
    ui->tEditQues->show();
    QString name = ui->lEdName->text();
    uint quesCount = QA[0].size();
    uint totalScore = 0, userScore = 0;
    qDebug() << usersAnswers << "\n\n";
    qDebug() << QA[1] << "\n\n";
    for(uint i = 0; i < quesCount; ++i)
    {
        totalScore+=QA[2][i].toUInt();
        if(usersAnswers[i] == QA[1][i]) userScore+=QA[2][i].toUInt();
    }
    TestResult result;
    strcpy(result.name, name.toStdString().c_str());    //число символов в ui->lEdName ограничено 50
    result.userScore = userScore; result.totalScore = totalScore;
    QString filePath = QApplication::applicationDirPath() + "/testResults.dat";
    QFile file(filePath);
    if(file.open(QIODevice::Append))
    {
        QDataStream strm(&file);
        strm.setVersion(QDataStream::Qt_5_9);
        strm << result;
        if(strm.status() != QDataStream::Ok)
            ui->tEditQues->setText("Произошла ошибка записи результатов тестирования в файл");
        else
        {
            QString testFinishingText = "Тестирование завершено.\nВы получили " + QString::number(userScore) +
                    " баллов из " + QString::number(totalScore) + " возможных";
            ui->tEditQues->setText(testFinishingText);
        }
    }
    file.close();
}

void TestWgt::on_btnFinish_clicked()
{
    //в случае, если пользователь нажимает "Закончить тестирование" на последнем вопросе, то ответ на этот вопрос
    if(QA[3][index] == "Закрытый") usersAnswers[index] = ui->rbtnsGroup->checkedButton()->text();
    else usersAnswers[index] = ui->lEdUserAnswer->text();
    ui->btnToNextQ->hide();
    ui->btnToPrevQ->hide();
    ui->btnFinish->hide();
    ui->lblQueLvl->setText("Введите свое имя");
    ui->tEditQues->hide();
    for (uint i = 0; i < 9; ++i) rbtns[i]->hide();
    ui->lEdUserAnswer->hide();
    ui->lEdName->show(); ui->btnOk->show();
}

void TestWgt::GetQueAndAnsFromFile(const QString &filePath)
{
    QFile file(filePath);
    if(file.exists())
    {
        if(file.open(QIODevice::ReadOnly))
        {
            QTextStream strm(&file);
            strm.setCodec("UTF-8");
            //testTheme = strm.readLine(); //получение темы тестирования
            //allQA = GetQues(strm);
            QString text = strm.readAll();
            file.close();

            QJsonParseError error;
            QJsonDocument doc = QJsonDocument::fromJson(text.toUtf8(), &error);
            qDebug() << "Error: " << error.errorString() << error.offset << error.error;

            if (doc.isObject())
            {
                QJsonObject json = doc.object();
                testTheme = json["testTheme"].toString();
                QJsonArray questions = json["questions"].toArray();
                uint queNum = 0;
                foreach (const QJsonValue & value, questions) // - итерирование по вопросам
                {
                    if (value.isObject()) // - каждый вопрос объект
                    {
                        QueThemes.resize(queNum+1);
                        QueVariants.resize(queNum+1);
                        QJsonObject question = value.toObject();
                        // - парсинг тем вопроса
                        QJsonArray questionThemes = question["themes"].toArray();
                        foreach(const QJsonValue & value, questionThemes)
                        {
                            if(value.isString()) QueThemes[queNum].push_back(value.toString());
                        }
                        if(QueThemes[queNum].contains(testTheme))
                        {
                            // - парсинг вида вопроса
                            if(question["type"].isString()) QA[3].push_back(question["type"].toString());
                            // - парсинг текста вопроса
                            if(question["text"].isString()) QA[0].push_back(question["text"].toString());
                            // - парсинг вариантов ответов
                            if(QA[3][queNum] == "Открытый") QueVariants[queNum].push_back("");
                            else
                            {
                                QJsonArray variantsArray;
                                if(question["variants"].isArray())
                                {
                                    variantsArray = question["variants"].toArray();
                                    foreach(const QJsonValue & value, variantsArray)
                                    {
                                        if(value.isString()) QueVariants[queNum].push_back(value.toString());
                                    }
                                }
                            }
                            // - парсинг правильного ответа
                            if(question["correctAnswer"].isString()) QA[1].push_back(question["correctAnswer"].toString());
                            // - парсинг уровня сложности вопроса
                            if(question["lvl"].isString()) QA[2].push_back(question["lvl"].toString());
                        }
                        else
                        {
                            QueThemes.pop_back();
                            queNum--;
                        }
                    }
                    qDebug() << QA[1] << "\n\n";
                    queNum++;
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

void TestWgt::OutQueAndAns(const uint &queNum)
{
    ui->lblQueLvl->setText("Уровень сложности: " + QA[2][queNum]);
    ui->tEditQues->setText(QA[0][queNum]);
    for (uint i = 0; i < 9; ++i) rbtns[i]->hide();
    ui->lEdUserAnswer->hide();
    if(QA[3][queNum] == "Закрытый") //если вопрос закрытый, то выводим варианты ответов
    {
        uint variantsCount = QueVariants[queNum].size();
        for(uint j = 0; j < variantsCount; ++j)
        {
            rbtns[j]->setText(QueVariants[queNum][j]);
            rbtns[j]->show();
        }
        if(!usersAnswers[queNum].isEmpty()) //если пользователь отвечал уже на этот вопрос, то отметить его ответ
        {
            uint i = 0;
            while(i < 9 && rbtns[i]->text() != usersAnswers[queNum]) ++i;
            if(i < 9) rbtns[i]->setChecked(true);
        }
    }
    else
    {
        ui->lEdUserAnswer->show();
        ui->lEdUserAnswer->setPlaceholderText("Введите ответ");
    }
}
