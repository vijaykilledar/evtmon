#include <fstream>
#include <iostream>

#include "dconfig.h"
#include "global.h"

DaemonConfig& DaemonConfig::instance() {
    static DaemonConfig insta;
    return insta;
} 

bool DaemonConfig::load_config(const char* conf_file) {
    std::ifstream infile(conf_file);
    json j;
    infile >> j;
    std::cout << j["evt_monitors"];
    return true;
}
