#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QDebug>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QString>
#include <iostream>
#include <QTextEdit>
#include "../../lib/ConnectorSimple.cpp"

class MyForm : public QWidget
{

public:
    MyForm(QWidget *parent = nullptr) : QWidget(parent)
    {

        setWindowTitle("Granit-Client");

        QVBoxLayout *layoutV = new QVBoxLayout(this);
        QHBoxLayout *layoutH = new QHBoxLayout(this);

        ip = "127.0.0.1";
        port = 4000;

        QSpacerItem* VSpacer = new QSpacerItem(10,20,QSizePolicy::Minimum);//, QSizePolicy::Expanding); - нужно для пропорционального масштабирования при растягивании окна
        layoutV->addItem(VSpacer);

        QHBoxLayout *textLayout = new QHBoxLayout;
        label3 = new QLabel(this);
        label3->setText("Сообщение на сервер: ");
        label4 = new QLabel(this);
        label4->setText("Сообщение от сервера: ");
        label3->setStyleSheet("QLabel { font-weight: bold; color: salmon;font-size: 130%;}");
        label4->setStyleSheet("QLabel { font-weight: bold; color: salmon;font-size: 130%;}");
        textLayout->addWidget(label3);
        textLayout->addWidget(label4);
        layoutV->addLayout(textLayout);


        textEdit1 = new QTextEdit();
        textEdit1->setPlainText("");
        layoutH->addWidget(textEdit1);
        textEdit2 = new QTextEdit();
        textEdit2->setPlainText("");
        textEdit2->setReadOnly(true);
        layoutH->addWidget(textEdit2);
        layoutV->addLayout(layoutH);

        QSpacerItem* VSpacerButton = new QSpacerItem(2,4,QSizePolicy::Minimum);//, QSizePolicy::Expanding); - нужно для пропорционального масштабирования при растягивании окна
        layoutV->addItem(VSpacerButton);

        QHBoxLayout *buttonLayout = new QHBoxLayout;
        faqButton = new QPushButton(" ");
        faqButton->setObjectName("faqButton");
        startButton = new QPushButton("Отправить");
        startButton->setObjectName("startButton");
        exitButton = new QPushButton("Выйти");
        exitButton->setObjectName("exitButton");
        buttonLayout->addWidget(faqButton);
        buttonLayout->addWidget(startButton);
        buttonLayout->addWidget(exitButton);
        layoutV->addLayout(buttonLayout);
        startButton->installEventFilter(this);//фильтр события для кнопки Start
        faqButton->installEventFilter(this);//фильтр события для кнопки F.A.Q.
        exitButton->installEventFilter(this);//фильтр события для кнопки Exit
        startButton->setStyleSheet("QPushButton{background-color: salmon;color:bisque;}");
        faqButton->setStyleSheet("QPushButton{background-color: salmon;color:bisque;}");
        exitButton->setStyleSheet("QPushButton{background-color: salmon;color:bisque;}");

        faqButton->hide();
        startButton->hide();
        exitButton->hide();
        textEdit1->hide();
        textEdit2->hide();
        label3->hide();
        label4->hide();

        startButton2 = new QPushButton(this);
        startButton2->setText("Start");
        startButton2->setObjectName("startButton2");
        startButton2->installEventFilter(this);
        labelIp = new QLabel(this);
        labelPort = new QLabel(this);
        labelIp->setText("IP: ");
        labelPort->setText("Port: ");
        QHBoxLayout *hlay2 = new QHBoxLayout(this);

        const QString &ip_const = QString::fromStdString(ip);
        TIP = new QLineEdit(this);
        TIP->setText(ip_const);

        std::string port_string = std::to_string(port);
        const QString &port_const = QString::fromStdString(port_string);
        TPort = new QLineEdit(this);
        TPort->setText(port_const);

        hlay2->addWidget(labelIp);
        hlay2->addWidget(TIP);
        hlay2->addWidget(labelPort);
        hlay2->addWidget(TPort);
        layoutV->addWidget(startButton2);
        layoutV->addLayout(hlay2);

    }

protected:
    bool eventFilter(QObject *obj, QEvent *event) override
        {
           
            if (obj == startButton && event->type() == QEvent::MouseButtonPress)
            { 
                    ConnectorSimple client("127.0.0.1", 4000);
                    std::map<std::string, std::string> data2 = {
                        {"message", "hello"}
                    };
                    nlohmann::json data = data2;   
                    const QString &port_const = QString::fromStdString(client.makeRequest(data.dump()));
                    textEdit2->setText(port_const);
            }
            else if (obj == faqButton && event->type() == QEvent::MouseButtonPress)
            {

            }
            else if (obj == exitButton && event->type() == QEvent::MouseButtonPress)
            {
                this->close();
            }
            else if (obj == startButton2 && event->type() == QEvent::MouseButtonPress)
            {
                startButton2->hide();
                TIP->hide();
                TPort->hide();
                labelIp->hide();
                labelPort->hide();
                this->resize(600,400);
                faqButton->show();
                startButton->show();
                exitButton->show();
                textEdit1->show();
                textEdit2->show();
                label3->show();
                label4->show();
                port = (TPort->text()).toInt();
                ip = (TIP->text()).toStdString();
                
            }
            return QWidget::eventFilter(obj, event);
        }


private:
    QPushButton *startButton;
    QPushButton *startButton2;
    QPushButton *faqButton;
    QPushButton *exitButton;
    QTextEdit *textEdit1;
    QTextEdit *textEdit2;
    QWidget *FaqWindow;
    QLabel *FaqLabel;
    QLabel *label3;
    QLabel *label4;
    QLabel *labelIp;
    QLabel *labelPort;
    QLineEdit *TIP;
    QLineEdit *TPort;
    int port;
    std::string ip;

};


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyForm form;
    form.show();
    return a.exec();

}
