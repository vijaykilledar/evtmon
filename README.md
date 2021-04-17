## evtmond
evtmond is a system event monitoring daemon which allows you to configure different monitors which will monitor the system events and pass it to configured collectors.

Monitors and collectors are written in such a way that every monitor and collector source code will generate shared object files. Core daemon will load the shared object files as per configuration present in config/ json file.

![evtmond block](doc/evtmond.png?raw=true "Block Diagram")

