#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "bluetoothlowenergyclient.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void init();
    void update(QString message);

private:
    Ui::MainWindow              *ui;
    BluetoothLowEnergyClient    *bluetoothClient;

private slots:
    void on_connectButton_clicked();
    void on_getMeasurementMultiplierButton_clicked();
    void on_getBaselinesButton_clicked();
    void on_dumpMemoryButton_clicked();
    void on_resetMemoryButton_clicked();
    void on_getRealTimeClockButton_clicked();
    void on_setTimeClockButton_clicked();
    void on_disconnectButton_clicked();
    void updateDisplay();
    void clearDisplay();
    void doneDisconnecting();
    void doneConnecting();
};

#endif // MAINWINDOW_H
