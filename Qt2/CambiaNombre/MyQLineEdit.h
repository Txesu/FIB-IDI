#include <QLineEdit>

class MyQLineEdit: public QLineEdit //el slot que volem heredar
{
    Q_OBJECT
public:
        MyQLineEdit(QWidget *parent);

public slots:
    void tractaReturn();
signals:
    void returnPressed(const QString &);
};
