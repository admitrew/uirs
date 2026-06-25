#pragma once

#include <QGraphicsScene>
#include <QString>

class QKeyEvent;
class QGraphicsSceneMouseEvent;
class LineItem;

class MapScene : public QGraphicsScene
{
    Q_OBJECT

public:
    enum class Mode {
        Select,
        AddPoint,
        AddLine
    };

    explicit MapScene(QObject* parent = nullptr);

    void setSelectMode();
    void setAddPointMode(const QString& pointType);
    void setAddLineMode(const QString& lineType);

    Mode mode() const;
    QString currentPointType() const;
    QString currentLineType() const;

    void cancelCurrentLine();

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

private:
    void finishCurrentLine(bool keepLine);

    Mode m_mode = Mode::Select;

    QString m_currentPointType = "station";
    QString m_currentLineType = "wall";

    LineItem* m_currentLine = nullptr;
};