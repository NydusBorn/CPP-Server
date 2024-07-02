#include <QApplication>
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QComboBox>
#include <C:\Users\Alex\Desktop\CPP-Server-master\lib\Connector.cpp>
#include <iostream>
#include <nlohmann/json.hpp>


int main(int argc, char *argv[])
{
    Connector client("127.0.0.1", 4000);

    QApplication a(argc, argv);

    QWidget w;
    QVBoxLayout *layout = new QVBoxLayout(&w);

    QLabel *label = new QLabel(&w);
    label->setText("Количество активных пользователей: ");
    layout->addWidget(label);

    QLineEdit *lineEdit = new QLineEdit(&w);
    layout->addWidget(lineEdit);

    QComboBox *comboBox = new QComboBox(&w);
    layout->addWidget(comboBox);


    w.show();

    return a.exec();
}