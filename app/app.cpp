#include "app.h"
#include "app_p.h"
#include "framework/common/projectconfig.h"
#include "qmlmanager.h"
#include <SyberosGuiCache>
#include <QDBusInterface>
#include <QDBusPendingReply>

using namespace NativeSdk;

const QString COMPOSITOR_IO_SERVICE ="com.syberos.iomanager";
const QString COMPOSITOR_IO_PATH ="/com/syberos/compositor/IOManager";
const QString COMPOSITOR_IO_INTERFACE ="com.syberos.compositor.IOManager";

// ====== AppPrivate ======
AppPrivate::AppPrivate(ExtensionSystem::SignalManager *signalManager, QObject *parent) 
    : QObject(parent), m_signalManager(signalManager)
{
}

AppPrivate::~AppPrivate()
{
    m_signalManager = nullptr;
}

void AppPrivate::setAppOrientation(const QString &callbackID, const QVariantMap &params)
{
    qDebug() << Q_FUNC_INFO << callbackID << params;

    QString curOrientation = params.value("orientation", "1").toString();

    QQuickView *m_view = SYBEROS::SyberosGuiCache::qQuickView();
    QList<QQuickItem*> webviews = m_view->findChildren<QQuickItem*>("webView");
    qDebug() << Q_FUNC_INFO << "webviews****" << webviews.size() << webviews;

    QmlManager qmlManager;

    for (int i = webviews.size()-1; i >= 0; i--) {
        qDebug() << Q_FUNC_INFO << "webview index****" << i;
        qmlManager.call(webviews.at(i), "setPageOrientation(" + curOrientation + ")");
    }
    m_signalManager->success(callbackID.toLong(), true);
}

void AppPrivate::quit()
{
    qDebug() << Q_FUNC_INFO << "即将退出应用..." ;

    ProjectConfig *projectConfig = ProjectConfig::instance();

    QString id = QString("%1/%2").arg(projectConfig->getSopid()).arg(projectConfig->getProjectName());
    QStringList list;
    list << id;

    QDBusInterface manager(COMPOSITOR_IO_SERVICE, COMPOSITOR_IO_PATH, COMPOSITOR_IO_INTERFACE, QDBusConnection::systemBus());
    QDBusPendingReply< void > reply = manager.call("quitApps", list);
    reply.waitForFinished();
    if (reply.isError()) {
        QDBusError err = reply.error();
        qWarning() << Q_FUNC_INFO << "quit failed:" << err.name() << err.message();
    }
}


// ====== App ======
App::App()
{
    d = new AppPrivate(signalManager(), this);
}

void App::invoke(const QString &callbackID, const QString &actionName, const QVariantMap &params)
{
    qDebug() << Q_FUNC_INFO << " callbackID:" << callbackID << "actionName:" << actionName << "params:" << params;

    if (actionName == "setAppOrientation") {
        d->setAppOrientation(callbackID, params);

    } else if (actionName == "quit") {
        d->quit();
    }
}
