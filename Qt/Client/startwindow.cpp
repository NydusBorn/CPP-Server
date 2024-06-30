#include "startwindow.h"
#include <QMessageBox>
#include "mainwindow.h"

StartWindow::StartWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::StartWindow)
{
    ui->setupUi(this);
}

StartWindow::~StartWindow()
{
    delete ui;
}

void StartWindow::on_pushButton_clicked()
{
    if (ui->textEdit->document()->isEmpty()){
        QMessageBox::information(this, "Внимание!","Вы не заполнили нужное поле");
    }
    else{
        close();
        mainwindow = new MainWindow(this);
        mainwindow -> show();
    }
}

