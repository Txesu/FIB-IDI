#include "MyQLineEdit.h"
MyQLineEdit::MyQLineEdit(QWidget *parent): QLineEdit(parent)
{
}
void MyQLineEdit::tractaReturn()
{
    emit returnPressed(text());
}
