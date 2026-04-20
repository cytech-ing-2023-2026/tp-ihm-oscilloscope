#include <QGuiApplication>
#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QCommandLineParser>

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

    QCommandLineParser parser;
    parser.setApplicationDescription(
        QStringLiteral(
            "Simulated sensor: registers with the server via TCP, then streams data via UDP."
        )
    );
    parser.addHelpOption();
    parser.addVersionOption();

    // Host
    QCommandLineOption hostOpt(
        QStringLiteral("host"),
        QStringLiteral("Server IP address (default: 127.0.0.1)."),
        QStringLiteral("ip"),
        QStringLiteral("127.0.0.1")
    );
    parser.addOption(hostOpt);
    // Port
    QCommandLineOption portOpt(
        QStringLiteral("port"),
        QStringLiteral("Server port (default: 1234)."),
        QStringLiteral("port"),
        QStringLiteral("1234")
    );
    parser.addOption(portOpt);
    
    parser.process(app);

    const QHostAddress serverAddress(parser.value(hostOpt));
    if (serverAddress.isNull()) {
        qCritical() << "Invalid server address:" << parser.value(hostOpt);
        return 1;
    }

    bool ok = false;
    // Port
    const quint16 serverPort = static_cast<quint16>(parser.value(portOpt).toUInt(&ok));
    if (!ok || serverPort == 0) {
        qCritical() << "Invalid port:" << parser.value(portOpt);
        return 1;
    }

    tcpSocket.connectToHost(serverAddress, serverPort);
    udpSocket.connectToHost(serverAddress, serverPort);

    qmlRegisterType<OscilloscopeItem>("App", 1, 0, "Oscilloscope");
    engine.rootContext()->setContextProperty("appState", &appState);
    engine.load(QStringLiteral("qrc:/tp2/Main.qml"));

    return app.exec();
}
