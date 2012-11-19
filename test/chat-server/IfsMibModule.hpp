/*
 * Filename: IfsMibModule.hpp
 * Description: MIB Classes
 * Author: Ashish Lal <ashish.lal@infinetfs.com>
 * Company: Infinet Financial Systems Ltd.
 * Created: 2012-09-05
 * Version: 1.0
 * Compiler: g++
 *
 * Copyright 2012 by Infinet Financial Systems Ltd. All Rights Reserved.
 */

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
    STATS_DN,
    STATS_LOAD_FACTOR
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
    TURRET_STATS_TOTAL_RHS_INTERFACE_UP,
    TURRET_STATS_TOTAL_REJECTED_CALLS,
    TURRET_STATS_TOTAL_DROPPED_CALLS
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

//Netsnmp_Node_Handler InfinetfsStatsHandler;
//Netsnmp_Node_Handler HandleStatsParameter;
//Netsnmp_Node_Handler HandleMixerStatsParameter;
//Netsnmp_Node_Handler HandleTurretStatsParameter;
//Netsnmp_Node_Handler HandleGatewayStatsParameter;

int InfinetfsStatsHandler(netsnmp_mib_handler *handler,
                   netsnmp_handler_registration *reginfo,
                   netsnmp_agent_request_info *reqinfo,
                   netsnmp_request_info *requests);

int HandleStatsParameter(netsnmp_mib_handler *handler,
                   netsnmp_handler_registration *reginfo,
                   netsnmp_agent_request_info *reqinfo,
                   netsnmp_request_info *requests);

int HandleTurretStatsParameter(netsnmp_mib_handler *handler,
                   netsnmp_handler_registration *reginfo,
                   netsnmp_agent_request_info *reqinfo,
                   netsnmp_request_info *requests);

int HandleMixerStatsParameter(netsnmp_mib_handler *handler,
                   netsnmp_handler_registration *reginfo,
                   netsnmp_agent_request_info *reqinfo,
                   netsnmp_request_info *requests);

int HandleGatewayStatsParameter(netsnmp_mib_handler *handler,
                   netsnmp_handler_registration *reginfo,
                   netsnmp_agent_request_info *reqinfo,
                   netsnmp_request_info *requests);

// Stats common to Mixer, Turret and Gateway
class CStats 
{
public:
    CStats() {};
    CStats(string host, string port, string org, string site, string CN, string DN, string load_factor);
    ~CStats() {};
    //friend Netsnmp_Node_Handler InfinetfsStatsHandler;
    //friend Netsnmp_Node_Handler HandleStatsParameter;
    friend int HandleStatsParameter(netsnmp_mib_handler *handler,
                   netsnmp_handler_registration *reginfo,
                   netsnmp_agent_request_info *reqinfo,
                   netsnmp_request_info *requests);
    friend int InfinetfsStatsHandler(netsnmp_mib_handler *handler,
                   netsnmp_handler_registration *reginfo,
                   netsnmp_agent_request_info *reqinfo,
                   netsnmp_request_info *requests);
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

class MixerStats: public CStats {

public:
    MixerStats() {};
    MixerStats(string host, string port, string org, string site, 
	string CN, string DN);
    ~MixerStats() {};
    //friend Netsnmp_Node_Handler HandleMixerStatsParameter;
    friend int HandleMixerStatsParameter(netsnmp_mib_handler *handler,
                   netsnmp_handler_registration *reginfo,
                   netsnmp_agent_request_info *reqinfo,
                   netsnmp_request_info *requests);
    virtual void InitStatsTable() ;

    STAT_SET(num_mixes);
    STAT_SET(max_mixes);

    STAT_GET(num_mixes);
    STAT_GET(max_mixes);

private:
    string m_num_mixes;
    string m_max_mixes;
};

class TurretStats: public CStats {

public:
    TurretStats() {};
    TurretStats(string host, string port, string org, string site, string CN, string DN);
    ~TurretStats() {};
   // friend Netsnmp_Node_Handler HandleTurretStatsParameter;
    friend int HandleTurretStatsParameter(netsnmp_mib_handler *handler,
                   netsnmp_handler_registration *reginfo,
                   netsnmp_agent_request_info *reqinfo,
                   netsnmp_request_info *requests);
    virtual void InitStatsTable() ;
    STAT_SET(heartbeat_timer);
    STAT_SET(total_turret_calls);
    STAT_SET(total_turret_lhs_calls);
    STAT_SET(total_turret_rhs_calls);
    STAT_SET(total_turret_incoming_calls);
    STAT_SET(total_turret_lhs_incoming_calls);
    STAT_SET(total_turret_rhs_incoming_calls);
    STAT_SET(total_turret_outgoing_calls);
    STAT_SET(total_turret_lhs_outgoing_calls);
    STAT_SET(total_turret_rhs_outgoing_calls);
    STAT_SET(lhs_interface_up);
    STAT_SET(rhs_interface_up);
    STAT_SET(total_turret_rejected_calls);
    STAT_SET(total_turret_dropped_calls);

