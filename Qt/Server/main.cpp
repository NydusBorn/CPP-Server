#include <QApplication>
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <string>
#include <QPainter>
#include "../../lib/ConnectorSimple.cpp"
#include <iostream>
#include <nlohmann/json.hpp>
#include <QPen>
#include <QTimer>
#include <cmath>





class MyServer : public QWidget
{

public:
    MyServer(QWidget *parent = nullptr) : QWidget(parent)
    {

        port = 4000;
        thread1 =  4;
        server = std::make_unique<ConnectorSimple>(port,thread1);

        layout = new QVBoxLayout(this);

        label = new QLabel(this);
        label->setText("Введи port: ");
        label->setStyleSheet("QLabel { font-weight: bold; color: salmon;font-size: 130%;}");
        layout->addWidget(label);

        std::string port_string = std::to_string(port);
        const QString &port_const = QString::fromStdString(port_string);
        lineEdit = new QLineEdit(this);
        lineEdit->setText(port_const);
        layout->addWidget(lineEdit);
        
        label2 = new QLabel(this);
        label2->setText("Введи количество threads: ");
        label2->setStyleSheet("QLabel { font-weight: bold; color: salmon;font-size: 130%;}");
        layout->addWidget(label2);

        std::string threads_string = std::to_string(thread1);
        const QString &threads_const = QString::fromStdString(threads_string);
        lineEdit2 = new QLineEdit(this);
        lineEdit2->setText(threads_const);
        layout->addWidget(lineEdit2);

        startButt = new QPushButton("Start",this);
        startButt->setObjectName("startButt");
        startButt->installEventFilter(this);
        startButt->setStyleSheet("QPushButton{background-color: salmon;color:bisque;}");
        layout->addWidget(startButt);

        ipLabel = new QLabel(this);
        idLabel = new QLabel(this);
        uniqID = new QLabel(this);
        uniqIP = new QLabel(this);
        layout->addWidget(uniqIP);
        layout->addWidget(uniqID);
        layout->addWidget(ipLabel);
        layout->addWidget(idLabel);
        uniqID->hide();
        uniqIP->hide();

    }


protected:
    bool eventFilter(QObject *obj, QEvent *event) override
        {
           
            if (obj == startButt && event->type() == QEvent::MouseButtonPress)
            { 
                port = (lineEdit->text()).toInt();
                thread1 = (lineEdit2->text()).toInt();
                
                lineEdit2->hide();
                lineEdit->hide();
                label->hide();
                label2->hide();
                this->resize(400,200);
                idLabel->setText("Port: "+lineEdit->text());
                ipLabel->setText("Threads: "+lineEdit2->text());
                uniqID->show();
                uniqIP->show();
                startButt->hide();

                timer = new QTimer(this);
                connect(timer, &QTimer::timeout, this, &MyServer::updateUniqueIDs);
                timer->start(500);//раз в пол секунды обновляем значение 
            }
            return QWidget::eventFilter(obj, event);
        }
private slots:
    void updateUniqueIDs()
    {
        int uniqint = server->getUniqueIDs();
        std::string uniqID_int = std::to_string(uniqint);
        const QString& uniqID_const = QString::fromStdString(uniqID_int);
        uniqID->setText("UniqID count: " + uniqID_const);

        int uniqintIP = server->getUniqueAddresses();
        std::string uniqIP_int = std::to_string(uniqintIP);
        const QString &uniqIP_const = QString::fromStdString(uniqIP_int);
        uniqIP->setText("UniqIP count: " + uniqIP_const);

    }
private:
    QPushButton *startButt;
    int port;
    int thread1;
    QLabel *label;
    QLabel *label2;
    QLineEdit *lineEdit2;
    QLineEdit *lineEdit;
    QTimer* timer;
    QVBoxLayout *layout;
    QLabel *uniqID;
    QPainter *painter;
    QPen *pen;
    QLabel *uniqIP;
    QLabel *ipLabel;
    QLabel *idLabel;
    std::unique_ptr<ConnectorSimple> server;

};


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyServer server;
    server.show();
    return a.exec();
}
