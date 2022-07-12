#include "location.h"
#include <QDebug>
#include "framework/common/errorinfo.h"
using namespace NativeSdk;

Location::Location()
{

}

void Location::invokeInitialize()
{
    qDebug() << Q_FUNC_INFO << "Initialize";
    initDBusLocate();
    initPosSource();
    initSatSource();
}


void Location::invoke(const QString &callbackID, const QString &actionName, const QVariantMap &params)
{
    qDebug() << Q_FUNC_INFO << "  callbackID:" << callbackID << "actionName:" << actionName << "params:" << params;

    if(actionName == "currentPosition"){
        currentPosition(callbackID, params);
    }
}

//系统是否打开GPS定位
bool Location::isOpen()
{
    return  m_flagLocate & m_flagGps;
}


//返回当前位置
void Location::currentPosition(const QString &callbackID, const QVariantMap &params)
{
    qDebug() << Q_FUNC_INFO << "  callbackID:" << callbackID << "params:" << params;

    qDebug() << Q_FUNC_INFO << "Switch state" << isOpen();
    qDebug() << Q_FUNC_INFO << "m_jsObjPos isEmpty" << m_jsObjPos.isEmpty();
    if(!isOpen())
    {
        signalManager()->failed(callbackID.toLong(),ErrorInfo::InvalidParameter,"No openSwitch");
        return;
    }
    else if(m_jsObjPos.isEmpty())
    {
        signalManager()->failed(callbackID.toLong(),ErrorInfo::InvalidParameter,"Data isEmpty");
        return;
    }

    signalManager()->success(callbackID.toLong(), QVariant(m_jsObjPos));
}


void Location::initDBusLocate()
{
    m_dbusLocate = new QDBusInterface("org.freedesktop.GeoClue2",
                                       "/org/freedesktop/GeoClue2/Manager",
                                       "org.freedesktop.GeoClue2.Manager",
                                       QDBusConnection::sessionBus(),
                                       this);
    if(m_dbusLocate->isValid() != true)
    {
        qDebug() << Q_FUNC_INFO << "m_dbusLocate is not valid.";
        qDebug() << Q_FUNC_INFO << "try systembus.";
        m_dbusLocate = new QDBusInterface("org.freedesktop.GeoClue2",
                                       "/org/freedesktop/GeoClue2/Manager",
                                       "org.freedesktop.GeoClue2.Manager",
                                       QDBusConnection::systemBus(),
                                       this);
    }
    if(m_dbusLocate->isValid() != true)
    {
        qDebug() << Q_FUNC_INFO << "m_dbusLocate is not valid on systembus.";
        return;
    }
    //bool bDbusConn =  connect(m_dbusLocate,&QDBusInterface::SwitchChanged,this,&Location::sltSwitchChanged);

   bool bDbusConn =  QObject::connect(m_dbusLocate,
                     SIGNAL(SwitchChanged(bool, bool, bool)),
                     this,
                     SLOT(sltSwitchChanged(bool, bool, bool)));
    qDebug() << Q_FUNC_INFO << "bDbusConn : "<< bDbusConn;


    QDBusMessage reply = m_dbusLocate->call("GetSwitch");
    if(reply.type() == QDBusMessage::ErrorMessage)
    {
        qDebug() << Q_FUNC_INFO << "reply : error : " << reply.errorMessage();
        return;
    }
    if(reply.arguments().length() < 3)
    {
        qDebug() << Q_FUNC_INFO << "reply : param error";
        return;
    }

    m_flagLocate = reply.arguments().at(0).toBool();
    m_flagGps = reply.arguments().at(1).toBool();
    m_flagBase = reply.arguments().at(2).toBool();
    qDebug() << Q_FUNC_INFO << "flag:" << m_flagLocate << m_flagGps << m_flagBase;
}


