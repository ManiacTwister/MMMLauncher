#include "filedownloader.h"

FileDownloader::FileDownloader(QUrl url, QObject *parent) :
    QObject(parent)
{
    connect(&m_WebCtrl, SIGNAL(finished(QNetworkReply*)),
                SLOT(fileDownloaded(QNetworkReply*)));

    QNetworkRequest request(url);
    m_WebCtrl.get(request);
}

FileDownloader::~FileDownloader()
{

}

void FileDownloader::fileDownloaded(QNetworkReply* pReply)
{
    reply = pReply;
    m_DownloadedData = pReply->readAll();
    //emit a signal
    pReply->deleteLater();
    emit downloaded();
}

QByteArray FileDownloader::downloadedData() const
{
    return m_DownloadedData;
}
