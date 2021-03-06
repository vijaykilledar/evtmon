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
#include "evt_collector.h"
#include "evt_monitor.h"
#include "event_queue.h"
#include<stdlib.h>

std::map<std::string, EventMonitor *> g_evt_monitors;
std::map<std::string, EventCollector *> g_evt_collectors;
std::map<std::string, std::thread> g_monitors_pool;
std::map<std::string, std::thread> g_collectors_pool;
std::thread g_event_proc;

void do_heartbeat() {

}

void config_daemon() {
    
    log(LOG::DEBUG, "%s",__FUNCTION__);
    Json::Value conf = DaemonConfig::instance().get_config();
    Json::FastWriter fastWriter;
    std::string str_conf = fastWriter.write(conf);
    log(LOG::DEBUG, "Daemon Configuration %s", str_conf.c_str());

    EventCollector * (*create_collector)();
    std::string collector_location = conf["collector_location"].asString();
    for (int idx = 0; idx < conf["evt_collectors"].size(); idx++)
    {
        Json::Value collector = conf["evt_collectors"][idx];
        std::string collector_name = collector["name"].asString();
        std::string soname = collector_location + "lib" + collector_name + ".so";
        log(LOG::DEBUG, "Registering evt collector using:%s", soname.c_str());
        void *handle = dlopen(soname.c_str(), RTLD_NOW);
        if (handle != NULL) {
            create_collector = (EventCollector * (*)())dlsym(handle, "create_collector");
            if (g_evt_collectors.find(collector_name) == g_evt_collectors.end()) {
                g_evt_collectors[collector_name] = (EventCollector *)create_collector();
                g_evt_collectors[collector_name]->configure(collector);
                g_evt_collectors[collector_name]->init();
                g_collectors_pool[collector_name] = std::thread(&EventCollector::start, g_evt_collectors[collector_name]);
            }
        } else {
            log(LOG::ERROR,"Failed to register evt Collector:%s using:%s Error:%s", collector_name.c_str(), soname.c_str(), dlerror());
        }
    }

    log(LOG::DEBUG,"Starting event processor");
    EventQueue *eptr = EventQueue::get_instance2();
    g_event_proc = std::thread(&EventQueue::processor, eptr, g_evt_collectors);
    g_event_proc.detach();

    EventMonitor * (*create)();
    std::string monitor_location = conf["monitor_location"].asString();
    for (int idx = 0; idx < conf["evt_monitors"].size(); idx++)
    {
        Json::Value monitor = conf["evt_monitors"][idx];
        std::string monitor_name = monitor["name"].asString();
        std::string soname = monitor_location + "lib" + monitor_name + ".so";
        log(LOG::DEBUG, "Registering evt monior using:%s", soname.c_str());
        void *handle = dlopen(soname.c_str(), RTLD_NOW);
        if (handle != NULL) {
            create = (EventMonitor * (*)())dlsym(handle, "create_monitor");
            if (g_evt_monitors.find(monitor_name) == g_evt_monitors.end()) {
                g_evt_monitors[monitor_name] = (EventMonitor *)create();
                g_evt_monitors[monitor_name]->configure(eptr, monitor);
                g_evt_monitors[monitor_name]->init();
                g_monitors_pool[monitor_name] = std::thread(&EventMonitor::start, g_evt_monitors[monitor_name]);
            }
        } else {
            log(LOG::ERROR,"Failed to register evt monior:%s using:%s Error:%s", monitor_name.c_str(), soname.c_str(), dlerror());
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
