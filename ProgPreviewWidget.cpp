#include <ProgPreviewWidget.h>
#include <QApplication>

ProgPreviewWidget::ProgPreviewWidget(QWidget *parent) :
	QLabel(parent) {
	resetPreview();
}

ProgPreviewWidget::~ProgPreviewWidget() {}

void ProgPreviewWidget::updatePreview() {
	pixmap = QPixmap::fromImage(img);
	this->setPixmap(pixmap);
	QApplication::processEvents();
}

void ProgPreviewWidget::resetPreview() {
	img = QImage(512, 512, QImage::Format_RGB32);
	img.fill(qRgb(0, 0, 0));
	updatePreview();
}

void ProgPreviewWidget::saveImage(QString filename) {
	img.save(filename);
}

void ProgPreviewWidget::darkenImage() {
	for (int i = 0; i < img.width(); ++i) {
		for (int j = 0; j < img.height(); ++j) {
			QRgb curr = img.pixel(i, j);
			img.setPixel(i, j, qRgb(qRed(curr) / 2, qGreen(curr) / 2, qBlue(curr) / 2));
		}
	}
	updatePreview();
}