#ifndef CONNEXIONHANDLER_H
#define CONNEXIONHANDLER_H

#include "init.h"

#include <QBluetoothDeviceDiscoveryAgent>
#include "bluetoothlowenergyclient.h"
#include <QEventLoop>
#include <QHash>

class ConnexionHandler : public QObject
{
    Q_OBJECT

public:
    ConnexionHandler();
    ~ConnexionHandler();
    [[ noreturn ]] void start();

protected slots:
    void deviceScanError(QBluetoothDeviceDiscoveryAgent::Error error);
    void addDevice(const QBluetoothDeviceInfo &device);
    void processDevices();

private:
    QBluetoothDeviceDiscoveryAgent              *deviceDiscoveryAgent;
    QHash<QString, BluetoothClient*>            clientList;
    QHash<QString, BluetoothLowEnergyClient*>   bluetoothLowEnergyClientList;
    QList<QString>                              bluetoothLowEnergyDevicesList = {"80:1F:12:B1:3C:D7", "80:1F:12:B1:3A:C8", "80:1F:12:B1:3C:D2", "80:1F:12:B1:3C:B8", "80:1F:12:B1:3A:FA"};
    QList<QString>                              discoveredDevicesList;
    ofstream                                    logFile;

signals:
    void scanProcessingEnded();
};

#endif // CONNEXIONHANDLER_H
