## evtmond

> :warning: Under Construction

evtmond is a system event monitoring daemon which allows you to configure different monitors which will monitor the system events and pass it to configured collectors.

Monitors and collectors are written in such a way that every monitor and collector source code will generate shared object files. Core daemon will load the shared object files as per configuration present in config/ json file.

![evtmond block](doc/evtmond.png?raw=true "Block Diagram")

Currently only one monitor and collector is configured and supported and that can be configured and enabled using the below way.
Before running daemon please update the collector configuration in config/evtmond.json
 ```
 ./autogen.sh
 mkdir build && cd build
 ./configure --enable-netintfmon --enable-collecttofile
 make
 make install
 sudo evtmond -c ../config/evtmond.json -f -d
 ```
Above steps will create network interface monitor and collectofile collector. netintfmon is interface link status monitor which will listen for interface link status change. Events will get sent to the collector in this case collecttofile collector will collect the events and write it to file path mentioned in the config.json
Do ifconfig interface_name down/up and check events and interface status is getting updated in syslog message.
