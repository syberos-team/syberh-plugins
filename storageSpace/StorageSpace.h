#ifndef STORAGESPACE_H
#define STORAGESPACE_H

#include "storageSpace_global.h"

#include <QObject>
#include <QtPlugin>
#include "iplugin/iplugin.h"

class STORAGESPACE_EXPORT StorageSpace: public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.syberos.syberh.SyberhPlugin" FILE "plugin.json")

public:
    StorageSpace();

    //插件接受JS SDK调用方法,需要实现该方法，处理插件逻辑
    void invoke(const QString &callbackID, const QString &actionName, const QVariantMap &params);

    //查询系统存储信息
    void getSystemStorage(const QString &callbackID, const QVariantMap &params);

    //只读分区
    void getSystemRoot(const QString &callbackID, const QVariantMap &params);
    //数据区
    void getSystemData(const QString &callbackID, const QVariantMap &params);
    //内置存储卡
    void getIntStorage(const QString &callbackID, const QVariantMap &params);
    //外置存储卡
    void getExtStorage(const QString &callbackID, const QVariantMap &params);
    //内存
    void getMemory(const QString &callbackID, const QVariantMap &params);
};

#endif // STORAGESPACE_H
