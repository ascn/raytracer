#ifndef __PROG_PREVIEW_WIDGET_H__
#define __PROG_PREVIEW_WIDGET_H__

#include <QLabel>
#include <QWidget>
#include <QImage>
#include <QPixmap>

class ProgPreviewWidget : public QLabel {
	Q_OBJECT

public:
	explicit ProgPreviewWidget(QWidget *parent = 0);
	virtual ~ProgPreviewWidget();
	void resetPreview();
	void saveImage(QString filename);
	void darkenImage();

	QImage img;

public slots:
	void updatePreview();

private:
	QPixmap pixmap;
};

#endif // __PROG_PREVIEW_WIDGET_H__