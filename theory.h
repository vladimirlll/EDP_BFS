#ifndef THEORY_H
#define THEORY_H

#include <QWidget>
#include <QFile>
#include <QDebug>

class QWebEngineView;
class QPushButton;

class Theory : public QWidget
{
    Q_OBJECT

public:
    explicit Theory(QWidget *parent = nullptr);
    static QString FromUrlToFilePath(QUrl url);
    static int GetPageNumFromFilePath(QString filePath);
    static QString GetPagePathFromPageNum(int pageNum);
    ~Theory();

private slots:
    void on_btnToNextPage_clicked();
    void on_btnToPrevPage_clicked();
    void on_btnToSubsectionStart_clicked();
    void on_btnToContent_clicked();
    void on_btnOpenDictionary_clicked();
    void on_urlChanged(const QUrl &url);

private:
    int GetCurrentSubsectionPageNum();
    void GetDefinitionsPages();
    void GetSubsectionsPages();
    void Go();
    QPushButton* btnToNextPage;
    QPushButton* btnToPrevPage;
    QPushButton* btnToContent;
    QPushButton* btnToSubsectionStart;
    QPushButton* btnOpenDictionary;
    QWebEngineView* webView;
    QString filePath;
    int currentSubsectionPageNum;
    QVector<QVector<QString>> definitionsPages;     //в первой строке - названия определений, во второй - номера страниц, на которых они расположены
    QVector<QVector<QString>> subsectionsPages;     //в первой строке - названия подразделов, во второй - номера страниц, на которых они расположены
    uint currentPageNum;
};

#endif // THEORY_H
