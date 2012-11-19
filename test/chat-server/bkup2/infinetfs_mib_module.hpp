#ifndef INFINETFS_MIB_MODULE_HPP
#define INFINETFS_MIB_MODULE_HPP

#include <iostream>
#include <string>
#include <map>
#include <set>
#include <list>
#include <typeinfo>
#include <boost/date_time.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_io.hpp> 

using namespace std;

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

enum StatsParameters {
    STATS_HOST_NAME = 1, 
    STATS_ATP_PORT, 
    STATS_ORG, 
    STATS_SITE, 
    STATS_CN, 
    STATS_DN
};

enum MixerStatsParameters {
    MIXER_STATS_NUM_MIXES=1,
    MIXER_STATS_MAX_MIXES
};

enum TurretStatsParameters {
    TURRET_STATS_HEARTBEAT_TIMER=1,
    TURRET_STATS_TOTAL_CALLS,
    TURRET_STATS_TOTAL_LHS_CALLS,
    TURRET_STATS_TOTAL_RHS_CALLS,
    TURRET_STATS_TOTAL_INCOMING_CALLS,
    TURRET_STATS_TOTAL_LHS_INCOMING_CALLS,
    TURRET_STATS_TOTAL_RHS_INCOMING_CALLS,
    TURRET_STATS_TOTAL_OUTGOING_CALLS,
    TURRET_STATS_TOTAL_LHS_OUTGOING_CALLS,
    TURRET_STATS_TOTAL_RHS_OUTGOING_CALLS,
    TURRET_STATS_TOTAL_LHS_INTERFACE_UP,
    TURRET_STATS_TOTAL_RHS_INTERFACE_UP
};

enum GatewayStatsParameters {
    GATEWAY_STATS_TOTAL_CALLS=1,
    GATEWAY_STATS_MAX_CALLS,
    GATEWAY_STATS_REJECTED_CALLS,
    GATEWAY_STATS_DROPPED_CALLS,
    GATEWAY_STATS_PARKED_DEVICE
};

#define GENERATE_STATS_PARAMETER_HANDLER(index,my_name_oid) {\
      oid name_oid[] = { 1, 3, 6, 1, 4, 1, 39282, 1, index };\
      netsnmp_register_scalar(\
            netsnmp_create_handler_registration(\
                "infinetfsStatsTable/my_name_oid", HandleStatsParameter,\
                name_oid, OID_LENGTH(name_oid),\
                HANDLER_CAN_RONLY));\
    }

#define GENERATE_MIXER_STATS_PARAMETER_HANDLER(index,my_name_oid) {\
      oid name_oid[] = { 1, 3, 6, 1, 4, 1, 39282, 2, index };\
      netsnmp_register_scalar(\
            netsnmp_create_handler_registration(\
                "infinetfsMixerStatsTable/my_name_oid", HandleMixerStatsParameter,\
                name_oid, OID_LENGTH(name_oid),\
                HANDLER_CAN_RONLY));\
    }


#define GENERATE_TURRET_STATS_PARAMETER_HANDLER(index,my_name_oid) {\
      oid name_oid[] = { 1, 3, 6, 1, 4, 1, 39282, 3, index };\
      netsnmp_register_scalar(\
            netsnmp_create_handler_registration(\
                "infinetfsTurretStatsTable/my_name_oid", HandleTurretStatsParameter,\
                name_oid, OID_LENGTH(name_oid),\
                HANDLER_CAN_RONLY));\
    }

#define GENERATE_GATEWAY_STATS_PARAMETER_HANDLER(index,my_name_oid) {\
      oid name_oid[] = { 1, 3, 6, 1, 4, 1, 39282, 4, index };\
      netsnmp_register_scalar(\
            netsnmp_create_handler_registration(\
                "infinetfsGatewayStatsTable/my_name_oid", HandleGatewayStatsParameter,\
                name_oid, OID_LENGTH(name_oid),\
                HANDLER_CAN_RONLY));\
    }

#define STAT_SET(NAME) \
    virtual void set_##NAME(string& s) { m_##NAME = s; };
#define STAT_GET(NAME) \
    virtual string& get_##NAME() { return m_##NAME; };

extern oid infinetfs_stats_oid[];
extern oid infinetfs_mixerstats_oid[] ;
extern oid infinetfs_turretstats_oid[];
extern oid infinetfs_gwstats_oid[];

Netsnmp_Node_Handler InfinetfsStatsHandler;
Netsnmp_Node_Handler HandleStatsParameter;
Netsnmp_Node_Handler HandleMixerStatsParameter;
Netsnmp_Node_Handler HandleTurretStatsParameter;
Netsnmp_Node_Handler HandleGatewayStatsParameter;

