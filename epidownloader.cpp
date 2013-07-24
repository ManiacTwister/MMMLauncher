#include "epidownloader.h"
#include <QDateTime>
#include <QUrl>
#include <QNetworkRequest>
#include <QDebug>

EpiDownloader::EpiDownloader(QString dlurl, QObject *parent):QNetworkAccessManager(parent)
{
    QUrl url(dlurl);

    connect(this, SIGNAL(finished(QNetworkReply*)), this, SLOT(fin(QNetworkReply*)));

    QNetworkRequest req(url);
    mCurrentReply = this->get(req);
    connect(mCurrentReply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(dlProgress(qint64,qint64)));
    downloadTime.start();
}

void EpiDownloader::fin(QNetworkReply * pReply)
{
    if(pReply->error() != QNetworkReply::NoError)
        qDebug() << pReply->errorString();

    if(pReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 307 || pReply->rawHeaderList().contains("Location"))
    {
        QNetworkRequest req(pReply->header(QNetworkRequest::LocationHeader).toString());
        filename = pReply->header(QNetworkRequest::LocationHeader).toString();
        mCurrentReply = this->get(req);
        connect(mCurrentReply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(dlProgress(qint64,qint64)));
        return;
    }

    m_DownloadedData = pReply->readAll();
    reply = pReply;
    //emit a signal
    pReply->deleteLater();
    emit downloaded();
}

void EpiDownloader::dlProgress(qint64 bytesReceived, qint64 bytesToal)
{
    // calculate the download speed
    double speed = bytesReceived * 1000.0 / downloadTime.elapsed();
    QString unit;
    if (speed < 1024) {
        unit = "bytes/sec";
    } else if (speed < 1024*1024) {
        speed /= 1024;
        unit = "kB/s";
    } else {
        speed /= 1024*1024;
        unit = "MB/s";
    }
    emit downloadProgress(bytesReceived, bytesToal, speed, unit);
}

QByteArray EpiDownloader::downloadedData() const
{
    return m_DownloadedData;
}
