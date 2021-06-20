#include <QApplication>
#include <QThread>
#include <QMutex>
#include <QMessageBox>
#include <QString>
#include <QPainter>
#include <QPalette>

#include <iostream>
#include <string>

#include <QColor>
#include <QLabel>
#include <QtDebug>
#include <QString>
#include <QPushButton>

#include "LeptonThread.h"
#include "MyLabel.h"
#include "TempLabel.h"

#include "scaleFactor.h"


void printUsage(char *cmd) {
        char *cmdname = basename(cmd);
	printf("Usage: %s [OPTION]...\n"
               " -h      display this help and exit\n"
               " -cm x   select colormap\n"
               "           1 : rainbow\n"
               "           2 : grayscale\n"
               "           3 : ironblack [default]\n"
               " -tl x   select type of Lepton\n"
               "           2 : Lepton 2.x [default]\n"
               "           3 : Lepton 3.x\n"
               "               [for your reference] Please use nice command\n"
               "                 e.g. sudo nice -n 0 ./%s -tl 3\n"
               " -ss x   SPI bus speed [MHz] (10 - 30)\n"
               "           20 : 20MHz [default]\n"
               " -min x  override minimum value for scaling (0 - 65535)\n"
               "           [default] automatic scaling range adjustment\n"
               "           e.g. -min 30000\n"
               " -max x  override maximum value for scaling (0 - 65535)\n"
               "           [default] automatic scaling range adjustment\n"
               "           e.g. -max 32000\n"
               " -d x    log level (0-255)\n"
               "", cmdname, cmdname);
	return;
}

int main( int argc, char **argv )
{
	int typeColormap = 3; // colormap_ironblack
	int typeLepton = 2; // Lepton 2.x
	int spiSpeed = 20; // SPI bus speed 20MHz
	int rangeMin = -1; //
	int rangeMax = -1; //
	int loglevel = 0;
	for(int i=1; i < argc; i++) {
		if (strcmp(argv[i], "-h") == 0) {
			printUsage(argv[0]);
			exit(0);
		}
		else if (strcmp(argv[i], "-d") == 0) {
			int val = 3;
			if ((i + 1 != argc) && (strncmp(argv[i + 1], "-", 1) != 0)) {
				val = std::atoi(argv[i + 1]);
				i++;
			}
			if (0 <= val) {
				loglevel = val & 0xFF;
			}
		}
		else if ((strcmp(argv[i], "-cm") == 0) && (i + 1 != argc)) {
			int val = std::atoi(argv[i + 1]);
			if ((val == 1) || (val == 2)) {
				typeColormap = val;
				i++;
			}
		}
		else if ((strcmp(argv[i], "-tl") == 0) && (i + 1 != argc)) {
			int val = std::atoi(argv[i + 1]);
			if (val == 3) {
				typeLepton = val;
				i++;
			}
		}
		else if ((strcmp(argv[i], "-ss") == 0) && (i + 1 != argc)) {
			int val = std::atoi(argv[i + 1]);
			if ((10 <= val) && (val <= 30)) {
				spiSpeed = val;
				i++;
			}
		}
		else if ((strcmp(argv[i], "-min") == 0) && (i + 1 != argc)) {
			int val = std::atoi(argv[i + 1]);
			if ((0 <= val) && (val <= 65535)) {
				rangeMin = val;
				i++;
			}
		}
		else if ((strcmp(argv[i], "-max") == 0) && (i + 1 != argc)) {
			int val = std::atoi(argv[i + 1]);
			if ((0 <= val) && (val <= 65535)) {
				rangeMax = val;
				i++;
			}
		}
	}

	//create the app
	QApplication a( argc, argv );
	
	QWidget *myWidget = new QWidget;
	myWidget->setGeometry(400, 300, 340, 290);

	QPalette pal;

	// set black background
	pal.setColor(QPalette::Background, Qt::black);
	myWidget->setAutoFillBackground(true);
	myWidget->setPalette(pal);


	//create an image placeholder for myLabel
	//fill the top left corner with red, just bcuz
	QImage myImage;
	myImage = QImage(160, 120, QImage::Format_RGB888);
	QRgb red = qRgb(255,0,0);
	for(int i=0;i<80;i++) {
		for(int j=0;j<60;j++) {
			myImage.setPixel(i, j, red);
		}
	}

	//create a label, and set it's image to the placeholder
	MyLabel myLabel(myWidget);
	myLabel.setGeometry(10, 10, 160*scaleFactor, 120*scaleFactor);
	myLabel.setPixmap(QPixmap::fromImage(myImage));
	//create a FFC button
	//QPushButton *button1 = new QPushButton("FFC Reset", myWidget);
	//button1->setGeometry(320/2-100, 600, 100, 30);


	// __ ISHRAQ __ CREATE CUSTOM LABEL TO SHOW TEMPERATURE [Future plan]
	TempLabel * tempLabel = new TempLabel(myWidget);
	tempLabel->setAlignment(Qt::AlignCenter);
	tempLabel->connectToLabel(&myLabel);
	tempLabel->setGeometry(640 + 20, 50 + 0, 620, 500);

		

	QLabel * bottomBox = new QLabel(myWidget);
	bottomBox->setGeometry(10, 480 + 20, 1260, 210);

	QFont x = bottomBox->font();
	x.setPointSize(22);
	x.setBold(false);
	bottomBox->setFont(x);
	bottomBox->setAlignment(Qt::AlignCenter);
	bottomBox->setText("<font color='white'>Please make sure that your forehead is visible inside the green box.</font>");

	//create a thread to gather SPI data
	//when the thread emits updateImage, the label should update its image accordingly
	LeptonThread *thread = new LeptonThread();
	
	std::cout << "Thread created" << std::endl;


	// ISHRAQ _ CUSTOM LABEL UPDATE FROM thread
	std::string s;
	s =   "X: " + thread->lastPoint.x();
	s +=  ", Y: " + thread->lastPoint.y();

	tempLabel->setText("<font color='white'>Waiting for next scan</font>");

	// SET FONT OF LABEL:
	QFont font = tempLabel->font();
	font.setPointSize(22);
	font.setBold(true);
	tempLabel->setFont(font);


	// ISHRAQ _ connect Mylabel object to thread, for temp extraction
	myLabel.connectToThread(thread);
	std::cout << "Connected to Thread" << std::endl;

	thread->setLogLevel(loglevel);
	thread->useColormap(typeColormap);
	thread->useLepton(typeLepton);
	thread->useSpiSpeedMhz(spiSpeed);
	thread->setAutomaticScalingRange();
	if (0 <= rangeMin) thread->useRangeMinValue(rangeMin);
	if (0 <= rangeMax) thread->useRangeMaxValue(rangeMax);
	QObject::connect(thread, SIGNAL(updateImage(QImage)), &myLabel, SLOT(setImage(QImage)));
	QObject::connect(thread, SIGNAL(updateText(QString)), tempLabel, SLOT(setTemp(QString)));

	
	//connect ffc button to the thread's ffc action
	//QObject::connect(button1, SIGNAL(clicked()), thread, SLOT(performFFC()));
	
	std::cout << "Before start" << std::endl;
	thread->start();
	std::cout << "After start" << std::endl;
	//thread->performFFC();
	std::cout << "After FFC" << std::endl;
	
	//TODO: connect text area to TemperatureValue
	//test_1: connect text area to pixel coords

	std::cout << "Program launched" << std::endl;
	myWidget->showFullScreen();
	//myWidget->show();
	return a.exec();
}

