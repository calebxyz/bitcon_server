#ifndef COMMANDS_H
#define COMMANDS_H

#include <QDialog>

namespace Ui {
class CCommands;
}

class CCommands : public QDialog
{
    Q_OBJECT

public:
    explicit CCommands(QWidget *parent = 0);
    ~CCommands();

private:
    Ui::CCommands *ui;
};

#endif // COMMANDS_H
