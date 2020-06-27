#ifndef _CONFIG_H_
#define _CONFIG_H_

class DaemonConfig {
    private:
        std::string m_conf_file; 
    public:
        DaemonConfig(const char *conf_file) {
            m_conf_file = conf_file;
        }
        load();
};
#endif //_CONFIG_H_
