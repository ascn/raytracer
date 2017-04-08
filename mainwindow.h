#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <QMainWindow>
#include <QLabel>
#include <QPixmap>
#include <QImage>
#include <QMenu>
#include <QAction>

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	virtual ~MainWindow();

public slots:

private:
	QLabel *imgLabel;
	QPixmap pixmap;
	QImage img;

	void createActions();
	void createMenus();
	QMenu *fileMenu;
	QAction *loadSceneAct;
	QAction *renderAct;
	QAction *saveImageAct;

private slots:
	void loadScene();
	void renderScene();
	void saveImage();
};

#endif // __MAINWINDOW_H__