#pragma once

#include <QPen>
#include <QBrush>
#include <QString>

class StyleManager
{
public:
    static QPen linePen(const QString& type);
    static QBrush pointBrush(const QString& type);
};