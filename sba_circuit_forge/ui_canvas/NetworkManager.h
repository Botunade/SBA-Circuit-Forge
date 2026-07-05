#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QGraphicsScene>

class NetworkManager : public QObject {
    Q_OBJECT

public:
    static NetworkManager& getInstance();

    void validate_canvas(QGraphicsScene* scene);

private slots:
    void onReplyFinished(QNetworkReply* reply);

private:
    NetworkManager();
    ~NetworkManager();

    QNetworkAccessManager* manager;

    // Delete copy constructor and assignment operator for Singleton
    NetworkManager(NetworkManager const&) = delete;
    void operator=(NetworkManager const&) = delete;
};

#endif // NETWORKMANAGER_H