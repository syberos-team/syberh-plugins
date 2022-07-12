#include "StorageSpace.h"

#include <QDebug>
#include "framework/common/errorinfo.h"

using namespace NativeSdk;

#include <cstoragemanager.h>

#define GB_UNIT (1024 *1024 * 1024)
#define GB_UNIT2 (1024 *1024)

StorageSpace::StorageSpace()
{
}


void StorageSpace::invoke(const QString &callbackID, const QString &actionName, const QVariantMap &params)
{
    qDebug() << Q_FUNC_INFO << "  callbackID:" << callbackID << "actionName:" << actionName << "params:" << params;

    if(actionName == "getSystemStorage"){
        getSystemStorage(callbackID, params);
    }
    else if(actionName == "getSystemRoot"){
        getSystemRoot(callbackID, params);
    }
    else if(actionName == "getSystemData"){
        getSystemData(callbackID, params);
    }
    else if(actionName == "getIntStorage"){
        getIntStorage(callbackID, params);
    }
    else if(actionName == "getExtStorage"){
        getExtStorage(callbackID, params);
    }
    else if(actionName == "getMemory"){
        getMemory(callbackID, params);
    }

}

void StorageSpace::getSystemStorage(const QString &callbackID, const QVariantMap &params)
{
    Q_UNUSED(params);

    qDebug() << Q_FUNC_INFO << "  callbackID:" << callbackID << "params:" << params;

    CStorageManager systemStorageMgr;
    QString sSystemRoot = QString("可用%1GB 共%2GB").arg(systemStorageMgr.storageFreeSize(CStorageManager::SystemRoot) / GB_UNIT,0,'f',2).arg(systemStorageMgr.storageTotalSize(CStorageManager::SystemRoot) / GB_UNIT,0,'f',2);
    QString sSystemData = QString("可用%1GB 共%2GB").arg(systemStorageMgr.storageFreeSize(CStorageManager::SystemData) / GB_UNIT,0,'f',2).arg(systemStorageMgr.storageTotalSize(CStorageManager::SystemData) / GB_UNIT,0,'f',3);
    QString sIntStorage = QString("可用%1GB 共%2GB").arg(systemStorageMgr.storageFreeSize(CStorageManager::IntStorage) / GB_UNIT,0,'f',2).arg(systemStorageMgr.storageTotalSize(CStorageManager::IntStorage) / GB_UNIT,0,'f',2);
    QString sExtStorage = QString("可用%1GB 共%2GB").arg(systemStorageMgr.storageFreeSize(CStorageManager::ExtStorage) / GB_UNIT,0,'f',2).arg(systemStorageMgr.storageTotalSize(CStorageManager::ExtStorage) / GB_UNIT,0,'f',2);
    QString sMemory = QString("可用%1GB 共%2GB").arg(systemStorageMgr.memFreeSize() / GB_UNIT2,0,'f',2).arg(systemStorageMgr.memTotalSize() / GB_UNIT2,0,'f',2);

    QJsonObject json;
    json.insert("SystemRoot", sSystemRoot);
    json.insert("SystemData", sSystemData);
    json.insert("IntStorage", sIntStorage);
    json.insert("ExtStorage", sExtStorage);
    json.insert("Memory", sMemory);

    qDebug() << "getSystemStorage: " << json << endl;

    signalManager()->success(callbackID.toLong(), QVariant(json));
}

void StorageSpace::getSystemRoot(const QString &callbackID, const QVariantMap &params)
{
    Q_UNUSED(params);

    qDebug() << Q_FUNC_INFO << "  callbackID:" << callbackID << "params:" << params;

    CStorageManager systemStorageMgr;

    QJsonObject json;
    json.insert("free", systemStorageMgr.storageFreeSize(CStorageManager::SystemRoot)/ GB_UNIT);
    json.insert("total", systemStorageMgr.storageTotalSize(CStorageManager::SystemRoot)/ GB_UNIT);


    qDebug() << "getSystemRoot: " << json << endl;

    signalManager()->success(callbackID.toLong(), QVariant(json));
}

void StorageSpace::getSystemData(const QString &callbackID, const QVariantMap &params)
{
    Q_UNUSED(params);

    qDebug() << Q_FUNC_INFO << "  callbackID:" << callbackID << "params:" << params;

    CStorageManager systemStorageMgr;

    QJsonObject json;
    json.insert("free", systemStorageMgr.storageFreeSize(CStorageManager::SystemData)/ GB_UNIT);
    json.insert("total", systemStorageMgr.storageTotalSize(CStorageManager::SystemData)/ GB_UNIT);


    qDebug() << "getSystemData: " << json << endl;

    signalManager()->success(callbackID.toLong(), QVariant(json));
}

void StorageSpace::getIntStorage(const QString &callbackID, const QVariantMap &params)
{
    Q_UNUSED(params);

    qDebug() << Q_FUNC_INFO << "  callbackID:" << callbackID << "params:" << params;

    CStorageManager systemStorageMgr;

    QJsonObject json;
    json.insert("free", systemStorageMgr.storageFreeSize(CStorageManager::IntStorage)/ GB_UNIT);
    json.insert("total", systemStorageMgr.storageTotalSize(CStorageManager::IntStorage)/ GB_UNIT);


    qDebug() << "getIntStorage: " << json << endl;

    signalManager()->success(callbackID.toLong(), QVariant(json));
}

void StorageSpace::getExtStorage(const QString &callbackID, const QVariantMap &params)
{
    Q_UNUSED(params);

    qDebug() << Q_FUNC_INFO << "  callbackID:" << callbackID << "params:" << params;

    CStorageManager systemStorageMgr;

    QJsonObject json;
    json.insert("free", systemStorageMgr.storageFreeSize(CStorageManager::ExtStorage)/ GB_UNIT);
    json.insert("total", systemStorageMgr.storageTotalSize(CStorageManager::ExtStorage)/ GB_UNIT);


    qDebug() << "getExtStorage: " << json << endl;

    signalManager()->success(callbackID.toLong(), QVariant(json));
}

void StorageSpace::getMemory(const QString &callbackID, const QVariantMap &params)
{
    Q_UNUSED(params);

    qDebug() << Q_FUNC_INFO << "  callbackID:" << callbackID << "params:" << params;

    CStorageManager systemStorageMgr;

    QJsonObject json;
    json.insert("free", systemStorageMgr.memFreeSize() / GB_UNIT2);
    json.insert("total", systemStorageMgr.memTotalSize() / GB_UNIT2);


    qDebug() << "getMemory: " << json << endl;

    signalManager()->success(callbackID.toLong(), QVariant(json));
}
