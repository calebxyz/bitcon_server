#ifndef COMMANDS_H
#define COMMANDS_H

#include <QDialog>
#include "servermanager.h"
#include "showresp.h"
#include <utility>
#include <iostream>
#include <QComboBox>

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

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

private:

    bool runCommand(const uint32_t ind, const QString& cmd, const QString& args, QString& reslt);

    void initCombo(QComboBox* combo, CServerManager::TServTable& data);

    template <typename TFunc, typename TRet = decltype((std::declval<TFunc>())()), typename... TArgs>
    TRet execute(int32_t ind, TFunc exe, TRet badVal, TArgs&&... args)
    {
        TRet rv(badVal);

        if (ind > -1)
        {
           rv = exe(ind, std::forward<TArgs>(args)...);
        }

        return rv;
    }



    Ui::CCommands *ui;

    CServerManager& m_serverMng = CServerManager::getReference();
    int m_index = -1;
    CShowResp m_respShower;
};

#endif // COMMANDS_H
