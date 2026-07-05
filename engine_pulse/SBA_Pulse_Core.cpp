#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>

// This must match the ngspice shared library header
#include "sharedspice.h"

//
// NGSPICE Callback Implementations
//

// Callback for ngspice to send text output
int MySendChar(char* text, int id, void* user_data) {
    if (text) {
        std::cout << "[ngspice] " << text << std::endl;
    }
    return 0;
}

// Callback for ngspice to send simulation status
int MySendStat(char* stat, int id, void* user_data) {
    if (stat) {
        std::cout << "[ngspice status] " << stat << std::endl;
    }
    return 0;
}

// Callback for ngspice to send actual data vector arrays during simulation
int MySendData(pvecvaluesall vdata, int numstructs, int id, void* user_data) {
    if (vdata) {
        // Here we could extract live mathematical outputs to pass to the python UI
        // For now, we will simply log the raw data counts or a tiny bit of info.
        for (int i = 0; i < vdata->veccount; i++) {
            if (vdata->vecsa[i]) {
                std::cout << "[ngspice data] node: " << vdata->vecsa[i]->name
                          << ", val: " << vdata->vecsa[i]->creal << std::endl;
            }
        }
    }
    return 0;
}

// Callback for ngspice to request a controlled exit
int MyControlledExit(int exit_status, NG_BOOL immediate, NG_BOOL quit_exit, int id, void* user_data) {
    std::cout << "[ngspice exit] status: " << exit_status
              << ", immediate: " << (immediate ? "true" : "false")
              << ", quit_exit: " << (quit_exit ? "true" : "false") << std::endl;
    return 0;
}

// Callback for ngspice to send initialization vector data
int MySendInitData(pvecinfoall idata, int id, void* user_data) {
    std::cout << "[ngspice init data] Vector count: " << idata->veccount << std::endl;
    return 0;
}

// Callback for ngspice to indicate background thread status
int MyBGThreadRunning(NG_BOOL is_running, int id, void* user_data) {
    std::cout << "[ngspice bg thread] running: " << (is_running ? "true" : "false") << std::endl;
    return 0;
}

//
// SBA Pulse Functionality
//

void initialize_engine() {
    std::cout << "Initializing Ngspice Engine..." << std::endl;
    int ret = ngSpice_Init(
        &MySendChar,
        &MySendStat,
        &MyControlledExit,
        &MySendData,
        &MySendInitData,
        &MyBGThreadRunning,
        nullptr
    );

    if (ret != 0) {
        std::cerr << "Failed to initialize ngspice! Code: " << ret << std::endl;
        exit(1);
    }
    std::cout << "Ngspice Engine Initialized Successfully." << std::endl;
}

// Replace the mock implementation with actual ngspice mathematical simulation
void generate_netlist(const std::vector<std::string>& blueprint_buffer) {
    std::cout << "Generating Netlist and Executing Simulation..." << std::endl;

    // ngSpice_Circ takes an array of char* ending with NULL
    std::vector<char*> circarray;
    for (const auto& line : blueprint_buffer) {
        circarray.push_back(strdup(line.c_str()));
    }
    circarray.push_back(nullptr);

    int ret = ngSpice_Circ(circarray.data());

    // Free the strdup'd strings
    for (size_t i = 0; i < circarray.size() - 1; ++i) {
        free(circarray[i]);
    }

    if (ret != 0) {
        std::cerr << "Failed to load circuit! Code: " << ret << std::endl;
        return;
    }

    // Run the simulation in background thread, so it can return data callbacks
    ngSpice_Command(const_cast<char*>("bg_run"));

    // Wait until simulation is finished (a simple loop for demonstration)
    while(ngSpice_running()) {
        // just wait
    }

    std::cout << "Simulation Execution Complete." << std::endl;
}

int main() {
    std::cout << "SBA Pulse Core Starting..." << std::endl;
    initialize_engine();

    // Example usage of generate_netlist with a real SPICE netlist
    std::vector<std::string> sample_netlist = {
        "Sample Circuit",
        "V1 1 0 DC 5",
        "R1 1 2 1k",
        "R2 2 0 1k",
        ".dc V1 0 10 1",
        ".end"
    };

    generate_netlist(sample_netlist);

    std::cout << "SBA Pulse Core Exiting..." << std::endl;
    return 0;
}
