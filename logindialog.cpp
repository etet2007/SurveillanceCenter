#include "logindialog.h"
#include "ui_logindialog.h"

#include "QMessageBox.h"
#include "QTextCodec.h"
#include <QDebug>
//好用，可以显示中文。
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_pushButton_clicked()
{
//    qDebug()<<"accepted.";
    if(this->ui->usernameLineEdit->text().trimmed()== tr("admin") &&
            this->ui->passwordLineEdit->text().trimmed()==tr("admin"))
    {
        accept();
    }
    else
    {
        QMessageBox::warning(this,"警告!","用户名或密码错误！",QMessageBox::Yes);
        this->ui->usernameLineEdit->clear();
        this->ui->passwordLineEdit->clear();
        this->ui->usernameLineEdit->setFocus();
    }
}

void LoginDialog::on_pushButton_2_clicked()
{
    this->close();
}
