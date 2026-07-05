#include "MainWindow.h"
#include "SbaCircuitModule.h"
#include "NetworkManager.h"
#include <QVBoxLayout>
#include <QWidget>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene);

    setCentralWidget(view);
    resize(800, 600);
    setWindowTitle("SBA Circuit Forge");

    populate_mock_canvas();

    // Trigger mock network request
    NetworkManager::getInstance().validate_canvas(scene);
}

MainWindow::~MainWindow() {}

void MainWindow::populate_mock_canvas() {
    // Add some mock components
    sba_architecture::ComponentDef comp1;
    comp1.set_id("COMP_1");
    comp1.set_type("Resistor");
    comp1.set_canvas_x(-100);
    comp1.set_canvas_y(0);

    auto pin1 = comp1.add_pins();
    pin1->set_name("A");
    pin1->set_relative_x(-20);
    pin1->set_relative_y(0);

    auto pin2 = comp1.add_pins();
    pin2->set_name("B");
    pin2->set_relative_x(20);
    pin2->set_relative_y(0);

    SbaCircuitModule *module1 = new SbaCircuitModule(comp1);
    scene->addItem(module1);
}

void MainWindow::on_fault_received(const std::vector<std::string>& targets) {
    std::cout << "Fault received, highlighting targets..." << std::endl;
    for (QGraphicsItem* item : scene->items()) {
        SbaCircuitModule* module = dynamic_cast<SbaCircuitModule*>(item);
        if (module) {
            for (const std::string& target : targets) {
                if (module->getId() == target) {
                    module->highlightError();
                }
            }
        }
    }
}