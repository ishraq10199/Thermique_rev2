#include "MyLabel.h"
#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>
#include <iomanip>
#include <ctime>

#include <stdio.h>
#include <stdlib.h>

#include "rectangle.h"

#include "scaleFactor.h"

MyLabel::MyLabel(QWidget *parent) : QLabel(parent)
{
  lastPoint = QPoint(-1, -1);

  int x1, x2, y1, y2;
  
  x1 = POINT_x1;
  y1 = POINT_y1;
  x2 = POINT_x2;
  y2 = POINT_y2;

  // std::ifstream file;
  // file.open("coords.txt", std::ifstream::in);
  // // ISHRAQ __ CHANGE RECTANGLE WHICH IS TO BE SCANNED

  // int x1, y1, x2, y2;
  // if(file.is_open()){
  //   file >> x1 >> y1 >> x2 >> y2;
  //   //std::cout << "X1: " << x1 << ", Y1: " << y1 << std::endl;
  //   file.close();
  // }
  // else{
  //   std::cout << "Error getting co-ordinates." << std::endl;
  // }

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
    std::cout << "Temperature: "<< this->thread->getTempFromXY(lastPoint.x(),lastPoint.y()) << " Fahrenheit" << std::endl;
    this->thread->lastPoint = lastPoint;
  }
  else if (event->button() == Qt::RightButton) {
    // std::cout << "Scanning area bounded by (" << areaPointTopLeft.x() << ", " << areaPointTopLeft.y() << 
    //             ") and (" << areaPointBottomRight.x() << ", " << areaPointBottomRight.x() << ")" << std::endl;
    
    std::cout << this->thread->getTempFromArea(areaPointTopLeft.x(), areaPointTopLeft.y(), areaPointBottomRight.x(), areaPointBottomRight.y()) << std::endl;
  }
}

void MyLabel::keyPressEvent(QKeyEvent *event){
  // Qt::EnterKeyType = Qt::EnterKeyReturn;
  if(event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter){
    // IMPLEMENT DATE AND TIME:
    std::cout << "______________________________________________________\n" << std::endl;
    time_t now = time(0);
    tm * t = localtime(&now);
    std::cout << "Timestamp: " << t->tm_mday << "-" << t->tm_mon << "-" << t->tm_year+1900 << " [" <<
                t->tm_hour << ":" << t->tm_min << ":" << t->tm_sec%60 << "]" << std::endl; 
    /*
    struct tm {
    int tm_sec;   // seconds of minutes from 0 to 61
    int tm_min;   // minutes of hour from 0 to 59
    int tm_hour;  // hours of day from 0 to 24
    int tm_mday;  // day of month from 1 to 31
    int tm_mon;   // month of year from 0 to 11
    int tm_year;  // year since 1900
    int tm_wday;  // days since sunday
    int tm_yday;  // days since January 1st
    int tm_isdst; // hours of daylight savings time
    }
    */
    std::cout << "RFID scanned number: " << strRead << std::endl;
    float result = this->thread->getTempFromArea(POINT_x1, POINT_y1, POINT_x2, POINT_y2);
    std::cout << "Recorded temperature: " << result << std::endl;

    //TEMPORARY - SWITCH TO DIFFERENT FORMAT AFTER DB INTEGRATION
    std::cout << "\n______________________________________________________" << std::endl;
    

    strRead = "";
  }
  else if(event->key() == Qt::Key_R){
    this->thread->performFFC();
  }
  else{
    strRead = strRead + event->text().toStdString();
  }
}