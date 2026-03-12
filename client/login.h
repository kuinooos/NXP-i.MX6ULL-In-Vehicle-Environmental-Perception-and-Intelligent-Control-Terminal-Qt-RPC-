#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QtSql/QSqlDatabase>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

    void initDatabase();

private slots:
    void on_pushButton_clicked();

    void on_lineEdit_textEdited(const QString &arg1);

    void on_lineEdit_textChanged(const QString &arg1);

    void on_lineEdit_2_textChanged(const QString &arg1);

private:
    Ui::Login *ui;
    QSqlDatabase db;

    QString username;
    QString password;
};

#endif // LOGIN_H
