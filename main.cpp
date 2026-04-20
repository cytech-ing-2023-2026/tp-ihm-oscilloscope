#include <QGuiApplication>
#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTcpSocket>
#include <QUdpSocket>

#include "AppState.hpp"
#include "OscilloscopeItem.hpp"

uint8_t readUInt8Tcp(QTcpSocket* socket) { return socket->read(1)[0]; }

static char bufTcp[1];

void writeUInt8Tcp(QTcpSocket* socket, uint8_t value) {
    bufTcp[0] = value;
    socket->write(bufTcp, 1);
}

static char bufUdp[1];

void writeUInt8Udp(QUdpSocket* socket, uint8_t value) {
    bufUdp[0] = value;
    socket->write(bufUdp, 1);
}

int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);
    AppState appState(&app);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
        []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);

    QTcpSocket tcpSocket = QTcpSocket(&app);
    QObject::connect(&tcpSocket, &QTcpSocket::connected, &app, [&tcpSocket] {
        qInfo() << "TCP connected";
        writeUInt8Tcp(&tcpSocket, 0x02);
        tcpSocket.flush();
    });
    QObject::connect(&tcpSocket, &QTcpSocket::errorOccurred, &app,
                     [](QAbstractSocket::SocketError error) { qInfo() << "Error " << error; });
    QObject::connect(&tcpSocket, &QTcpSocket::readyRead, &app, [&tcpSocket, &appState] {
        uint8_t sensorCount = readUInt8Tcp(&tcpSocket);

        qInfo() << "Received" << sensorCount << "sensors";

        for (int i = 0; i < sensorCount; i++) {
            uint8_t id = readUInt8Tcp(&tcpSocket);
            uint8_t red = readUInt8Tcp(&tcpSocket);
            uint8_t green = readUInt8Tcp(&tcpSocket);
            uint8_t blue = readUInt8Tcp(&tcpSocket);
            QColor color = QColor::fromRgb(red, green, blue);

            qInfo() << "Received sensor" << i + 1 << "/" << sensorCount << ":" << id << color;
            appState.addSensor(id, color);
        }
    });

    QUdpSocket udpSocket = QUdpSocket(&app);
    QObject::connect(&udpSocket, &QUdpSocket::connected, &app, [&udpSocket] {
        qInfo() << "UDP connected";
        writeUInt8Udp(&udpSocket, 0x11);
        udpSocket.flush();
    });

    QObject::connect(&udpSocket, &QUdpSocket::readyRead, &app, [&udpSocket, &appState] {
        while (udpSocket.hasPendingDatagrams()) {
            QByteArray datagram;
            datagram.resize(udpSocket.pendingDatagramSize());
            udpSocket.readDatagram(datagram.data(), datagram.size());

            QDataStream stream(datagram);
            stream.setByteOrder(QDataStream::BigEndian);
            stream.setFloatingPointPrecision(QDataStream::SinglePrecision);

            uint8_t id;
            float value;

            stream >> id;
            stream >> value;

            appState.setSensorValue(id, value);
        }
    });

    tcpSocket.connectToHost("10.8.0.5", 1234);
    udpSocket.connectToHost("10.8.0.5", 1234);

    qmlRegisterType<OscilloscopeItem>("App", 1, 0, "Oscilloscope");
    engine.rootContext()->setContextProperty("appState", &appState);
    engine.load(QStringLiteral("qrc:/tp2/Main.qml"));

    return app.exec();
}
