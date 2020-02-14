#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "bluetoothlowenergyclient.h"

namespace Ui {
class MainWindow;
}

/// @brief Class for GUI
/// @author Ali Ghezal

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /// @brief Construct GUI window
    explicit MainWindow(QWidget *parent = nullptr);
    /// Class destructor
    ~MainWindow();
    /// @brief Update display.
    /// @details Update the message displayed in the text area.
    /// @param message  new message to display
    void update(QString message);

private:
    Ui::MainWindow              *ui;
    BluetoothLowEnergyClient    *bluetoothClient;

private slots:
    void on_connectButton_clicked();
    void on_getMeasurementMultiplierButton_clicked();
    void on_getBaselinesButton_clicked();
    void on_dumpMemoryButton_clicked();
    void on_getStoredDataButton_clicked();
    void on_resetMemoryButton_clicked();
    void on_getRealTimeClockButton_clicked();
    void on_setTimeClockButton_clicked();
    void on_getAlarmSetPointButton_clicked();
    void on_setAlarmSetPointButton_clicked();
    void on_disconnectButton_clicked();
    void on_getBatteryLevelButton_clicked();
    void on_getMemoryUsageLevelButton_clicked();
    void on_resetBaselinesButton_clicked();
    void on_getFirmwareVersionButton_clicked();
    void updateDisplay();
    void clearDisplay();
    void doneDisconnecting();
    void doneConnecting();
};

#endif // MAINWINDOW_H
