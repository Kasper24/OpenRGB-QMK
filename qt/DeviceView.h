#ifndef DEVICEVIEW_H
#define DEVICEVIEW_H

#include <QWidget>
#include "RGBController.h"

class DeviceView : public QWidget
{
    Q_OBJECT
public:
    explicit DeviceView(QWidget *parent = 0);

    virtual QSize sizeHint () const;
    virtual QSize minimumSizeHint () const;

    void setController(RGBController * controller_ptr);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *);

private:
    QSize initSize;
    bool mouseDown;
    bool ctrlDown;
    bool mouseMoved;
	QRect selectionRect;
    QPoint lastMousePos;
    QVector<int> previousSelection;
    QVector<int> selectedLeds;
	QVector<bool> selectionFlags;
    QVector<bool> previousFlags;

    RGBController* controller;

	QColor posColor(const QPoint &point);
	void updateSelection();

signals:
	void selectionChanged(QVector<int>);

public slots:
    bool selectLed(int);
	bool selectLeds(QVector<int>);
    bool selectZone(int zone, bool add = false);
    void clearSelection(); // Same as selecting the entire device
    void setSelectionColor(RGBColor);
};

#endif // DEVICEVIEW_H
