#ifndef SBA_PULSE_CORE_H
#define SBA_PULSE_CORE_H

#include <string>
#include <vector>
#include <iostream>
#include <functional>
#include "circuit.pb.h"

// Define a simple callback type for telemetry updates
typedef std::function<void(const std::string&, double, double)> TelemetryCallback;

class SBAPulseCore {
public:
    SBAPulseCore();
    ~SBAPulseCore();

    // DLL Linkage simulation
    bool load_ngspice_dll(const std::string& path);

    // Netlist Compilation
    std::string generate_netlist(const std::vector<sba_architecture::ComponentDef>& components);

    // Live Parameter Tuning
    bool alter_parameter(const std::string& target_component, double new_value);

    // Telemetry Event Dispatcher
    void subscribe_telemetry(TelemetryCallback callback);

    // Mock functions to represent Ngspice callbacks
    static int SendChar(char* text, int id, void* data);
    static int SendStat(char* text, int id, void* data);
    static int ControlledExit(int status, bool unload, bool exit_upon_quit, int id, void* data);
    static int SendData(void* vdata, int num_vecs, int id, void* data);
    static int SendInitData(void* idata, int id, void* data);

private:
    std::vector<TelemetryCallback> telemetry_subscribers;

    void dispatch_telemetry(const std::string& node, double voltage, double current);
};

#endif // SBA_PULSE_CORE_H