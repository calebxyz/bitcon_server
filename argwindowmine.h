#ifndef ARGWINDOW_H
#define ARGWINDOW_H

#include <QDialog>
#include <future>

namespace Ui {
class cargWindow;
}

class cargWindow : public QDialog
{
    Q_OBJECT

public:
    explicit cargWindow(QWidget *parent = 0);
    ~cargWindow();

    void show();

    inline QString getBlocks()
    {
        auto future = m_blocksPromis.get_future();
        future.wait();
        return future.get();
    }

private slots:
    void on_pushButton_clicked();

private:
    Ui::cargWindow *ui;
    std::promise<QString> m_blocksPromis;
    bool m_exit = false;
};

#endif // ARGWINDOW_H
