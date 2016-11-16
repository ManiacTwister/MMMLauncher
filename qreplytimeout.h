#ifndef QREPLYTIMEOUT
#define QREPLYTIMEOUT
#include <QTimer>
#include <QNetworkReply>
/**
Usage:
new QReplyTimeout(yourReply, msec);

When the timeout is reached the given reply is closed if still running
*/
class QReplyTimeout : public QObject {
  Q_OBJECT
public:
  QReplyTimeout(QNetworkReply* reply, const int timeout) : QObject(reply) {
    Q_ASSERT(reply);
    if (reply) {
      QTimer::singleShot(timeout, this, SLOT(timeout()));
    }
  }

signals:
        void timedOut();
private slots:
  void timeout() {
    QNetworkReply* reply = static_cast<QNetworkReply*>(parent());
    if (reply->isRunning()) {
      reply->close();
    }
    emit timedOut();
  }
};
#endif // QREPLYTIMEOUT

