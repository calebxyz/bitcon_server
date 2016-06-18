#include "bitcoinserver.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BitcoinServer w;
    w.show();

    return a.exec();
}
