#ifndef APP_H
#define APP_H

#include <QObject>
#include <QtPlugin>

#include "iplugin/iplugin.h"
#include "app_global.h"

class APPSHARED_EXPORT App: public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.syberos.syberh.SyberhPlugin" FILE "plugin.json")

public:
    Q_INVOKABLE App();

    void invoke(const QString &callbackID, const QString &actionName, const QVariantMap &params);

    void setAppOrientation(const QString &callbackID, const QVariantMap &params);

    void  quit();
};

#endif // APP_H
