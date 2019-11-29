#include "bluetoothlowenergyclient.h"

BluetoothLowEnergyClient::BluetoothLowEnergyClient(QString address) : BluetoothClient(address), m_service(nullptr),
    timeClockSet(false)
{
    qDebug() << Q_FUNC_INFO;
    // init controller
    m_controller =  new QLowEnergyController(QBluetoothAddress(address), this);
    // init data handler
    dataHandler = new KinventKForceDataHandler();
}

BluetoothLowEnergyClient::~BluetoothLowEnergyClient()
{
    qDebug() << Q_FUNC_INFO;

    // disconnect frm device before deleting the controller
    if (m_controller)
        m_controller->disconnectFromDevice();
    delete m_controller;
    delete dataHandler;
}

KinventKForceDataHandler* BluetoothLowEnergyClient::getDataHandler()
{
    return dataHandler;
}

QString BluetoothLowEnergyClient::getMessage()
{
    return dataHandler->getMessage();
}

void BluetoothLowEnergyClient::start()
{
    // connect the controller's signals tpo slots
    connect(m_controller, SIGNAL(serviceDiscovered(QBluetoothUuid)), this, SLOT(ajouterService(QBluetoothUuid)));
    connect(m_controller, SIGNAL(connected()), this, SLOT(deviceDiscoverServices()));
    connect(m_controller, SIGNAL(disconnected()), this, SLOT(deviceDisconnected()));

    qDebug() << Q_FUNC_INFO << "demande de connexion";
    m_controller->setRemoteAddressType(QLowEnergyController::PublicAddress);
    // connect to device and start discovering services
    m_controller->connectToDevice();
}

void BluetoothLowEnergyClient::stop()
{
    //    qDebug() << Q_FUNC_INFO << deviceAddress;
    if (m_controller)
        m_controller->disconnectFromDevice();
}

void BluetoothLowEnergyClient::read()
{
    // if service and read characteristic are available
    if(m_service && m_txCharacteristic.isValid())
    {
        if (m_txCharacteristic.properties() & QLowEnergyCharacteristic::Read)
        {
            m_service->readCharacteristic(m_txCharacteristic);
            //            qDebug() << Q_FUNC_INFO << m_txCharacteristic.value() << m_compteur;
            //            qDebug() << (int)qFromLittleEndian<quint8>(m_txCharacteristic.value().constData());
            emit compteurChange();
        }
    }
}

void BluetoothLowEnergyClient::write(const QByteArray &data)
{
    // if service and write characteristic are available
    if(m_service && m_rxCharacteristic.isValid())
    {
        if (m_rxCharacteristic.properties() & QLowEnergyCharacteristic::Write)
        {
            m_service->writeCharacteristic(m_rxCharacteristic, data, QLowEnergyService::WriteWithResponse);
            // TODO: deal with the case where data length > MAX_SIZE
        }
    }
}

void BluetoothLowEnergyClient::gererNotification(bool notification)
{
    if(m_service && m_txCharacteristic.isValid())
    {
        if (m_txCharacteristic.properties() & QLowEnergyCharacteristic::Notify)
        {
            QLowEnergyDescriptor descripteurNotification = m_txCharacteristic.descriptor(QBluetoothUuid::ClientCharacteristicConfiguration);
            if (descripteurNotification.isValid())
            {
                // active la notification : 0100 ou désactive 0000
                qDebug() << Q_FUNC_INFO << "modification notification" << m_txCharacteristic.uuid().toString() << notification;
                if(notification)
                    m_service->writeDescriptor(descripteurNotification, QByteArray::fromHex("0100"));
                else
                    m_service->writeDescriptor(descripteurNotification, QByteArray::fromHex("0000"));
            }
        }
    }
}

// TODO
void BluetoothLowEnergyClient::setTimeClock()
{
    time_t now = time(nullptr);
    tm *ltm = localtime(&now);

    qInfo() << "Time clock set to:" << endl;
    qInfo() << "Year: " << 1900 + ltm->tm_year<<endl;
    qInfo() << "Month: "<< 1 + ltm->tm_mon<< endl;
    qInfo() << "Day: "<<  ltm->tm_mday << endl;
    qInfo() << "Day of week: "<<  ltm->tm_wday << endl;
    qInfo() << "Time: "<< ltm->tm_hour << ":" << 1 + ltm->tm_min << ":" << 1 + ltm->tm_sec << endl;

    QByteArray timeClock;
    QByteArray ba;
    ba.setNum(1900 + ltm->tm_year, 16);
    timeClock.append(ba);
    ba.setNum(1 + ltm->tm_min, 16);
    timeClock.append(ba);
    ba.setNum(1 + ltm->tm_sec, 16);
    timeClock.append(ba);
    ba.setNum(1900 + ltm->tm_year, 16);
    timeClock.append(ba);
    ba.setNum(1 + ltm->tm_mon, 16);
    timeClock.append(ba);
    ba.setNum(ltm->tm_mday, 16);
    timeClock.append(ba);
    ba.setNum(ltm->tm_wday, 16);
    timeClock.append(ba);

    QByteArray command = QByteArray::fromHex(SET_TIME_CLOCK_COMMAND);
    command.append(timeClock);
    write(command);
}

void BluetoothLowEnergyClient::getRealTimeClock()
{
    request = GET_REAL_TIME_CLOCK;
    QByteArray command = QByteArray::fromHex(GET_REAL_TIME_CLOCK_COMMAND);
    write(command);
}

void BluetoothLowEnergyClient::getMeasurementMultiplier()
{
    request = GET_MEASUREMENT_MULTIPLIER;
    QByteArray command = QByteArray::fromHex(GET_MEASURE_MULYIPLIER_COMMAND);
    write(command);
}

