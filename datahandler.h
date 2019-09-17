#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include "init.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <QDir>

/// @brief Class for generic data handler
/// @author Ali Ghezal

class DataHandler : public QObject
{
    Q_OBJECT

public:
    /// @brief Construct data handler
    DataHandler();
    /// Class destructor
    virtual ~DataHandler();
    /// @brief Process memory dump data
    /// @param address  the bluetooth device IP address
    /// @param receivedData  the received data
    virtual void processData(QString &deviceAddress, QByteArray &receivedData);

protected:
    ofstream    logFile;
    int         byteArrayToInt(const QByteArray &bytes);
    double      byteArrayToDouble(const QByteArray &bytes);

signals:
    void processingFinished();
};

#endif // DATAHANDLER_H
