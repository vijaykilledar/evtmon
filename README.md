## evtmond
evtmond is a system event monitoring daemon which allows you to configure different monitors which will monitor the system events and pass it to configured collectors.

Monitors and collectors are written in such a way that every monitor and collector source code will generate shared object files. Core daemon will load the shared object files as per configuration present in config/ json file.

![evtmond block](doc/evtmond.png?raw=true "Block Diagram")

Currently only one monitor is configured and supported and that can be configured and enabled using the below way.
 
 1. ./autogen.sh
 2. mkdir build && cd build
 3. ./configure --enable-netintfmon
 4. sudo ./evtmond -c ../config/evtmond.json -f -d

Above steps will create network interface monitor and start a thread for the same. You can go to /var/log and do tail -f syslog to see the status of the monitor events.
Do ifconfig interface_name down/up and check events and interface status is getting updated in syslog message.
