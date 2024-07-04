#include <QApplication>
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <string>
#include "../../lib/Connector.cpp"
#include <iostream>
#include <nlohmann/json.hpp>


int main(int argc, char *argv[])
{
    int port = 4000;
    int threads = 4;
    Connector server(port,threads);
    std::unique_ptr<Connector> server2;

    QApplication a(argc, argv);

    QWidget w;
    QVBoxLayout *layout = new QVBoxLayout(&w);

    QLabel *label = new QLabel(&w);
    label->setText("Введи port: ");
    layout->addWidget(label);

    QLineEdit *lineEdit = new QLineEdit(&w);
    layout->addWidget(lineEdit);
    
    QLabel *label2 = new QLabel(&w);
    label2->setText("Введи количество threads: ");
    layout->addWidget(label2);

    QLineEdit *lineEdit2 = new QLineEdit(&w);
    layout->addWidget(lineEdit2);

    QPushButton *startButt = new QPushButton("Start",&w);
    layout->addWidget(startButt);

    QComboBox *comboBox = new QComboBox(&w);
    layout->addWidget(comboBox);

    QLabel *uniqID = new QLabel(&w);
    QLabel *uniqIP = new QLabel(&w);
    string check = server->getUniqueIDs();
    layout->addWidget(uniqID);


    w.show();

    return a.exec();
}