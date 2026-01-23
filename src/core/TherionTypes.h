#pragma once
#include <QString>

enum class PointType {
    Station,
    Entrance,
    Fix,
    Continuation,
    Label,
    Remark,
    Danger,
    Height,
    AirDraught,
    WaterFlow,
    Other
};

enum class LineType {
    Wall,
    Survey,
    Water,
    Border,
    Rock,
    Slope,
    Chimney,
    Pitch,
    Flowstone,
    Label,
    Other
};

QString toTherion(PointType type);
QString toTherion(LineType type);
