#ifndef SETTINGSWIN_H
#define SETTINGSWIN_H

#include <QDialog>

namespace Ui {
class CSettingsWin;
}

class CSettingsWin : public QDialog
{
    Q_OBJECT

public:
    explicit CSettingsWin(QWidget *parent = 0);
    ~CSettingsWin();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_lineEdit_editingFinished();

private:
    Ui::CSettingsWin *ui;

    int m_numOfDockers;
};

#endif // SETTINGSWIN_H
