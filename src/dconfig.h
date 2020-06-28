#ifndef _DCONFIG_H_
#define _DCONFIG_H_

class DaemonConfig {
    private:
        DaemonConfig() {};
        DaemonConfig(DaemonConfig const& copy) {};
        DaemonConfig& operator=(DaemonConfig const& copy) {};
        std::string m_conf_file;
    public:
        static DaemonConfig& instance(); 
        bool load_config(const char *conf_file);
};
#endif //_DCONFIG_H_
