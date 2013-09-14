#include "newtabdialog.h"
#include "ui_newtabdialog.h"
#include "Parsers/categoryparser.h"
#include "Parsers/authorparser.h"
#include <QDebug>

NewTabDialog::NewTabDialog(AuthorParser* authors, CategoryParser* categories, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewTabDialog)
{
    ui->setupUi(this);
    ui->tabType->addItem("");
    ui->tabType->addItem(tr("Category"), NewTabDialog::CatgeoryTab);
    ui->tabType->addItem(tr("Author"), NewTabDialog::AuthorTab);
    ui->stackedWidget->setCurrentIndex(2);
    connect(ui->tabType , SIGNAL(currentIndexChanged(int)),this,SLOT(tabTypeChanged(int)));

    foreach (Category* category, categories->categories) {
        if(category->getParentUid() == -1)
            continue;
        ui->category->addItem(category->getTitle(), category->getUid());
    }
    foreach (Author* author, authors->authors) {
        ui->author->addItem(author->getName(), author->getUid());
    }
}

NewTabDialog::~NewTabDialog()
{
    delete ui;
}

void NewTabDialog::tabTypeChanged(int index)
{
    if(ui->tabType->itemData(index).isNull()) {
        ui->stackedWidget->setCurrentIndex(2);
        return;
    }

    if(ui->tabType->itemData(index).toInt() == NewTabDialog::CatgeoryTab) {
        ui->stackedWidget->setCurrentIndex(0);
    } else if(ui->tabType->itemData(index).toInt() == NewTabDialog::AuthorTab) {
        ui->stackedWidget->setCurrentIndex(1);
    } else {
        ui->stackedWidget->setCurrentIndex(2);
    }

}

int NewTabDialog::getTabType()
{
    return ui->tabType->currentIndex();
}

int NewTabDialog::getCategory()
{
    return ui->category->itemData(ui->category->currentIndex()).toInt();
}

int NewTabDialog::getAuthor()
{
    return ui->author->itemData(ui->author->currentIndex()).toInt();
}
