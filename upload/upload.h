#ifndef UPLOAD_H
#define UPLOAD_H

#include <QObject>
#include <QtPlugin>

#include "iplugin/iplugin.h"
#include "upload_global.h"
#include "util/uploadmanager.h"


class TaskInfo{
public:
    // 任务ID
    QString uploadID;
    // 下载
    UploadManager *uploadManager;
};

class UPLOADSHARED_EXPORT Upload: public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.syberos.syberh.SyberhPlugin" FILE "plugin.json")

public:
    Q_INVOKABLE Upload();

    void invoke(const QString &callbackID, const QString &action, const QVariantMap &params);

    //下载状态
    enum ProgressStatus {
        Started = 1,
        Downloading = 2,
        Completed = 3
    };

private:
    long globalCallbackID;
    // 上传
    void start(const QString &callbackID, const QVariantMap &params);

    // 取消
    void cancel(const QString &callbackID, const QVariantMap &params);

    // 删除指针
    void deleteTask(const QString &taskId);

    qint64 m_bytesTotal;

    bool m_error;

public slots:
    // 开始上传
    void onStarted(const QString &callBackID);

    // 上传进度信息
    void onUploadProgress(const QString &callBackID, qint64 bytesReceived, qint64 bytesTotal);

    // 上传完成
    void onFinished(const QString &callBackID);

    // 下载过程中出现错误，关闭下载，并上报错误，这里未上报错误类型，可自己定义进行上报;
    void onError(const QString &callBackID, qint64 statusCode, const QString &error);
};

#endif // UPLOAD_H
