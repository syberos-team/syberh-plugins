#include "network.h"
#include <QDebug>
#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QNetworkReply>
#include <QJsonDocument>
#include "framework/common/errorinfo.h"
#include "util/validator.h"
#include "util/util.h"

// 默认的 method
#define _NET_DEF_METHOD "get"
// 默认的 dateType
#define _NET_DEF_DATATYPE "text"


using namespace NativeSdk;

Network::Network()
{
}

Network::~Network()
{
  delete manager;
}

void Network::invokeInitialize()
{
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(finished(QNetworkReply *)));
}

void Network::invoke(const QString &callbackID, const QString &actionName, const QVariantMap &params)
{
    qDebug() << Q_FUNC_INFO << "  callbackID:" << callbackID << "actionName:" << actionName << "params:" << params;

    // 检查网络
    if (!Validator::netWorkConnected()) {
        signalManager()->failed(callbackID.toLong(), ErrorInfo::NetworkError, ErrorInfo::message(ErrorInfo::NetworkError, "请检查网络状态"));
        return;
    }

    QString url = params.value("url").toString();
    QString method = params.value("method").toString();
    if (method.isEmpty()) {
        qDebug() << Q_FUNC_INFO << "params type is null, set default values: get callbackID is " << callbackID << endl;
        method = _NET_DEF_METHOD;
    }

    //检查url
    if (!Validator::isHttpUrl(url)) {
        qDebug() << "url parameter is not starts with http or https: " << params << endl;
        signalManager()->failed(callbackID.toLong(), ErrorInfo::InvalidParameter, ErrorInfo::message(ErrorInfo::InvalidParameter, "URL无效"));
        return;
    }


    QString dataType = params.value("dataType").toString();
    if (dataType.isEmpty()) {
        dataType = _NET_DEF_DATATYPE;
    }

    if (!Util::Equals(dataType, "json", true) && !Util::Equals(dataType, "text", true)) {
        signalManager()->failed(callbackID.toLong(), ErrorInfo::InvalidParameter, ErrorInfo::message(ErrorInfo::InvalidParameter, "dataType"));
        return;
    }

    QNetworkRequest request;

    if (url.startsWith("https", Qt::CaseInsensitive)) {
        QSslConfiguration config;
        config.setPeerVerifyMode(QSslSocket::VerifyNone);
        config.setProtocol(QSsl::UnknownProtocol);
        request.setSslConfiguration(config);
    }

    QVariantMap headers = params.value("header").toMap();

    qDebug() << Q_FUNC_INFO << "header is " << headers << endl;

    QVariantMap::Iterator headerIt = headers.begin();
    while (headerIt != headers.end()) {
        request.setRawHeader(headerIt.key().toUtf8(), headerIt.value().toString().toUtf8());
        headerIt++;
    }
    if (!headers.contains("Content-Type")) {
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    }

    request.setUrl(url);

    QNetworkReply *reply;

    if (Util::Equals(method, "get", true)) {
        reply = manager->get(request);
    } else if (Util::Equals(method, "post", true)) {
        QVariantMap data = params.value("data").toMap();
        QVariantMap::Iterator it = data.begin();
        QString content;
        while (it != data.end()) {
            content.append(it.key()).append("=").append(it.value().toString());
            it++;
            if (it != data.end()) {
                content.append("&");
            }
        }
        reply = manager->post(request, content.toUtf8());
    } else if (Util::Equals(method, "put", true)) {
        QVariantMap data = params.value("data").toMap();
        QVariantMap::Iterator it = data.begin();
        QString content;
        while (it != data.end()) {
            content.append(it.key()).append("=").append(it.value().toString());
            it++;
            if (it != data.end()) {
                content.append("&");
            }
        }
        reply = manager->put(request, content.toUtf8());
    } else if (Util::Equals(method, "delete", true)) {
        reply = manager->deleteResource(request);
    } else {
        signalManager()->failed(callbackID.toLong(), ErrorInfo::InvalidParameter, ErrorInfo::message(ErrorInfo::InvalidParameter, "type"));
        return;
    }

    TaskInfo *taskInfo = new TaskInfo();
    taskInfo->dataType = dataType;
    taskInfo->reply = reply;

    tasks[callbackID] = taskInfo;
}

void Network::finished(QNetworkReply *reply)
{
  qDebug() << Q_FUNC_INFO << endl;

  QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

  qDebug() << Q_FUNC_INFO << " statusCode: " << statusCode.toString() << " error: " << reply->error() << endl;

  QJsonObject headers;

  QList<QPair<QByteArray, QByteArray>> ls = reply->rawHeaderPairs();
  for (qint64 i = 0; i < ls.size(); i++) {
    QByteArray key = ls.at(i).first;
    QByteArray val = ls.at(i).second;
    headers.insert(QString(key), QString(val));
  }

  QString result = QString(reply->readAll());

  qDebug() << Q_FUNC_INFO << " result: " << result << " url:" << reply->url().url() << endl;

  QString callbackID;
  QString datatype;

  QMap<QString, TaskInfo *>::Iterator it = tasks.begin();
  while (it != tasks.end()) {
    TaskInfo *taskInfo = it.value();
    if (taskInfo->reply == reply) {
      callbackID = it.key();
      datatype = taskInfo->dataType;
      tasks.remove(it.key());
      delete taskInfo;
      taskInfo = NULL;
      break;
    }
    it++;
  }

  if (QNetworkReply::NoError == reply->error()) {
    QJsonObject json;
    json.insert("statusCode", statusCode.toString());
    json.insert("header", headers);
    if (Util::Equals(datatype, "json", true)) {
      json.insert("data", QJsonDocument::fromJson(result.toLocal8Bit().data()).object());
      signalManager()->success(callbackID.toLong(), json);
    } else {
      json.insert("data", result);
      signalManager()->success(callbackID.toLong(), json);
    }
  } else {
    qDebug() << Q_FUNC_INFO << " result error " << endl;
    signalManager()->failed(callbackID.toLong(), ErrorInfo::NetworkError, ErrorInfo::message(ErrorInfo::NetworkError, reply->errorString()));
  }

  reply->deleteLater();
}
