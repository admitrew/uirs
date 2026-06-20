#include "EditorMainWindow.h"
#include "MapScene.h"
#include "../parser/Th2Parser.h"

#include <QGraphicsView>
#include <QDebug>
#include <QTimer>
#include <QPainter>

EditorMainWindow::EditorMainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    m_scene = new MapScene(this);
    m_view = new QGraphicsView(m_scene, this);

    m_view->setRenderHint(QPainter::Antialiasing);

    setCentralWidget(m_view);
    resize(1000, 700);
    setWindowTitle("Therion Preview Editor");

    Th2Parser parser;

    QString filePath = "C:/speleoproj/speleoproj/OldGrotto.th2";

    if (!parser.parseFile(filePath)) {
        qDebug() << "Не удалось открыть файл:" << filePath;
        return;
    }

    qDebug() << "Загружено линий:" << parser.lines().size();
    qDebug() << "Загружено точек:" << parser.points().size();

    for (LineItem* line : parser.lines()) {
        m_scene->addItem(line);
    }

    for (PointItem* point : parser.points()) {
        m_scene->addItem(point);
    }

    QRectF rect = m_scene->itemsBoundingRect();

    if (!rect.isEmpty()) {
        m_scene->setSceneRect(rect.adjusted(-50, -50, 50, 50));

        QTimer::singleShot(0, this, [this]() {
            m_view->fitInView(m_scene->sceneRect(), Qt::KeepAspectRatio);
        });
    }
}