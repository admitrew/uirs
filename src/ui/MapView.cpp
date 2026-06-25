#include "MapView.h"

#include <QWheelEvent>
#include <QMouseEvent>
#include <QScrollBar>
#include <QtMath>

MapView::MapView(QWidget* parent)
    : QGraphicsView(parent)
{
    setDragMode(QGraphicsView::NoDrag);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    setMouseTracking(true);
}

void MapView::wheelEvent(QWheelEvent* event)
{
    const double zoomFactor = 1.15;

    if (event->angleDelta().y() > 0) {
        scale(zoomFactor, zoomFactor);
    } else if (event->angleDelta().y() < 0) {
        scale(1.0 / zoomFactor, 1.0 / zoomFactor);
    }

    event->accept();
}

void MapView::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::MiddleButton) {
        m_isPanning = true;
        m_lastPanPoint = event->pos();

        setCursor(Qt::ClosedHandCursor);

        event->accept();
        return;
    }

    QGraphicsView::mousePressEvent(event);
}

void MapView::mouseMoveEvent(QMouseEvent* event)
{
    if (m_isPanning) {
        QPoint delta = event->pos() - m_lastPanPoint;
        m_lastPanPoint = event->pos();

        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
        verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());

        event->accept();
        return;
    }

    QGraphicsView::mouseMoveEvent(event);
}

void MapView::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::MiddleButton && m_isPanning) {
        m_isPanning = false;
        setCursor(Qt::ArrowCursor);

        event->accept();
        return;
    }

    QGraphicsView::mouseReleaseEvent(event);
}