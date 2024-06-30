#include "mainwindow.h"
#include <QMessageBox>
#include "faq.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QMessageBox::question(this, "", "Have a nice day!", QMainWindow::close()); //уведомление "вопрос" при закрытии, но на самом деле вопроса нет :)
}


void MainWindow::on_pushButton_2_clicked()
{
    ui->lineEdit->setText("Trying connection...");
    ui->lineEdit_2->setText("Server is not alive");
    ui->lineEdit->setText("Connection faled");
    ui->textEdit->setText("Что-то отправляется на сервер");
    ui->textEdit_2->setText("Что-то с сервера приходит");
}


void MainWindow::on_pushButton_3_clicked()
{
    faq = new FAQ(this); //выделяем память под новое окно this - текущий класс будет родительским
    faq -> show(); //показываем созданное новое окно
}

