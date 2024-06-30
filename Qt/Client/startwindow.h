#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class StartWindow;
}

class StartWindow : public QDialog
{
    Q_OBJECT

public:
    explicit StartWindow(QWidget *parent = nullptr);
    ~StartWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::StartWindow *ui;
    MainWindow *mainwindow;
};

#endif // STARTWINDOW_H
