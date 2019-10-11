#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    prt = new QSerialPort(this);
    ui->setupUi(this);

    setWindowTitle(tr("ДХС 529"));

    //tmr = new QTimer(this);

    ui->pbtWrite->setDisabled(true);
    ui->pbtCancel->setDisabled(true);

    connect(ui->pbtnStart, SIGNAL(clicked()), this, SLOT(Start()));
    connect(prt, SIGNAL(readyRead()), this, SLOT(Read_COM()));

    Refresh_COM();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::Open_COM()
{
    prt->setPortName(currentPort);
    if (currentPort != ""){

        if (prt->open(QIODevice::ReadWrite)){
            Config_Port();
            qDebug() << "COM-port is opened";
//            BlockInterface();
            prt->write("I'm ready for work!\n");
        }
        else{
            QMessageBox::critical(this, tr("Ошибка"), tr("Ошибка открытия порта"));
            return 0;
        }
    }
    else{
        QMessageBox::information(this, tr("Предупреждение"), tr("Не выбран COM порт"));
        return 0;
    }
    return 1;
}

int MainWindow::getVal(QString str, QString separator1, QString separator2, bool *ok)
{
    QString tmp;
    int begin = str.contains(separator1)?str.indexOf(separator1)+1:-1,
        end = str.contains(separator2)?str.indexOf(separator2,begin):-1;
    int res = 0;

    tmp.clear();
    for (int i = begin; i < end; i++){
        if (str.at(i) > '9') tmp += static_cast<QString>(str.at(i).toLatin1()+7);
        else tmp += static_cast<QString>(str.at(i));
    }
    res = tmp.toLatin1().toInt(ok, 16);
    return res;
}

void MainWindow::Refresh_COM()
{
    int defPort = 0;
    ports.clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        ports.append(info.portName());
    }
    if (currentPort != ""){
        for (int i = 0; i < ports.length(); ++i)
            if (ports.at(i) == currentPort){
                defPort = i;
                break;
            }
    }
    else
        defPort = 0;


    ui->cmbCOM->clear();

    for(int i = 0; i < ports.length(); ++i){
        ui->cmbCOM->addItem(ports.at(i), ports.at(i));
    }

    ui->cmbCOM->setCurrentIndex(defPort);
    currentPort = ui->cmbCOM->currentText();
    qDebug() << "Refresh_COM::currentPort=" << currentPort;
}

void MainWindow::Config_Port()
{
    if (prt->isOpen()){
        prt->setFlowControl(flow);
        prt->setParity(parity);
        prt->setDataBits(databits);
        prt->setBaudRate(baudrate);
        prt->setStopBits(stopbits);
    }
}

void MainWindow::Read_COM()
{
    QByteArray buf, u1, u2, derivate, result;
//    int N, N1,
//            firstOccurences,
//            separator,
//            firstOccurences_resh,firstOccurences_warn;

//    if (prt->bytesAvailable() < 18) return;

    buf = prt->readAll();

    qDebug() << buf;

//    N =  buf.count('#');
//    N1 = buf.count('!');

//    firstOccurences_resh = buf.indexOf('#',0);
//    firstOccurences_warn = buf.indexOf('!',0);

//    if ((N == N1)&&(firstOccurences_resh < firstOccurences_warn)){
//        firstOccurences = -1;
//        for (char i = 0; i < N; ++i){
//            firstOccurences = buf.indexOf('#',firstOccurences+1);

//            switch (buf[firstOccurences+1]) {
//            case 0x60:{
//                separator = buf.indexOf('%', firstOccurences);
//                adc1 = buf.mid(separator+1, 4);
//                ui->leADC1->setText(adc1);
//                separator = buf.indexOf('%', separator+1);
//                adc2 = buf.mid(separator+1, 4);
//                ui->leADC2->setText(adc2);
//                break;
//            }
//            case 0x61:{
//                separator = buf.indexOf('%', firstOccurences);
//                kadc2 = buf.mid(separator+1, 4);
//                ui->leKADC2->setText(kadc2);
//                separator = buf.indexOf('%', separator+1);
//                temperature = buf.mid(separator+1, 4);
//                ui->leTemperature->setText(temperature);
//                break;
//            }
//            case 0x64:{
//                //separator = buf.indexOf('%', firstOccurences);
//                res = buf.mid(firstOccurences+3, 4);
//                res.insert(3, ',');
//                ui->leWResult->setText(res);
//                break;
//            }
//            default:
//                break;
//            }

//            ++firstOccurences;
//        }
//    }

//    prt->clear(QSerialPort::AllDirections);

//    QThread::msleep(40);

//    prt->write(message);
    int lastPos;
    bool ok;
//    QString str="ighWq#U1%0012!lmkJ#DS%0:12!lmkJ#RS%0?85!lmkJ#RS%0?yy8ing";
    QString packet, command;
    int U1, U2, RS, DS;
    lastPos = 0;
    while ((lastPos = incomming.indexIn(buf, lastPos)) != -1) {
        lastPos += incomming.matchedLength();
        packet = incomming.cap(0);
        command = packet.mid(1,2);

        if (command == "U1"){
            ok = false;
            U1 = getVal(packet, "%", "!", &ok);

            if (ok) qDebug() << "U1=" << U1;
            else qDebug() << "error during U1 conversion";
        }
            else
        if (command == "U2"){
            ok = false;
            U2 = getVal(packet, "%", "!", &ok);

            if (ok) qDebug() << "U2=" << U2;
            else qDebug() << "error during U2 conversion";
        }
            else
        if (command == "DS"){
            ok = false;
            DS = getVal(packet, "%", "!", &ok);

            if (ok) qDebug() << "DS=" << DS;
            else qDebug() << "error during DS conversion";
        }
            else
        if (command == "RS"){
            ok = false;
            RS = getVal(packet, "%", "!", &ok);

            if (ok) qDebug() << "RS=" << RS;
            else qDebug() << "error during RS conversion";
        }
        qDebug() << packet << ",  " << command;
    }

}

void MainWindow::Close_COM()
{

}

void MainWindow::Start()
{
    if (Open_COM()){

    }
}


void MainWindow::on_cmbCOM_currentIndexChanged(const QString &arg1)
{
    currentPort = arg1;
}
