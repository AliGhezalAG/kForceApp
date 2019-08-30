#ifndef KINVENTKFORCEDATAHANDLER_H
#define KINVENTKFORCEDATAHANDLER_H

#include "datahandler.h"

class KinventKForceDataHandler : public DataHandler
{
    Q_OBJECT

public:
    KinventKForceDataHandler();
    ~KinventKForceDataHandler();
    void processData(QString &deviceAddress, QByteArray &receivedData);
    void setMeasurementMultiplier(const QByteArray &data);
    void setBaseline(const QByteArray &data);
    void setRealTimeClock(const QByteArray &data);
    double getMeasurementMultiplier();
    int getBaseline1();
    int getBaseline2();
    QString getMessage();
    void clearMessage();

private:
    double  measurementMultiplier;
    int     baseline1;
    int     baseline2;
    tm      *realTimeClock;
    QString message;

signals:
    void processMeasurementMultiplierFinished();
    void processBaselineFinished();
    void processRealTimeClockFinished();
};

#endif // KINVENTKFORCEDATAHANDLER_H
