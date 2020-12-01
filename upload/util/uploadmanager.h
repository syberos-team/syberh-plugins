#ifndef UPLOADMANAGER_H
#define UPLOADMANAGER_H
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
#include <QHttpMultiPart>


class UploadManager : public QObject
{
    Q_OBJECT
public:
    UploadManager();
    ~UploadManager();

    //设置UploadId
    void setUploadId(const QString &uploadId);

    // 开始上传文件
    void uploadFile(const QVariantMap &params);

    // 停止下载工作
    void stopWork();

private:
    QNetworkAccessManager *m_networkManager;
    QNetworkReply *m_reply;

    bool m_isStop;

    QString m_uploadId;

    qint64 m_bytesTotal;

    QFile *m_file;

    QHttpMultiPart *m_multiPart;

signals:

    void signalStarted(const QString &uploadId);

    void signalUploadProcess(const QString &uploadId, qint64 bytesReceived, qint64 bytesTotal);

    //
    void signalReplyFinished(const QString &uploadId);

    void signalError(const QString &uploadId, qint64 statusCode, const QString &error);

public slots:
    // 上传进度信息
    void onUploadProgress(qint64 bytesReceived, qint64 bytesTotal);
    // 上传完成
    void onFinished();
    // 下载过程中出现错误，关闭下载，并上报错误，这里未上报错误类型，可自己定义进行上报;
    void onError(QNetworkReply::NetworkError code);
};

#endif // UPLOADMANAGER_H
