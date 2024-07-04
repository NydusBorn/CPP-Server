#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QDebug>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <iostream>
#include <QTextEdit>
#include "../../lib/Connector.cpp"

class MyForm : public QWidget
{

public:
    MyForm(QWidget *parent = nullptr) : QWidget(parent)
    {

        setWindowTitle("Granit-Client");

        QVBoxLayout *layoutV = new QVBoxLayout(this);
        QHBoxLayout *layoutH = new QHBoxLayout(this);

        // QLabel *LabelError1 = new QLabel(this);
        // LabelError1->setText("Состояние подключения: ");
        // LabelError1->setStyleSheet("QLabel { font-weight: bold; color: salmon; font-size: 130%;}");
        // QLabel *LabelError2 = new QLabel(this);
        // LabelError2->setText("В активно поиске коннекта");
        // QHBoxLayout *errorLayout = new QHBoxLayout;
        // errorLayout->addWidget(LabelError1);
        // errorLayout->addWidget(LabelError2);
        // QLabel *label1 = new QLabel(this);
        // label1->setText("Статус: В активном поиске коннекта");
        // label1->setStyleSheet("QLabel{font-weight bolder;}");
        // layoutV->addLayout(errorLayout);

        // QLabel *label2 = new QLabel(this);
        // label2->setText("Ошибки: Ошибок не обнаружено");
        // layoutV->addWidget(label2);
        // QLabel *LabelError3 = new QLabel(this);
        // QLabel *LabelError4 = new QLabel(this);
        // LabelError3->setText("Ошибки: ");
        // LabelError4->setText("Ошибок не обнаружено");
        // LabelError3->setStyleSheet("QLabel { font-weight: bold; color: salmon;font-size: 130%;}");
        // QHBoxLayout *errorLayout2 = new QHBoxLayout;
        // errorLayout2->addWidget(LabelError3);
        // errorLayout2->addWidget(LabelError4);
        // layoutV->addLayout(errorLayout2);

        QSpacerItem* VSpacer = new QSpacerItem(10,20,QSizePolicy::Minimum);//, QSizePolicy::Expanding); - нужно для пропорционального масштабирования при растягивании окна
        layoutV->addItem(VSpacer);

        QHBoxLayout *textLayout = new QHBoxLayout;
        label3 = new QLabel(this);
        label3->setText("Сообщение на сервер: ");
        // layoutV->addWidget(label3);
        label4 = new QLabel(this);
        label4->setText("Сообщение от сервера: ");
        // layoutV->addWidget(label4);
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
        faqButton->setObjectName("startButton");
        startButton = new QPushButton("Отправить");
        startButton->setObjectName("faqButton");
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
        TIP = new QLineEdit(this);
        TPort = new QLineEdit(this);
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
                textEdit1->setPlainText("Технические характеристики BMW M5 седан F90:\nДвигатель: бензиновый, 8 цилиндров, 4 клапана на цилиндр, 4395 см³, V-образный.\nМаксимальная мощность: 625 л. с.\nМаксимальный крутящий момент: 750 Н∙м.\nКоробка передач: автоматическая, 8 передач.\nПривод: полный.\nМаксимальная скорость: 250 км/ч.\nВремя разгона до 100 км/ч: 3,3 с.\nРасход топлива: смешанный цикл — 10,8 л/100 км, городской цикл — 15,2 л/100 км, загородный цикл — 8,7 л/100 км.\nСтрана производства — Германия.");
                textEdit2->setPlainText("BMW M5 стал на днях самым быстрым серийным седаном SIC.\n«Эмка» проехала круг за 2 минуты 22,828 секунды. Нам это, конечно, ни о чем не говорит, потому что мы не знаем даже, кто был «четырехдверным лидером» до М5. Известно, что абсолютный рекорд этой трассы — 1 минута 31,68 секунды и его установил Льюис Хэмилтон на болиде Ф-1 Mercedes AMG F1 W08 EQ Power+.");
             }
            else if (obj == faqButton && event->type() == QEvent::MouseButtonPress)
            {
                // textEdit1->setPlainText(" Германия.");
                // textEdit2->setPlainText("BMW M5");

                // Создание нового пустого окна
                // QWidget *emptyWindow = new QWidget();
                // emptyWindow->resize(300,400);
                // emptyWindow->show();
            }
            else if (obj == exitButton && event->type() == QEvent::MouseButtonPress)
            {
                this->close();
            }
            else if (obj == startButton2 && event->type() == QEvent::MouseButtonPress)
            {
                // this->close();
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

};

class MyStartForm : public QWidget
{

public:
    MyStartForm(QWidget *parent = nullptr) : QWidget(parent)
    {

        setWindowTitle("Granit-Client");

        startButton = new QPushButton(this);
        startButton->setText("Start");
        startButton->setObjectName("startButton");
        QLabel *labelIp = new QLabel(this);
        QLabel *labelPort = new QLabel(this);
        labelIp->setText("IP: ");
        labelPort->setText("Port: ");
        QVBoxLayout *hlay = new QVBoxLayout(this);
        QHBoxLayout *hlay2 = new QHBoxLayout(this);
        QLineEdit *TIP = new QLineEdit(this);
        QLineEdit *TPort = new QLineEdit(this);
        hlay2->addWidget(labelIp);
        hlay2->addWidget(TIP);
        hlay2->addWidget(labelPort);
        hlay2->addWidget(TPort);
        hlay->addWidget(startButton);
        hlay->addLayout(hlay2);

    }

protected:
    bool eventFilter(QObject *obj, QEvent *event) override
        {
            if (obj == startButton && event->type() == QEvent::MouseButtonPress)
            {
                this->close();
            }

            return QWidget::eventFilter(obj, event);
        }


private:
    QPushButton *startButton;
    // QTextEdit *textEdit1;
    // QTextEdit *textEdit2;
    // QLabel *FaqLabel1;
    // QLabel *FaqLabel2;
};


void mesge(){
    std::map<std::string, std::string> data2 = {
        {"message", "hello"}
    };
    nlohmann::json data = data2;
    int port = 4000;
    Connector client("127.0.0.1",port);
    std::cout << client.makeRequest(data.dump()) << std::endl;
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyForm form;
    form.show();
    return a.exec();

}
