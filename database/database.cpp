#include <QDebug>
#include <QtSql>
#include <QObject>
#include <QDebug>
#include <QException>

#include "database.h"
#include "helper.h"
#include "framework/common/errorinfo.h"

using namespace NativeSdk;

Database::Database()
{
}

void Database::invokeInitialize()
{
    dataDir = Helper::instance()->getDataRootPath();
    myConnection = QSqlDatabase::addDatabase("QSQLITE");
}


void Database::invoke(const QString &callbackID, const QString &actionName, const QVariantMap &params)
{
    qDebug() << Q_FUNC_INFO << "  callbackID:" << callbackID << "actionName:" << actionName << "params:" << params;
    if (actionName == "createTable"){
        createTable(callbackID, params);
    }else if (actionName == "query"){
        query(callbackID, params);
    }else if (actionName == "execute"){
        execute(callbackID, params);
    }else if (actionName == "isDatabaseExists"){
        isDatabaseExists(callbackID, params);
    }else if (actionName == "isTableExists"){
        isTableExists(callbackID, params);
    }
}

void Database::createTable(const QString &callbackID, const QVariantMap &params){
    qDebug() << Q_FUNC_INFO << "createTable" << params << endl;
    QString databaseName = params.value("databaseName").toString();//数据库名
    QString sql = params.value("sql").toString();//创建表sql
    QString sqlindex = params.value("sqlindex").toString();//创建索引sql

    if(databaseName.isEmpty()){
        qDebug() << Q_FUNC_INFO << "数据库名不能为空" << endl;
        signalManager()->failed(callbackID.toLong(), ErrorInfo::InvalidParameter, "不合法的参数:数据库名不能为空");
        return;
    }
    if(sql.isEmpty()){
        qDebug() << Q_FUNC_INFO << "sql不能为空" << endl;
        signalManager()->failed(callbackID.toLong(), ErrorInfo::InvalidParameter, "不合法的参数:sql不能为空");
        return;
    }

    //连接数据库
    
    QDir d(dataDir);
    if (!d.exists()) {
        qDebug() << Q_FUNC_INFO << "目录不存在" << endl;
        checkOrCreateDir(dataDir);
    }
    QString dbPath = dataDir + "/" + databaseName+".sqlite";
    qDebug() << Q_FUNC_INFO << "数据库路径 = " << dbPath << endl;

    myConnection.setDatabaseName(dbPath);

    //打开数据库
    bool isOK = myConnection.open();
    if (!isOK) {
        qDebug() << Q_FUNC_INFO << "打开数据库失败" << endl;
        signalManager()->failed(callbackID.toLong(), ErrorInfo::databaseError, "数据库错误:打开数据库失败");
        return;
    }

    //执行创建表sql
    QSqlQuery query(myConnection);
    if (!query.exec(sql)) {
        qDebug() << Q_FUNC_INFO << "执行sql失败:" << query.lastError();
        signalManager()->failed(callbackID.toLong(), ErrorInfo::databaseError, "数据库错误:创建表失败");
        myConnection.close();
        return;
    }

    if(!sqlindex.isEmpty()){
        //执行创建索引sql
        if (!query.exec(sqlindex)) {
            qDebug() << Q_FUNC_INFO << "执行sql失败:" << query.lastError();
            signalManager()->failed(callbackID.toLong(), ErrorInfo::databaseError, "数据库错误:创建索引失败");
            myConnection.close();
            return;
        }
    }
    //关闭数据库
    myConnection.close();
    qDebug() << Q_FUNC_INFO << "createTable:result" << true << endl;
    signalManager()->success(callbackID.toLong(), true);
}

