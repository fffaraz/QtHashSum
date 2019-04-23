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
    explicit DuplicateDialog(const QList<QString> &pathlist, QWidget *parent = nullptr);
    ~DuplicateDialog();

private:
    Ui::DuplicateDialog *ui;
};

#endif // DUPLICATEDIALOG_H
