#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThreadPool>

#include "filehasher.h"
#include "progressdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnBrowse_clicked();
    void on_btnStart_clicked();
    void on_btnBrowseDir_clicked();
    void on_btnStartDir_clicked();
    void on_cmbThreads_currentIndexChanged(const QString &arg1);

private:
    Ui::MainWindow *ui = nullptr;
    int maxThreadCount = 0;
};

#endif // MAINWINDOW_H
