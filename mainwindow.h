#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <QMainWindow>
#include <QLabel>
#include <QPixmap>
#include <QImage>
#include <QMenu>
#include <QAction>
#include <QGroupBox>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>

#include <scene/camera.h>
#include <scene/scene.h>

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	virtual ~MainWindow();

public slots:

private:
	Scene *scene;
	Camera *camera;

	// GUI variables
	QLabel *imgLabel;
	QPixmap pixmap;
	QImage img;

	void createActions();
	void createMenus();
	QMenu *fileMenu;
	QAction *loadSceneAct;
	QAction *renderAct;
	QAction *saveImageAct;

	void createOptionDock();
	QDockWidget *optionDock;
	QGroupBox *optionDockContents;
	QSpinBox *recursionDepthBox;
	QComboBox *AABox;

	QGroupBox *AOSettings;
	QSpinBox *AOSamplesBox;
	QDoubleSpinBox *AOSpreadBox;
	QDoubleSpinBox *AODistanceBox;
	QPushButton *AOButton;

	QLabel *recursionDepthLabel;
	QLabel *AALabel;
	QLabel *AOSamplesLabel;
	QLabel *AOSpreadLabel;
	QLabel *AODistanceLabel;

private slots:
	void loadScene();
	void renderScene();
	void saveImage();
	void genAOPass();
};

#endif // __MAINWINDOW_H__