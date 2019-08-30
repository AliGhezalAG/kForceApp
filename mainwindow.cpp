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

void MainWindow::init()
{

}

void MainWindow::update(QString message)
{
    ui->textBrowser->append(message);
}

void MainWindow::updateDisplay()
{
    QString msg = bluetoothClient->getMessage();
    if(msg.length() != 0){
        update(msg);
        bluetoothClient->getDataHandler()->clearMessage();
    }
}

void MainWindow::clearDisplay()
{
    ui->textBrowser->clear();
}

void MainWindow::on_connectButton_clicked(){
    clearDisplay();
    update("Connecting to device...");
    bluetoothClient = new BluetoothLowEnergyClient(ui->knobSelectionComboBox->currentText());

    connect(bluetoothClient, &BluetoothLowEnergyClient::deviceConnected, this, &MainWindow::doneConnecting);
    connect(bluetoothClient, &BluetoothLowEnergyClient::doneProcessing, this, &MainWindow::doneDisconnecting);

    connect(bluetoothClient->getDataHandler(), &KinventKForceDataHandler::processMeasurementMultiplierFinished, this, &MainWindow::updateDisplay);
    connect(bluetoothClient->getDataHandler(), &KinventKForceDataHandler::processBaselineFinished, this, &MainWindow::updateDisplay);
    connect(bluetoothClient->getDataHandler(), &KinventKForceDataHandler::processingFinished, this, &MainWindow::updateDisplay);
    connect(bluetoothClient->getDataHandler(), &KinventKForceDataHandler::processRealTimeClockFinished, this, &MainWindow::updateDisplay);

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
