#include "qextract.h"
#include <QDebug>
#include <QDir>

QExtract::QExtract(QString extractTo, QString archive, int atype)
{
    extractPath = extractTo;
    archiveFile = archive;
    extractProcess = new QProcess(this);
    archiveType = atype;

    connect(extractProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readOutputSlot()));
    connect(extractProcess, SIGNAL(readyReadStandardError()), this, SLOT(readErrorSlot()));
    connect(extractProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(finished(int,QProcess::ExitStatus)));
    #ifdef Q_OS_LINUX
        extractBinaryPath = "";
    #elif defined(Q_OS_WINDOWS)
        extractBinaryPath = QDir::currentPath() + "/Packer/";
    #endif
}

void QExtract::start()
{
    QStringList parameters;
    QString basePath(extractPath);
    extractProcess->setWorkingDirectory(extractPath);

    if(archiveType == ArchiveType::ZipArchive) {
        parameters << "-o" << archiveFile << "-d" << extractPath;
        extractProcess->start(QString(extractBinaryPath + "unzip"), parameters);
    } else if(archiveType == ArchiveType::RarArchive) {
        //-o+ tells unrar to automatically overwrite.
        //-p- tells unrar to not prompt for password. Not sure what the outcome will be. think is will probably just end maybe with error?
        parameters << "e" << "-o+" << "-p-" << archiveFile << basePath;
        extractProcess->start(QString("unrar"), parameters);
    }


    qDebug() << "extract launch: " << archiveFile;



    if (!extractProcess->waitForStarted(500000))
        qDebug() << "process didn't start in QExtract::start";
    if (logFile.isOpen())
        logFile.close();
    logFile.setFileName(extractPath + "/extractOutput.txt");
    logFile.open(QIODevice::WriteOnly);
}

void QExtract::finished(int exitCode, QProcess::ExitStatus exitStatus)
{
    logFile.close();
    qDebug() << "inside QExtract::finished. exitCode is "<< exitCode << " exitStatus is " << exitStatus;
    emit endExtractSignal();
}

void QExtract::readOutputSlot()
{
    while (extractProcess->canReadLine())
    {
        QByteArray line(extractProcess->readLine());
        //    qDebug() << line;
        if ((!line.contains('%')) && (!line.trimmed().isEmpty()))
        {
            line = line.trimmed();
            logFile.write(line);
            logFile.write("\n");
            qDebug() << line;
        }
    }
}

void QExtract::readErrorSlot()
{
    qDebug() << "error doing extract";
    qDebug() << extractProcess->readAllStandardError();
    if (extractProcess->state() == QProcess::Running)
    {
        qDebug() << "attempting to terminate qunrarprocess.";
        extractProcess->terminate();
    }
}