void BluetoothLowEnergyClient::getBaseline()
{
    request = GET_BASELINES;
    QByteArray command = QByteArray::fromHex(GET_BASELINE_COMMAND);
    write(command);
}

void BluetoothLowEnergyClient::getBatteryLevel()
{
    request = GET_BATTERY_LEVEL;
    QByteArray command = QByteArray::fromHex(GET_BATTERY_LEVE_COMMAND);
    write(command);
}

void BluetoothLowEnergyClient::getFirmwareVersion()
{
    request = GET_FIRMWARE_VERSION;
    QByteArray command = QByteArray::fromHex(GET_FIRMWARE_VERSION_COMMAND);
    write(command);
}

void BluetoothLowEnergyClient::getData()
{
    request = GET_MEASUREMENT;
    QByteArray command = QByteArray::fromHex(GET_MEMORY_DUMP_COMMAND);
    write(command);
}

void BluetoothLowEnergyClient::resetMemory()
{
    QByteArray command = QByteArray::fromHex(RESET_MEMORY_COMMAND);
    write(command);
}

void BluetoothLowEnergyClient::resetBaselines()
{
    QByteArray command = QByteArray::fromHex(RESET_BASELINE_COMMAND);
    write(command);
}

void BluetoothLowEnergyClient::connecterService(QLowEnergyService *service)
{
    m_service = service;

    // if current service characteristics need to be discovered
    if (m_service->state() == QLowEnergyService::DiscoveryRequired)
    {
        // Slot to change a characteristic
        connect(m_service, SIGNAL(characteristicChanged(QLowEnergyCharacteristic,QByteArray)), this, SLOT(serviceCharacteristicChanged(QLowEnergyCharacteristic,QByteArray)));
        // Slot to get a characteristic
        connect(m_service, SIGNAL(stateChanged(QLowEnergyService::ServiceState)), this, SLOT(serviceDetailsDiscovered(QLowEnergyService::ServiceState)));

        qDebug() << Q_FUNC_INFO << "Discovering service details";
        m_service->discoverDetails();
    }
}

void BluetoothLowEnergyClient::ajouterService(QBluetoothUuid serviceUuid)
{
    // when a service is discovered, create a corresponding object
    QLowEnergyService *service = m_controller->createServiceObject(serviceUuid);
    // and connect to service for characteristic discovery
    connecterService(service);
}

void BluetoothLowEnergyClient::serviceCharacteristicChanged(const QLowEnergyCharacteristic &c, const QByteArray &value)
{
    // if read characteristic changed, process the received data based on the request type
    if (c.uuid().toString() == CHARACTERISTIC_UUID)
    {
        switch(request)
        {
        case GET_MEASUREMENT_MULTIPLIER  :
            dataHandler->setMeasurementMultiplier(value);
            request = NO_REQUEST;
            break;
        case GET_BASELINES:
            dataHandler->setBaseline(value);
            request = NO_REQUEST;
            break;
        case GET_MEASUREMENT :
            receivedData.append(value);
            if (receivedData.size() >= MEMORY_SIZE){
                dataHandler->processData(this->deviceAddress, receivedData);
                request = NO_REQUEST;
                receivedData.clear();
            }
            break;
        case GET_REAL_TIME_CLOCK :
            dataHandler->setRealTimeClock(value);
            request = NO_REQUEST;
            break;
        case GET_BATTERY_LEVEL :
            dataHandler->setBatteryLevel(value);
            request = NO_REQUEST;
            break;
        case GET_FIRMWARE_VERSION :
            dataHandler->setFirmwareVersion(value);
            request = NO_REQUEST;
            break;
        default:
            break;
        }
    }
}

void BluetoothLowEnergyClient::serviceDetailsDiscovered(QLowEnergyService::ServiceState newState)
{
    Q_UNUSED(newState)

    // décourverte ?
    if (newState != QLowEnergyService::ServiceDiscovered)
    {
        return;
    }

    QLowEnergyService *service = qobject_cast<QLowEnergyService *>(sender());
    //    qDebug() << Q_FUNC_INFO << "service" << service->serviceUuid().toString();

    if (service->serviceUuid().toString() == SERVICE_UUID)
    {
        foreach (QLowEnergyCharacteristic c, service->characteristics())
        {
            //            qDebug() << Q_FUNC_INFO << "characteristic" << c.uuid().toString();
            if (c.uuid().toString() == CHARACTERISTIC_UUID)
            {
                //                qDebug() << Q_FUNC_INFO << "my characteristic TX" << c.uuid().toString() << (c.properties() & QLowEnergyCharacteristic::Notify);
                m_txCharacteristic = c;

                QLowEnergyDescriptor descripteurNotification = c.descriptor(QBluetoothUuid::ClientCharacteristicConfiguration);
                if (descripteurNotification.isValid())
                {
                    // active la notification : 0100 ou désactive 0000
                    //                    qDebug() << Q_FUNC_INFO << "modification notification" << c.uuid().toString();
                    service->writeDescriptor(descripteurNotification, QByteArray::fromHex("0100"));
                }

                //                qDebug() << Q_FUNC_INFO << "my characteristic RX" << c.uuid().toString() << (c.properties() & QLowEnergyCharacteristic::Write);
                m_service = service;
                m_rxCharacteristic = c;
            }
        }

        emit deviceConnected();
    }
}

void BluetoothLowEnergyClient::deviceDiscoverServices()
{
    //    qDebug() << Q_FUNC_INFO;
    m_controller->discoverServices();
}

void BluetoothLowEnergyClient::deviceDisconnected()
{
    emit doneProcessing();
}

QLowEnergyController::ControllerState BluetoothLowEnergyClient::getControllerState()
{
    return m_controller->state();
}
