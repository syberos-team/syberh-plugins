#ifndef LOCATION_H
#define LOCATION_H

#include <QObject>
#include <QtPlugin>
#include "location_global.h"
#include "iplugin/iplugin.h"

#include <QDBusInterface>
#include <QGeoPositionInfoSource>
#include <QGeoSatelliteInfoSource>

class LOCATION_EXPORT Location: public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.syberos.syberh.SyberhPlugin" FILE "plugin.json")

public:
	Location();

    // 调用invoke前的初始化工作，该方法只会被调用一次
    void invokeInitialize();
    //插件接受JS SDK调用方法,需要实现该方法，处理插件逻辑
    void invoke(const QString &callbackID, const QString &actionName, const QVariantMap &params);

    /**
     * @brief currentPosition 系统是否打开GPS定位
     * @param params 无参数
     * @return 成功则返回true。
     *      失败则 false。
     */
    bool isOpen();

    /**
     * @brief currentPosition 返回当前位置
     * @param params 无参数
     * @return 成功则返回true。
     *      失败则返回错误码。
     */
    void currentPosition(const QString &callbackID, const QVariantMap &params);

    //初始化DBus
    void initDBusLocate();
    //初始化GPS
    void initPosSource();
    //初始化卫星数
    void initSatSource();

public slots:
     //位置更新槽函数
     void sltPositionUpdated(const QGeoPositionInfo &posInfo);
     //位置更新错误槽函数
     void sltPositionError(QGeoPositionInfoSource::Error enuPosError);
     //系统定位服务开关变化
     void sltSwitchChanged(bool bFlagLocate, bool bFlagGps, bool bFlagBase);

private:

     QDBusInterface *m_dbusLocate;
     bool m_flagLocate;
     bool m_flagGps;
     bool m_flagBase;

    QGeoPositionInfoSource *m_posSrc;
    //bool m_bLocateFlag;
    //位置相关信息
    QJsonObject m_jsObjPos;

    QGeoSatelliteInfoSource *m_satSrc;
    QList<int> m_satIDList;
    QList<int> m_satValList;

};

#endif // LOCATION_H
