#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QImage>
#include <QPixmap>
#include <QAction>
#include <QMenu>
#include <QFileDialog>
#include <QMenuBar>

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
	this->setWindowTitle("Raytracer");
	imgLabel = new QLabel(this);

	QGridLayout *mainGridLayout = new QGridLayout;
	setCentralWidget(new QWidget());
	centralWidget()->setLayout(mainGridLayout);

	mainGridLayout->addWidget(imgLabel);
	imgLabel->show();

	createActions();
	createMenus();

	img = QImage(512, 512, QImage::Format_RGB888);
	img.fill(qRgb(0, 0, 0));
	pixmap = QPixmap::fromImage(img);
	imgLabel->setPixmap(pixmap);

}

MainWindow::~MainWindow() {}

void MainWindow::loadScene() {
	QString filename = QFileDialog::getOpenFileName(this,
		tr("Load scene..."), "./", tr("Scene files (*.json)"));
	if (filename == "") { return; }
	// CALL JSONREADER HERE TO LOAD SCENE
}

void MainWindow::saveImage() {
	QString filename = QFileDialog::getSaveFileName(this,
		tr("Save image"), "./",
		tr("Image files (*.ppm *.png *.jpg *.bmp)"));
	if (filename == "") { return ; }
	img.save(filename);
}

void MainWindow::renderScene() {
	// CALL RENDER HERE
}

void MainWindow::createActions() {
	loadSceneAct = new QAction(tr("&Load scene..."), this);
	loadSceneAct->setShortcuts(QKeySequence::Open);
	loadSceneAct->setShortcut(tr("Ctrl+L"));
	connect(loadSceneAct, &QAction::triggered, this, &MainWindow::loadScene);

	saveImageAct = new QAction(tr("&Save image"), this);
	saveImageAct->setShortcuts(QKeySequence::Save);
	connect(saveImageAct, &QAction::triggered, this, &MainWindow::saveImage);

	renderAct = new QAction(tr("&Render scene"), this);
	renderAct->setShortcut(tr("Ctrl+R"));
	connect(renderAct, &QAction::triggered, this, &MainWindow::renderScene);
}

void MainWindow::createMenus() {
	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(loadSceneAct);
	fileMenu->addAction(saveImageAct);
	fileMenu->addSeparator();
	fileMenu->addAction(renderAct);
}