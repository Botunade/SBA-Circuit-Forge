#include "NetworkManager.h"
#include "circuit.pb.h"
#include <QNetworkRequest>
#include <QUrl>
#include <iostream>
#include <QApplication>
#include "MainWindow.h"

NetworkManager::NetworkManager() {
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &NetworkManager::onReplyFinished);
}

NetworkManager::~NetworkManager() {}

NetworkManager& NetworkManager::getInstance() {
    static NetworkManager instance;
    return instance;
}

void NetworkManager::validate_canvas(QGraphicsScene* scene) {
    // Mock packaging canvas state into CircuitBlueprint
    sba_architecture::CircuitBlueprint blueprint;
    blueprint.set_project_name("MockProject");

    // Create dummy component to trigger the mock fault logic
    sba_architecture::ComponentDef* comp = blueprint.add_components();
    comp->set_id("COMP_1");

    std::string serialized_data;
    blueprint.SerializeToString(&serialized_data);

    QNetworkRequest request(QUrl("http://127.0.0.1:8000/api/watchdog_validate"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-protobuf");

    manager->post(request, QByteArray::fromStdString(serialized_data));
}

void NetworkManager::onReplyFinished(QNetworkReply* reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response_data = reply->readAll();

        sba_architecture::FaultDiagnostic fault;
        if (fault.ParseFromArray(response_data.data(), response_data.size())) {
            std::cout << "Received diagnostic: " << fault.chat_message() << std::endl;

            std::vector<std::string> targets;
            for (const auto& target : fault.highlight_targets()) {
                targets.push_back(target);
            }

            // Find main window and dispatch
            for (QWidget* widget : QApplication::topLevelWidgets()) {
                MainWindow* mw = qobject_cast<MainWindow*>(widget);
                if (mw) {
                    mw->on_fault_received(targets);
                }
            }
        }
    } else {
        std::cerr << "Network Error: " << reply->errorString().toStdString() << std::endl;
    }
    reply->deleteLater();
}