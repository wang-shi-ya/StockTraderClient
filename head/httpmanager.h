#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QUrl>
#include <QProcessEnvironment>

class HttpManager : public QObject
{
    Q_OBJECT

public:
    static HttpManager &instance();

    void setBaseUrl(const QString &url);
    QString baseUrl() const { return m_baseUrl; }

    void setAuthToken(const QString &token);
    void clearAuthToken();
    bool hasAuthToken() const { return !m_authToken.isEmpty(); }

    QNetworkReply *get(const QString &path);
    QNetworkReply *post(const QString &path, const QJsonObject &body = QJsonObject());
    QNetworkReply *put(const QString &path, const QJsonObject &body = QJsonObject());
    QNetworkReply *del(const QString &path);

    static QJsonObject parseResponse(QNetworkReply *reply);
    static bool isSuccess(const QJsonObject &response);
    static QString errorMessage(const QJsonObject &response);
    static QJsonObject dataObject(const QJsonObject &response);
    static QJsonArray dataArray(const QJsonObject &response);

signals:
    void tokenExpired();

private:
    explicit HttpManager(QObject *parent = nullptr);
    ~HttpManager() override = default;
    HttpManager(const HttpManager &) = delete;
    HttpManager &operator=(const HttpManager &) = delete;

    QNetworkRequest createRequest(const QString &path);
    void checkTokenExpired(const QJsonObject &response);

    QNetworkAccessManager *m_nam;
    QString m_baseUrl;
    QString m_authToken;
};
