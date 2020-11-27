#ifndef IMAGE_H
#define IMAGE_H

#include <QObject>
#include <QtPlugin>

#include "iplugin/iplugin.h"
#include "image_global.h"
#include "qmlmanager.h"

using namespace NativeSdk;

class IMAGESHARED_EXPORT Image: public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.syberos.syberh.SyberhPlugin" FILE "plugin.json")

public:
    Q_INVOKABLE Image();

    void invoke(const QString &callbackID, const QString &action, const QVariantMap &params);

    void chooseImage(const QString &callbackID, const QVariantMap &params);
    void previewImage(const QString &callbackID, const QVariantMap &params);
    //base64 转 QImage
    QImage base64ToImg(const QString& str);

private:
    QmlManager qmlManager;
    QmlObject *chooseImageQml;
    QmlObject *previewQml;
    long globalCallbackID;


    /**
     * @brief openUrl 唤起应用,打开页面
     * @param callBackID 唤起应用的任务Id
     * @param filePath 文件路径
     * @return 成功则发送成功信号。
     *      失败则发送失败信号。
     */
    void saveImageToPhotosAlbum(const QString &callbackID, const QVariantMap &params);


    /**
     * @brief getImageInfo 获取图片的详细
     * @param callBackID 任务Id
     * @return 成功则发送成功信号。
     *      失败则发送失败信号。
     */
    void getImageInfo(const QString &callbackID, const QVariantMap &params);

public slots:
    void chooseCancel();
    void receiveUrls(const QString &filesPath);
    void cameraSuccess(const QString &filePath);
    void cameraCancel();
    void albumCancel();
    void previewImageSuccess();
};

#endif // IMAGE_H
