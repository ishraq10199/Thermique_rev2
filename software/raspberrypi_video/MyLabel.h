#ifndef MYLABEL_H
#define MYLABEL_H

#include <QtCore>
#include <QWidget>
#include <QLabel>

#include <LeptonThread.h>


//we extend QLabel to give it an extra slot, setImage
//this is because we can't pass a QPixmap from our thread
//so we have to pass a QImage and turn the QImage into a QPixmap on our end

class MyLabel : public QLabel {
  Q_OBJECT;
  LeptonThread * thread;

  public:
    MyLabel(QWidget *parent = 0);
    ~MyLabel();
    void connectToThread(LeptonThread * lt);

  public slots:
    void setImage(QImage);
};

#endif
