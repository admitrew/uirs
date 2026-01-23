EditorToolBar::EditorToolBar(QWidget* parent) : QToolBar(parent) {
    pointAction = addAction("Point");
    lineAction  = addAction("Line");

    pointAction->setCheckable(true);
    lineAction->setCheckable(true);

    QActionGroup* group = new QActionGroup(this);
    group->addAction(pointAction);
    group->addAction(lineAction);

    pointBox = new QComboBox(this);
    lineBox  = new QComboBox(this);

    // Point types
    pointBox->addItem("station", (int)PointType::Station);
    pointBox->addItem("entrance", (int)PointType::Entrance);
    pointBox->addItem("fix", (int)PointType::Fix);
    pointBox->addItem("label", (int)PointType::Label);
    pointBox->addItem("danger", (int)PointType::Danger);

    // Line types
    lineBox->addItem("wall", (int)LineType::Wall);
    lineBox->addItem("survey", (int)LineType::Survey);
    lineBox->addItem("water", (int)LineType::Water);

    addWidget(pointBox);
    addWidget(lineBox);

    connect(pointAction, &QAction::triggered, this, [&]{
        m_mode = ToolMode::AddPoint;
        emit toolChanged();
    });

    connect(lineAction, &QAction::triggered, this, [&]{
        m_mode = ToolMode::AddLine;
        emit toolChanged();
    });
}
