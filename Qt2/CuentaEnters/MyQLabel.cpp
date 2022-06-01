#include "MyQLabel.h"
MyQLabel::MyQLabel(QWidget *parent): QLabel(parent)
{
}
void MyQLabel::CuentaEnters()
{
    numero_enters++;
    
    setText(QString::number(numero_enters));
}
