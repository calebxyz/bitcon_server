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

void cargWindow::show()
{
    m_exit = false;
    ui->lineEdit->setText("0");
    QDialog::show();

    while (!m_exit)
    {
        std::this_thread::sleep_for(std::chrono::duration<double>(0.1));
    }
}

void cargWindow::on_pushButton_clicked()
{
    QString lineTxt(ui->lineEdit->text());

    bool ok(false);

    lineTxt.toLongLong(&ok);

    if (ok)
    {
        m_blocksPromis.set_value(std::move(std::move(lineTxt)));
    }
    else
    {
        m_blocksPromis.set_value("0");
    }

    m_exit = true;

    QDialog::hide();
}