    STAT_GET(heartbeat_timer);
    STAT_GET(total_turret_calls);
    STAT_GET(total_turret_lhs_calls);
    STAT_GET(total_turret_rhs_calls);
    STAT_GET(total_turret_incoming_calls);
    STAT_GET(total_turret_lhs_incoming_calls);
    STAT_GET(total_turret_rhs_incoming_calls);
    STAT_GET(total_turret_outgoing_calls);
    STAT_GET(total_turret_lhs_outgoing_calls);
    STAT_GET(total_turret_rhs_outgoing_calls);
    STAT_GET(lhs_interface_up);
    STAT_GET(rhs_interface_up);
    STAT_GET(total_turret_rejected_calls);
    STAT_GET(total_turret_dropped_calls);
private:
    string m_heartbeat_timer;
    string m_total_turret_calls;
    string m_total_turret_lhs_calls;
    string m_total_turret_rhs_calls;
    string m_total_turret_incoming_calls;
    string m_total_turret_lhs_incoming_calls;
    string m_total_turret_rhs_incoming_calls;
    string m_total_turret_outgoing_calls;
    string m_total_turret_lhs_outgoing_calls;
    string m_total_turret_rhs_outgoing_calls;
    string m_lhs_interface_up;
    string m_rhs_interface_up;
    string m_total_turret_rejected_calls;
    string m_total_turret_dropped_calls;
};

class GatewayStats: public CStats {

public:
    GatewayStats() {};
    GatewayStats(string host, string port, string org, string site, string CN, string DN);
    ~GatewayStats() {};
    //friend Netsnmp_Node_Handler HandleGatewayStatsParameter;
    friend int HandleGatewayStatsParameter(netsnmp_mib_handler *handler,
                   netsnmp_handler_registration *reginfo,
                   netsnmp_agent_request_info *reqinfo,
                   netsnmp_request_info *requests);
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

#define MIB_COMMON_STATS_SET(param) \
    virtual void set_##param(string & s) \
    { \
        m_stats->set_##param(s); \
    };

#define MIB_COMMON_STATS_GET(param) \
    virtual string& get_##param() \
    { \
        return m_stats->get_##param(); \
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
        if(mStats)  return mStats->get_##param(); \
	return m_ret; \
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
        if(tStats)  return tStats->get_##param(); \
	return m_ret; \
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
        if(gwStats)  return gwStats->get_##param(); \
	return m_ret; \
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
    MIB_TURRET_STATS_SET(total_turret_calls);
    MIB_TURRET_STATS_SET(total_turret_lhs_calls);
    MIB_TURRET_STATS_SET(total_turret_rhs_calls);
    MIB_TURRET_STATS_SET(total_turret_incoming_calls);
    MIB_TURRET_STATS_SET(total_turret_lhs_incoming_calls);
    MIB_TURRET_STATS_SET(total_turret_rhs_incoming_calls);
    MIB_TURRET_STATS_SET(total_turret_outgoing_calls);
    MIB_TURRET_STATS_SET(total_turret_lhs_outgoing_calls);
    MIB_TURRET_STATS_SET(total_turret_rhs_outgoing_calls);
    MIB_TURRET_STATS_SET(lhs_interface_up);
    MIB_TURRET_STATS_SET(rhs_interface_up);
    MIB_TURRET_STATS_SET(total_turret_rejected_calls);
    MIB_TURRET_STATS_SET(total_turret_dropped_calls);
    
    MIB_TURRET_STATS_GET(heartbeat_timer);
    MIB_TURRET_STATS_GET(total_turret_calls);
    MIB_TURRET_STATS_GET(total_turret_lhs_calls);
    MIB_TURRET_STATS_GET(total_turret_rhs_calls);
    MIB_TURRET_STATS_GET(total_turret_incoming_calls);
    MIB_TURRET_STATS_GET(total_turret_lhs_incoming_calls);
    MIB_TURRET_STATS_GET(total_turret_rhs_incoming_calls);
    MIB_TURRET_STATS_GET(total_turret_outgoing_calls);
    MIB_TURRET_STATS_GET(total_turret_lhs_outgoing_calls);
    MIB_TURRET_STATS_GET(total_turret_rhs_outgoing_calls);
    MIB_TURRET_STATS_GET(lhs_interface_up);
    MIB_TURRET_STATS_GET(rhs_interface_up);
    MIB_TURRET_STATS_GET(total_turret_rejected_calls);
    MIB_TURRET_STATS_GET(total_turret_dropped_calls);
    
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
    string m_ret;
};

template <class T> void
IfsMib<T>::InitStatsTable()
{
    m_ret = "";
    m_stats = new T;
    if(m_stats) m_stats->InitStatsTable();
};


#endif /* INFINETFS_MIB_MODULE_HPP */
