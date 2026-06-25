#include "EditorMainWindow.h"
#include "MapScene.h"

#include "../parser/Th2Parser.h"
#include "../parser/Th2Writer.h"

#include <QGraphicsView>
#include <QFileDialog>
#include <QMessageBox>
#include <QMenuBar>
#include <QAction>
#include <QActionGroup>
#include <QKeySequence>
#include <QTimer>
#include <QPainter>
#include <QDebug>
#include <QStatusBar>
#include <QToolBar>
#include <QComboBox>
#include <QLabel>
#include <QSignalBlocker>

EditorMainWindow::EditorMainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    m_scene = new MapScene(this);
    m_view = new QGraphicsView(m_scene, this);

    m_view->setRenderHint(QPainter::Antialiasing);
    m_view->setFocusPolicy(Qt::StrongFocus);
    m_view->setFocus();
    m_view->setMouseTracking(true);

    setCentralWidget(m_view);
    resize(1000, 700);
    setWindowTitle("Therion Preview Editor");

    createMenus();
    createToolBar();

    setSelectMode();
}

void EditorMainWindow::createMenus()
{
    createFileMenu();
    createToolsMenu();
}

void EditorMainWindow::createFileMenu()
{
    QMenu* fileMenu = menuBar()->addMenu("Файл");

    QAction* openAction = fileMenu->addAction("Открыть .th2");
    openAction->setShortcut(QKeySequence::Open);

    QAction* saveAsAction = fileMenu->addAction("Сохранить как .th2");
    saveAsAction->setShortcut(QKeySequence::SaveAs);

    connect(openAction, &QAction::triggered, this, &EditorMainWindow::openTh2File);
    connect(saveAsAction, &QAction::triggered, this, &EditorMainWindow::saveTh2FileAs);
}

void EditorMainWindow::createToolsMenu()
{
    QMenu* toolsMenu = menuBar()->addMenu("Инструменты");

    QActionGroup* toolGroup = new QActionGroup(this);
    toolGroup->setExclusive(true);

    QAction* selectAction = toolsMenu->addAction("Выбор / редактирование");
    selectAction->setCheckable(true);
    selectAction->setChecked(true);
    toolGroup->addAction(selectAction);

    connect(selectAction, &QAction::triggered, this, &EditorMainWindow::setSelectMode);

    toolsMenu->addSeparator();

    QMenu* pointMenu = toolsMenu->addMenu("Добавить point");

    auto addPointAction = [this, pointMenu, toolGroup](const QString& title, const QString& type) {
        QAction* action = pointMenu->addAction(title);
        action->setCheckable(true);
        toolGroup->addAction(action);

        connect(action, &QAction::triggered, this, [this, type]() {
            setAddPointMode(type);
        });
    };

    addPointAction("station", "station");
    addPointAction("label", "label");
    addPointAction("gradient", "gradient");
    addPointAction("water-flow", "water-flow");
    addPointAction("entrance", "entrance");
    addPointAction("stalagmite", "stalagmite");

    QMenu* lineMenu = toolsMenu->addMenu("Добавить line");

    auto addLineAction = [this, lineMenu, toolGroup](const QString& title, const QString& type) {
        QAction* action = lineMenu->addAction(title);
        action->setCheckable(true);
        toolGroup->addAction(action);

        connect(action, &QAction::triggered, this, [this, type]() {
            setAddLineMode(type);
        });
    };

    addLineAction("wall", "wall");
    addLineAction("border", "border");
    addLineAction("pit", "pit");
    addLineAction("floor-step", "floor-step");
    addLineAction("rock-border", "rock-border");
    addLineAction("water-flow", "water-flow");
    addLineAction("gradient", "gradient");
}

