#include "kinventkforcedatahandler.h"

KinventKForceDataHandler::KinventKForceDataHandler(){}
KinventKForceDataHandler::~KinventKForceDataHandler(){}

void KinventKForceDataHandler::setMeasurementMultiplier(const QByteArray &data)
{
    // get value from hexified data
    QByteArray val = QByteArray::fromHex(data.toHex(':'));
    // devide by the measurement multiplier denominator to get the final value
    measurementMultiplier = val.toInt()/ MEASUREMENT_MULTIPLIER_DENOMINATOR;
    // create the update message
    message = "Measurement multiplier : " + QString::number(measurementMultiplier, 'g', 6);
    emit processMeasurementMultiplierFinished();
}

void KinventKForceDataHandler::setBaseline(const QByteArray &data)
{
    // get both values from read data
    baseline1 = byteArrayToInt(data.mid(0,2));
    baseline2 = byteArrayToInt(data.mid(2,2));
    // create the update message
    message = "Baseline 1 : "
            + QString::number(baseline1)
            + "\n"
            + "Baseline 2 : "
            + QString::number(baseline2);
    emit processBaselineFinished();
}

void KinventKForceDataHandler::setBatteryLevel(const QByteArray &data)
{
    // get second byte from read data
    batteryLevel = byteArrayToInt(data.mid(1,1));
    // create the update message
    message = "Battery level : "
            + QString::number(batteryLevel);
    emit processBatteryLevelFinished();
}

void KinventKForceDataHandler::setFirmwareVersion(const QByteArray &data)
{
    // get second byte from read data
    QString version(data);
    firmwareVersion = version;
    // create the update message
    message = "Firmware : "
            + firmwareVersion;
    emit processFirmwareVersionFinished();
}

void KinventKForceDataHandler::setRealTimeClock(const QByteArray &data)
{
    // initiate a new tm from the received data
    realTimeClock = new tm();
    realTimeClock->tm_hour = byteArrayToInt(data.mid(0,1)); //hours
    realTimeClock->tm_min = byteArrayToInt(data.mid(1,1));  //minutes
    realTimeClock->tm_sec = byteArrayToInt(data.mid(2,1));  //seconds
    realTimeClock->tm_year = byteArrayToInt(data.mid(3,1))+100; //year
    realTimeClock->tm_mon = byteArrayToInt(data.mid(4,1))-1;    //month
    realTimeClock->tm_mday = byteArrayToInt(data.mid(5,1)); //month day
    realTimeClock->tm_wday = byteArrayToInt(data.mid(6,1)); //weak day

    // put the formatted tm into a string stream
    stringstream transTime;
    transTime << "Real time clock : " << put_time(realTimeClock, "%a %d %b %Y - %I:%M:%S%p");
    // get the stream into a string and set the update message
    message = QString::fromStdString(transTime.str());
    emit processRealTimeClockFinished();
}

