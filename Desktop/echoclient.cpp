#include "echoclient.h"
#include <QtCore/QDebug>

QT_USE_NAMESPACE

EchoClient::EchoClient(const QUrl &url, QObject *parent) :
    QObject(parent),
    m_url(url)
{
    qDebug() << "WebSocket server:" << url;
    connect(&m_webSocket, &QWebSocket::connected, this, &EchoClient::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &EchoClient::closed);
    connect(&m_webSocket, static_cast<void(QWebSocket::*)(QAbstractSocket::SocketError)>(&QWebSocket::error),
        [=](QAbstractSocket::SocketError error){ qDebug() << "Error establishing socket: " << error; });
    m_webSocket.open(QUrl(url));
}

void EchoClient::onConnected()
{
    qDebug() << "WebSocket connected";
    connect(&m_webSocket, &QWebSocket::textMessageReceived,
            this, &EchoClient::onTextMessageReceived);
    m_webSocket.sendTextMessage(QStringLiteral("{\"type\": \"connection\", \"content\": '{\"user\": \"pmauldin\", \"computerName\": \"macbook\"}'}"));
}

void EchoClient::onTextMessageReceived(QString message)
{
    qDebug() << "Message received:" << message;
    if (message == QString("wake")) {
        qDebug() << "Command to wake recieved.";
        system("caffeinate -u -t 2");
    }
//    m_webSocket.close();
}
