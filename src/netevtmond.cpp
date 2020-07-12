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

#include "netevtmond.h"
#include "global.h"
#include "dconfig.h"
#include "intf_evt_monitor.h"
void do_heartbeat() {

}

int main(int argc, char *argv[]) {
    
    int opt;
    bool is_daemon = true;
    char *conf_file = nullptr;

    while ((opt = getopt(argc, argv, "fhc:")) != -1) {
        switch (opt) {
            case 'f':
                is_daemon = false;
                break;
            case 'c':
                conf_file = optarg;
                break;
            case '?':
            case 'h':
                fprintf(stderr, "Usage: %s \n -f\tdont fork\n-c\t config file path\n",argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    //load config
    bool config_load = config.load_config((conf_file != nullptr) ? conf_file : CONF_FILE);
    if(!config_load) {
        syslog(LOG_ERR,"Failed to load config. Please check config file.");
        exit(EXIT_FAILURE);
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
        syslog(LOG_NOTICE, "Successfully started netevtmond");
        // Generate a session ID for the child process
        sid = setsid();
        // Ensure a valid SID for the child process
        if (sid < 0) {
            // Log failure and exit
            syslog(LOG_ERR, "Could not generate session ID for child process");

            // If a new session ID could not be generated, we must terminate the child process
            // or it will be orphaned
            exit(EXIT_FAILURE);
        }

        // Change the current working directory to a directory guaranteed to exist
        if ((chdir("/")) < 0) {
            // Log failure and exit
            syslog(LOG_ERR, "Could not change working directory to /");

            // If our guaranteed directory does not exist, terminate the child process to ensure
            // the daemon has not been hijacked
            exit(EXIT_FAILURE);
        }

        // A daemon cannot use the terminal, so close standard file descriptors for security reasons
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
    }
    
    // Daemon-specific intialization should go here
    const int SLEEP_INTERVAL = 5;
    // Enter daemon loop
    while (1) {
      // Execute daemon heartbeat, where your recurring activity occurs
      do_heartbeat();

      // Sleep for a period of time
      sleep(SLEEP_INTERVAL);
    }

    // Close system logs for the child process
    syslog(LOG_NOTICE, "Stopping netevtmond");
    closelog();

    // Terminate the child process when the daemon completes
    exit(EXIT_SUCCESS);
}