void KinventKForceDataHandler::processData(QString &deviceAddress, QByteArray &receivedData)
{

    // Save the data to a file.
    QSaveFile file("./results/dataDump");
    file.open(QIODevice::WriteOnly);
    file.write(receivedData);
    // Calling commit() is mandatory, otherwise nothing will be written.
    file.commit();

    // get the system time clock
    std::time_t timeDate = std::time(nullptr);
    string timeDateStr = std::ctime(&timeDate);
    timeDateStr.erase(std::remove(timeDateStr.begin(), timeDateStr.end(), '\n'), timeDateStr.end());

    // define the output file name
    QString resultFileName = deviceAddress + "-results.csv";

    // create the output folder if it doesn't exist
    QDir dir(RESULTS_DIR_PATH);
    if (!dir.exists())
        dir.mkpath(".");

    // open the output file stream
    logFile.open (RESULTS_DIR_PATH + resultFileName.toStdString(), ios::out | ios::app);

    // set the output file header
    logFile << "Time/date, Device address, Measurement multiplier, Baseline 1, Baseline 2, Timestamp, Nb of measures, Base measure 1, Base measure 2, Measure 1, Measure 2" << endl;

    // init counters
    int packetsCount = 0; //packets counter
    int count = 0;  // processed data size counter
    QByteArray dataNb = receivedData.mid(count,3);

    // while all data is not yet processed
    while(count < receivedData.size()){

        // get the entry data size (in number of bytes)
        dataNb = receivedData.mid(count,3);
        int entryDataNb = byteArrayToInt(receivedData.mid(count,3));

        // if no data, break
        if (entryDataNb == 0)
            break;

        // get the entry timestamp
        int entryTimeStamp = byteArrayToInt(receivedData.mid(count+3,4)) + TIMESTAMP_OFFSET;

        // write the general infos in the output file
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

        // process measurements packets
        // the general infos data is encoded in 7 bytes, so the measurement packet starts at the entry start index + 7
        // each measurement is encoded in 4 bytes (2 for channel 1 and 2 for channel 2) so we use a step of 4
        for (int j = count + 7; j < count + 7 + static_cast<int>(entryDataNb) ; j+=4) {
            // get the base measurments
            int baseMes1 = byteArrayToInt(receivedData.mid(j,2));
            int baseMes2 = byteArrayToInt(receivedData.mid(j+2,2));
            // calculate the final measurment value using the formula: final measurement val = (baseline - base measurment val) x measurment multiplier
            double mes1 = (baseline1 - baseMes1) * measurementMultiplier;
            double mes2 = (baseline2 - baseMes2) * measurementMultiplier;

            // write the measurement values in the output file
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

        // increment the processed packets size coounter
        count += entryDataNb+7;
        // increment the packet number counter
        packetsCount ++;
    }

    // close the output file stream
    logFile.close();
    // create the update message
    message =   "Data processing finished! \n" +
            QString::number(packetsCount) + " entries received \n" +
            resultFileName + " created successfully!";
    emit processingFinished();
}

void KinventKForceDataHandler::processStoredData(QString &deviceAddress, QByteArray &receivedData)
{
    // get the system time clock
    std::time_t timeDate = std::time(nullptr);
    string timeDateStr = std::ctime(&timeDate);
    timeDateStr.erase(std::remove(timeDateStr.begin(), timeDateStr.end(), '\n'), timeDateStr.end());

    // define the output file name
    QString resultFileName = deviceAddress + "-results.csv";

    // create the output folder if it doesn't exist
    QDir dir(RESULTS_DIR_PATH);
    if (!dir.exists())
        dir.mkpath(".");

    // open the output file stream
    logFile.open (RESULTS_DIR_PATH + resultFileName.toStdString(), ios::out | ios::app);

    // set the output file header
    logFile << "Time/date, Device address, Timestamp, Nb of measures, Measure 1, Measure 2" << endl;

    // init counters
    int packetsCount = byteArrayToInt(receivedData.mid(3,3));
    int entryDataNb = 0;
    int count = 6;

    for (int i=0 ; i<packetsCount ; i++){
        // get the entry data size (in number of bytes)
        entryDataNb = byteArrayToInt(receivedData.mid(count,3));

        // get the entry timestamp
        int entryTimeStamp = byteArrayToInt(receivedData.mid(count+3,4)) + TIMESTAMP_OFFSET;

        // write the general infos in the output file
        logFile << timeDateStr << ","
                << deviceAddress.toStdString() << ","
                << entryTimeStamp << ","
                << entryDataNb << ","
                <<  ","
                <<  ","
                <<  ","
                << endl;

        // process measurements packets
        // the general infos data is encoded in 7 bytes, so the measurement packet starts at the entry start index + 7
        // each measurement is encoded in 6 bytes (3 for channel 1 and 3 for channel 2) so we use a step of 6
        for (int j = count + 7; j < count + 7 + (static_cast<int>(entryDataNb)*6) ; j+=6) {
            double mes1 = byteArrayToInt(receivedData.mid(j,3))/1000.0;
            double mes2 = byteArrayToInt(receivedData.mid(j+3,3))/1000.0;

            // write the measurement values in the output file
            logFile << ","
                    << ","
                    << ","
                    << ","
                    << mes1 << ","
                    << mes2
                    << endl;
        }
        // increment the processed packets size coounter
        // 10 = 7 for general infos + 3 for episeparator
        count += (static_cast<int>(entryDataNb)*6)+10;
    }

    // close the output file stream
    logFile.close();
    // create the update message
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
