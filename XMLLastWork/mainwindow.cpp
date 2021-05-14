#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCloseEvent>

#include "mymicro.h"

using namespace MyXMLStruct;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    xmlFile.ReadNoteSystemFromXML(noteSys);
    ConnectEvent();
    InitUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ConnectEvent()
{
    connect(ui->addButton,&QPushButton::clicked,this,&MainWindow::OnAddBookClicked);
    connect(ui->deleteButton,&QPushButton::clicked,this,&MainWindow::OnDelBookClicked);
    connect(ui->addChapterButton,&QPushButton::clicked,this,&MainWindow::OnAddChapterClicked);
    connect(ui->delChapterButton,&QPushButton::clicked,this,&MainWindow::OnDelChapterClicked);

    connect(ui->bookName,&QLineEdit::editingFinished,this,&MainWindow::OnBookNameChanged);
    connect(ui->bookAuthor,&QLineEdit::editingFinished,this,&MainWindow::OnBookAuthorChanged);
    connect(ui->bookEditor,&QLineEdit::editingFinished,this,&MainWindow::OnBookEditChanged);
    connect(ui->bookVersion,&QLineEdit::editingFinished,this,&MainWindow::OnBookVersionChaned);

    connect(ui->saveMyFeelingButton,&QPushButton::clicked,this,&MainWindow::OnSaveMyFeelingClicked);
    connect(ui->saveChNoteButton,&QPushButton::clicked,this,&MainWindow::OnSaveChNoteClicked);

    connect(ui->bookList,&QListWidget::clicked,this,&MainWindow::OnBookListClicked);
    connect(ui->chapterList,&QListWidget::clicked,this,&MainWindow::OnChapterListClicked);

    connect(ui->frontChapterButton,&QPushButton::clicked,this,&MainWindow::OnFrontChapterClicked);
    connect(ui->nextChapterButton,&QPushButton::clicked,this,&MainWindow::OnNextChapterClicked);

    connect(ui->bookList,&QListWidget::doubleClicked,this,&MainWindow::OnBookDoubleClicked);
    connect(ui->bookList,&QListWidget::itemChanged,this,&MainWindow::OnBookListBookNameChanged);

    connect(ui->chapterList,&QListWidget::doubleClicked,this,&MainWindow::OnChapterDoubleClicked);
    connect(ui->chapterList,&QListWidget::itemChanged,this,&MainWindow::OnChaterNameChanged);
}

void MainWindow::InitUI()
{
    for(auto& i:noteSys.noteVec){
        ui->bookList->addItem(i.book.name);
    }
}

void MainWindow::ResetUI(int bookIndex)
{
    //设置书籍信息
    ui->bookName->setText(noteSys.noteVec[bookIndex].book.name);
    ui->bookAuthor->setText(noteSys.noteVec[bookIndex].book.author);
    ui->bookEditor->setText(noteSys.noteVec[bookIndex].book.editor);
    ui->bookVersion->setText(noteSys.noteVec[bookIndex].book.version);
    //设置章节笔记
    ui->chapterList->clear();
    for(auto& i:noteSys.noteVec[bookIndex].notes.noteVec){
        ui->chapterList->addItem(i.chapterName);
    }
    //设置读后感
    ui->myFeeling->setText(noteSys.noteVec[bookIndex].myFeeling);
    //清空笔记框
    ui->bookMarkEdit->clear();
}

void MainWindow::ClearUI()
{
    ui->bookName->clear();
    ui->bookAuthor->clear();
    ui->bookEditor->clear();
    ui->bookVersion->clear();

    ui->chapterList->clear();

    ui->myFeeling->clear();

    ui->bookMarkEdit->clear();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    xmlFile.WriteNoteSystemToXML(noteSys);

    event->accept();
}

void MainWindow::OnAddBookClicked()
{
    QString tempStr = "新建图书";
    Note tempNote(tempStr);
    noteSys.noteVec.push_back(tempNote);
    ui->bookList->addItem(tempStr);
}

void MainWindow::OnDelBookClicked()
{
    int index = ui->bookList->currentRow();

    if(index<0) return;

    auto it = noteSys.noteVec.begin()+index;
    noteSys.noteVec.erase(it);

    auto bklsit = ui->bookList->currentItem();
    ui->bookList->removeItemWidget(bklsit);
    delete bklsit;

    ClearUI();
}

void MainWindow::OnAddChapterClicked()
{
    int nowBookIndex = ui->bookList->currentRow();
    if(nowBookIndex<0) return;

    QString tempName = "新建章节";
    ChapterNote tempNote(tempName);
    noteSys.noteVec[nowBookIndex].notes.noteVec.push_back(tempNote);
    ui->chapterList->addItem(tempName);
}

void MainWindow::OnDelChapterClicked()
{
    //有效性判断
    int nowBookIndex = ui->bookList->currentRow();
    if(nowBookIndex<0) return;
    int nowChapterIndex = ui->chapterList->currentRow();
    if(nowChapterIndex<0) return;
    //从数据中删除
    auto it = noteSys.noteVec[nowBookIndex].notes.noteVec.begin()+nowChapterIndex;
    noteSys.noteVec[nowBookIndex].notes.noteVec.erase(it);
    //从gui中删除
    auto chIt = ui->chapterList->currentItem();
    ui->chapterList->removeItemWidget(chIt);
    delete chIt;

    ui->bookMarkEdit->clear();
}