//初始化GPS
void Location::initPosSource()
{
    QStringList srcList = QGeoPositionInfoSource::availableSources();
    qDebug() << Q_FUNC_INFO << "srcList : " << srcList;

    m_posSrc = QGeoPositionInfoSource::createDefaultSource(this);
    if(!m_posSrc)
    {
        qDebug() << Q_FUNC_INFO << "m_posSrc error";
        return;
    }

    m_posSrc->setPreferredPositioningMethods(QGeoPositionInfoSource::AllPositioningMethods);
    m_posSrc->setUpdateInterval(500);
    m_posSrc->startUpdates();
    bool bcon11 = connect(m_posSrc,&QGeoPositionInfoSource::positionUpdated,this,&Location::sltPositionUpdated);
    bool bcon22 = connect(m_posSrc,static_cast<void (QGeoPositionInfoSource::*)(QGeoPositionInfoSource::Error)>(&QGeoPositionInfoSource::error) ,this,&Location::sltPositionError);

    qDebug()<< Q_FUNC_INFO << "connect: " << bcon11 << bcon22;
}


//初始化卫星数
void Location::initSatSource()
{
    QStringList srcList = QGeoSatelliteInfoSource::availableSources();
    qDebug() << Q_FUNC_INFO << "QGeoSatelliteInfoSource : " << srcList;

    m_satSrc = QGeoSatelliteInfoSource::createDefaultSource(this);
    if(!m_satSrc)
    {
        qDebug()<< Q_FUNC_INFO << "initSatSource : error";
        return;
    }

//    m_satSrc->setUpdateInterval(1000);
//    connect(m_satSrc,
//            SIGNAL(satellitesInViewUpdated(QList<QGeoSatelliteInfo>)),
//            this,
//            SLOT(slotSatInViewUpdated(QList<QGeoSatelliteInfo>)));
//    connect(m_satSrc,
//            SIGNAL(satellitesInUseUpdated(QList<QGeoSatelliteInfo>)),
//            this,
//            SLOT(slotSatInUseUpdated(QList<QGeoSatelliteInfo>)));
//    connect(m_satSrc,
//            SIGNAL(error(QGeoSatelliteInfoSource::Error)),
//            this,
//            SLOT(slotSatError(QGeoSatelliteInfoSource::Error)));

    qDebug()<< Q_FUNC_INFO << "initSatSource : successful";
}


//位置更新槽函数
void Location::sltPositionUpdated(const QGeoPositionInfo &posInfo)
{
    qDebug() << Q_FUNC_INFO << "posInfo  : " << &posInfo;
   QString strLon = QString("%1").arg(posInfo.coordinate().longitude());
   QString strLat = QString("%1").arg(posInfo.coordinate().latitude());
   QString infoAlt = QString("%1").arg(posInfo.coordinate().altitude());
   QString infoSpeed = QString("%1").arg(posInfo.attribute(QGeoPositionInfo::GroundSpeed));
   QString infoDirection = QString("%1").arg(posInfo.attribute(QGeoPositionInfo::Direction));

   qDebug() << Q_FUNC_INFO << " strLon : " << strLon ;
   qDebug() << Q_FUNC_INFO << " strLat : " << strLat ;
   qDebug() << Q_FUNC_INFO << " infoAlt : " << infoAlt ;
   qDebug() << Q_FUNC_INFO << " infoSpeed : " << infoSpeed;
   qDebug() << Q_FUNC_INFO << " infoDirection : " << infoDirection ;

   m_jsObjPos.insert ("Lon",strLon);
   m_jsObjPos.insert ("Lat",strLat);
   m_jsObjPos.insert ("Alt",infoAlt);
   m_jsObjPos.insert ("Speed",infoSpeed);
   m_jsObjPos.insert ("Direction",infoDirection);

}

//位置更新错误槽函数
void Location::sltPositionError(QGeoPositionInfoSource::Error enuPosError)
{
    QString errStr = QString("PosErr:%1").arg(enuPosError);
    qDebug() << Q_FUNC_INFO << "sltPositionError : " << errStr;
    signalManager()->failed(0,ErrorInfo::InvalidParameter,errStr);
}


//系统定位服务开关变化
void Location::sltSwitchChanged(bool bFlagLocate, bool bFlagGps, bool bFlagBase)
{
    m_flagLocate = bFlagLocate;
    m_flagGps = bFlagGps;
    m_flagBase = bFlagBase;
    qDebug() << Q_FUNC_INFO << m_flagLocate << m_flagGps << m_flagBase;
}

