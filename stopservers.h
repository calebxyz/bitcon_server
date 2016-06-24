#ifndef STOPSERVERS_H
#define STOPSERVERS_H

#include <QDialog>

namespace Ui {
class CStopServers;
}

class CStopServers : public QDialog
{
    Q_OBJECT

public:
    explicit CStopServers(QWidget *parent = 0);
    ~CStopServers();

    virtual void show();

private slots:
    void on_pushButton_clicked();

private:
    Ui::CStopServers *ui;
};

#endif // STOPSERVERS_H
