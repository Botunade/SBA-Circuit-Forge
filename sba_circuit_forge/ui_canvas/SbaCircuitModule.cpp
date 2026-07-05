#include "SbaCircuitModule.h"

SbaCircuitModule::SbaCircuitModule(const sba_architecture::ComponentDef& def)
    : componentDef(def), hasError(false) {
    setPos(def.canvas_x(), def.canvas_y());
    setFlags(ItemIsMovable | ItemIsSelectable);
}

QRectF SbaCircuitModule::boundingRect() const {
    return QRectF(-30, -30, 60, 60);
}

void SbaCircuitModule::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    QPen pen(hasError ? Qt::red : Qt::black);
    pen.setWidth(2);
    painter->setPen(pen);
    painter->setBrush(Qt::lightGray);

    painter->drawRect(boundingRect());

    // Draw text
    painter->setPen(Qt::black);
    painter->drawText(boundingRect(), Qt::AlignCenter, QString::fromStdString(componentDef.id()));

    // Draw pins
    painter->setBrush(Qt::yellow);
    for (const auto& pin : componentDef.pins()) {
        painter->drawEllipse(QPointF(pin.relative_x(), pin.relative_y()), 5, 5);
    }
}

std::string SbaCircuitModule::getId() const {
    return componentDef.id();
}

void SbaCircuitModule::highlightError() {
    hasError = true;
    update();
}