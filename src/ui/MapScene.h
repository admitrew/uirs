#pragma once

#include <QGraphicsScene>
#include <QString>

class QKeyEvent;
class QGraphicsSceneMouseEvent;

class MapScene : public QGraphicsScene
{
    Q_OBJECT

public:
    enum class Mode {
        Select,
        AddPoint
    };

    explicit MapScene(QObject* parent = nullptr);

    void setSelectMode();
    void setAddPointMode(const QString& pointType);

    Mode mode() const;
    QString currentPointType() const;

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

private:
    Mode m_mode = Mode::Select;
    QString m_currentPointType = "station";
};