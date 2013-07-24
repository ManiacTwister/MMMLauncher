#ifndef EPIDOWNLOADER_H
#define EPIDOWNLOADER_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QString>
#include <QTime>

class EpiDownloader : public QNetworkAccessManager
{
    Q_OBJECT
public:
    explicit EpiDownloader(QString dlurl, QObject *parent = 0);
    QByteArray downloadedData() const;
    QNetworkReply *reply;
    QString filename;
    QTime downloadTime;

signals:
        void downloaded();
        void downloadProgress(qint64 bytesReceived, qint64 bytesToal, double speed, QString unit);

private slots:
    void fin(QNetworkReply *);
    void dlProgress(qint64 bytesReceived, qint64 bytesToal);

private:
    QByteArray m_DownloadedData;
    QNetworkReply* mCurrentReply;

};

#endif // EPIDOWNLOADER_H
