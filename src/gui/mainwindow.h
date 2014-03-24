#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QTimer>
#include "mycryptographichash.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnBrowse_clicked();
    void on_btnStart_clicked();
    void timer_timeout();
    void chash_finished();

private:
    Ui::MainWindow *ui;
    MyCryptographicHash *chash;
    QFile *file;
    QTimer timer;
};

#endif // MAINWINDOW_H
