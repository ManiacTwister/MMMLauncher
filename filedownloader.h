#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class FileDownloader : public QObject
{
    Q_OBJECT
public:
    explicit FileDownloader(QUrl url, QObject *parent = 0);

    virtual ~FileDownloader();

    QByteArray downloadedData() const;
    QNetworkReply *reply;

signals:
        void downloaded();

private slots:

    void fileDownloaded(QNetworkReply* pReply);

private:

    QNetworkAccessManager m_WebCtrl;

    QByteArray m_DownloadedData;

};

#endif // FILEDOWNLOADER_H
