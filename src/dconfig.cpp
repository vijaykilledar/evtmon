#include <fstream>
#include <iostream>

#include "dconfig.h"

DaemonConfig& DaemonConfig::instance() {
    static DaemonConfig insta;
    return insta;
} 

bool DaemonConfig::load_config(const char* conf_file) {
    std::ifstream infile(conf_file);
    Json::Reader reader;
    reader.parse(infile, m_config);
    return true;
}
