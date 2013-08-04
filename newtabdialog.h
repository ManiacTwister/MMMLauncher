#ifndef NEWTABDIALOG_H
#define NEWTABDIALOG_H

#include <QDialog>
#include "Parsers/categoryparser.h"
#include "Parsers/authorparser.h"

namespace Ui {
class NewTabDialog;
}

class NewTabDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit NewTabDialog(AuthorParser *authors, CategoryParser *categories, QWidget *parent = 0);
    ~NewTabDialog();
    int getTabType();
    int getCategory();
    int getAuthor();
    static const int CatgeoryTab = 1;
    static const int AuthorTab = 2;
    
private:
    Ui::NewTabDialog *ui;

private slots:
    void tabTypeChanged(int index);
};

#endif // NEWTABDIALOG_H