void Database::query(const QString &callbackID, const QVariantMap &params){
    qDebug() << Q_FUNC_INFO << "selectOperate" << params << endl;
    QString databaseName = params.value("databaseName").toString();//数据库名
    QString sqlQuery = params.value("sql").toString();

    // 检验参数
    if(sqlQuery.isEmpty()){
        qDebug() << Q_FUNC_INFO << "sql不能为空" << endl;
        signalManager()->failed(callbackID.toLong(), ErrorInfo::InvalidParameter, "不合法的参数:sql不能为空");
        return;
    }

    // 数据库是否存在
    bool isDbExists = checkDatabaseExists(databaseName);
    if (!isDbExists) {
        qDebug() << Q_FUNC_INFO << "数据库不存在" << endl;
        signalManager()->failed(callbackID.toLong(), ErrorInfo::databaseError, "数据库错误:数据库不存在");
        return;
    }

    
    QString dbPath = dataDir + "/" + databaseName+".sqlite";
    myConnection.setDatabaseName(dbPath);
    bool isOK = myConnection.open();
    if (!isOK) {
        qDebug() << Q_FUNC_INFO << "打开数据库失败" << endl;
        signalManager()->failed(callbackID.toLong(), ErrorInfo::databaseError, "数据库错误:打开数据库失败");
        return;
    }

    QSqlQuery query(myConnection);
    if (!query.exec(sqlQuery)) {
        qDebug() << Q_FUNC_INFO << "查询数据失败,error:" << query.lastError();
        signalManager()->failed(callbackID.toLong(), ErrorInfo::databaseError, "数据库错误:查询数据失败");
        myConnection.close();
        return;
    }

    QJsonArray jsonArr;
    QSqlRecord rec;
    while(query.next()) {
        QJsonObject jsonObj;
        rec = query.record();
        for(int i = 0 ; i<rec.count(); ++i){
            jsonObj.insert(rec.fieldName(i), QJsonValue::fromVariant(rec.value(i)));
        }
        jsonArr.append(jsonObj);
    }

    myConnection.close();
    qDebug() << Q_FUNC_INFO << "selectOperate:jsonArr" << jsonArr << endl;
    signalManager()->success(callbackID.toLong(), QVariant(jsonArr));
}

void Database::execute(const QString &callbackID, const QVariantMap &params){
    qDebug() << Q_FUNC_INFO << "selectOperate" << params << endl;
    QString databaseName = params.value("databaseName").toString();//数据库名
    QString sqlQuery = params.value("sql").toString();

    // 校验参数
    if(sqlQuery.isEmpty()){
        qDebug() << Q_FUNC_INFO << "sql不能为空" << endl;
        signalManager()->failed(callbackID.toLong(), ErrorInfo::InvalidParameter, "不合法的参数:sql不能为空");
        return;
    }

    // 数据库是否存在
    bool isDbExists = checkDatabaseExists(databaseName);
    if (!isDbExists) {
        qDebug() << Q_FUNC_INFO << "数据库不存在" << endl;
        signalManager()->failed(callbackID.toLong(), ErrorInfo::databaseError, "数据库错误:数据库不存在");
        return;
    }

    
    QString dbPath = dataDir + "/" + databaseName+".sqlite";
    myConnection.setDatabaseName(dbPath);

    bool isOK = myConnection.open();
    if (!isOK) {
        qDebug() << Q_FUNC_INFO << "打开数据库失败" << endl;
        signalManager()->failed(callbackID.toLong(), ErrorInfo::databaseError, "数据库错误:打开数据库失败");
        return;
    }

    QSqlQuery query(myConnection);
    if (!query.exec(sqlQuery)) {
        qDebug() << Q_FUNC_INFO << "操作失败,error:" << query.lastError();
        signalManager()->failed(callbackID.toLong(), ErrorInfo::databaseError, "数据库错误:操作失败");
        myConnection.close();
        return;
    }

    myConnection.close();
    qDebug() << Q_FUNC_INFO << "execOperate:result" << true << endl;
    signalManager()->success(callbackID.toLong(), true);
}

bool Database::isDatabaseExists(const QString &callbackID, const QVariantMap &params){
    qDebug() << Q_FUNC_INFO << "isDataExists" << params << endl;
    QString databaseName = params.value("databaseName").toString();//数据库名

    // 数据库是否存在
    bool isDbExists = checkDatabaseExists(databaseName);
    if (!isDbExists) {
        qDebug() << Q_FUNC_INFO << "数据库不存在" << endl;
        signalManager()->failed(callbackID.toLong(), ErrorInfo::databaseError, "数据库错误:数据库不存在");
        return false;
    }
    qDebug() << Q_FUNC_INFO << "isDataExists:result" << true << endl;
    signalManager()->success(callbackID.toLong(), true);
    return true;
}

