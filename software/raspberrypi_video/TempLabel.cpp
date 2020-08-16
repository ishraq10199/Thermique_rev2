#include "TempLabel.h"
#include <iostream>
#include <string>
#include <fstream>

#include <stdio.h>
#include <stdlib.h>


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
