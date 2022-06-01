#include <QLabel>

class MyQLabel: public QLabel //el slot que volem heredar
{
    Q_OBJECT
public:
        MyQLabel(QWidget *parent);
        int numero_enters = 0;
public slots:
    void CuentaEnters();
};
