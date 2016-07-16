#include "argwindowmine.h"
#include "ui_argwindowmine.h"

cargWindow::cargWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cargWindow)
{
    ui->setupUi(this);
}

cargWindow::~cargWindow()
{
    delete ui;
}

void cargWindow::show(std::function<bool (const QString&)> func, QString lable)
{
    m_callback = std::move(func);
    ui->label->setText(lable);
    ui->lineEdit->setText("0");
    QDialog::show();
}

void cargWindow::on_pushButton_clicked()
{
    QString lineTxt(ui->lineEdit->text());

    bool ok(false);

    lineTxt.toLongLong(&ok);

    if (!ok)
    {
        lineTxt = "0";
    }

    m_callback(lineTxt);

    QDialog::hide();
}
