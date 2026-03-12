#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "led_control.h"
#include <QDebug>
#include <QTimer>
#include "rpc_client.h"
#include "getweather.h"
#include <cJSON.h>
#include "detectpeople.h"
#include "login.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    MainWindow(Login *login, QWidget *parent = nullptr);
    ~MainWindow();

    enum class EnumWeather{
        sunny = 0,
        clear = 1,
        cloudy = 4,
        partlyCloudy = 5,
        mostlyCloudy = 7,
        overcast = 9,
        shower = 10,
        thunderShower = 11,
        lightRain = 13,
        moderateRain = 14,
        heavyRain = 15,
        storm = 16,
        heavyStorm = 17
    };
private slots:

    void on_setTextOfTemphumi();

    void on_checkBox_curtain_stateChanged(int arg1);

    void on_checkBox_light_stateChanged(int arg1);

    void on_setWeather();

    void on_detectPeopleComing();
private:
    Ui::MainWindow *ui;

    led_control led;
    int sign_led = 1;

    int sign_curtain = 1;

    char humi;
    char temp;

    GetWeather *weather;
    DetectPeople *detect;

    int changePage = 0;
    Login *mlogin;
    QTimer* timer_detect;
};
#endif // MAINWINDOW_H
