#include "LineItem.h"

LineItem::LineItem(LineType t): type(t){}

void LineItem::addPoint(QPointF p){ pts.push_back(p); updateLast(p); }

void LineItem::updateLast(QPointF p){
    if(pts.isEmpty()) return;
    QPainterPath path(pts.first());
    for(auto& pt:pts) path.lineTo(pt);
    path.lineTo(p);
    setPath(path);
}

QString LineItem::toTh2() const {
    QString s="line "+toTherion(type)+"\n";
    for(auto& p:pts) s+=QString("  %1 %2\n").arg(p.x()).arg(p.y());
    return s+"endline\n";
}
