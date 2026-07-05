#include "SBA_Pulse_Core.h"
#include <sstream>

SBAPulseCore::SBAPulseCore() {
    std::cout << "SBA Pulse Core initialized." << std::endl;
}

SBAPulseCore::~SBAPulseCore() {
    std::cout << "SBA Pulse Core destroyed." << std::endl;
}

bool SBAPulseCore::load_ngspice_dll(const std::string& path) {
    // Mock DLL loading
    std::cout << "Loading ngspice.dll from: " << path << std::endl;
    return true;
}

std::string SBAPulseCore::generate_netlist(const std::vector<sba_architecture::ComponentDef>& components) {
    std::stringstream ss;
    ss << "* SBA Circuit Forge Generated Netlist\n";

    for (const auto& comp : components) {
        ss << comp.id() << " ";
        // Mocking pins as connected to dummy nodes based on their names
        for (const auto& pin : comp.pins()) {
            ss << comp.id() << "_" << pin.name() << " ";
        }
        // If type is a resistor or generic value, mock a value (e.g., 1k)
        if (comp.type() == "Resistor") {
            ss << "1k\n";
        } else if (comp.type() == "Capacitor") {
            ss << "1uF\n";
        } else {
             ss << " *Model=" << comp.type() << "\n";
        }
    }

    ss << ".end\n";
    return ss.str();
}

bool SBAPulseCore::alter_parameter(const std::string& target_component, double new_value) {
    std::cout << "Altering parameter: " << target_component << " to " << new_value << std::endl;
    // In actual implementation, call ngSpice_Command with "alter ..."
    return true;
}

void SBAPulseCore::subscribe_telemetry(TelemetryCallback callback) {
    telemetry_subscribers.push_back(callback);
}

void SBAPulseCore::dispatch_telemetry(const std::string& node, double voltage, double current) {
    for (auto& cb : telemetry_subscribers) {
        cb(node, voltage, current);
    }
}

// Static mock Ngspice callbacks
int SBAPulseCore::SendChar(char* text, int id, void* data) {
    std::cout << "Ngspice [Char]: " << text << std::endl;
    return 0;
}

int SBAPulseCore::SendStat(char* text, int id, void* data) {
    std::cout << "Ngspice [Stat]: " << text << std::endl;
    return 0;
}

int SBAPulseCore::ControlledExit(int status, bool unload, bool exit_upon_quit, int id, void* data) {
    std::cout << "Ngspice [Exit]: " << status << std::endl;
    return 0;
}

int SBAPulseCore::SendData(void* vdata, int num_vecs, int id, void* data) {
    // In reality, this parses vecinfoall structs
    // Mock parsing and dispatching
    if (data) {
        SBAPulseCore* engine = static_cast<SBAPulseCore*>(data);
        engine->dispatch_telemetry("node1", 5.0, 0.02);
    }
    return 0;
}

int SBAPulseCore::SendInitData(void* idata, int id, void* data) {
    std::cout << "Ngspice [Init Data]" << std::endl;
    return 0;
}