void MainWindow::OnBookListClicked()
{
    if(ui->bookList->currentRow()<0) return;
    ResetUI(ui->bookList->currentRow());
}

void MainWindow::OnChapterListClicked()
{
    int nowIndex = ui->bookList->currentRow();
    if(nowIndex<0) return;
    int nowChapterIndex = ui->chapterList->currentRow();
    if(nowChapterIndex<0) return;

    ui->bookMarkEdit->clear();
    ui->bookMarkEdit->setText(noteSys.noteVec[nowIndex].notes.noteVec[nowChapterIndex].chNote);
}

void MainWindow::OnBookNameChanged()
{
    int nowIndex = ui->bookList->currentRow();
    if(nowIndex<0) return;

    QString nowName = ui->bookName->text();
    if(noteSys.noteVec[nowIndex].book.name==nowName) return;
    noteSys.noteVec[nowIndex].book.name=nowName;

    ui->bookList->currentItem()->setText(nowName);
}

void MainWindow::OnBookAuthorChanged()
{
    int nowIndex = ui->bookList->currentRow();
    if(nowIndex<0) return;

    QString nowAuthor = ui->bookAuthor->text();
    if(noteSys.noteVec[nowIndex].book.author==nowAuthor) return;
    noteSys.noteVec[nowIndex].book.author=nowAuthor;
}

void MainWindow::OnBookEditChanged()
{
    int nowIndex = ui->bookList->currentRow();
    if(nowIndex<0) return;

    QString nowEdit = ui->bookEditor->text();
    if(noteSys.noteVec[nowIndex].book.editor==nowEdit) return;
    noteSys.noteVec[nowIndex].book.editor=nowEdit;
}

void MainWindow::OnBookVersionChaned()
{
    int nowIndex = ui->bookList->currentRow();
    if(nowIndex<0) return;

    QString nowVersion = ui->bookVersion->text();
    if(noteSys.noteVec[nowIndex].book.version==nowVersion) return;
    noteSys.noteVec[nowIndex].book.version=nowVersion;
}

void MainWindow::OnSaveMyFeelingClicked()
{
    int nowIndex = ui->bookList->currentRow();
    if(nowIndex<0) return;

    QString nowStr = ui->myFeeling->toPlainText();
    noteSys.noteVec[nowIndex].myFeeling = nowStr;
}

void MainWindow::OnSaveChNoteClicked()
{
    int nowIndex = ui->bookList->currentRow();
    if(nowIndex<0) return;
    int nowChapterIndex = ui->chapterList->currentRow();
    if(nowChapterIndex<0) return;

    QString nowStr = ui->bookMarkEdit->toPlainText();
    noteSys.noteVec[nowIndex].notes.noteVec[nowChapterIndex].chNote=nowStr;
}

void MainWindow::OnFrontChapterClicked()
{
    int bookIndex = ui->bookList->currentRow();
    if(bookIndex<0) return;
    int nowChapterIndex = ui->chapterList->currentRow();
    if(nowChapterIndex<0) return;
    if(nowChapterIndex<1) return;

    ui->chapterList->setCurrentRow(nowChapterIndex-1);
    ui->bookMarkEdit->clear();
    ui->bookMarkEdit->setText(noteSys.noteVec[bookIndex].notes.noteVec[nowChapterIndex-1].chNote);
}

void MainWindow::OnNextChapterClicked()
{
    int bookIndex = ui->bookList->currentRow();
    if(bookIndex<0) return;

    int nowChapterIndex = ui->chapterList->currentRow();
    if(nowChapterIndex<0) return;
    if(nowChapterIndex==ui->chapterList->count()-1) return;

    ui->chapterList->setCurrentRow(nowChapterIndex+1);
    ui->bookMarkEdit->clear();
    ui->bookMarkEdit->setText(noteSys.noteVec[bookIndex].notes.noteVec[nowChapterIndex+1].chNote);
}

void MainWindow::OnBookDoubleClicked()
{
    if(ui->bookList->currentRow()<0) return;

    auto it = ui->bookList->currentItem();
    it->setFlags(it->flags()|Qt::ItemIsEditable);
}

void MainWindow::OnBookListBookNameChanged()
{
    int bookIndex = ui->bookList->currentRow();
    if(bookIndex<0) return;

    QString nowName = ui->bookList->currentItem()->text();

    noteSys.noteVec[bookIndex].book.name = nowName;

    ui->bookName->setText(nowName);
}

void MainWindow::OnChapterDoubleClicked()
{
    if(ui->bookList->currentRow()<0) return;
    if(ui->chapterList->currentRow()<0) return;
    auto it = ui->chapterList->currentItem();
    it->setFlags(it->flags()|Qt::ItemIsEditable);
}

void MainWindow::OnChaterNameChanged()
{
    int bookIndex = ui->bookList->currentRow();
    if(bookIndex<0) return;
    int chapterIndex = ui->chapterList->currentRow();
    if(chapterIndex<0) return;

    noteSys.noteVec[bookIndex].notes.noteVec[chapterIndex].chapterName = ui->chapterList->currentItem()->text();
}
