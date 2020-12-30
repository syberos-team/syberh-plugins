#ifndef APP_P_H
#define APP_P_H

#include <QObject>

#include "iplugin/iplugin.h"

class AppPrivate : public QObject
{
    Q_OBJECT
public:
    AppPrivate(ExtensionSystem::SignalManager *signalManager, QObject *parent = nullptr);
    ~AppPrivate();

    void setAppOrientation(const QString &callbackID, const QVariantMap &params);

    void quit();

private:
    ExtensionSystem::SignalManager *m_signalManager;
};

#endif // APP_P_H