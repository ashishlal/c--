#ifndef _IFS_AGENT_HPP_
#define _IFS_AGENT_HPP_

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/thread.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <functional>

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

enum MibVersion {
    MIB_VERSION_1 = 3,
    MIB_VERSION_2,
    MIB_VERSION_3
};

const unsigned int MIB_VERSION = MIB_VERSION_1;
const std::string turret_cn = "atp_turret";

enum ATPDeviceTypes {
    ATP_DEV_MIXER=1,
    ATP_DEV_TURRET,
    ATP_DEV_GATEWAY
};

enum ATPErrorTypes {
    ATP_FATAL,
    ATP_ERROR
};

enum SnmpStats {
    atp_host_name = 1,
    atp_port,
    atp_org,
    atp_site,
    atp_cn,
    atp_load_factor,
    STAT_END_COMMON_STATS,
    
    // Mixer Stats
    num_mixes,
    max_mixes,
    STAT_END_MIXER_STATS,

    // Turret Stats
    turret_total_calls,
    turret_total_rejected_calls,
    turret_total_dropped_calls,
    turret_current_calls,
    turret_average_call_duration,
    turret_longest_call_duration,
    STAT_END_TURRET_STATS,

    gw_total_calls,
    gw_current_calls,
    gw_max_calls,
    gw_total_incoming_calls,
    gw_total_outgoing_calls,
    gw_rejected_calls,
    gw_dropped_calls,
    gw_average_call_duration,
    gw_longest_call_duration,
};

#define GENERATE_AGENT_COMMON_STATS_PARAMETER_HANDLER(index, common_name, my_name_oid) {\
    oid name_oid[] = { 1, 3, 6, 1, 4, 1, 39282, MIB_VERSION, 0, 1, index };\
    netsnmp_handler_registration *h = \
        netsnmp_create_handler_registration( \
            my_name_oid, HandleAgentCommonStatsParameter,\
            name_oid, OID_LENGTH(name_oid),\
            HANDLER_CAN_RONLY);\
    h->contextName = const_cast<char *>(strdup(common_name.c_str())); \
    netsnmp_register_scalar(h); \
}

#define GENERATE_AGENT_MIXER_STATS_PARAMETER_HANDLER(index, common_name, my_name_oid) {\
    oid name_oid[] = { 1, 3, 6, 1, 4, 1, 39282, MIB_VERSION, 0, 2, index };\
    netsnmp_handler_registration *h = \
        netsnmp_create_handler_registration( \
            my_name_oid, HandleAgentMixerStatsParameter,\
            name_oid, OID_LENGTH(name_oid),\
            HANDLER_CAN_RONLY);\
    h->contextName = const_cast<char *>(strdup(common_name.c_str())); \
    netsnmp_register_scalar(h); \
}

#define GENERATE_AGENT_TURRET_STATS_PARAMETER_HANDLER(index, common_name, my_name_oid) {\
    oid name_oid[] = { 1, 3, 6, 1, 4, 1, 39282, MIB_VERSION, 0, 3, index };\
    netsnmp_handler_registration *h = \
        netsnmp_create_handler_registration( \
            my_name_oid, HandleAgentTurretStatsParameter,\
            name_oid, OID_LENGTH(name_oid),\
            HANDLER_CAN_RONLY);\
    h->contextName = const_cast<char *>(strdup(common_name.c_str())); \
    netsnmp_register_scalar(h); \
}

#define GENERATE_AGENT_GATEWAY_STATS_PARAMETER_HANDLER(index, common_name, my_name_oid) {\
    oid name_oid[] = { 1, 3, 6, 1, 4, 1, 39282, MIB_VERSION, 0, 4, index };\
    netsnmp_handler_registration *h = \
        netsnmp_create_handler_registration( \
            my_name_oid, HandleAgentGatewayStatsParameter,\
            name_oid, OID_LENGTH(name_oid),\
            HANDLER_CAN_RONLY);\
    h->contextName = const_cast<char *>(strdup(common_name.c_str())); \
    netsnmp_register_scalar(h); \
}

#define AGENT_COMMON_STATS_SET(param) \
virtual void set_##param(std::string & s) \
{ \
   m_common_stats[param] = s; \
   return; \
};

#define AGENT_COMMON_STATS_GET(param) \
virtual std::string& get_##param() \
{ \
    return m_common_stats[param]; \
};

class IfsAgent;
class NetSnmpLibResources {
public:
    NetSnmpLibResources() {};
    ~NetSnmpLibResources() {};
    
