#include "lovecommunicationtcp.h"

#include <utility>
#include "src/Controller/Intersynth/floating_fix.h"
#include "src/Controller/LfoWaveType.h"


LoveCommunicationTcp::LoveCommunicationTcp(std::function<void(QTcpSocket::SocketState state)> onStateChange)
        : onStateChange(std::move(onStateChange)) {

}


bool LoveCommunicationTcp::connectToServer(QString ip, int port) {
    // Connect to the server at IP address 192.168.1.100 and port 1234
    socket.connectToHost(ip, port);
    QObject::connect(&socket, &QTcpSocket::stateChanged, onStateChange);

    // Wait for the connection to be established
    if (socket.waitForConnected()) {
        return true;
    } else {
        qDebug() << "Error connecting to server:" << socket.errorString();
        return false;
    }
}

void LoveCommunicationTcp::connectToServerAsync(QString ip, int port) {
    // Connect to the server at IP address 192.168.1.100 and port 1234
    socket.connectToHost(ip, port);
    QObject::connect(&socket, &QTcpSocket::stateChanged, onStateChange);
}

void LoveCommunicationTcp::disconnectFromServer() {
    // Close the socket
    socket.close();
}

QByteArray LoveCommunicationTcp::getMessageBytes() {
    if (socket.waitForReadyRead()) {
        QByteArray read_data = socket.read(1024);
        return read_data;
    } else {
        return "No message came before time was up";
    }
}

void LoveCommunicationTcp::sendMessageBytes(QByteArray msg) {
    socket.write(msg);
    // Wait for the message to be sent
    if (socket.waitForBytesWritten()) {
        // qDebug() << "Message sent successfully!";
    } else {
        qDebug() << "Error sending message:" << socket.errorString();
    }
}

QString LoveCommunicationTcp::getMessageString() {
    return QString::fromUtf8(getMessageBytes());
}

void LoveCommunicationTcp::sendMessageString(QString msg) {
    sendMessageBytes(msg.toUtf8());
}

bool LoveCommunicationTcp::isConnected() {
    return socket.state() == QTcpSocket::SocketState::ConnectedState;
}

QString LoveCommunicationTcp::sendOperatorValue(int operator_id, bool attack, int envelope_index, float frequency,
                                                float amplitude) {
    QByteArray bytes;
    bytes.append((unsigned char) (0x10 + operator_id));
    bytes.append((attack ? 0x80 : 0x00) + envelope_index);
    unsigned char buf[5];
    store_float_in_buffer(buf, frequency);
    bytes.append(QByteArray::fromRawData((char *) buf, 5));
    store_float_in_buffer(buf, amplitude);
    bytes.append(QByteArray::fromRawData((char *) buf, 5));
    sendMessageBytes(bytes);
    return QString::fromUtf8(bytes.toHex(' '));
}

QString LoveCommunicationTcp::addModulator(int operator_id, int modulator_id) {
    QByteArray bytes;
    bytes.append((unsigned char) (0x40 + operator_id));
    bytes.append((unsigned char) (0x10 + modulator_id));
    sendMessageBytes(bytes);
    return QString::fromUtf8(bytes.toHex(' '));
}

QString LoveCommunicationTcp::removeModulator(int operator_id, int modulator_id) {
    QByteArray bytes;
    bytes.append((unsigned char) (0x40 + operator_id));
    bytes.append((unsigned char) (0x00 + modulator_id));
    sendMessageBytes(bytes);
    return QString::fromUtf8(bytes.toHex(' '));
}

QString LoveCommunicationTcp::addCarrier(int operator_id) {
    QByteArray bytes;
    bytes.append((unsigned char) (0x50 + operator_id));
    sendMessageBytes(bytes);
    return QString::fromUtf8(bytes.toHex(' '));
}

QString LoveCommunicationTcp::removeCarrier(int operator_id) {
    QByteArray bytes;
    bytes.append((unsigned char) (0x60 + operator_id));
    sendMessageBytes(bytes);
    return QString::fromUtf8(bytes.toHex(' '));
}

