#include "MyLabel.h"
#include <iostream>

MyLabel::MyLabel(QWidget *parent) : QLabel(parent)
{
  lastPoint = QPoint(-1, -1);
  areaPointTopLeft = QPoint(50, 50);
  areaPointBottomRight = QPoint(80, 80);
  extractedTemp = 0.0;
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
    std::cout << "Temperature: "<< this->thread->getTempFromXY(lastPoint.x(), lastPoint.y()) << "°C" << std::endl;
    this->thread->lastPoint = lastPoint;
  }
  else if (event->button() == Qt::RightButton) {
    std::cout << "Scanning area bounded by (" << areaPointTopLeft.x() << ", " << areaPointTopLeft.y() << 
                ") and (" << areaPointBottomRight.x() << ", " << areaPointBottomRight.x() << ")" << std::endl;
    std::cout << this->thread->getTempFromArea(areaPointTopLeft.x(), areaPointTopLeft.y(), areaPointBottomRight.x(), areaPointBottomRight.y()) << std::endl;
  }
}