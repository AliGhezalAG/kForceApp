#include "datahandler.h"

DataHandler::DataHandler(){}
DataHandler::~DataHandler(){}
void DataHandler::processData(QString&, QByteArray&){}

int DataHandler::byteArrayToInt(const QByteArray &bytes){
    // get the hexified value from the byte array
    QByteArray bytesToHex = bytes.toHex(0);
    bool ok;
    // convert the byte array to int using base 16
    int hex = bytesToHex.toInt(&ok, 16);
    return hex;
}

double DataHandler::byteArrayToDouble(const QByteArray &bytes){
    // get the hexified value from the byte array
    QByteArray bytesToHex = bytes.toHex(0);
    // convert the byte array to double
    double hex = bytesToHex.toDouble();
    return hex;
}
