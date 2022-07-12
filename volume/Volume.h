#ifndef VOLUME_H
#define VOLUME_H

#include "volume_global.h"

#include <QObject>
#include <QtPlugin>
#include "iplugin/iplugin.h"


class VOLUME_EXPORT Volume: public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.syberos.syberh.SyberhPlugin" FILE "plugin.json")


public:
    Volume();

    //插件接受JS SDK调用方法,需要实现该方法，处理插件逻辑
    void invoke(const QString &callbackID, const QString &actionName, const QVariantMap &params);

    //音量
    void setVolume(const QString &callbackID, const QVariantMap &params);

    //获取音量
    void getVolume(const QString &callbackID, const QVariantMap &params);
};

#endif // VOLUME_H
