#include <QApplication>
#include <QComboBox>
#include "QWindow"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWindow w;
    w.show();
    return a.exec();
}
