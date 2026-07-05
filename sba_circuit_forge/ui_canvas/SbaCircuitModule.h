#ifndef SBACIRCUITMODULE_H
#define SBACIRCUITMODULE_H

#include <QGraphicsItem>
#include <QPainter>
#include "circuit.pb.h"

class SbaCircuitModule : public QGraphicsItem {
public:
    SbaCircuitModule(const sba_architecture::ComponentDef& def);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    std::string getId() const;
    void highlightError();

private:
    sba_architecture::ComponentDef componentDef;
    bool hasError;
};

#endif // SBACIRCUITMODULE_H