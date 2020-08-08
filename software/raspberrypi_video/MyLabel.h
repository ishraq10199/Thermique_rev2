#ifndef MYLABEL_H
#define MYLABEL_H

#include <QtCore>
#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <LeptonThread.h>


//we extend QLabel to give it an extra slot, setImage
//this is because we can't pass a QPixmap from our thread
//so we have to pass a QImage and turn the QImage into a QPixmap on our end

class MyLabel : public QLabel {
  Q_OBJECT;
  LeptonThread * thread;
  QPoint lastPoint;
  QPoint areaPointTopLeft;
  QPoint areaPointBottomRight;
  float extractedTemp;


  public:
    MyLabel(QWidget *parent = 0);
    ~MyLabel();
    void connectToThread(LeptonThread * lt);

  // ISHRAQ __ Overriding the following protected functions
  protected:
    void mousePressEvent(QMouseEvent *event) override;
    //void mouseMoveEvent(QMouseEvent *event) override;

  public slots:
    void setImage(QImage);
};

#endif
