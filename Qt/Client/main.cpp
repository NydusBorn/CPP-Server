#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QDebug>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>

class MyForm : public QWidget
{

public:
    MyForm(QWidget *parent = nullptr) : QWidget(parent)
    {
        QVBoxLayout *layoutV = new QVBoxLayout(this);
        QHBoxLayout *layoutH = new QHBoxLayout(this);


        QLabel *label1 = new QLabel(this);
        label1->setText("Статус: В активном поиске коннекта");
        layoutV->addWidget(label1);

        QLabel *label2 = new QLabel(this);
        label2->setText("Ошибки: Ошибок не обнаружено");
        layoutV->addWidget(label2);

        QSpacerItem* VSpacer = new QSpacerItem(10,20,QSizePolicy::Minimum);//, QSizePolicy::Expanding); - нужно для пропорционального масштабирования при растягивании окна
        layoutV->addItem(VSpacer);

        QHBoxLayout *textLayout = new QHBoxLayout;
        QLabel *label3 = new QLabel(this);
        label3->setText("Сообщение на сервер: ");
        layoutV->addWidget(label3);
        QLabel *label4 = new QLabel(this);
        label4->setText("Сообщение от сервера: ");
        layoutV->addWidget(label4);
        textLayout->addWidget(label3);
        textLayout->addWidget(label4);
        layoutV->addLayout(textLayout);


        textEdit1 = new QTextEdit();
        textEdit1->setPlainText("");
        layoutH->addWidget(textEdit1);
        textEdit2 = new QTextEdit();
        textEdit2->setPlainText("");
        layoutH->addWidget(textEdit2);
        layoutV->addLayout(layoutH);

        QSpacerItem* VSpacerButton = new QSpacerItem(2,4,QSizePolicy::Minimum);//, QSizePolicy::Expanding); - нужно для пропорционального масштабирования при растягивании окна
        layoutV->addItem(VSpacerButton);

        QHBoxLayout *buttonLayout = new QHBoxLayout;
        QPushButton *faqButton = new QPushButton("F.A.Q.");
        startButton = new QPushButton("Start");
        startButton->setObjectName("startButton");
        QPushButton *exitButton = new QPushButton("Exit");
        buttonLayout->addWidget(faqButton);
        buttonLayout->addWidget(startButton);
        buttonLayout->addWidget(exitButton);
        layoutV->addLayout(buttonLayout);
        startButton->installEventFilter(this);//фильтр события для кнопки Start
    }

protected:
    bool eventFilter(QObject *obj, QEvent *event) override
        {
            if (obj == startButton && event->type() == QEvent::MouseButtonPress)
            {
            textEdit1->setPlainText("Технические характеристики BMW M5 седан F90:\nДвигатель: бензиновый, 8 цилиндров, 4 клапана на цилиндр, 4395 см³, V-образный.\nМаксимальная мощность: 625 л. с.\nМаксимальный крутящий момент: 750 Н∙м.\nКоробка передач: автоматическая, 8 передач.\nПривод: полный.\nМаксимальная скорость: 250 км/ч.\nВремя разгона до 100 км/ч: 3,3 с.\nРасход топлива: смешанный цикл — 10,8 л/100 км, городской цикл — 15,2 л/100 км, загородный цикл — 8,7 л/100 км.\nСтрана производства — Германия.");
            textEdit2->setPlainText("BMW M5 стал на днях самым быстрым серийным седаном SIC.\n«Эмка» проехала круг за 2 минуты 22,828 секунды. Нам это, конечно, ни о чем не говорит, потому что мы не знаем даже, кто был «четырехдверным лидером» до М5. Известно, что абсолютный рекорд этой трассы — 1 минута 31,68 секунды и его установил Льюис Хэмилтон на болиде Ф-1 Mercedes AMG F1 W08 EQ Power+.");
             }
            return QWidget::eventFilter(obj, event);
        }


private:
    QPushButton *startButton;
    QTextEdit *textEdit1;
    QTextEdit *textEdit2;
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyleSheet("QPushButton#startButton{background-color: salmon;color:bisque;}");
    MyForm form;
    form.show();
    return a.exec();
}
