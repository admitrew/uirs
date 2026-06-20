#include "EditorMainWindow.h"
#include "MapScene.h"

#include <QGraphicsView>

EditorMainWindow::EditorMainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    m_scene = new MapScene(this);
    m_view = new QGraphicsView(m_scene, this);

    setCentralWidget(m_view);
    resize(1000, 700);
    setWindowTitle("Therion Preview Editor");
}