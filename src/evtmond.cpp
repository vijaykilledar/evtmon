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

#include "evtmond.h"
#include "global.h"
#include "dconfig.h"
#include "intf_evt_monitor.h"
#include "logger.h"

std::map<std::string, EventMonitor *> g_evt_monitors;

void do_heartbeat() {

}

void config_daemon() {
    Json::Value conf = DaemonConfig::instance().get_config();
}

int main(int argc, char *argv[]) {
    
    int opt;
    bool is_daemon = true, debug_enabled = false;
    char *conf_file = nullptr;

    while ((opt = getopt(argc, argv, "fhc:")) != -1) {
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
