#include "lovecommunicationtcp.h"
//#include "floating_fix.h"
#include "src/Controller/Intersynth/floating_fix.h"


LoveCommunicationTcp::LoveCommunicationTcp()
{

}


bool LoveCommunicationTcp::connectToServer(QString ip, int port)
{
// Connect to the server at IP address 192.168.1.100 and port 1234
socket.connectToHost(ip, port);

// Wait for the connection to be established
if (socket.waitForConnected()) {
return true;
} else {
qDebug() << "Error connecting to server:" << socket.errorString();
return false;
}
}

void LoveCommunicationTcp::disconnectFromServer()
{
    // Close the socket
    socket.close();
}

QByteArray LoveCommunicationTcp::getMessageBytes(){
    if(socket.waitForReadyRead()){
        QByteArray read_data = socket.read(1024);
        return read_data;
    }
    else{
        return "No message came before time was up";
    }
}

void LoveCommunicationTcp::sendMessageBytes(QByteArray msg){
socket.write(msg);
// Wait for the message to be sent
if (socket.waitForBytesWritten()) {
qDebug() << "Message sent successfully!";
} else {
qDebug() << "Error sending message:" << socket.errorString();
}
}

QString LoveCommunicationTcp::getMessageString(){
    return QString::fromUtf8(getMessageBytes());
}

void LoveCommunicationTcp::sendMessageString(QString msg){
sendMessageBytes(msg.toUtf8());
}

QString LoveCommunicationTcp::sendOperatorValue(int operator_id, bool attack, int envelope_index, float frequency, float amplitude)
{
    QByteArray bytes;
    bytes.append((unsigned char)(0x10 + operator_id));
    bytes.append((attack ? 0x80 : 0x00) + envelope_index);
    unsigned char buf[5];
    fragment_floating(frequency, buf);
    bytes.append(QByteArray::fromRawData((char*)buf, 5));
    fragment_floating(amplitude, buf);
    bytes.append(QByteArray::fromRawData((char*)buf, 5));
    return QString::fromUtf8(bytes.toHex(' '));
}

void LoveCommunicationTcp::addModulator(int operator_id, int modulator_id)
{
}

void LoveCommunicationTcp::removeModulator(int operator_id, int modulator_id)
{
}

void LoveCommunicationTcp::addCarrier(int operator_id)
{
}

void LoveCommunicationTcp::removeCarrier(int operator_id)
{
}