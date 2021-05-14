#include "myxmlstruct.h"
#include <QDebug>

void MyXMLStruct::Book::InitSelf(QDomElement &e)
{
    QDomNodeList list = e.childNodes();
    name = list.at(0).toElement().text();
    author = list.at(1).toElement().text();
    editor = list.at(2).toElement().text();
    version = list.at(3).toElement().text();
}

void MyXMLStruct::Book::WriteSelf()
{
    qDebug()<<"book name = "<<name<<endl;
    qDebug()<<"book author = "<<author<<endl;
    qDebug()<<"book editor = "<<editor<<endl;
    qDebug()<<"book version = "<<version<<endl;
}

MyXMLStruct::Book::Book(QString bookName)
    :name(bookName){}

void MyXMLStruct::ChapterNote::InitSelf(QDomElement &e)
{
    QDomNodeList list = e.childNodes();
    chapterName = list.at(0).toElement().text();
    chNote = list.at(1).toElement().text();
}

void MyXMLStruct::ChapterNote::WriteSelf()
{
    qDebug()<<"chapter name = "<<chapterName<<endl;
    qDebug()<<"chNote = "<<chNote<<endl;
}

MyXMLStruct::ChapterNote::ChapterNote(QString chapterName)
    :chapterName(chapterName){}

void MyXMLStruct::ChapterNotes::InitSelf(QDomElement &e)
{
    QDomNodeList list = e.childNodes();
    for(int i=0;i<list.count();++i){
        ChapterNote chapterNote;
        QDomElement subElem = list.at(i).toElement();
        chapterNote.InitSelf(subElem);
        noteVec.push_back(chapterNote);
    }
}

void MyXMLStruct::ChapterNotes::WriteSelf()
{
    for(auto&i:noteVec)
        i.WriteSelf();
}

void MyXMLStruct::Note::InitSelf(QDomElement &e)
{
    QDomNodeList list = e.childNodes();
    QDomElement bookElem = list.at(0).toElement();
    book.InitSelf(bookElem);
    learnTime = list.at(1).toElement().text().toInt();
    QDomElement noteElem = list.at(2).toElement();
    notes.InitSelf(noteElem);
    myFeeling = list.at(3).toElement().text();
}

void MyXMLStruct::Note::WriteSelf()
{
    book.WriteSelf();
    qDebug()<<"learn time = "<<learnTime<<endl;
    notes.WriteSelf();
    qDebug()<<"my feeling = "<<myFeeling<<endl;
}

MyXMLStruct::Note::Note(QString bookName)
    :book(bookName),learnTime(0){}

void MyXMLStruct::NoteSystem::InitSelf(QDomElement &e)
{
    QDomNodeList list = e.childNodes();
    for(int i=0;i<list.count();++i){
        Note tempNote;
        QDomElement tempElem = list.at(i).toElement();
        tempNote.InitSelf(tempElem);
        noteVec.push_back(tempNote);
    }
}

void MyXMLStruct::NoteSystem::WriteSelf()
{
    for(auto&i:noteVec)
        i.WriteSelf();
}
