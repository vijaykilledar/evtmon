#include "collecttofile.h"
#include <fstream>
#include <unistd.h>

extern "C" EventCollector* create_collector() {
    return new CollectToFile("collecttofile");
}


bool CollectToFile::start() {
    std::ofstream outfile;
    std::string f_name = m_conf["config"]["path"].asString();
    while(true) {
        outfile.open(f_name.c_str(), std::ios_base::app);
        outfile << collect_event();
        outfile.close();
    }
    return true;
}
