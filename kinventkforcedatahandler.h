#ifndef KINVENTKFORCEDATAHANDLER_H
#define KINVENTKFORCEDATAHANDLER_H

#include "datahandler.h"
#include <QSaveFile>

/// @brief Class for Kinvent KForce data handler
/// @author Ali Ghezal

class KinventKForceDataHandler : public DataHandler
{
    Q_OBJECT

public:
    /// @brief Construct Kinvent KForce data handler
    KinventKForceDataHandler();
    /// Class destructor
    ~KinventKForceDataHandler();
    /// @brief Process memory dump data
    /// @param deviceAddress  the bluetooth device IP address
    /// @param receivedData  the received data
    void processData(QString &deviceAddress, QByteArray &receivedData);
    /// @brief Process stored data
    /// @param deviceAddress  the bluetooth device IP address
    /// @param receivedData  the received data
    void processStoredData(QString &deviceAddress, QByteArray &receivedData);
    /// @brief Set measurement multiplier from received data
    /// @param data  the received data
    void setMeasurementMultiplier(const QByteArray &data);
    /// @brief Set baselines from received data
    /// @param data  the received data
    void setBaseline(const QByteArray &data);
    /// @brief Set the battery level from received data
    /// @param data  the received data
    void setBatteryLevel(const QByteArray &data);
    /// @brief Set the memory usage level from received data
    /// @param data  the received data
    void setMemoryUsageLevel(const QByteArray &data);
    /// @brief Set the firmware version from received data
    /// @param data  the received data
    void setFirmwareVersion(const QByteArray &data);
    /// @brief Set real time clock from received data
    /// @param data  the received data
    void setRealTimeClock(const QByteArray &data);
    /// @brief Set alarm set-point from received data
    /// @param data  the received data
    void setAlarmSetPoint(const QByteArray &data);
    /// @brief Measurement multiplier getter
    double getMeasurementMultiplier();
    /// @brief Baseline 1 getter
    int getBaseline1();
    /// @brief Baseline 2 getter
    int getBaseline2();
    /// @brief Update message getter
    QString getMessage();
    /// @brief Clear update message
    void clearMessage();

private:
    double  measurementMultiplier;
    int     baseline1;
    int     baseline2;
    int     batteryLevel;
    int     memoryUsageLevel;
    tm      *realTimeClock;
    QString message;
    QString firmwareVersion;

signals:
    void processMeasurementMultiplierFinished();
    void processBaselineFinished();
    void processBatteryLevelFinished();
    void processMemoryUsageLevelFinished();
    void processRealTimeClockFinished();
    void processFirmwareVersionFinished();
};

#endif // KINVENTKFORCEDATAHANDLER_H
