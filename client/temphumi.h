#ifndef TEMPHUMI_H
#define TEMPHUMI_H

#include <QLabel>
#include <QThread>
class Temphumi : public QObject
{
   Q_OBJECT
public:
    explicit Temphumi(QObject *parent = nullptr);
    ~Temphumi();

public slots:
    void startWork();

signals:
    void newData(int temperature,int humidity);
    void finished();

private:
    int fd;
    bool running = true;

};

#endif // TEMPHUMI_H
