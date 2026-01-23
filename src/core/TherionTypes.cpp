#include "TherionTypes.h"

QString toTherion(PointType t) {
    switch (t) {
    case PointType::Station: return "station";
    case PointType::Entrance: return "entrance";
    case PointType::Fix: return "fix";
    case PointType::Continuation: return "continuation";
    case PointType::Label: return "label";
    case PointType::Remark: return "remark";
    case PointType::Danger: return "danger";
    case PointType::Height: return "height";
    case PointType::AirDraught: return "air-draught";
    case PointType::WaterFlow: return "water-flow";
    default: return "unknown";
    }
}

QString toTherion(LineType t) {
    switch (t) {
    case LineType::Wall: return "wall";
    case LineType::Survey: return "survey";
    case LineType::Water: return "water";
    case LineType::Border: return "border";
    case LineType::Rock: return "rock";
    case LineType::Slope: return "slope";
    case LineType::Chimney: return "chimney";
    case LineType::Pitch: return "pitch";
    case LineType::Flowstone: return "flowstone";
    case LineType::Label: return "label";
    default: return "unknown";
    }
}
