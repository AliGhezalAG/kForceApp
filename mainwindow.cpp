#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::update(QString message)
{
    ui->textBrowser->append(message);
}

void MainWindow::updateDisplay()
{
    // get the current update message
    QString msg = bluetoothClient->getMessage();
    // if any
    if(msg.length() != 0){
        // update the display with the new message
        update(msg);
        // clear the message
        bluetoothClient->getDataHandler()->clearMessage();
    }
}

void MainWindow::clearDisplay()
{
    ui->textBrowser->clear();
}

// start a new connexion
void MainWindow::on_connectButton_clicked(){
    // clear display
    clearDisplay();
    update("Connecting to device...");
    // create a new BLE client with the selected device IP address
    bluetoothClient = new BluetoothLowEnergyClient(ui->knobSelectionComboBox->currentText());

    // connect signals to slots
    connect(bluetoothClient, &BluetoothLowEnergyClient::deviceConnected, this, &MainWindow::doneConnecting);
    connect(bluetoothClient, &BluetoothLowEnergyClient::doneProcessing, this, &MainWindow::doneDisconnecting);
    connect(bluetoothClient->getDataHandler(), &KinventKForceDataHandler::processMeasurementMultiplierFinished, this, &MainWindow::updateDisplay);
    connect(bluetoothClient->getDataHandler(), &KinventKForceDataHandler::processBaselineFinished, this, &MainWindow::updateDisplay);
    connect(bluetoothClient->getDataHandler(), &KinventKForceDataHandler::processingFinished, this, &MainWindow::updateDisplay);
    connect(bluetoothClient->getDataHandler(), &KinventKForceDataHandler::processRealTimeClockFinished, this, &MainWindow::updateDisplay);

    // start the client
    bluetoothClient->start();
};

void MainWindow::on_getMeasurementMultiplierButton_clicked()
{
    update("Getting measurement multiplier...");
    bluetoothClient->getMeasurementMultiplier();
};

void MainWindow::on_getBaselinesButton_clicked()
{
    update("Getting baselines...");
    bluetoothClient->getBaseline();
};

void MainWindow::on_dumpMemoryButton_clicked()
{
    update("Getting data...");
    bluetoothClient->getData();
};

void MainWindow::on_resetMemoryButton_clicked()
{
    // when reset memory is requested, we trigger a message ox to make sure this is the command the user really wanted
    QMessageBox msgBox;
    msgBox.setText("Your device stored data will be deleted.");
    msgBox.setInformativeText("Are you sure you want to reset the memory?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();

    switch (ret) {
    case QMessageBox::Yes:
        update("Resetting memory... done!");
        bluetoothClient->resetMemory();
        break;
    case QMessageBox::No:
        qInfo() << "Data not deleted";
        break;
    default:
        // should never be reached
        break;
    }
};

void MainWindow::on_getRealTimeClockButton_clicked()
{
    update("Getting real time clock...");
    bluetoothClient->getRealTimeClock();
};

void MainWindow::on_setTimeClockButton_clicked()
{
    update("Setting real time clock... Done!");
    bluetoothClient->setTimeClock();
}

void MainWindow::on_disconnectButton_clicked()
{
    update("Disconnecting...");
    bluetoothClient->stop();

};

void MainWindow::doneDisconnecting()
{
    update("Device disconnected!");
}

void MainWindow::doneConnecting()
{
    update("Device connected!");
}
