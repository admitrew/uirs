#include "EditorMainWindow.h"
#include "MapScene.h"
#include "PointItem.h"
#include "LineItem.h"

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
#include <QDockWidget>
#include <QWidget>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QGraphicsItem>
#include <QList>
#include <QFileInfo>
#include <QCloseEvent>

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
    resize(1100, 700);

    createMenus();
    createToolBar();
    createPropertiesPanel();

    connect(m_scene, &QGraphicsScene::selectionChanged,
            this, &EditorMainWindow::updatePropertiesPanel);

    connect(m_scene, &QGraphicsScene::changed,
            this, [this]() {
                if (!m_isLoadingFile) {
                    markModified();
                }
            });

    setSelectMode();
    updatePropertiesPanel();
    updateWindowTitle();
}

void EditorMainWindow::closeEvent(QCloseEvent* event)
{
    if (maybeSaveCurrentFile()) {
        event->accept();
    } else {
        event->ignore();
    }
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

    QAction* saveAction = fileMenu->addAction("Сохранить");
    saveAction->setShortcut(QKeySequence::Save);

    QAction* saveAsAction = fileMenu->addAction("Сохранить как .th2");
    saveAsAction->setShortcut(QKeySequence::SaveAs);

    connect(openAction, &QAction::triggered, this, &EditorMainWindow::openTh2File);
    connect(saveAction, &QAction::triggered, this, &EditorMainWindow::saveCurrentFile);
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

    toolBar->addSeparator();

    m_showPropertiesAction = toolBar->addAction("Свойства");
    m_showPropertiesAction->setToolTip("Показать панель свойств выбранного объекта");

    connect(m_selectToolAction, &QAction::triggered, this, &EditorMainWindow::setSelectMode);

    connect(m_addPointToolAction, &QAction::triggered, this, [this]() {
        setAddPointMode(m_pointTypeCombo->currentText());
    });

    connect(m_addLineToolAction, &QAction::triggered, this, [this]() {
        setAddLineMode(m_lineTypeCombo->currentText());
    });

    connect(m_showPropertiesAction, &QAction::triggered,
            this, &EditorMainWindow::showPropertiesPanel);

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

void EditorMainWindow::createPropertiesPanel()
{
    m_propertiesDock = new QDockWidget("Свойства", this);
    m_propertiesDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    QWidget* panel = new QWidget(m_propertiesDock);

    QVBoxLayout* mainLayout = new QVBoxLayout(panel);

    m_selectedObjectLabel = new QLabel("Объект не выбран", panel);
    mainLayout->addWidget(m_selectedObjectLabel);

    QFormLayout* formLayout = new QFormLayout();

    m_typeEdit = new QLineEdit(panel);
    m_optionsEdit = new QLineEdit(panel);

    formLayout->addRow("Тип:", m_typeEdit);
    formLayout->addRow("Options:", m_optionsEdit);

    mainLayout->addLayout(formLayout);

    m_applyPropertiesButton = new QPushButton("Применить", panel);
    mainLayout->addWidget(m_applyPropertiesButton);

    mainLayout->addStretch();

    panel->setLayout(mainLayout);

    m_propertiesDock->setWidget(panel);
    addDockWidget(Qt::RightDockWidgetArea, m_propertiesDock);

    connect(m_applyPropertiesButton, &QPushButton::clicked,
            this, &EditorMainWindow::applyPropertiesToSelectedItem);

    connect(m_typeEdit, &QLineEdit::returnPressed,
            this, &EditorMainWindow::applyPropertiesToSelectedItem);

    connect(m_optionsEdit, &QLineEdit::returnPressed,
            this, &EditorMainWindow::applyPropertiesToSelectedItem);

    clearPropertiesPanel();
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

void EditorMainWindow::showPropertiesPanel()
{
    if (!m_propertiesDock) {
        return;
    }

    m_propertiesDock->show();
    m_propertiesDock->raise();
    m_propertiesDock->activateWindow();

    updatePropertiesPanel();
}

void EditorMainWindow::clearPropertiesPanel()
{
    if (!m_selectedObjectLabel || !m_typeEdit || !m_optionsEdit || !m_applyPropertiesButton) {
        return;
    }

    m_selectedObjectLabel->setText("Объект не выбран");

    m_typeEdit->clear();
    m_optionsEdit->clear();

    m_typeEdit->setEnabled(false);
    m_optionsEdit->setEnabled(false);
    m_applyPropertiesButton->setEnabled(false);
}

void EditorMainWindow::updatePropertiesPanel()
{
    if (!m_selectedObjectLabel || !m_typeEdit || !m_optionsEdit || !m_applyPropertiesButton) {
        return;
    }

    QList<QGraphicsItem*> selected = m_scene->selectedItems();

    if (selected.size() != 1) {
        clearPropertiesPanel();

        if (selected.size() > 1) {
            m_selectedObjectLabel->setText("Выбрано несколько объектов");
        }

        return;
    }

    QGraphicsItem* item = selected.first();

    QSignalBlocker typeBlocker(m_typeEdit);
    QSignalBlocker optionsBlocker(m_optionsEdit);

    if (auto* point = dynamic_cast<PointItem*>(item)) {
        m_selectedObjectLabel->setText("Выбран объект: point");

        m_typeEdit->setText(point->therionType());
        m_optionsEdit->setText(point->options());

        m_typeEdit->setEnabled(true);
        m_optionsEdit->setEnabled(true);
        m_applyPropertiesButton->setEnabled(true);

        return;
    }

    if (auto* line = dynamic_cast<LineItem*>(item)) {
        m_selectedObjectLabel->setText("Выбран объект: line");

        m_typeEdit->setText(line->therionType());
        m_optionsEdit->setText(line->options());

        m_typeEdit->setEnabled(true);
        m_optionsEdit->setEnabled(true);
        m_applyPropertiesButton->setEnabled(true);

        return;
    }

    clearPropertiesPanel();
    m_selectedObjectLabel->setText("Неизвестный объект");
}

void EditorMainWindow::applyPropertiesToSelectedItem()
{
    QList<QGraphicsItem*> selected = m_scene->selectedItems();

    if (selected.size() != 1) {
        return;
    }

    QGraphicsItem* item = selected.first();

    QString newType = m_typeEdit->text().trimmed();
    QString newOptions = m_optionsEdit->text().trimmed();

    if (newType.isEmpty()) {
        QMessageBox::warning(
            this,
            "Ошибка",
            "Тип объекта не может быть пустым."
        );
        return;
    }

    if (auto* point = dynamic_cast<PointItem*>(item)) {
        point->setTherionType(newType);
        point->setOptions(newOptions);
        point->update();

        markModified();
        statusBar()->showMessage("Свойства point обновлены: " + newType);
        updatePropertiesPanel();
        return;
    }

    if (auto* line = dynamic_cast<LineItem*>(item)) {
        line->setTherionType(newType);
        line->setOptions(newOptions);
        line->update();

        markModified();
        statusBar()->showMessage("Свойства line обновлены: " + newType);
        updatePropertiesPanel();
        return;
    }
}

void EditorMainWindow::markModified()
{
    setModified(true);
}

void EditorMainWindow::setModified(bool modified)
{
    if (m_isModified == modified) {
        return;
    }

    m_isModified = modified;
    updateWindowTitle();
}

bool EditorMainWindow::maybeSaveCurrentFile()
{
    if (!m_isModified) {
        return true;
    }

    QMessageBox::StandardButton result = QMessageBox::warning(
        this,
        "Несохранённые изменения",
        "В файле есть несохранённые изменения.\nСохранить их?",
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel
    );

    if (result == QMessageBox::Save) {
        return saveCurrentFile();
    }

    if (result == QMessageBox::Discard) {
        return true;
    }

    return false;
}

void EditorMainWindow::openTh2File()
{
    if (!maybeSaveCurrentFile()) {
        return;
    }

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

bool EditorMainWindow::saveCurrentFile()
{
    if (m_currentFilePath.isEmpty()) {
        return saveTh2FileAs();
    }

    return writeTh2File(m_currentFilePath);
}

bool EditorMainWindow::saveTh2FileAs()
{
    QString filePath = QFileDialog::getSaveFileName(
        this,
        "Сохранить файл Therion",
        m_currentFilePath,
        "Therion scraps (*.th2);;Все файлы (*.*)"
    );

    if (filePath.isEmpty()) {
        return false;
    }

    if (!filePath.endsWith(".th2", Qt::CaseInsensitive)) {
        filePath += ".th2";
    }

    m_currentFilePath = filePath;
    updateWindowTitle();

    return writeTh2File(filePath);
}

bool EditorMainWindow::writeTh2File(const QString& filePath)
{
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

    setModified(false);
    statusBar()->showMessage("Файл сохранён: " + filePath, 5000);

    return true;
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

    m_isLoadingFile = true;

    m_headerLines = parser.headerLines();
    m_scrapLine = parser.scrapLine();
    m_endScrapLine = parser.endScrapLine();
    m_areaBlocks = parser.areaBlocks();
    m_currentFilePath = filePath;

    qDebug() << "Area blocks:" << m_areaBlocks.size();

    m_scene->cancelCurrentLine();
    m_scene->clear();

    for (LineItem* line : parser.lines()) {
        m_scene->addItem(line);
    }

    for (PointItem* point : parser.points()) {
        m_scene->addItem(point);
    }

    m_isLoadingFile = false;
    setModified(false);

    qDebug() << "Открыт файл:" << filePath;
    qDebug() << "Загружено линий:" << parser.lines().size();
    qDebug() << "Загружено точек:" << parser.points().size();

    updateWindowTitle();

    fitSceneToView();
    updatePropertiesPanel();
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

void EditorMainWindow::updateWindowTitle()
{
    QString title = "Therion Preview Editor — ";

    if (m_currentFilePath.isEmpty()) {
        title += "новый файл";
    } else {
        QFileInfo info(m_currentFilePath);
        title += info.fileName();
    }

    if (m_isModified) {
        title += " *";
    }

    setWindowTitle(title);
}