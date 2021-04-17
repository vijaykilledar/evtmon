#include <dirent.h>
#include <iterator>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <syslog.h>
#include <unistd.h>
#include <vector>
#include <thread>
#include <map>
#include <dlfcn.h>
#include <thread>
#include "evtmond.h"
#include "global.h"
#include "evtmon_config.h"
#include "common/logger.h"
#include "evt_monitor.h"

/*TODO:Need better place or handling of starting monitors
e.g Build monitor and collector as shared object and load them.
*/
std::map<std::string, EventMonitor *> g_evt_monitors;
std::map<std::string, std::thread> g_monitors_pool;
void do_heartbeat() {

}

void config_daemon() {
    log(LOG::DEBUG, "%s",__FUNCTION__);
    Json::Value conf = DaemonConfig::instance().get_config();
    Json::FastWriter fastWriter;
    std::string str_conf = fastWriter.write(conf);
    log(LOG::DEBUG, "Daemon Configuration %s", str_conf.c_str());
    EventMonitor * (*create)();
    for(int idx = 0; idx < conf["evt_monitors"].size(); idx++)
    {
        Json::Value monitor = conf["evt_monitors"][idx];
        std::string monitor_name = monitor["name"].asString();
        std::string soname = std::string(".libs/lib") + monitor_name + ".so";
        void *handle = dlopen(soname.c_str(), RTLD_LAZY);
        create = (EventMonitor * (*)())dlsym(handle, "create_monitor");
        if(g_evt_monitors.find(monitor_name) == g_evt_monitors.end()) {
            g_evt_monitors[monitor_name] = (EventMonitor *)create();
            g_evt_monitors[monitor_name]->configure(monitor);
            g_evt_monitors[monitor_name]->init();
            g_monitors_pool[monitor_name] = std::thread(&EventMonitor::start, g_evt_monitors[monitor_name]);
        }
    }
}

int main(int argc, char *argv[]) {
    
    int opt;
    bool is_daemon = true, debug_enabled = false;
    char *conf_file = nullptr;
    EventMonitor *evtmon;

    while ((opt = getopt(argc, argv, "fhdc:")) != -1) {
        switch (opt) {
            case 'f':
                is_daemon = false;
                break;
            case 'c':
                conf_file = optarg;
                break;
            case 'd':
                debug_enabled = true;
                break;
            case '?':
            case 'h':
                fprintf(stdout,"Usage: %s \n                                \
                                 \r-f     dont fork\n                       \
                                 \r-c     config file path\n                \
                                 \r-d     debug log enabled\n",             \
                                 argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    //load config
    bool config_load = config.load_config((conf_file != nullptr) ? conf_file : CONF_FILE);
    if(!config_load) {
        log(LOG::ERROR, "Failed to load config. Please check config file.");
        exit(EXIT_FAILURE);
    }
    
    if(debug_enabled) {
        Logger::instance().set_log_level(LOG::DEBUG);
    } 
    
    if (is_daemon) {
    
        pid_t pid, sid;
        pid = fork();

        if (pid > 0) {
          exit(EXIT_SUCCESS);
        }
        // A process ID lower than 0 indicates a failure in either process
        else if (pid < 0) {
          exit(EXIT_FAILURE);
        }
        // The parent process has now terminated, and the forked child process will continue
        // (the pid of the child process was 0)
        // Since the child process is a daemon, the umask needs to be set so files and logs can be written
       
        umask(0);
        // Open system logs for the child process
        openlog("netevtmond", LOG_NOWAIT | LOG_PID, LOG_USER);
        log(LOG::INFO, "Successfully started netevtmond");
        // Generate a session ID for the child process
        sid = setsid();
        // Ensure a valid SID for the child process
        if (sid < 0) {
            // Log failure and exit
            log(LOG::ERROR, "Could not generate session ID for child process");

            // If a new session ID could not be generated, we must terminate the child process
            // or it will be orphaned
            exit(EXIT_FAILURE);
        }

        // Change the current working directory to a directory guaranteed to exist
        if ((chdir("/")) < 0) {
            log(LOG::ERROR, "Could not change working directory to /");

            exit(EXIT_FAILURE);
        }

        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
    }
    
    const int SLEEP_INTERVAL = 5;
    
    config_daemon();

    while (1) {
      do_heartbeat();

      sleep(SLEEP_INTERVAL);
    }

    syslog(LOG_NOTICE, "Stopping netevtmond");
    closelog();

    exit(EXIT_SUCCESS);
}
