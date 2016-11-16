#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <QFileDialog>

SettingsDialog::SettingsDialog(QString currentFolder, bool aktionsliste, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    connect(ui->browseButton , SIGNAL(clicked()), this, SLOT(browse()));
    folder = currentFolder;
    ui->folderInput->setText(folder);
    ui->checkBox->setChecked(aktionsliste);
}

void SettingsDialog::browse()
{
    QString destination;
    while(!QDir(destination).exists() || destination.isEmpty()) {
        destination = QFileDialog::getExistingDirectory(this, "Select destionation folder", ".",
                                                                        QFileDialog::ShowDirsOnly
                                                                        | QFileDialog::DontResolveSymlinks);
    }
    folder = destination;

    ui->folderInput->setText(folder);
}

QString SettingsDialog::getFolder()
{
    return folder;
}

bool SettingsDialog::getAktionslisteEnabled()
{
    return ui->checkBox->isChecked();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}
