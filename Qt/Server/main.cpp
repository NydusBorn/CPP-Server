#include <QApplication>
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QComboBox>

int main(int argc, char *argv[])
{
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

    // Connector *con = new Connector(this);
    // label->setText(con.getUniqueAddresses);


    w.show();

    return a.exec();
}