    static int HandleAgentTurretStatsParameter(
        netsnmp_mib_handler *handler,
        netsnmp_handler_registration *reginfo,
        netsnmp_agent_request_info *reqinfo,
        netsnmp_request_info *requests);
    static int HandleAgentGatewayStatsParameter(
        netsnmp_mib_handler *handler,
        netsnmp_handler_registration *reginfo,
        netsnmp_agent_request_info *reqinfo,
        netsnmp_request_info *requests);
    static int HandleAgentMixerStatsParameter(
        netsnmp_mib_handler *handler,
        netsnmp_handler_registration *reginfo,
        netsnmp_agent_request_info *reqinfo,
        netsnmp_request_info *requests);
    static int HandleAgentCommonStatsParameter(
        netsnmp_mib_handler *handler,
        netsnmp_handler_registration *reginfo,
        netsnmp_agent_request_info *reqinfo,
        netsnmp_request_info *requests);
    static int MyTable_handler( 
            netsnmp_mib_handler *handler,
            netsnmp_handler_registration *reginfo,
            netsnmp_agent_request_info *reqinfo,
            netsnmp_request_info *requests );

    static int snmp_input(int operation, netsnmp_session * session, 
        int reqid, netsnmp_pdu *pdu, void *magic);
    static void optProc(int argc, char *const *argv, int opt);
    static int snmpTrap(int argc, char **argv);
    static bool initAgentX(std::string &cn, std::string &agentx_loc);
    static void initCommonStatsTable();
    static IfsAgent *agent;
    struct CommonStatsTableEntry
    {
        unsigned commonStatsTableIndex;
        std::string host_name;    
        int valid; 
    };
    static struct CommonStatsTableEntry commonStatsTableEntry;
};

typedef std::function<unsigned int ()> SnmpCallback;

class IfsAgent: private NetSnmpLibResources {
public:
    IfsAgent(boost::asio::io_service& io_service);
    ~IfsAgent() { shutdown(); };
    
    virtual bool initialise(int dev, std::string& host, std::string& atp_port,
                            std::string& site, std::string& org, std::string& common_name);
    virtual void mainloop_nonblocking(const boost::system::error_code& error);
    virtual void shutdown();
    virtual int snmpTrap(int level, std::string & errorMsg);
   
    /* Common Stats */
    AGENT_COMMON_STATS_SET(atp_host_name);
    AGENT_COMMON_STATS_SET(atp_port);
    AGENT_COMMON_STATS_SET(atp_org);
    AGENT_COMMON_STATS_SET(atp_site);
    AGENT_COMMON_STATS_SET(atp_cn);
    AGENT_COMMON_STATS_SET(atp_load_factor);

    AGENT_COMMON_STATS_GET(atp_host_name);
    AGENT_COMMON_STATS_GET(atp_port);
    AGENT_COMMON_STATS_GET(atp_org);
    AGENT_COMMON_STATS_GET(atp_site);
    AGENT_COMMON_STATS_GET(atp_cn);
    AGENT_COMMON_STATS_GET(atp_load_factor);

    virtual void set_monitoring_host_port(std::string &s)
    {
        m_monitoring_agent_addr_port = s;
    }
    virtual std::string& get_monitoring_host_port()
    {
        return m_monitoring_agent_addr_port;
    }
    virtual void set_agentx_socket_loc(std::string &s)
    {
        m_agentx_loc = s;
    }
    virtual std::string& get_agentx_socket_loc()
    {
        return m_agentx_loc;
    }
    virtual void set_disable_snmp(std::string &s);
    virtual bool& get_disable_snmp()
    {
        return m_disable_snmp;
    }
    virtual void add_stat(unsigned int name, SnmpCallback cb)
    {
        m_stat_map[name] = cb;
    }
    template<class T>
    void add_stat(unsigned int n, T& obj, unsigned int (T::*cb)())
    {
        add_stat(n, std::bind(cb, &obj));
    }
    virtual unsigned int get_stat(unsigned int n) { return (m_stat_map[n])(); };
    virtual std::string& get_common_stat(unsigned int n) { return m_common_stats[n]; };
private:
    int m_deviceTypeBeingMonitored;
    int m_init;
    std::string m_monitoring_agent_addr_port;
    std::string m_agentx_loc;
    boost::asio::deadline_timer m_main_loop_timer;
    char *m_argv[10];
    bool m_disable_snmp;

    // Common Stats
    std::map<unsigned int, std::string> m_common_stats;
    // Mixer, Turret and GW stats callbacks
    std::map<unsigned int, SnmpCallback> m_stat_map;
};

#endif // _IFS_AGENT_HPP_
