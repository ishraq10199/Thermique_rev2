#ifndef TEMPLABEL_H
#define TEMPLABEL_H

#include <QtCore>
#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <MyLabel.h>

//we extend QLabel to give it an extra slot, setImage
//this is because we can't pass a QPixmap from our thread
//so we have to pass a QImage and turn the QImage into a QPixmap on our end

class TempLabel : public QLabel {
  Q_OBJECT;
  QString temp;
  MyLabel * picLabel;

  public:
    TempLabel(QWidget * widget = nullptr);
    void connectToLabel(MyLabel * ml);
    ~TempLabel();

  public slots:
    void setTemp(QString);
};

#endif
