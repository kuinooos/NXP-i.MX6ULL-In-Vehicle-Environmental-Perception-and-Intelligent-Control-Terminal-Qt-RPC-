#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QString>

MainWindow::MainWindow(Login* login,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,  mlogin(login) // 保存 Login 指针
{
    ui->setupUi(this);
    QTimer* timer_dht11 = new QTimer(this);
    connect(timer_dht11,&QTimer::timeout,this,&MainWindow::on_setTextOfTemphumi);
    timer_dht11->setInterval(1000);
    timer_dht11->start();


    weather = new GetWeather();
    weather->initWeatherSocket();
    QTimer* timer_weather = new QTimer(this);
    connect(timer_weather, &QTimer::timeout, this, &MainWindow::on_setWeather);
    timer_weather->setInterval(5000);  // 5秒一次请求
    timer_weather->start();

    detect = new DetectPeople();
    timer_detect = new QTimer(this);
    connect(timer_detect, &QTimer::timeout,detect,&DetectPeople::sigio_handler);
    connect(timer_detect, &QTimer::timeout,this,&MainWindow::on_detectPeopleComing);
    timer_detect->setInterval(1000);
    timer_detect->start();
}

MainWindow::~MainWindow()
{
    weather->destorySocket();
    delete ui;
}

void MainWindow::on_detectPeopleComing(){
    changePage = detect->get_people_coming();
    if(changePage!=0){
        mlogin->show();
        timer_detect->stop();
    }
};

void MainWindow::on_setTextOfTemphumi()
{
    rpc_dht11_read(&humi,&temp);
    ui->label_displayhumi->setText(QString("humi: %1").arg((int)humi));
    ui->label_displaytemp->setText(QString("temp: %1℃").arg((int)temp));
    qDebug() << QString("humi: %1%").arg((int)humi);
    qDebug() << QString("temp: %1℃").arg((int)temp);
};

void MainWindow::on_checkBox_curtain_stateChanged(int arg1)
{
    qDebug() << "curtain click!";
    sign_curtain = arg1==0?0:1;
    int err = rpc_curtain_control(sign_curtain);
    if(err == -1){
        qDebug() << "curtain_control failed!";
        return;
    }

    if(sign_curtain == 0){
        qDebug() << "curtain off";
    }else{
        qDebug() << "curtain on";
    }
}

void MainWindow::on_checkBox_light_stateChanged(int arg1)
{
    qDebug() << "light click!";
    sign_led = arg1==0?0:1;
    int err = rpc_led_control(sign_led);
    if(err == -1){
        qDebug() << "led_control failed!";
        return;
    }

    if(sign_led == 0){
        qDebug() << "led off";
    }else{
        qDebug() << "led on";
    }
}

void MainWindow::on_setWeather()
{
    char* array[3];
    int ret = 0;
    ret = weather->getWeather(&array[0],&array[1],&array[2]);
    if(ret<0){
        qDebug() << "Set weather failed!";
        return;
    }
    QString strText;
    strText = QString::fromUtf8(array[0]);
    qDebug() << "Text: " << strText;
    ui->label_wheatherText->setText("          "+strText+"℃");
    QString strTemp;
    strTemp = QString::fromUtf8(array[2]);
    qDebug() << "Temp: " << strTemp ;
    ui->label_wheathertemp->setText("          "+strTemp);

    QString strCode;
    QString imagePath;
    QString styleSheet;
    strCode = QString::fromUtf8(array[1]);
    int intCode = strCode.toInt();
    switch (static_cast<MainWindow::EnumWeather>(intCode)) {
       case MainWindow::EnumWeather::sunny:
       imagePath = ":/image/Sun.png"; // 替换为实际的图片路径
       styleSheet = QString("QLabel#label_wheather { border-image: url(%1); }").arg(imagePath);
       ui->label_wheather->setStyleSheet(styleSheet);
       break;
       case MainWindow::EnumWeather::heavyRain:
       imagePath = ":/image/Rain.png"; // 替换为实际的图片路径
       styleSheet = QString("QLabel#label_wheather { border-image: url(%1); }").arg(imagePath);
       ui->label_wheather->setStyleSheet(styleSheet);
       break;
       case MainWindow::EnumWeather::lightRain:
       imagePath = ":/image/Rain.png"; // 替换为实际的图片路径
       styleSheet = QString("QLabel#label_wheather { border-image: url(%1); }").arg(imagePath);
       ui->label_wheather->setStyleSheet(styleSheet);
       break;
       case MainWindow::EnumWeather::moderateRain:
       imagePath = ":/image/Rain.png"; // 替换为实际的图片路径
       styleSheet = QString("QLabel#label_wheather { border-image: url(%1); }").arg(imagePath);
       ui->label_wheather->setStyleSheet(styleSheet);
       break;
       case MainWindow::EnumWeather::cloudy:
       imagePath = ":/image/Cloud.png"; // 替换为实际的图片路径
       styleSheet = QString("QLabel#label_wheather { border-image: url(%1); }").arg(imagePath);
       ui->label_wheather->setStyleSheet(styleSheet);
       break;
       case MainWindow::EnumWeather::mostlyCloudy:
       imagePath = ":/image/Cloud.png"; // 替换为实际的图片路径
       styleSheet = QString("QLabel#label_wheather { border-image: url(%1); }").arg(imagePath);
       ui->label_wheather->setStyleSheet(styleSheet);
       break;
       case MainWindow::EnumWeather::partlyCloudy:
       imagePath = ":/image/Cloud.png"; // 替换为实际的图片路径
       styleSheet = QString("QLabel#label_wheather { border-image: url(%1); }").arg(imagePath);
       ui->label_wheather->setStyleSheet(styleSheet);
       break;
       case MainWindow::EnumWeather::clear:
       imagePath = ":/image/Sun.png"; // 替换为实际的图片路径
       styleSheet = QString("QLabel#label_wheather { border-image: url(%1); }").arg(imagePath);
       ui->label_wheather->setStyleSheet(styleSheet);
       break;
       case MainWindow::EnumWeather::storm:
       imagePath = ":/image/Rian.png"; // 替换为实际的图片路径
       styleSheet = QString("QLabel#label_wheather { border-image: url(%1); }").arg(imagePath);
       ui->label_wheather->setStyleSheet(styleSheet);
       break;
       case MainWindow::EnumWeather::heavyStorm:
       imagePath = ":/image/Rain.png"; // 替换为实际的图片路径
       styleSheet = QString("QLabel#label_wheather { border-image: url(%1); }").arg(imagePath);
       ui->label_wheather->setStyleSheet(styleSheet);
       break;
       case MainWindow::EnumWeather::overcast:
       imagePath = ":/image/overcast.png"; // 替换为实际的图片路径
       styleSheet = QString("QLabel#label_wheather { border-image: url(%1); }").arg(imagePath);
       ui->label_wheather->setStyleSheet(styleSheet);
       break;
       case MainWindow::EnumWeather::shower:
       imagePath = ":/image/Rain.png"; // 替换为实际的图片路径
       styleSheet = QString("QLabel#label_wheather { border-image: url(%1); }").arg(imagePath);
       ui->label_wheather->setStyleSheet(styleSheet);
       break;
       case MainWindow::EnumWeather::thunderShower:
       imagePath = ":/image/thunderShower.png"; // 替换为实际的图片路径
       styleSheet = QString("QLabel#label_wheather { border-image: url(%1); }").arg(imagePath);
       ui->label_wheather->setStyleSheet(styleSheet);
       break;
   }
}


