#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QString currentFolder, QWidget *parent = 0);
    QString getFolder();
    ~SettingsDialog();

private:
    QString folder;
    Ui::SettingsDialog *ui;

private slots:
    void browse();
};

#endif // SETTINGSDIALOG_H
