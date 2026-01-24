#include "Window.h"

Window::Window(QWidget *parent): QMainWindow(parent){
    ui.setupUi(this);
	QPixmap background(":/resource/Images/BackGround.jpg");
	QPixmap card(":/resource/Images/Skin2.png");
	ui.BackGround->setPixmap(background);
	QRect sourceRect(0, 0, 124, 173);
	ui.img->setPixmap(card.copy(sourceRect));
	qDebug() << "Window initialized.";
}

Window::~Window(){
}	
