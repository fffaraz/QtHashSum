#ifndef DUPLICATEDIALOG_H
#define DUPLICATEDIALOG_H

#include <QDialog>

namespace Ui {
class DuplicateDialog;
}

class DuplicateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DuplicateDialog(const QList<QString> &pathlist, QString dir, QWidget *parent = nullptr);
    ~DuplicateDialog();

private:
    Ui::DuplicateDialog *ui;

private slots:
    void remove_clicked();
};

#endif // DUPLICATEDIALOG_H
