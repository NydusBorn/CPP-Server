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


class MyServer : public QWidget
{

public:
    MyServer(QWidget *parent = nullptr) : QWidget(parent)
    {

        port = 4000;
        thread1 =  4;
        server = std::make_unique<ConnectorSimple>(port,thread1);
        // std::unique_ptr<ConnectorSimple> server2 = std::make_unique<ConnectorSimple>(port,threads);

        QVBoxLayout *layout = new QVBoxLayout(this);

        QLabel *label = new QLabel(this);
        label->setText("Введи port: ");
        layout->addWidget(label);

        std::string port_string = std::to_string(port);
        const QString &port_const = QString::fromStdString(port_string);
        lineEdit = new QLineEdit(this);
        lineEdit->setText(port_const);
        layout->addWidget(lineEdit);
        
        QLabel *label2 = new QLabel(this);
        label2->setText("Введи количество threads: ");
        layout->addWidget(label2);

        std::string threads_string = std::to_string(thread1);
        const QString &threads_const = QString::fromStdString(threads_string);
        lineEdit2 = new QLineEdit(this);
        lineEdit2->setText(threads_const);
        layout->addWidget(lineEdit2);

        startButt = new QPushButton("Start",this);
        startButt->setObjectName("startButt");
        startButt->installEventFilter(this);
        layout->addWidget(startButt);

        QComboBox *comboBox = new QComboBox(this);
        layout->addWidget(comboBox);

        int uniqint = server->getUniqueIDs();
        uniqID = new QLabel(this);
        QLabel *uniqIP = new QLabel(this);
        std::string uniqID_int = std::to_string(uniqint);
        const QString &uniqID_const = QString::fromStdString(uniqID_int);
        uniqID->setText(uniqID_const);
        layout->addWidget(uniqID);

        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &MyServer::updateUniqueIDs);
        timer->start(1000);
        
    }

protected:
    bool eventFilter(QObject *obj, QEvent *event) override
        {
           
            if (obj == startButt && event->type() == QEvent::MouseButtonPress)
            { 
                port = (lineEdit->text()).toInt();
                thread1 = (lineEdit2->text()).toInt();
                // int pre_port = 4000;
                // port = &pre_port;
                // int pre_threads = 4;
                // threads = &pre_threads;
                // std::unique_ptr<ConnectorSimple> server2 = std::make_unique<ConnectorSimple>(port,threads);
            }
            return QWidget::eventFilter(obj, event);
        }
    //  void paintEvent(QPaintEvent* event) override {
    //     Q_UNUSED(event);
    //     QPainter *painter = new QPainter(this);
    //     painter->setRenderHint(QPainter::Antialiasing);

    //     QVector<double> timeValues = {0.0, 1.0, 2.0, 3.0}; // Время в секундах
    //     QVector<int> paramValues = {10, 20, 15, 30}; // Параметр request

    //     QRectF plotRect(20, 20, width() - 40, height() - 40);

    //     QPen pen(Qt::blue);
    //     pen.setWidth(2);
    //     painter->setPen(pen);

    //     // Строим график
    //     for (int i = 1; i < timeValues.size(); ++i) {
    //         QPointF p1(plotRect.left() + timeValues[i - 1] * plotRect.width(),
    //                     plotRect.bottom() - paramValues[i - 1] * plotRect.height() / 50.0);
    //         QPointF p2(plotRect.left() + timeValues[i] * plotRect.width(),
    //                     plotRect.bottom() - paramValues[i] * plotRect.height() / 50.0);
    //         painter->drawLine(p1, p2);
    //     }
    // }
private slots:
    void updateUniqueIDs()
    {
        int uniqint = server->getUniqueIDs();
        std::string uniqID_int = std::to_string(uniqint);
        const QString& uniqID_const = QString::fromStdString(uniqID_int);
        uniqID->setText("UniqID count: " + uniqID_const);
    }
private:
    QPushButton *startButt;
    int port;
    int thread1;
    QLineEdit *lineEdit2;
    QLineEdit *lineEdit;
    QTimer* timer;
    QLabel *uniqID;
    std::unique_ptr<ConnectorSimple> server;

};


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyServer server;
    server.show();
    return a.exec();
}