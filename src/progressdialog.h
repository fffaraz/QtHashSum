#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QProgressBar>
#include <QTimer>
#include <QElapsedTimer>

#include "filehasher.h"
#include "resultdialog.h"

struct ProgressData
{
    QProgressBar *pb = nullptr;
    QLabel *label = nullptr;
};

namespace Ui {
class ProgressDialog;
}

class ProgressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProgressDialog(QVector<FileHasher*> jobs, QWidget *parent = nullptr);
    ~ProgressDialog();

private:
    void timer_timeout();
    Ui::ProgressDialog *ui;
    QVector<FileHasher*> jobs;
    QVector<ProgressData> pds;
    QTimer timer;
    QElapsedTimer elapsedtimer;
};

#endif // PROGRESSDIALOG_H