bool Database::isTableExists(const QString &callbackID, const QVariantMap &params){
    qDebug() << Q_FUNC_INFO << "isTableExists" << params << endl;
    QString tableName = params.value("tableName").toString();
    QString databaseName = params.value("databaseName").toString();//数据库名

    if(tableName.isEmpty()){
        qDebug() << Q_FUNC_INFO << "表名不能为空" << endl;
        signalManager()->failed(callbackID.toLong(), ErrorInfo::InvalidParameter, "不合法的参数:表名不能为空");
        return false;
    }

    // 数据库是否存在
    bool isDbExists = checkDatabaseExists(databaseName);
    if (!isDbExists) {
        qDebug() << Q_FUNC_INFO << "数据库不存在" << endl;
        signalManager()->failed(callbackID.toLong(), ErrorInfo::databaseError, "数据库错误:数据库不存在");
        return false;
    }

    
    QString dbPath = dataDir + "/" + databaseName+".sqlite";
    myConnection.setDatabaseName(dbPath);

    bool isOK = myConnection.open();
    if (!isOK) {
        qDebug() << Q_FUNC_INFO << "打开数据库失败" << endl;
        signalManager()->failed(callbackID.toLong(), ErrorInfo::databaseError, "数据库错误:打开数据库失败");
        return false;
    }

    QSqlQuery query(myConnection);
    //判断表是否存在
    QString sqlQuery = "select * from sqlite_master where type = 'table' and name = '" + tableName + "'";
    if (!query.exec(sqlQuery)) {
        qDebug() << Q_FUNC_INFO << "执行SQL失败,error:" << query.lastError();
        signalManager()->failed(callbackID.toLong(), ErrorInfo::databaseError, "数据库错误:表不存在");
        myConnection.close();
        return false;
    }

    // 指针指向第一个结果，false： 表不存在
    if (!query.first()) {
        qDebug() << Q_FUNC_INFO << "表不存在，查询第一条结果失败,error:" << query.lastError();
        signalManager()->failed(callbackID.toLong(), ErrorInfo::databaseError, "数据库错误:表不存在");
        myConnection.close();
        return false;
    }

    // 查询结果
    QSqlRecord rec = query.record();
    QVariant tName = rec.value("name");

    // 表名无效，说明表不存在
    if (!tName.isValid()) {
        qDebug() << Q_FUNC_INFO << tableName << "表不存在,error:" << query.lastError();
        signalManager()->failed(callbackID.toLong(), ErrorInfo::databaseError, "数据库错误:表不存在");
        myConnection.close();
        return false;
    }

    myConnection.close();
    qDebug() << Q_FUNC_INFO << "isTableExists:result" << true << endl;
    signalManager()->success(callbackID.toLong(), true);
    return true;
}

bool Database::checkOrCreateDir(const QString &path){
    qDebug() << Q_FUNC_INFO << path << endl;
    QDir __dir;
    if (!__dir.exists(path)) {
        qDebug()  << Q_FUNC_INFO << "指定目录: " << path << "不存在，则尝试创建";
        if (!__dir.mkpath(path)) {
            return false;
        } else {
            qDebug()  << Q_FUNC_INFO << "创建指定目录成功: " << path ;

            QFile file(path);
            if (!file.setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner | QFileDevice::ExeOwner
                                        | QFileDevice::ReadGroup | QFileDevice::WriteGroup | QFileDevice::ExeGroup
                                        | QFileDevice::ReadOther | QFileDevice::WriteOther | QFileDevice::ExeOther)) {
            }
            return true;
        }
    }

    QFileInfo fi(path);
    if (!fi.isWritable()) {
        return false;
    }
    return true;
}

bool Database::checkDatabaseExists(const QString &dbName) {
    qDebug() << Q_FUNC_INFO << dbName << endl;

    if(dbName.isEmpty()){
        qDebug() << Q_FUNC_INFO << "数据库名不能为空" << endl;
        return false;
    }
    QString dbPath = dataDir + "/" + dbName+".sqlite";
    qDebug() << Q_FUNC_INFO << "dbPath" << dbPath << endl;

    //判断数据库是否存在
    QFile file(dbPath);
    if(!file.exists()){
        qDebug() << Q_FUNC_INFO << "数据库不存在" << endl;
        return false;
    }

    qDebug() << Q_FUNC_INFO << "checkDatabaseExists:result" << true << endl;
    return true;
}
