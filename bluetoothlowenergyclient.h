#ifndef BLUETOOTHLOWENERGYCLIENT_H
#define BLUETOOTHLOWENERGYCLIENT_H

#include "bluetoothclient.h"
#include "kinventkforcedatahandler.h"
#include <QLowEnergyController>

/// @brief Requests to device
enum Request {
    /// Get measurement multiplier request
    GET_MEASUREMENT_MULTIPLIER,
    /// Get baselines request
    GET_BASELINES,
    /// Get measurements (dump memory data) request
    GET_MEASUREMENT,
    /// Get measurements (stored data) request
    GET_STORED_MEASUREMENT,
    /// Get real time clock request
    GET_REAL_TIME_CLOCK,
    /// Get alarm set point request
    GET_ALARM_SET_POINT,
    /// Get battery level request
    GET_BATTERY_LEVEL,
    /// Get memory usage level request
    GET_MEMORY_USAGE_LEVEL,
    /// Get firmware version request
    GET_FIRMWARE_VERSION,
    /// No request
    NO_REQUEST
};

/// @brief Class for Bluetooth low energy (BLE) client
/// @author Ali Ghezal

class BluetoothLowEnergyClient : public BluetoothClient
{
    Q_OBJECT

public:
    /// @brief Construct Bluetooth low energy client
    /// @param address  the bluetooth device IP address
    BluetoothLowEnergyClient(QString address);
    /// Class destructor
    ~BluetoothLowEnergyClient();
    /// @brief Start client.
    void start();
    /// @brief Read data from the Rx characteristic.
    Q_INVOKABLE void read();
    /// @brief Write data to the Tx characteristic.
    /// @param data  data to write
    Q_INVOKABLE void write(const QByteArray &data);
    /// @brief Handle a notification.
    /// @param notification  notification
    Q_INVOKABLE void gererNotification(bool notification);
    /// @brief Controller state getter.
    QLowEnergyController::ControllerState getControllerState();
    /// @brief Data handler getter.
    KinventKForceDataHandler* getDataHandler();
    /// @brief Update message getter.
    QString getMessage();
    /// @brief Command to set time clock.
    void setTimeClock();
    /// @brief Command to set alarm set-point.
    void setAlarmSetPoint(int hours, int minutes);
    /// @brief Command to set alarm period.
    void setAlarmPeriod(int periodToSet);
    /// @brief Command to get real time clock.
    void getRealTimeClock();
    /// @brief Command to get alarm set-point
    void getAlarmSetPoint();
    /// @brief Command to get measurement multiplier.
    void getMeasurementMultiplier();
    /// @brief Command to get baselines.
    void getBaseline();
    /// @brief Command to dump all data from memory.
    void getData();
    /// @brief Command to g.et stored data.
    void getStoredData();
    /// @brief Command to reset internal memory.
    void resetMemory();
    /// @brief Command to reset the baselines.
    void resetBaselines();
    /// @brief Command to get the battery level.
    void getBatteryLevel();
    /// @brief Command to get the memory usage level.
    void getMemoryUsageLevel();
    /// @brief Command to get the firmware version.
    void getFirmwareVersion();
    /// @brief Stop client.
    void stop();

protected slots:
    void deviceDisconnected();
    /// @brief Connect to service.
    /// @param service service to connect to
    void connecterService(QLowEnergyService *service);
    /// @brief Add found service.
    /// @param found service uuid
    void ajouterService(QBluetoothUuid serviceUuid);
    /// @brief Process discovered service.
    /// @param newState discovered service state
    void serviceDetailsDiscovered(QLowEnergyService::ServiceState newState);
    /// @brief Process changed characteristic.
    /// @param c characteristic
    /// @param value characteristic new value
    void serviceCharacteristicChanged(const QLowEnergyCharacteristic &c, const QByteArray &value);
    /// @brief Start service discovery.
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
