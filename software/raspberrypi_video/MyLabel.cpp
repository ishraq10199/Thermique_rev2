#include "MyLabel.h"
#include <iostream>
#include <string>
#include <fstream>

#include <stdio.h>
#include <stdlib.h>

#define POINT_x1 60
#define POINT_y1 45
#define POINT_x2 100
#define POINT_y2 75

#define scaleFactor 6

MyLabel::MyLabel(QWidget *parent) : QLabel(parent)
{
  lastPoint = QPoint(-1, -1);

  std::ifstream file;
  file.open("coords.txt", std::ifstream::in);
  // ISHRAQ __ CHANGE RECTANGLE WHICH IS TO BE SCANNED

  int x1, y1, x2, y2;
  if(file.is_open()){
    file >> x1 >> y1 >> x2 >> y2;
    //std::cout << "X1: " << x1 << ", Y1: " << y1 << std::endl;
    file.close();
  }
  else{
    std::cout << "Error getting co-ordinates." << std::endl;
  }

  areaPointTopLeft = QPoint(x1, y1);
  areaPointBottomRight = QPoint(x2, y2);

  extractedTemp = 0.0;
  setFocusPolicy(Qt::StrongFocus);
  strRead = "";
}
MyLabel::~MyLabel()
{
}

//when the system calls setImage, we'll set the label's pixmap
void MyLabel::setImage(QImage image) {
  QPixmap pixmap = QPixmap::fromImage(image);
  int w = this->width();
  int h = this->height();
  setPixmap(pixmap.scaled(w, h, Qt::KeepAspectRatio));
}
void MyLabel::connectToThread(LeptonThread * lt){
  this->thread = lt;

}

void MyLabel::mousePressEvent(QMouseEvent *event){
  if (event->button() == Qt::LeftButton) {
    lastPoint = event->pos();
    lastPoint = QPoint((int)(lastPoint.x()/scaleFactor), (int)(lastPoint.y()/scaleFactor));
    std::cout << "Temperature: "<< this->thread->getTempFromXY(lastPoint.x(),lastPoint.y()) << "°C" << std::endl;
    this->thread->lastPoint = lastPoint;
  }
  else if (event->button() == Qt::RightButton) {
    std::cout << "Scanning area bounded by (" << areaPointTopLeft.x() << ", " << areaPointTopLeft.y() << 
                ") and (" << areaPointBottomRight.x() << ", " << areaPointBottomRight.x() << ")" << std::endl;
    std::cout << this->thread->getTempFromArea(areaPointTopLeft.x(), areaPointTopLeft.y(), areaPointBottomRight.x(), areaPointBottomRight.y()) << std::endl;
  }
}

void MyLabel::keyPressEvent(QKeyEvent *event){
  // Qt::EnterKeyType = Qt::EnterKeyReturn;
  if(event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter){
    std::cout << "RECEIVED FROM SCAN: " << strRead << std::endl;
    // this->thread->getTempFromArea(areaPointTopLeft.x(), areaPointTopLeft.y(), areaPointBottomRight.x(), areaPointBottomRight.y());
    this->thread->getTempFromArea(POINT_x1, POINT_y1, POINT_x2, POINT_y2);
    strRead = "";
  }
  else if(event->key() == Qt::Key_R){
    this->thread->performFFC();
  }
  else{
    strRead = strRead + event->text().toStdString();
  }
}