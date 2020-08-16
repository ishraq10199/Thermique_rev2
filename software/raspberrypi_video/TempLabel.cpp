#include "TempLabel.h"
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

TempLabel::TempLabel(QWidget *parent) : QLabel(parent)
{

}
TempLabel::~TempLabel()
{
}

//when the system calls setImage, we'll set the label's pixmap
void TempLabel::setTemp(QString text) {
  setText(text);
}
void TempLabel::connectToLabel(MyLabel * ml){
  this->picLabel = ml;
}
