#include "faq.h"

FAQ::FAQ(QWidget *parent)
        : QDialog(parent), ui(new Ui::FAQ) {
    ui->setupUi(this);
}

FAQ::~FAQ() {
    delete ui;
}