class Stats 
{
public:
    Stats() {};
    Stats(string host, string port, string org, string site, string CN, string DN, string load_factor);
    ~Stats() {};
    friend Netsnmp_Node_Handler InfinetfsStatsHandler;
    friend Netsnmp_Node_Handler HandleStatsParameter;
    virtual void InitStatsTable();

    STAT_SET(host_name);
    STAT_SET(atp_port);
    STAT_SET(org);
    STAT_SET(site);
    STAT_SET(CN);
    STAT_SET(DN);
    STAT_SET(load_factor);

    STAT_GET(host_name);
    STAT_GET(atp_port);
    STAT_GET(org);
    STAT_GET(site);
    STAT_GET(CN);
    STAT_GET(DN);
    STAT_GET(load_factor);

private:
    string m_host_name;
    string m_atp_port;
    string m_org;
    string m_site;
    string m_CN;
    string m_DN;
    string m_load_factor;
    netsnmp_table_data_set *infinetfs_stats_table_set;
    netsnmp_table_row *m_row;
};

typedef map<string, string> Connection;
typedef set<Connection> Connections;

class MixerStats: public Stats {

public:
    MixerStats() {};
    MixerStats(string host, string port, string org, string site, 
	string CN, string DN);
    ~MixerStats() {};
    friend Netsnmp_Node_Handler HandleMixerStatsParameter;
    virtual void InitStatsTable() ;

    STAT_SET(num_mixes);
    STAT_SET(max_mixes);

    STAT_GET(num_mixes);
    STAT_GET(max_mixes);

    void add_connection(string a, string b);
    void del_connection(string a, string b);
    Connections& get_connections_table() { return m_connections_table; };
private:
    Connections m_connections_table;
    string m_num_mixes;
    string m_max_mixes;
};

class TurretStats: public Stats {

public:
    TurretStats() {};
    TurretStats(string host, string port, string org, string site, string CN, string DN);
    ~TurretStats() {};
    friend Netsnmp_Node_Handler HandleTurretStatsParameter;
    virtual void InitStatsTable() ;
    STAT_SET(heartbeat_timer);
    STAT_SET(total_calls);
    STAT_SET(total_lhs_calls);
    STAT_SET(total_rhs_calls);
    STAT_SET(total_incoming_calls);
    STAT_SET(total_lhs_incoming_calls);
    STAT_SET(total_rhs_incoming_calls);
    STAT_SET(total_outgoing_calls);
    STAT_SET(total_lhs_outgoing_calls);
    STAT_SET(total_rhs_outgoing_calls);
    STAT_SET(lhs_interface_up);
    STAT_SET(rhs_interface_up);

    STAT_GET(heartbeat_timer);
    STAT_GET(total_calls);
    STAT_GET(total_lhs_calls);
    STAT_GET(total_rhs_calls);
    STAT_GET(total_incoming_calls);
    STAT_GET(total_lhs_incoming_calls);
    STAT_GET(total_rhs_incoming_calls);
    STAT_GET(total_outgoing_calls);
    STAT_GET(total_lhs_outgoing_calls);
    STAT_GET(total_rhs_outgoing_calls);
    STAT_GET(lhs_interface_up);
    STAT_GET(rhs_interface_up);
private:
    string m_heartbeat_timer;
    string m_total_calls;
    string m_total_lhs_calls;
    string m_total_rhs_calls;
    string m_total_incoming_calls;
    string m_total_lhs_incoming_calls;
    string m_total_rhs_incoming_calls;
    string m_total_outgoing_calls;
    string m_total_lhs_outgoing_calls;
    string m_total_rhs_outgoing_calls;
    string m_lhs_interface_up;
    string m_rhs_interface_up;
};

class GatewayStats: public Stats {

public:
    GatewayStats() {};
    GatewayStats(string host, string port, string org, string site, string CN, string DN);
    ~GatewayStats() {};
    friend Netsnmp_Node_Handler HandleGatewayStatsParameter;
    virtual void InitStatsTable() ;
    STAT_SET(total_gw_calls);
    STAT_SET(max_gw_calls);
    STAT_SET(rejected_gw_calls);
    STAT_SET(dropped_gw_calls);
    STAT_SET(parked_audio_device);

    STAT_GET(total_gw_calls);
    STAT_GET(max_gw_calls);
    STAT_GET(rejected_gw_calls);
    STAT_GET(dropped_gw_calls);
    STAT_GET(parked_audio_device);
private:
    string m_total_gw_calls;
    string m_max_gw_calls;
    string m_rejected_gw_calls;
    string m_dropped_gw_calls;
    string m_parked_audio_device;
};

typedef boost::posix_time::ptime TimeStamp;
typedef boost::tuple<MixerStats *, TurretStats *, GatewayStats *, TimeStamp> IfsMibSnapShot;
typedef boost::tuple<MixerStats *, TurretStats *, GatewayStats *> IfsMibStats;
typedef list<IfsMibSnapShot> IfsMibSnapShotList;

