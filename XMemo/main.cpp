#include "XMemo.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    XMemo w;
    w.show();

    return a.exec();
}
