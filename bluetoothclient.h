#ifndef BLUETOOTHCLIENT_H
#define BLUETOOTHCLIENT_H

#include "init.h"

#include <QDebug>
#include "datahandler.h"

/// @brief Class for generic Bluetooth client
/// @author Ali Ghezal

class BluetoothClient : public QObject
{
    Q_OBJECT

public:
    /// @brief Construct Bluetooth client
    /// @param address  the bluetooth device IP address
    BluetoothClient(QString address);
    /// Class destructor
    virtual ~BluetoothClient();
    /// @brief Start client.
    virtual void start();

protected slots:
    /// @brief Stop client.
    virtual void stop();
    /// @brief Process the received data.
    virtual void processReceivedData();
    /// @brief Disconnect device.
    virtual void deviceDisconnected();

signals:
    void processingFinished();
    void doneProcessing();

protected:
    QString     deviceAddress;
    ofstream    logFile;
    QByteArray  receivedData;
};

#endif // BLUETOOTHCLIENT_H
