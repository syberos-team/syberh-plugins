#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QtPlugin>
#include <QSqlQueryModel>

#include "iplugin/iplugin.h"
#include "database_global.h"

class DATABASESHARED_EXPORT Database : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.syberos.syberh.SyberhPlugin" FILE "plugin.json")

public:
    Database();

    void invokeInitialize();
    void invoke(const QString &callbackID, const QString &actionName, const QVariantMap &params);
    
    /**
     * @brief createTable 创建数据库和表
     * @param params 参数
     * @return 成功则返回true。
     *      失败则返回错误码。
     */
    void createTable(const QString &callbackID, const QVariantMap &params);

    /**
     * @brief query 执行查询sql
     *
     * @param params 参数
     * @return 成功则返回数据。
     *      失败则返回错误码。
     */
    void query(const QString &callbackID, const QVariantMap &params);

    /**
     * @brief execute 执行操作sql
     * @param params 参数
     * @return 成功则返回true。
     *      失败则返回错误码。
     */
    void execute(const QString &callbackID, const QVariantMap &params);

    /**
     * @brief isDatabaseExists 判断数据库是否存在
     * @param params 参数
     * @return 成功则返回true。
     *      失败则返回错误码。
     */
    bool isDatabaseExists(const QString &callbackID, const QVariantMap &params);

    /**
     * @brief isTableExists 判断表是否存在
     * @param params 参数
     * @return 成功则返回true。
     *      失败则返回错误码。
     */
    bool isTableExists(const QString &callbackID, const QVariantMap &params);

private:
    QSqlDatabase myConnection;

    // app data存放位置
    QString dataDir;

    bool checkOrCreateDir(const QString &path);

    /**
     * @brief checkDatabaseExists 判断数据库是否存在, 公共方法
     * @param dbName 参数
     * @return 成功则返回true。
     *      失败则返回错误码。
     */
    bool checkDatabaseExists(const QString &dbName);
};

#endif // DATABASE_H
