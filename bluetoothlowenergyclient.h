#ifndef BLUETOOTHLOWENERGYCLIENT_H
#define BLUETOOTHLOWENERGYCLIENT_H

#include "bluetoothclient.h"
#include "kinventkforcedatahandler.h"
#include <QLowEnergyController>

enum Request {
    GET_MEASUREMENT_MULTIPLIER,
    GET_BASELINES,
    GET_MEASUREMENT,
    GET_REAL_TIME_CLOCK,
    NO_REQUEST
};

class BluetoothLowEnergyClient : public BluetoothClient
{
    Q_OBJECT

public:
    BluetoothLowEnergyClient(QString address);
    ~BluetoothLowEnergyClient();
    void start();
    Q_INVOKABLE void read();
    Q_INVOKABLE void write(const QByteArray &data);
    Q_INVOKABLE void gererNotification(bool notification);
    QLowEnergyController::ControllerState getControllerState();
    KinventKForceDataHandler* getDataHandler();
    QString getMessage();
    void setTimeClock();
    void getRealTimeClock();
    void getMeasurementMultiplier();
    void getBaseline();
    void getData();
    void resetMemory();
    void stop();

protected slots:
    void deviceDisconnected();
    void connecterService(QLowEnergyService *service);
    void ajouterService(QBluetoothUuid serviceUuid);
    void serviceDetailsDiscovered(QLowEnergyService::ServiceState newState);
    void serviceCharacteristicChanged(const QLowEnergyCharacteristic &c, const QByteArray &value);
    void deviceDiscoverServices();

private:
    QLowEnergyController           *m_controller;
    QLowEnergyService              *m_service;
    QLowEnergyCharacteristic        m_txCharacteristic;
    QLowEnergyCharacteristic        m_rxCharacteristic;
    bool                            timeClockSet;
    KinventKForceDataHandler        *dataHandler;
    Request                         request;

signals:
    void compteurChange();
    void processTimeClockFinished();
    void processMeasurementMultiplierFinished();
    void processBaselineFinished();
    void deviceConnected();
};

#endif // BLUETOOTHLOWENERGYCLIENT_H
