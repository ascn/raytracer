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
#include <QDoubleSpinBox>
#include <QIcon>

#include <raytracer/RaytraceEngine.h>
#include <scene/camera.h>
#include <scene/scene.h>
#include <scene/jsonreader.h>
#include <glm/glm.hpp>
#include <ProgPreviewWidget.h>

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
	scene(nullptr), camera(nullptr) {
	this->setWindowTitle("Raytracer");
	this->setWindowIcon(QIcon("icon.png"));

	imgDisplay = new ProgPreviewWidget(this);

	QGridLayout *mainGridLayout = new QGridLayout;
	setCentralWidget(new QWidget());
	centralWidget()->setLayout(mainGridLayout);

	mainGridLayout->addWidget(imgDisplay);
	imgDisplay->show();

	createActions();
	createMenus();

	createOptionDock();

	imgDisplay->resetPreview();

    camera = new Camera();
    scene = new Scene();

    connect(&re, &RaytraceEngine::updateGUI, imgDisplay, &ProgPreviewWidget::updatePreview);
}

MainWindow::~MainWindow() {}

void MainWindow::loadScene() {
	QString filename = QFileDialog::getOpenFileName(this,
		tr("Load scene..."), "./", tr("Scene files (*.json)"));
	if (filename == "") { return; }
	scene->reset();
    jsonreader::readJson(camera, scene, filename);
    imgDisplay->img = QImage(camera->width, camera->height, QImage::Format_RGB32);
    imgDisplay->updatePreview();
}

void MainWindow::saveImage() {
	QString filename = QFileDialog::getSaveFileName(this,
		tr("Save image"), "./",
		tr("Image files (*.png *.jpg)"));
	if (filename == "") { return ; }
	imgDisplay->saveImage(filename);
}

void MainWindow::renderScene() {
    re.render(*camera, *scene, imgDisplay->img,
    		recursionDepthBox->value(), glm::pow(2, AABox->currentIndex()),
    		multithreadingBox->isChecked());
	imgDisplay->updatePreview();
}

void MainWindow::genAOPass() {
	imgDisplay->img = re.generateAOPass(*camera, *scene,
		AOSamplesBox->value(), AOSpreadBox->value(), AODistanceBox->value());
	imgDisplay->updatePreview();
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

	multithreadingBox = new QCheckBox(optionDockContents);
	multithreadingBox->setChecked(true);
	multithreadingLabel = new QLabel(tr("Multithreading: "), optionDockContents);

	AABox = new QComboBox(optionDockContents);
	AABox->addItem(tr("None"));
	AABox->addItem(tr("4x SSAA"));
	AABox->addItem(tr("16x SSAA"));
	AABox->addItem(tr("64x SSAA"));
	AABox->addItem(tr("256x SSAA"));
	AALabel = new QLabel(tr("Anti-aliasing: "), optionDockContents);

	optionDock = new QDockWidget(tr("Settings"), this);
	optionDock->setAllowedAreas(Qt::RightDockWidgetArea);
	addDockWidget(Qt::RightDockWidgetArea, optionDock);

	QGridLayout *AOSettingsLayout = new QGridLayout;
	AOSettingsLayout->setVerticalSpacing(5);
	AOSettings = new QGroupBox(tr("Ambient occlusion"));
	AOSamplesLabel = new QLabel(tr("Samples: "), AOSettings);
	AOSpreadLabel = new QLabel(tr("Spread: "), AOSettings);
	AODistanceLabel = new QLabel(tr("Distance: "), AOSettings);

	AOSamplesBox = new QSpinBox(AOSettings);
	AOSamplesBox->setRange(1, 1000);
	AOSamplesBox->setSingleStep(1);

	AOSpreadBox = new QDoubleSpinBox(AOSettings);
	AOSpreadBox->setRange(0, 1);
	AOSpreadBox->setValue(1);

	AODistanceBox = new QDoubleSpinBox(AOSettings);
	AODistanceBox->setRange(0, 1000);
	AODistanceBox->setValue(0);

	AOButton = new QPushButton("Generate AO Pass", AOSettings);
    connect(AOButton, &QPushButton::clicked, this, &MainWindow::genAOPass);

	AOSettingsLayout->addWidget(AOSamplesLabel, 0, 0, Qt::AlignRight);
	AOSettingsLayout->addWidget(AOSpreadLabel, 1, 0, Qt::AlignRight);
	AOSettingsLayout->addWidget(AODistanceLabel, 2, 0, Qt::AlignRight);
	AOSettingsLayout->addWidget(AOSamplesBox, 0, 1);
	AOSettingsLayout->addWidget(AOSpreadBox, 1, 1);
	AOSettingsLayout->addWidget(AODistanceBox, 2, 1);
	AOSettingsLayout->addWidget(AOButton, 3, 0, 1, -1);

	QSpacerItem *spacer = new QSpacerItem(
					40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

	AOSettingsLayout->addItem(spacer, 4, 3, -1, -1, Qt::AlignTop);
	AOSettings->setLayout(AOSettingsLayout);

	optionDockLayout->addWidget(recursionDepthLabel, 0, 0, Qt::AlignRight);
	optionDockLayout->addWidget(recursionDepthBox, 0, 1);
	optionDockLayout->addWidget(multithreadingLabel, 1, 0, Qt::AlignRight);
	optionDockLayout->addWidget(multithreadingBox, 1, 1);
	optionDockLayout->addWidget(AALabel, 2, 0, Qt::AlignRight);
	optionDockLayout->addWidget(AABox, 2, 1);
	optionDockLayout->addWidget(AOSettings, 3, 0, -1, -1);

	optionDockLayout->addItem(spacer, 4, 2, -1, -1, Qt::AlignTop);
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
