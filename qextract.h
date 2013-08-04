#ifndef QEXTRACT_H
#define QEXTRACT_H

#include <QProcess>
#include <QFile>

class ArchiveType
{
public:
    static const int RarArchive = 0;
    static const int ZipArchive = 1;
};

class QExtract : public QObject
{
    Q_OBJECT
public:
    QExtract(QString extractTo, QString archive, int atype);

    public slots:
    void start();
    void finished(int exitCode, QProcess::ExitStatus exitStatus);
    void readOutputSlot();
    void readErrorSlot();

signals:
    void endExtractSignal();
    void UpdateProgressSignal(int PercentComplete);

private:
    QProcess *extractProcess;
    QFile logFile;
    QString extractPath;
    QString archiveFile;
    int archiveType;
    QString extractBinaryPath;
};

#endif // QEXTRACT_H