void EditorMainWindow::createToolBar()
{
    QToolBar* toolBar = addToolBar("Инструменты");
    toolBar->setMovable(false);

    QActionGroup* modeGroup = new QActionGroup(this);
    modeGroup->setExclusive(true);

    m_selectToolAction = toolBar->addAction("Выбор");
    m_selectToolAction->setCheckable(true);
    modeGroup->addAction(m_selectToolAction);

    m_addPointToolAction = toolBar->addAction("Точка");
    m_addPointToolAction->setCheckable(true);
    modeGroup->addAction(m_addPointToolAction);

    m_addLineToolAction = toolBar->addAction("Линия");
    m_addLineToolAction->setCheckable(true);
    modeGroup->addAction(m_addLineToolAction);

    toolBar->addSeparator();

    toolBar->addWidget(new QLabel("Point:", this));

    m_pointTypeCombo = new QComboBox(this);
    m_pointTypeCombo->addItem("station");
    m_pointTypeCombo->addItem("label");
    m_pointTypeCombo->addItem("gradient");
    m_pointTypeCombo->addItem("water-flow");
    m_pointTypeCombo->addItem("entrance");
    m_pointTypeCombo->addItem("stalagmite");
    m_pointTypeCombo->setCurrentText("station");
    toolBar->addWidget(m_pointTypeCombo);

    toolBar->addSeparator();

    toolBar->addWidget(new QLabel("Line:", this));

    m_lineTypeCombo = new QComboBox(this);
    m_lineTypeCombo->addItem("wall");
    m_lineTypeCombo->addItem("border");
    m_lineTypeCombo->addItem("pit");
    m_lineTypeCombo->addItem("floor-step");
    m_lineTypeCombo->addItem("rock-border");
    m_lineTypeCombo->addItem("water-flow");
    m_lineTypeCombo->addItem("gradient");
    m_lineTypeCombo->setCurrentText("wall");
    toolBar->addWidget(m_lineTypeCombo);

    connect(m_selectToolAction, &QAction::triggered, this, &EditorMainWindow::setSelectMode);

    connect(m_addPointToolAction, &QAction::triggered, this, [this]() {
        setAddPointMode(m_pointTypeCombo->currentText());
    });

    connect(m_addLineToolAction, &QAction::triggered, this, [this]() {
        setAddLineMode(m_lineTypeCombo->currentText());
    });

    connect(m_pointTypeCombo, &QComboBox::currentTextChanged, this, [this](const QString& type) {
        if (m_addPointToolAction && m_addPointToolAction->isChecked()) {
            setAddPointMode(type);
        }
    });

    connect(m_lineTypeCombo, &QComboBox::currentTextChanged, this, [this](const QString& type) {
        if (m_addLineToolAction && m_addLineToolAction->isChecked()) {
            setAddLineMode(type);
        }
    });

    updateToolBarState();
}

void EditorMainWindow::setSelectMode()
{
    m_scene->setSelectMode();

    if (m_selectToolAction) {
        m_selectToolAction->setChecked(true);
    }

    updateToolBarState();
    statusBar()->showMessage("Режим: выбор");
}

void EditorMainWindow::setAddPointMode(const QString& pointType)
{
    m_scene->setAddPointMode(pointType);

    if (m_pointTypeCombo) {
        QSignalBlocker blocker(m_pointTypeCombo);

        int index = m_pointTypeCombo->findText(pointType);
        if (index >= 0) {
            m_pointTypeCombo->setCurrentIndex(index);
        }
    }

    if (m_addPointToolAction) {
        m_addPointToolAction->setChecked(true);
    }

    updateToolBarState();
    statusBar()->showMessage("Режим: добавление point " + pointType);
}

void EditorMainWindow::setAddLineMode(const QString& lineType)
{
    m_scene->setAddLineMode(lineType);

    if (m_lineTypeCombo) {
        QSignalBlocker blocker(m_lineTypeCombo);

        int index = m_lineTypeCombo->findText(lineType);
        if (index >= 0) {
            m_lineTypeCombo->setCurrentIndex(index);
        }
    }

    if (m_addLineToolAction) {
        m_addLineToolAction->setChecked(true);
    }

    updateToolBarState();
    statusBar()->showMessage("Режим: добавление line " + lineType + " | ЛКМ — точка, ПКМ/Enter — завершить, Esc — отменить");
}

void EditorMainWindow::updateToolBarState()
{
    if (!m_pointTypeCombo || !m_lineTypeCombo) {
        return;
    }

    const bool pointMode = m_addPointToolAction && m_addPointToolAction->isChecked();
    const bool lineMode = m_addLineToolAction && m_addLineToolAction->isChecked();

    m_pointTypeCombo->setEnabled(pointMode);
    m_lineTypeCombo->setEnabled(lineMode);
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

    qDebug() << "Saving area blocks:" << m_areaBlocks.size();

    Th2Writer::write(
        m_scene,
        filePath,
        m_headerLines,
        m_scrapLine,
        m_endScrapLine,
        m_areaBlocks
    );

    qDebug() << "Файл сохранён:" << filePath;

    QMessageBox::information(
        this,
        "Сохранение завершено",
        "Файл сохранён:\n" + filePath
    );
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
    m_areaBlocks = parser.areaBlocks();

    qDebug() << "Area blocks:" << m_areaBlocks.size();

    m_scene->cancelCurrentLine();
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