#if 0
class IMibRequests
{
public:
    /* Common Stats */
    virtual void set_host_name(string& host)=0;
    virtual string& get_host_name()=0;
    virtual void set_atp_port(string& atp_port)=0;
    virtual string& get_atp_port()=0;
    virtual void set_org(string& org)=0;
    virtual string& get_org()=0;
    virtual void set_site(string& site)=0;
    virtual string& get_site()=0;
    virtual void set_CN(string& CN)=0;
    virtual string& get_CN()=0;
    virtual void set_DN(string& DN)=0;
    virtual string& get_DN()=0;
    virtual void set_load_factor(string& load_factor)=0;
    virtual string& get_load_factor()=0;

    /* Mixer stats */
    virtual void set_num_mixes(string& n)=0;
    virtual string& get_num_mixes()=0;
    virtual void set_max_mixes(string& n)=0;
    virtual string& get_max_mixes()=0;
    virtual void add_connection(string& a, string& b)=0;
    virtual void del_connection(string& a, string& b)=0;
    virtual void set_connections_table(Connections& c)=0;
    virtual Connections& get_connections_table()=0;

    /* Turret Stats */
    virtual void set_heartbeat_timer(string& timer)=0;
    virtual string& get_heartbeat_timer()=0;
    virtual void set_total_turret_calls(string& s)=0;
    virtual string& get_total_turret_calls()=0;
    virtual void set_total_lhs_calls(string& s)=0;
    virtual string& get_total_lhs_calls()=0;
    virtual void set_total_rhs_calls(string& s)=0;
    virtual string& get_total_rhs_calls()=0;
    virtual void set_total_incoming_calls(string& s)=0;
    virtual string& get_total_incoming_calls()=0;
    virtual void set_total_lhs_incoming_calls(string& s)=0;
    virtual string& get_total_rhs_incoming_calls()=0;
    virtual void set_total_outgoing_calls(string& s)=0;
    virtual string& get_total_outgoing_calls()=0;
    virtual void set_total_lhs_outgoing_calls(string& s)=0;
    virtual string& get_total_lhs_outgoing_calls()=0;
    virtual void set_total_rhs_outgoing_calls(string& s)=0;
    virtual string& get_total_rhs_outgoing_calls()=0;
    virtual void set_lhs_interface_up(string& s)=0;
    virtual string& get_lhs_interface_up()=0;
    virtual void set_rhs_interface_up(string& s)=0;
    virtual string& get_rhs_interface_up()=0;

    /*Gateway Stats */
    virtual void set_total_gw_calls(string& s)=0;
    virtual string& get_total_gw_calls()=0;
    virtual void set_max_calls(string& s)=0;
    virtual string& get_max_calls()=0;
    virtual void set_rejected_calls(string& s)=0;
    virtual string& get_rejected_calls()=0;
    virtual void set_dropped_calls(string& s)=0;
    virtual string& get_dropped_calls()=0;
    virtual void set_parked_audio_device(string& s)=0;
    virtual string& get_parked_audio_device()=0;
protected:
    IMibRequests() {};
    ~IMibRequests() {};
};

#endif

#define MIB_COMMON_STATS_SET(param) \
    virtual void set_##param(string & s) \
    { \
        m_stats->set_##param(s); \
    };

#define MIB_COMMON_STATS_GET(param) \
    virtual string& get_##param() \
    { \
        m_stats->get_##param(); \
    };

#define MIB_MIXER_STATS_SET(param) \
    virtual void set_##param(string & s) \
    { \
        MixerStats *m = dynamic_cast<MixerStats *>(m_stats); \
        if(m)  m->set_##param(s); \
    };

#define MIB_MIXER_STATS_GET(param) \
    virtual string& get_##param() \
    { \
        MixerStats *mStats = dynamic_cast<MixerStats *>(m_stats); \
        if(mStats)  mStats->get_##param(); \
    };

#define MIB_TURRET_STATS_SET(param) \
    virtual void set_##param(string & s) \
    { \
        TurretStats *tStats = dynamic_cast<TurretStats *>(m_stats); \
        if(tStats)  tStats->set_##param(s); \
    };

#define MIB_TURRET_STATS_GET(param) \
    virtual string& get_##param() \
    { \
        TurretStats *tStats = dynamic_cast<TurretStats *>(m_stats); \
        if(tStats)  tStats->get_##param(); \
    };

#define MIB_GATEWAY_STATS_SET(param) \
    virtual void set_##param(string & s) \
    { \
        GatewayStats *gwStats = dynamic_cast<GatewayStats *>(m_stats); \
        if(gwStats)  gwStats->set_##param(s); \
    };

