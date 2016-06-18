#ifndef STARTSERVERS_H
#define STARTSERVERS_H

#include <QDialog>
#include "servermanager.h"

namespace Ui {
class StartServers;
}

class CStartServers : public QDialog
{
    Q_OBJECT

public:
    explicit CStartServers(QWidget *parent = 0);
    ~CStartServers();

private slots:
    void on_pushButton_clicked();

private:
    Ui::StartServers *ui;
};

#endif // STARTSERVERS_H