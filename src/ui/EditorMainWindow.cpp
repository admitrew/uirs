#include "EditorMainWindow.h"
#include "MapScene.h"

#include "../parser/Th2Parser.h"
#include "../parser/Th2Writer.h"

#include <QGraphicsView>
#include <QFileDialog>
#include <QMessageBox>
#include <QMenuBar>
#include <QAction>
#include <QTimer>
#include <QPainter>
#include <QDebug>

EditorMainWindow::EditorMainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    m_scene = new MapScene(this);
    m_view = new QGraphicsView(m_scene, this);

    m_view->setRenderHint(QPainter::Antialiasing);

    setCentralWidget(m_view);
    resize(1000, 700);
    setWindowTitle("Therion Preview Editor");

    createMenus();
}

void EditorMainWindow::createMenus()
{
    QMenu* fileMenu = menuBar()->addMenu("Файл");

    QAction* openAction = fileMenu->addAction("Открыть .th2");
    openAction->setShortcut(QKeySequence::Open);

    QAction* saveAsAction = fileMenu->addAction("Сохранить как .th2");
    saveAsAction->setShortcut(QKeySequence::SaveAs);

    connect(openAction, &QAction::triggered, this, &EditorMainWindow::openTh2File);
    connect(saveAsAction, &QAction::triggered, this, &EditorMainWindow::saveTh2FileAs);
}

void EditorMainWindow::saveTh2FileAs()
{
    QString filePath = QFileDialog::getSaveFileName(
        this,
        "Сохранить файл Therion",
        QString(),
        "Therion scraps (*.th2);;Все файлы (*.*)"
    );

    if (filePath.isEmpty()) {
        return;
    }

    if (!filePath.endsWith(".th2", Qt::CaseInsensitive)) {
        filePath += ".th2";
    }

    Th2Writer::write(m_scene, filePath, m_headerLines, m_scrapLine, m_endScrapLine);

    qDebug() << "Файл сохранён:" << filePath;

    QMessageBox::information(
        this,
        "Сохранение завершено",
        "Файл сохранён:\n" + filePath
    );
}

void EditorMainWindow::openTh2File()
{
    QString filePath = QFileDialog::getOpenFileName(
        this,
        "Открыть файл Therion",
        QString(),
        "Therion scraps (*.th2);;Все файлы (*.*)"
    );

    if (filePath.isEmpty()) {
        return;
    }

    loadTh2File(filePath);
}

void EditorMainWindow::loadTh2File(const QString& filePath)
{
    Th2Parser parser;

    if (!parser.parseFile(filePath)) {
        QMessageBox::warning(
            this,
            "Ошибка открытия файла",
            "Не удалось открыть файл:\n" + filePath
        );
        return;
    }

    m_headerLines = parser.headerLines();
    m_scrapLine = parser.scrapLine();
    m_endScrapLine = parser.endScrapLine();

    m_scene->clear();

    for (LineItem* line : parser.lines()) {
        m_scene->addItem(line);
    }

    for (PointItem* point : parser.points()) {
        m_scene->addItem(point);
    }

    qDebug() << "Открыт файл:" << filePath;
    qDebug() << "Загружено линий:" << parser.lines().size();
    qDebug() << "Загружено точек:" << parser.points().size();

    setWindowTitle("Therion Preview Editor — " + filePath);

    fitSceneToView();
}

void EditorMainWindow::fitSceneToView()
{
    QRectF rect = m_scene->itemsBoundingRect();

    if (rect.isEmpty()) {
        return;
    }

    m_scene->setSceneRect(rect.adjusted(-50, -50, 50, 50));

    QTimer::singleShot(0, this, [this]() {
        m_view->fitInView(m_scene->sceneRect(), Qt::KeepAspectRatio);
    });
}