QString LoveCommunicationTcp::setAttackAmpEnvelopePoint(int point_index, float value, float time) {
    QByteArray bytes;
    bytes.append((unsigned char) (0x30 + 0x01));
    bytes.append(point_index);
    unsigned char buf[5];
    store_float_in_buffer(buf, value);
    bytes.append(QByteArray::fromRawData((char *) buf, 5));
    store_float_in_buffer(buf, time);
    bytes.append(QByteArray::fromRawData((char *) buf, 5));
    sendMessageBytes(bytes);
    return QString::fromUtf8(bytes.toHex(' '));
}

QString LoveCommunicationTcp::setReleaseAmpEnvelopePoint(int point_index, float value, float time) {
    QByteArray bytes;
    bytes.append((unsigned char) (0x30 + 0x02));
    bytes.append(point_index);
    unsigned char buf[5];
    store_float_in_buffer(buf, value);
    bytes.append(QByteArray::fromRawData((char *) buf, 5));
    store_float_in_buffer(buf, time);
    bytes.append(QByteArray::fromRawData((char *) buf, 5));
    sendMessageBytes(bytes);
    return QString::fromUtf8(bytes.toHex(' '));
}

QString LoveCommunicationTcp::setAttackAmpEnvelopeSize(int size) {
    QByteArray bytes;
    bytes.append((unsigned char) (0x30 + 0x01));
    bytes.append(size);
    unsigned char buf[5];
    store_float_in_buffer(buf, -1.0);
    bytes.append(QByteArray::fromRawData((char *) buf, 5));
    store_float_in_buffer(buf, -1.0);
    bytes.append(QByteArray::fromRawData((char *) buf, 5));
    sendMessageBytes(bytes);
    return QString::fromUtf8(bytes.toHex(' '));
}

QString LoveCommunicationTcp::setReleaseAmpEnvelopeSize(int size) {
    QByteArray bytes;
    bytes.append((unsigned char) (0x30 + 0x02));
    bytes.append(size);
    unsigned char buf[5];
    store_float_in_buffer(buf, -1.0);
    bytes.append(QByteArray::fromRawData((char *) buf, 5));
    store_float_in_buffer(buf, -1.0);
    bytes.append(QByteArray::fromRawData((char *) buf, 5));
    sendMessageBytes(bytes);
    return QString::fromUtf8(bytes.toHex(' '));
}

QString LoveCommunicationTcp::setLfoGlobalOptions(bool enabled, LfoWaveType type, float frequency) {
    QByteArray bytes;
    bool singleBitEnabled = !!enabled;
    bytes.append((unsigned char) (0xE0 + (singleBitEnabled << 3) + (unsigned char) type));
    unsigned char buf[5];
    store_float_in_buffer(buf, frequency);
    bytes.append(QByteArray::fromRawData((char *) buf, 5));
    sendMessageBytes(bytes);
    return QString::fromUtf8(bytes.toHex(' '));
}

QString LoveCommunicationTcp::setOperatorLfoValues(int operatorId, float frequencyAmount, float amplitudeAmount) {
    QByteArray bytes;
    bytes.append((unsigned char) (0xF0 + operatorId));
    unsigned char buf[5];
    store_float_in_buffer(buf, frequencyAmount);
    bytes.append(QByteArray::fromRawData((char *) buf, 5));
    store_float_in_buffer(buf, amplitudeAmount);
    bytes.append(QByteArray::fromRawData((char *) buf, 5));
    sendMessageBytes(bytes);
    return QString::fromUtf8(bytes.toHex(' '));
}
QString LoveCommunicationTcp::setOperatorAmpEnvelopeValue(int operatorId, int index, bool attack, float value, float time) {
    QByteArray bytes;
    // Operator amp envelope message tag is 0x0, so we only have to send the index (the other bytes will be zero automatically)
    bytes.append((unsigned char) (index));
    bytes.append(attack ? 0x1 : 0x2);
    unsigned char buf[5];
    store_float_in_buffer(buf, value);
    bytes.append(QByteArray::fromRawData((char *) buf, 5));
    store_float_in_buffer(buf, time);
    bytes.append(QByteArray::fromRawData((char *) buf, 5));
    return QString::fromUtf8(bytes.toHex(' '));
}