#include "login.h"
#include "ui_login.h"
#include <QFile>
#include <QDebug>
#include <QtSql/QSqlError>
#include <QMessageBox>
#include <QtSql/QSqlQuery>

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    // 连接数据库并检查是否成功
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("server.db");

    if (!db.open()) {
       qDebug() << "数据库连接失败：" << db.lastError().text();
    } else {
       qDebug() << "数据库连接成功！";
    }
}

Login::~Login()
{
    delete ui;
}

void Login::on_pushButton_clicked()
{
    // 登录验证逻辑
    QSqlQuery query;
    QString sql = QString("SELECT * FROM users WHERE username = '%1' AND password = '%2'")
                    .arg(username).arg(password);

    if (!query.exec(sql)) {
        qDebug() << "查询失败：" << query.lastError().text();
        return;
    }

    if (query.next()) {
        // 登录成功
        QMessageBox::information(this, "欢迎回家", "信息匹配成功!", QMessageBox::Ok);

        this->hide();
    } else {
        QMessageBox::warning(this, "登录失败", "用户名或密码错误！", QMessageBox::Ok | QMessageBox::Cancel);
    }
}



void Login::on_lineEdit_textChanged(const QString &arg1)
{
    username = arg1;
}

void Login::on_lineEdit_2_textChanged(const QString &arg1)
{
    password = arg1;
}

void Login::initDatabase()
{
    // 设置数据库连接
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("server.db");

    if (!db.open()) {
        qDebug() << "数据库连接失败：" << db.lastError().text();
        QMessageBox::warning(this, "登录失败", "用户名或密码错误！", QMessageBox::Ok | QMessageBox::Cancel);

        return;
    }

    // 创建用户表（如果不存在）
    QSqlQuery query(db);
    bool success = query.exec(
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "username TEXT UNIQUE NOT NULL, "
        "password TEXT NOT NULL)"
    );

    if (!success) {
        qDebug() << "创建表失败：" << query.lastError().text();
        QMessageBox::warning(this, "登录失败", "用户名或密码错误！", QMessageBox::Ok | QMessageBox::Cancel);

    }
}

void Login::on_lineEdit_textEdited(const QString &arg1)
{

}
