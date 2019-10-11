#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QRegExp>
#include <QMessageBox>
#include <QtSerialPort/QSerialPort>
#include <QSerialPortInfo>
#include <QThread>
#include <QList>

const QSerialPort::FlowControl    flow        = QSerialPort::NoFlowControl;//NoFlowControl;
const QSerialPort::Parity         parity      = QSerialPort::NoParity;//NoParity;
const QSerialPort::DataBits       databits    = QSerialPort::Data8;//Data8;
const QSerialPort::BaudRate       baudrate    = static_cast<QSerialPort::BaudRate>(9600);
const QSerialPort::StopBits       stopbits    = QSerialPort::OneStop;//OneStop

const QRegExp incomming("#[U,D,R][1,2,S]%[0-9, :-?][0-9, :-?][0-9, :-?][0-9, :-?]!");

typedef QList<QString> QPortName;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QSerialPort *prt;
    QPortName ports;
    QString currentPort;

    bool Open_COM();
    int getVal(QString str, QString separator1, QString separator2, bool* ok);

private slots:
    void Refresh_COM();
    void Config_Port();
    void Read_COM();
    void Close_COM();

    void Start();
    void on_cmbCOM_currentIndexChanged(const QString &arg1);
};
#endif // MAINWINDOW_H
