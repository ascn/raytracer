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
#include <QElapsedTimer>

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
	createStatsDock();

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
	QElapsedTimer timer;
	if (kdTreeBox->isChecked()) {
		timer.start();
	} else {
		kdTreeTimeValue->setText(tr("N/A"));
	}
    jsonreader::readJson(camera, scene, filename, kdTreeBox->isChecked());
    if (kdTreeBox->isChecked()) {
    	kdTreeTimeValue->setText(formatMs(timer.elapsed()));
    }
    imgDisplay->img = QImage(camera->width, camera->height, QImage::Format_RGB32);
    imgDisplay->img.fill(qRgb(0, 0, 0));
    imgDisplay->updatePreview();
    renderTimeValue->setText(tr("N/A"));
    int tris = 0;
    for (const auto &p : scene->primitives) {
    	tris += p->tris;
    }
    const QLocale &cLocale = QLocale::c();
    trisValue->setText(cLocale.toString(tris));
}

void MainWindow::saveImage() {
	QString filename = QFileDialog::getSaveFileName(this,
		tr("Save image"), "./",
		tr("Image files (*.png *.jpg)"));
	if (filename == "") { return ; }
	imgDisplay->saveImage(filename);
}

void MainWindow::renderScene() {
	imgDisplay->darkenImage();
	setAllOptionsEnabled(false);
	QElapsedTimer timer;
	timer.start();
    re.render(*camera, *scene, imgDisplay->img,
    		recursionDepthBox->value(), glm::pow(AABox->value(), 2),
    		multithreadingBox->isChecked());
    renderTimeValue->setText(formatMs(timer.elapsed()));
	imgDisplay->updatePreview();
	setAllOptionsEnabled(true);
}

void MainWindow::genAOPass() {
	setAllOptionsEnabled(false);
	imgDisplay->img = re.generateAOPass(*camera, *scene,
		AOSamplesBox->value(), AOSpreadBox->value(), AODistanceBox->value());
	imgDisplay->updatePreview();
	setAllOptionsEnabled(true);
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

	kdTreeBox = new QCheckBox(optionDockContents);
	kdTreeBox->setChecked(true);
	kdTreeLabel = new QLabel(tr("Kd-Tree: "), optionDockContents);

	AABox = new QSpinBox(optionDockContents);
	AABox->setValue(1);
	AABox->setSingleStep(1);
	AABox->setRange(1, 100);
	AALabel = new QLabel(tr("Square root of samples: "), optionDockContents);

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
	optionDockLayout->addWidget(kdTreeLabel, 2, 0, Qt::AlignRight);
	optionDockLayout->addWidget(kdTreeBox, 2, 1);
	optionDockLayout->addWidget(AALabel, 3, 0, Qt::AlignRight);
	optionDockLayout->addWidget(AABox, 3, 1);
	optionDockLayout->addWidget(AOSettings, 4, 0, -1, -1);

	optionDockLayout->addItem(spacer, 5, 2, -1, -1, Qt::AlignTop);
	optionDockContents->setLayout(optionDockLayout);
	optionDock->setWidget(optionDockContents);
}

void MainWindow::createStatsDock() {
	QGridLayout *statsDockLayout = new QGridLayout;
	statsDockLayout->setVerticalSpacing(5);
	statsDockContents = new QGroupBox(tr(""));

	renderTimeLabel = new QLabel(tr("Time to Render: "), statsDockContents);
	renderTimeValue = new QLabel(tr("N/A"), statsDockContents);

	kdTreeTimeLabel = new QLabel(tr("Time to build Kd-Tree: "), statsDockContents);
    kdTreeTimeValue = new QLabel(tr("N/A"), statsDockContents);

	trisLabel = new QLabel(tr("Tris: "), statsDockContents);
	trisValue = new QLabel(tr("N/A"), statsDockContents);

	geoIsectLabel = new QLabel(tr("Geometry Intersection Tests: "), statsDockContents);
	geoIsectValue = new QLabel(tr("N/A"));

	bboxIsectLabel = new QLabel(tr("Bounding Box Intersection Tests: "), statsDockContents);
	bboxIsectValue = new QLabel(tr("N/A"));

	statsDock = new QDockWidget(tr("Statistics"), this);
	statsDock->setAllowedAreas(Qt::RightDockWidgetArea);
	addDockWidget(Qt::RightDockWidgetArea, statsDock);

	QSpacerItem *spacer = new QSpacerItem(
					40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

	statsDockLayout->addWidget(renderTimeLabel, 0, 0, Qt::AlignRight);
	statsDockLayout->addWidget(renderTimeValue, 0, 1);
    statsDockLayout->addWidget(kdTreeTimeLabel, 1, 0, Qt::AlignRight);
    statsDockLayout->addWidget(kdTreeTimeValue, 1, 1);
	statsDockLayout->addWidget(trisLabel, 2, 0, Qt::AlignRight);
	statsDockLayout->addWidget(trisValue, 2, 1);
	statsDockLayout->addWidget(geoIsectLabel, 3, 0, Qt::AlignRight);
	statsDockLayout->addWidget(geoIsectValue, 3, 1);
	statsDockLayout->addWidget(bboxIsectLabel, 4, 0, Qt::AlignRight);
	statsDockLayout->addWidget(bboxIsectValue, 4, 1);
    statsDockLayout->addItem(spacer, 5, 2, -1, -1, Qt::AlignTop);

	statsDockContents->setLayout(statsDockLayout);
	statsDock->setWidget(statsDockContents);
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

void MainWindow::setAllOptionsEnabled(bool enabled) {
	recursionDepthBox->setEnabled(enabled);
	multithreadingBox->setEnabled(enabled);
	kdTreeBox->setEnabled(enabled);
	AABox->setEnabled(enabled);
	AOSamplesBox->setEnabled(enabled);
	AOSpreadBox->setEnabled(enabled);
	AODistanceBox->setEnabled(enabled);
	AOButton->setEnabled(enabled);
	loadSceneAct->setEnabled(enabled);
	saveImageAct->setEnabled(enabled);
	renderAct->setEnabled(enabled);
}

QString MainWindow::formatMs(qint64 ms) {
    if (ms < 1000) {
		return (QString::number(ms) + "ms");
	} else if (ms < 60000) {
		int sec = ms / 1000;
		return (QString::number(sec) + "s");
	} else if (ms < 3600000) {
		int min = ms / (1000 * 60);
		int sec = (ms / 1000) % 60;
		return (QString::number(min) + "m " + QString::number(sec) + "s");
	} else {
        int hrs = ms / (1000 * 60 * 60);
	}
}
