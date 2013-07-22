#ifndef MMMLUTILS_H
#define MMMLUTILS_H

#include <QObject>

QT_BEGIN_NAMESPACE
class QPrinter;
class QStatusBar;
class QWidget;
template <class T> class QList;
QT_END_NAMESPACE

class MMMLUtils : public QObject
{
    Q_OBJECT
public:
    explicit MMMLUtils();
    virtual ~MMMLUtils();
    static QString buildCompatibilityString();

};

#endif // MMMLUTILS_H
