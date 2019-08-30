#include "kinventkforcedatahandler.h"

KinventKForceDataHandler::KinventKForceDataHandler(){}
KinventKForceDataHandler::~KinventKForceDataHandler(){}

void KinventKForceDataHandler::setMeasurementMultiplier(const QByteArray &data)
{
    QByteArray val = QByteArray::fromHex(data.toHex(':'));
    measurementMultiplier = val.toInt()/ MEASUREMENT_MULTIPLIER_DENOMINATOR;
    message = "Measurement multiplier : " + QString::number(measurementMultiplier, 'g', 6);
    emit processMeasurementMultiplierFinished();
}

void KinventKForceDataHandler::setBaseline(const QByteArray &data)
{
    baseline1 = byteArrayToInt(data.mid(0,2));
    baseline2 = byteArrayToInt(data.mid(2,2));
    message =   "Baseline 1 : " + QString::number(baseline1) + "\n"
                                                               "Baseline 2 : " + QString::number(baseline2);
    emit processBaselineFinished();
}

void KinventKForceDataHandler::setRealTimeClock(const QByteArray &data)
{
    realTimeClock = new tm();
    realTimeClock->tm_hour = byteArrayToInt(data.mid(0,1));
    realTimeClock->tm_min = byteArrayToInt(data.mid(1,1));
    realTimeClock->tm_sec = byteArrayToInt(data.mid(2,1));
    realTimeClock->tm_year = byteArrayToInt(data.mid(3,1))+100;
    realTimeClock->tm_mon = byteArrayToInt(data.mid(4,1))-1;
    realTimeClock->tm_mday = byteArrayToInt(data.mid(5,1));
    realTimeClock->tm_wday = byteArrayToInt(data.mid(6,1));


    stringstream transTime;
    transTime << "Real time clock : " << put_time(realTimeClock, "%a %d %b %Y - %I:%M:%S%p");
    message = QString::fromStdString(transTime.str());
    emit processRealTimeClockFinished();
}

void KinventKForceDataHandler::processData(QString &deviceAddress, QByteArray &receivedData)
{
    int count = 0;
    std::time_t timeDate = std::time(nullptr);
    string timeDateStr = std::ctime(&timeDate);
    timeDateStr.erase(std::remove(timeDateStr.begin(), timeDateStr.end(), '\n'), timeDateStr.end());

    QString resultFileName = deviceAddress + "-results.csv";

    QDir dir(RESULTS_DIR_PATH);
    if (!dir.exists())
        dir.mkpath(".");

    logFile.open (RESULTS_DIR_PATH + resultFileName.toStdString(), ios::out | ios::app);

    logFile << "Time/date, Device address, Measurement multiplier, Baseline 1, Baseline 2, Timestamp, Nb of measures, Base measure 1, Base measure 2, Measure 1, Measure 2" << endl;

    int packetsCount = 0;
    while(count < receivedData.size()){

        int entryDataNb = byteArrayToInt(receivedData.mid(count,3));

        if (entryDataNb == 0)
            break;

        int entryTimeStamp = byteArrayToInt(receivedData.mid(count+3,4));

        logFile << timeDateStr << ","
                << deviceAddress.toStdString() << ","
                << measurementMultiplier << ","
                << baseline1 << ","
                << baseline2 << ","
                << entryTimeStamp << ","
                << (entryDataNb/4) << ","
                <<  ","
                 <<  ","
                  <<  ","
                   << endl;

        for (int j = count + 7; j < count + static_cast<int>(entryDataNb) ; j+=4) {
            int baseMes1 = byteArrayToInt(receivedData.mid(j,2));
            int baseMes2 = byteArrayToInt(receivedData.mid(j+2,2));
            double mes1 = (baseline1 - baseMes1) * measurementMultiplier;
            double mes2 = (baseline2 - baseMes2) * measurementMultiplier;

            logFile << ","
                    << ","
                    << ","
                    << ","
                    << ","
                    << ","
                    << ","
                    << baseMes1 << ","
                    << baseMes2 << ","
                    << mes1 << ","
                    << mes2
                    << endl;
        }

        count += entryDataNb+7;
        packetsCount ++;
    }

    logFile.close();
    message =   "Data processing finished! \n" +
                QString::number(packetsCount) + " entries received \n" +
                resultFileName + " created successfully!";
    emit processingFinished();
}

double KinventKForceDataHandler::getMeasurementMultiplier()
{
    return measurementMultiplier;
}

int KinventKForceDataHandler::getBaseline1()
{
    return baseline1;
}

int KinventKForceDataHandler::getBaseline2()
{
    return baseline2;
}

QString KinventKForceDataHandler::getMessage()
{
    return message;
}

void KinventKForceDataHandler::clearMessage()
{
    message.clear();
}
