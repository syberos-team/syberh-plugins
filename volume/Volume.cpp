#include "Volume.h"

#include <QDebug>
#include "framework/common/errorinfo.h"
#include <caudiomanager.h>

using namespace NativeSdk;

Volume::Volume()
{
}


void Volume::invoke(const QString &callbackID, const QString &actionName, const QVariantMap &params)
{
    qDebug() << Q_FUNC_INFO << "  callbackID:" << callbackID << "actionName:" << actionName << "params:" << params;

    if(actionName == "setVolume"){
        setVolume(callbackID, params);
    }
    else if(actionName == "getVolume"){
        getVolume(callbackID, params);
    }

}

void Volume::setVolume(const QString &callbackID, const QVariantMap &params)
{
    qDebug() << Q_FUNC_INFO << "  callbackID:" << callbackID << "params:" << params;

    bool bVolume = false;
    bool bRole = false;
    int iVolume = params.value("volume").toInt(&bVolume);
    int iRole = params.value("role").toInt(&bRole);
    qDebug() << Q_FUNC_INFO << " iVolume: " <<  iVolume << " iRole: " << iRole;


    if(!bVolume)
    {
        signalManager()->failed(callbackID.toLong(),ErrorInfo::InvalidParameter,"volume not number!");
        return;
    }
    if((iVolume<0) || (iVolume >100))
    {
        signalManager()->failed(callbackID.toLong(),ErrorInfo::InvalidParameter,"volume out of range!");
        return;
    }

    if(!bRole)
    {
        signalManager()->failed(callbackID.toLong(),ErrorInfo::InvalidParameter,"role not number!");
        return;
    }
    if((iRole<0) || (iRole >7))
    {
        signalManager()->failed(callbackID.toLong(),ErrorInfo::InvalidParameter,"role out of range!");
        return;
    }


    CAudioManager audioManager;
    audioManager.setVolume(CAudioManager::AM_ROLE_TYPE (iRole),iVolume);
    int iResultVolume = audioManager.getVolume(CAudioManager::AM_ROLE_TYPE (iRole));

    if((iResultVolume<0) || (iResultVolume >100))
    {
        signalManager()->failed(callbackID.toLong(),ErrorInfo::InvalidParameter,"read volume out of range!");
        return;
    }
    signalManager()->success(callbackID.toLong(), QVariant(iResultVolume));
}


void Volume::getVolume(const QString &callbackID, const QVariantMap &params)
{
    qDebug() << Q_FUNC_INFO << "  callbackID:" << callbackID << "params:" << params;

    bool bRole = false;
    int iRole = params.value("role").toInt(&bRole);
    qDebug() << Q_FUNC_INFO << " iRole: " << iRole;

    if(!bRole)
    {
        signalManager()->failed(callbackID.toLong(),ErrorInfo::InvalidParameter,"role not number!");
        return;
    }
    if((iRole<0) || (iRole >7))
    {
        signalManager()->failed(callbackID.toLong(),ErrorInfo::InvalidParameter,"role out of range!");
        return;
    }

    CAudioManager audioManager;
    int iResultVolume = audioManager.getVolume(CAudioManager::AM_ROLE_TYPE (iRole));

    if((iResultVolume<0) || (iResultVolume >100))
    {
        signalManager()->failed(callbackID.toLong(),ErrorInfo::InvalidParameter,"read volume out of range!");
        return;
    }

    signalManager()->success(callbackID.toLong(), QVariant(iResultVolume));
}

