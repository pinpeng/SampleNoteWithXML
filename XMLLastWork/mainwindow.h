#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QtXml>

#include "myxmlstruct.h"
#include "xmlfile.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //构造函数
    MainWindow(QWidget *parent = nullptr);
    //析构函数
    ~MainWindow();
private:
    Ui::MainWindow *ui;
    //XML文件读写系统
    XMLFile xmlFile;
    //笔记管理系统
    MyXMLStruct::NoteSystem noteSys;
private:
    //将所有connect函数放入一个函数中
    void ConnectEvent();
    void InitUI();
    void ResetUI(int bookIndex);
    void ClearUI();
    void closeEvent(QCloseEvent* event);
private slots:
    void OnAddBookClicked();
    void OnDelBookClicked();
    void OnAddChapterClicked();
    void OnDelChapterClicked();

    void OnBookListClicked();
    void OnChapterListClicked();

    void OnBookNameChanged();
    void OnBookAuthorChanged();
    void OnBookEditChanged();
    void OnBookVersionChaned();
    void OnSaveMyFeelingClicked();
    void OnSaveChNoteClicked();

    void OnFrontChapterClicked();
    void OnNextChapterClicked();

    void OnBookDoubleClicked();
    void OnBookListBookNameChanged();

    void OnChapterDoubleClicked();
    void OnChaterNameChanged();
};
#endif // MAINWINDOW_H
