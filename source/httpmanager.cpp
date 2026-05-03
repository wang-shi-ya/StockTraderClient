#include "httpmanager.h"
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcessEnvironment>
#include <QUrlQuery>

HttpManager &HttpManager::instance()
{
    static HttpManager inst;
    return inst;
}

HttpManager::HttpManager(QObject *parent)
    : QObject(parent)
    , m_nam(new QNetworkAccessManager(this))
{
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    m_baseUrl = env.value("API_BASE_URL", "http://localhost:3000");
}

void HttpManager::setBaseUrl(const QString &url)
{
    m_baseUrl = url;
}

void HttpManager::setAuthToken(const QString &token)
{
    m_authToken = token;
}

void HttpManager::clearAuthToken()
{
    m_authToken.clear();
}

QNetworkRequest HttpManager::createRequest(const QString &path)
{
    QUrl url(m_baseUrl + path);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    if (!m_authToken.isEmpty()) {
        request.setRawHeader("Authorization", ("Bearer " + m_authToken).toUtf8());
    }
    return request;
}

QNetworkReply *HttpManager::get(const QString &path)
{
    QNetworkReply *reply = m_nam->get(createRequest(path));
    return reply;
}

QNetworkReply *HttpManager::post(const QString &path, const QJsonObject &body)
{
    QNetworkRequest req = createRequest(path);
    QByteArray data = QJsonDocument(body).toJson(QJsonDocument::Compact);
    QNetworkReply *reply = m_nam->post(req, data);
    return reply;
}

QNetworkReply *HttpManager::put(const QString &path, const QJsonObject &body)
{
    QNetworkRequest req = createRequest(path);
    QByteArray data = QJsonDocument(body).toJson(QJsonDocument::Compact);
    QNetworkReply *reply = m_nam->put(req, data);
    return reply;
}

QNetworkReply *HttpManager::del(const QString &path)
{
    QNetworkReply *reply = m_nam->deleteResource(createRequest(path));
    return reply;
}

QJsonObject HttpManager::parseResponse(QNetworkReply *reply)
{
    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isObject()) {
        return doc.object();
    }
    return QJsonObject();
}

bool HttpManager::isSuccess(const QJsonObject &response)
{
    return response.value("code").toInt(-1) == 0;
}

QString HttpManager::errorMessage(const QJsonObject &response)
{
    return response.value("message").toString("Unknown error");
}

QJsonObject HttpManager::dataObject(const QJsonObject &response)
{
    return response.value("data").toObject();
}

QJsonArray HttpManager::dataArray(const QJsonObject &response)
{
    return response.value("data").toArray();
}

void HttpManager::checkTokenExpired(const QJsonObject &response)
{
    // If server returns 401-like response, emit tokenExpired
    QString msg = response.value("message").toString();
    if (msg.contains("登录已过期") || msg.contains("未登录")) {
        emit tokenExpired();
    }
}
