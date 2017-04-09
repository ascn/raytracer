#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QImage>
#include <QPixmap>
#include <QAction>
#include <QMenu>
#include <QFileDialog>
#include <QMenuBar>
#include <QSpacerItem>
#include <QDockWidget>
#include <QCheckBox>
#include <QComboBox>

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

	createOptionDock();

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

void MainWindow::createOptionDock() {
	QGridLayout *optionDockLayout = new QGridLayout;
	optionDockLayout->setVerticalSpacing(5);
	optionDockContents = new QGroupBox(tr(""));

	recursionDepthBox = new QSpinBox(optionDockContents);
	recursionDepthBox->setRange(1, 20);
	recursionDepthBox->setSingleStep(1);
	recursionDepthBox->setValue(5);
	recursionDepthLabel = new QLabel(tr("Recursion depth: "), optionDockContents);

	AABox = new QComboBox(optionDockContents);
	AABox->addItem(tr("None"));
	AABox->addItem(tr("2x AA"));
	AABox->addItem(tr("4x AA"));
	AABox->addItem(tr("8x AA"));
	AABox->addItem(tr("16x AA"));
	AALabel = new QLabel(tr("Anti-aliasing: "), optionDockContents);

	optionDock = new QDockWidget(tr("Raytracing options"), this);
	optionDock->setAllowedAreas(Qt::RightDockWidgetArea);
	addDockWidget(Qt::RightDockWidgetArea, optionDock);

	QSpacerItem *spacer = new QSpacerItem(
					40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

	optionDockLayout->addWidget(recursionDepthLabel, 0, 0, Qt::AlignRight);
	optionDockLayout->addWidget(recursionDepthBox, 0, 1);
	optionDockLayout->addWidget(AALabel, 1, 0, Qt::AlignRight);
	optionDockLayout->addWidget(AABox, 1, 1);

	optionDockLayout->addItem(spacer, 2, 2, -1, -1, Qt::AlignTop);
	optionDockContents->setLayout(optionDockLayout);
	optionDock->setWidget(optionDockContents);
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