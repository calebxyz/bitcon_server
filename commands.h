#ifndef COMMANDS_H
#define COMMANDS_H

#include <QDialog>
#include "servermanager.h"

namespace Ui {
class CCommands;
}

class CCommands : public QDialog
{
    Q_OBJECT

public:
    explicit CCommands(QWidget *parent = 0);
    ~CCommands();

    //overides the widget show function
    void show();


private slots:
    void on_comboBox_activated(int index);

private:
    Ui::CCommands *ui;

    CServerManager& m_serverMng = CServerManager::getReference();
    int m_index = -1;
};

#endif // COMMANDS_H