#define MIB_GATEWAY_STATS_GET(param) \
    virtual string& get_##param() \
    { \
        GatewayStats *gwStats = dynamic_cast<GatewayStats *>(m_stats); \
        if(gwStats)  gwStats->get_##param(); \
    };

template <class T>
class IfsMib:  public T  {
public:
    static IfsMib<T> * getInstance()
    {
        static IfsMib<T> * instance = new IfsMib<T> ;
        instance->InitStatsTable();
        return instance;
    }
    ~IfsMib() { delete m_stats; };
private:
    IfsMib() {};
    IfsMib(IfsMib const&);              // Singleton: Not Implemented
    void operator=(IfsMib const&);      // Singleton: Not Implemented
    virtual void InitStatsTable();

public:
#if 0
    IfsMib();
    ~IfsMib();
#endif

     /* Common Stats */
    MIB_COMMON_STATS_SET(host_name);
    MIB_COMMON_STATS_SET(atp_port);
    MIB_COMMON_STATS_SET(org);
    MIB_COMMON_STATS_SET(site);
    MIB_COMMON_STATS_SET(CN);
    MIB_COMMON_STATS_SET(DN);
    MIB_COMMON_STATS_SET(load_factor);

    MIB_COMMON_STATS_GET(host_name);
    MIB_COMMON_STATS_GET(atp_port);
    MIB_COMMON_STATS_GET(org);
    MIB_COMMON_STATS_GET(site);
    MIB_COMMON_STATS_GET(CN);
    MIB_COMMON_STATS_GET(DN);
    MIB_COMMON_STATS_GET(load_factor);

    /* Mixer Stats */
    MIB_MIXER_STATS_SET(num_mixes);
    MIB_MIXER_STATS_SET(max_mixes);

    MIB_MIXER_STATS_GET(num_mixes);
    MIB_MIXER_STATS_GET(max_mixes);

    /* Turret Stats */
    MIB_TURRET_STATS_SET(heartbeat_timer);
    MIB_TURRET_STATS_SET(total_calls);
    MIB_TURRET_STATS_SET(total_lhs_calls);
    MIB_TURRET_STATS_SET(total_rhs_calls);
    MIB_TURRET_STATS_SET(total_incoming_calls);
    MIB_TURRET_STATS_SET(total_lhs_incoming_calls);
    MIB_TURRET_STATS_SET(total_rhs_incoming_calls);
    MIB_TURRET_STATS_SET(total_outgoing_calls);
    MIB_TURRET_STATS_SET(total_lhs_outgoing_calls);
    MIB_TURRET_STATS_SET(total_rhs_outgoing_calls);
    MIB_TURRET_STATS_SET(lhs_interface_up);
    MIB_TURRET_STATS_SET(rhs_interface_up);

    MIB_TURRET_STATS_GET(heartbeat_timer);
    MIB_TURRET_STATS_GET(total_calls);
    MIB_TURRET_STATS_GET(total_lhs_calls);
    MIB_TURRET_STATS_GET(total_rhs_calls);
    MIB_TURRET_STATS_GET(total_incoming_calls);
    MIB_TURRET_STATS_GET(total_lhs_incoming_calls);
    MIB_TURRET_STATS_GET(total_rhs_incoming_calls);
    MIB_TURRET_STATS_GET(total_outgoing_calls);
    MIB_TURRET_STATS_GET(total_lhs_outgoing_calls);
    MIB_TURRET_STATS_GET(total_rhs_outgoing_calls);
    MIB_TURRET_STATS_GET(lhs_interface_up);
    MIB_TURRET_STATS_GET(rhs_interface_up);

    /* Gateway Stats */
    MIB_GATEWAY_STATS_SET(total_gw_calls);
    MIB_GATEWAY_STATS_SET(max_gw_calls);
    MIB_GATEWAY_STATS_SET(rejected_gw_calls);
    MIB_GATEWAY_STATS_SET(dropped_gw_calls);
    MIB_GATEWAY_STATS_SET(parked_audio_device);

    MIB_GATEWAY_STATS_GET(total_gw_calls);
    MIB_GATEWAY_STATS_GET(max_gw_calls);
    MIB_GATEWAY_STATS_GET(rejected_gw_calls);
    MIB_GATEWAY_STATS_GET(dropped_gw_calls);
    MIB_GATEWAY_STATS_GET(parked_audio_device);
private:
    T *m_stats;
};

#if 0
template <class T>
IfsMib<T>::IfsMib()
{
    m_stats = new T;

};

template <class T>
IfsMib<T>::~IfsMib()
{
    delete m_stats;
};
#endif

template <class T> void
IfsMib<T>::InitStatsTable()
{
    m_stats = new T;
    if(m_stats) m_stats->InitStatsTable();
};


#endif /* INFINETFS_MIB_MODULE_HPP */
