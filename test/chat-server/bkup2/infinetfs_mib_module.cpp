/*
 * Filename: infinet_mib_module.cpp
 * Description: ATP MIB Classes
 * Author: Ashish Lal <ashish.lal@infinetfs.com>
 * Company: Infinet Financial Systems Ltd.
 * Created: 2012-09-03
 * Version: 1.0
 * Compiler: g++
 *
 * Copyright 2012 by Infinet Financial Systems Ltd. All Rights Reserved.
 */
#include "infinetfs_mib_module.hpp"

oid infinetfs_stats_oid[] = { 1, 3, 6, 1, 4, 1, 39282, 1, 1 };
oid infinetfs_mixerstats_oid[] = { 1, 3, 6, 1, 4, 1, 39282, 2, 1 };
oid infinetfs_turretstats_oid[] = { 1, 3, 6, 1, 4, 1, 39282, 3, 1 };
oid infinetfs_gwstats_oid[] = { 1, 3, 6, 1, 4, 1, 39282, 4, 1 };
void *stats= NULL;
void *dStats = NULL;

Stats::Stats(string host, string port, string org, string site, string CN, string DN, string load_factor):
        m_host_name(host),
        m_atp_port(port),
        m_org(org),
        m_site(site),
        m_CN(CN),
        m_DN(DN),
        m_load_factor(load_factor) {};

int 
HandleStatsParameter(netsnmp_mib_handler *handler,
                   netsnmp_handler_registration *reginfo,
                   netsnmp_agent_request_info *reqinfo,
                   netsnmp_request_info *requests)
{

    int stats_parameter = requests->requestvb->name[requests->requestvb->name_length-2];
    char stats_parameter_value[1024];
    DEBUGMSGTL(("INFINETFS_MIB_MODULE", "Inside Handle stats parameter\n"));
    cout << "Inside handle stats parameter...." << endl;
    Stats *s = (dStats) ? (Stats *)dStats:(Stats *)stats;
    switch(stats_parameter) {
      case STATS_HOST_NAME:
        sprintf(stats_parameter_value, s->m_host_name.c_str(), sizeof(s->m_host_name.c_str()));
        break;
      case STATS_ATP_PORT:
        sprintf(stats_parameter_value, s->m_atp_port.c_str(), sizeof(s->m_atp_port.c_str()));
        break;
      case STATS_ORG:
        sprintf(stats_parameter_value, s->m_org.c_str(), sizeof(s->m_org.c_str()));
        break;
      case STATS_SITE:
        sprintf(stats_parameter_value, s->m_site.c_str(), sizeof(s->m_site.c_str()));
        break;
      case STATS_CN:
        sprintf(stats_parameter_value, s->m_CN.c_str(), sizeof(s->m_CN.c_str()));
        break;
      case STATS_DN:
        sprintf(stats_parameter_value, s->m_DN.c_str(), sizeof(s->m_DN.c_str()));
        break;
      default:
        sprintf(stats_parameter_value, "-1", sizeof("-1"));
        break;
    }
    DEBUGMSGTL(("INFINETFS_MIB_MODULE","Stats Parameter %d: %s\n", stats_parameter, stats_parameter_value));
    cout << "stats param " << stats_parameter << " --- " << stats_parameter_value << endl;
    snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR, (const u_char *)stats_parameter_value, (size_t)strlen(stats_parameter_value));
    return SNMP_ERR_NOERROR;
}

void Stats::InitStatsTable()
{
    DEBUGMSGTL(("INFINETFS_MIB_MODULE", "Initalizing Stats Module\n"));
    cout << "Initalizing Stats Module --- 0" << endl;
    if (!dStats) {
        stats = (void *)this;
    }
    GENERATE_STATS_PARAMETER_HANDLER(1, statsHostName);
    GENERATE_STATS_PARAMETER_HANDLER(2, statsAtpPort);
    GENERATE_STATS_PARAMETER_HANDLER(3, statsOrg);
    GENERATE_STATS_PARAMETER_HANDLER(4, statsSite);
    GENERATE_STATS_PARAMETER_HANDLER(5, statsCN);
    GENERATE_STATS_PARAMETER_HANDLER(6, statsDN);
   
    return;
}


int InfinetfsStatsHandler(netsnmp_mib_handler *handler,
                       netsnmp_handler_registration *reginfo,
                       netsnmp_agent_request_info *reqinfo,
                       netsnmp_request_info *requests) 
{
    unsigned char infinetfs_stats_string[1024] = "", tempstring[100];
    int dialstring;
    netsnmp_table_request_info *table_info;
    netsnmp_request_info *request;
    DEBUGMSGTL(("INFINETFS_MIB_MODULE", "Inside InfinetfsStatsHandler\n"));
    cout << "Inside InfinetfsStatsHandler...." << endl;
    switch (reqinfo->mode) {
      case MODE_GET:
      case MODE_GETNEXT:
          table_info = netsnmp_extract_table_info(requests);
          sprintf((char *)infinetfs_stats_string, "%s: 2, 3, 4, 5;%s: 2, 3, 4, 5", table_info->indexes->val.string, table_info->indexes->val.string);
          snmp_set_var_typed_value(requests->requestvb,
                                 ASN_OCTET_STR,
                                 (u_char *)&infinetfs_stats_string,
                                 strlen((char *)infinetfs_stats_string));
        break;
    }
    return SNMP_ERR_NOERROR;
}

MixerStats::MixerStats(string host, string port, string org, string site, string CN, string DN):
        Stats(host, port, org, site, CN, DN, ""),
        m_num_mixes(""),
        m_max_mixes("") {};

int 
HandleMixerStatsParameter(netsnmp_mib_handler *handler,
                   netsnmp_handler_registration *reginfo,
                   netsnmp_agent_request_info *reqinfo,
                   netsnmp_request_info *requests)
{

    int stats_parameter = requests->requestvb->name[requests->requestvb->name_length-2];
    char stats_parameter_value[1024];
    DEBUGMSGTL(("INFINETFS_MIB_MODULE", "Inside Handle mixer stats parameter\n"));
    cout << "Inside handle mixer stats parameter...." << endl;
    if(!dStats) {
        return SNMPERR_GENERR;
    }
    MixerStats *s = (MixerStats *)dStats;
    switch(stats_parameter) {
      case MIXER_STATS_NUM_MIXES:
        sprintf(stats_parameter_value, s->m_num_mixes.c_str(), sizeof(s->m_num_mixes.c_str()));
        break;
      case MIXER_STATS_MAX_MIXES:
        sprintf(stats_parameter_value, s->m_max_mixes.c_str(), sizeof(s->m_max_mixes.c_str()));
        break;
      default:
        sprintf(stats_parameter_value, "-1", sizeof("-1"));
        break;
    }
    DEBUGMSGTL(("INFINETFS_MIB_MODULE","Mixer Stats Parameter %d: %s\n", stats_parameter, stats_parameter_value));
    cout << "mixer stats param " << stats_parameter << " --- " << stats_parameter_value << endl;
    snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR, (const u_char *)stats_parameter_value, (size_t)strlen(stats_parameter_value));
    return SNMP_ERR_NOERROR;
}

void MixerStats::InitStatsTable()
{
    DEBUGMSGTL(("INFINETFS_MIB_MODULE", "Initalizing Mixer Stats Module\n"));
    cout << "Initalizing Mixer Stats Module --- " << endl;
    dStats = (void *)this;
    Stats::InitStatsTable();
    GENERATE_MIXER_STATS_PARAMETER_HANDLER(1, mixerStatsNumMixes);
    GENERATE_MIXER_STATS_PARAMETER_HANDLER(2, mixerStatsMaxMixes);
   
    return;
}

TurretStats::TurretStats(string host, string port, string org, string site, string CN, string DN):
        Stats(host, port, org, site, CN, DN, ""),
        m_heartbeat_timer(""),
        m_total_calls("") ,
        m_total_lhs_calls("") ,
        m_total_rhs_calls("") ,
        m_total_incoming_calls("") ,
        m_total_lhs_incoming_calls("") ,
        m_total_rhs_incoming_calls("") ,
        m_total_outgoing_calls("") ,
        m_total_lhs_outgoing_calls("") ,
        m_total_rhs_outgoing_calls("") ,
        m_lhs_interface_up("") ,
        m_rhs_interface_up("") 
{
};

int
HandleTurretStatsParameter(netsnmp_mib_handler *handler,
                   netsnmp_handler_registration *reginfo,
                   netsnmp_agent_request_info *reqinfo,
                   netsnmp_request_info *requests)
{

    int stats_parameter = requests->requestvb->name[requests->requestvb->name_length-2];
    char stats_parameter_value[1024];
    DEBUGMSGTL(("INFINETFS_MIB_MODULE", "Inside Handle mixer stats parameter\n"));
    cout << "Inside handle mixer stats parameter...." << endl;
    if(!dStats) {
        return SNMPERR_GENERR;
    }
    TurretStats *s = (TurretStats *)dStats;
    switch(stats_parameter) {
      case TURRET_STATS_HEARTBEAT_TIMER:
        sprintf(stats_parameter_value, s->m_heartbeat_timer.c_str(), sizeof(s->m_heartbeat_timer.c_str()));
        break;
      case TURRET_STATS_TOTAL_CALLS:
        sprintf(stats_parameter_value, s->m_total_calls.c_str(), sizeof(s->m_total_calls.c_str()));
        break;
      case TURRET_STATS_TOTAL_LHS_CALLS:
        sprintf(stats_parameter_value, s->m_total_lhs_calls.c_str(), sizeof(s->m_total_lhs_calls.c_str()));
        break;
      case TURRET_STATS_TOTAL_RHS_CALLS:
        sprintf(stats_parameter_value, s->m_total_rhs_calls.c_str(), sizeof(s->m_total_rhs_calls.c_str()));
        break;
      case TURRET_STATS_TOTAL_INCOMING_CALLS:
        sprintf(stats_parameter_value, s->m_total_incoming_calls.c_str(), sizeof(s->m_total_incoming_calls.c_str()));
        break;
      case TURRET_STATS_TOTAL_LHS_INCOMING_CALLS:
        sprintf(stats_parameter_value, s->m_total_lhs_incoming_calls.c_str(), sizeof(s->m_total_lhs_incoming_calls.c_str()));
        break;
      case TURRET_STATS_TOTAL_RHS_INCOMING_CALLS:
        sprintf(stats_parameter_value, s->m_total_rhs_incoming_calls.c_str(), sizeof(s->m_total_rhs_incoming_calls.c_str()));
        break;
      case TURRET_STATS_TOTAL_OUTGOING_CALLS:
        sprintf(stats_parameter_value, s->m_total_outgoing_calls.c_str(), sizeof(s->m_total_outgoing_calls.c_str()));
        break;
      case TURRET_STATS_TOTAL_LHS_OUTGOING_CALLS:
        sprintf(stats_parameter_value, s->m_total_lhs_outgoing_calls.c_str(), sizeof(s->m_total_lhs_outgoing_calls.c_str()));
        break;
      case TURRET_STATS_TOTAL_RHS_OUTGOING_CALLS:
        sprintf(stats_parameter_value, s->m_total_rhs_outgoing_calls.c_str(), sizeof(s->m_total_rhs_outgoing_calls.c_str()));
        break;
      case TURRET_STATS_TOTAL_LHS_INTERFACE_UP:
        sprintf(stats_parameter_value, s->m_lhs_interface_up.c_str(), sizeof(s->m_lhs_interface_up.c_str()));
        break;
      case TURRET_STATS_TOTAL_RHS_INTERFACE_UP:
        sprintf(stats_parameter_value, s->m_rhs_interface_up.c_str(), sizeof(s->m_rhs_interface_up.c_str()));
        break;
      default:
        sprintf(stats_parameter_value, "-1", sizeof("-1"));
        break;
    }
    DEBUGMSGTL(("INFINETFS_MIB_MODULE","Turret Stats Parameter %d: %s\n", stats_parameter, stats_parameter_value));
    cout << "turret stats param " << stats_parameter << " --- " << stats_parameter_value << endl;
    snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR, (const u_char *)stats_parameter_value, (size_t)strlen(stats_parameter_value));
    return SNMP_ERR_NOERROR;
}

void TurretStats::InitStatsTable()
{
    DEBUGMSGTL(("INFINETFS_MIB_MODULE", "Initalizing Turret Stats Module\n"));
    cout << "Initalizing Turret Stats Module --- " << endl;
    dStats = (void *)this;
    Stats::InitStatsTable();
    GENERATE_TURRET_STATS_PARAMETER_HANDLER(1, turretStatsHeartbeatTimer);
    GENERATE_TURRET_STATS_PARAMETER_HANDLER(2, turretStatsTotalCalls);
    GENERATE_TURRET_STATS_PARAMETER_HANDLER(3, turretStatsTotalLHSCalls);
    GENERATE_TURRET_STATS_PARAMETER_HANDLER(4, turretStatsTotalRHSCalls);
    GENERATE_TURRET_STATS_PARAMETER_HANDLER(5, turretStatsTotalIncomingCalls);
    GENERATE_TURRET_STATS_PARAMETER_HANDLER(6, turretStatsTotalLHSIncomingCalls);
    GENERATE_TURRET_STATS_PARAMETER_HANDLER(7, turretStatsTotalRHSIncomingCalls);
    GENERATE_TURRET_STATS_PARAMETER_HANDLER(8, turretStatsTotalOutgoingCalls);
    GENERATE_TURRET_STATS_PARAMETER_HANDLER(9, turretStatsTotalLHSOutgoingCalls);
    GENERATE_TURRET_STATS_PARAMETER_HANDLER(10, turretStatsTotalRHSOutgoingCalls);
    GENERATE_TURRET_STATS_PARAMETER_HANDLER(11, turretStatsLHSInterfaceUp);
    GENERATE_TURRET_STATS_PARAMETER_HANDLER(12, turretStatsRHSInterfaceUp);
   
    return;
}


GatewayStats::GatewayStats(string host, string port, string org, string site, string CN, string DN):
        Stats(host, port, org, site, CN, DN, ""),
        m_total_gw_calls(""),
        m_max_gw_calls(""),
        m_rejected_gw_calls(""),
        m_dropped_gw_calls(""),
        m_parked_audio_device("")
{
};

int 
HandleGatewayStatsParameter(netsnmp_mib_handler *handler,
                   netsnmp_handler_registration *reginfo,
                   netsnmp_agent_request_info *reqinfo,
                   netsnmp_request_info *requests)
{

    int stats_parameter = requests->requestvb->name[requests->requestvb->name_length-2];
    char stats_parameter_value[1024];
    DEBUGMSGTL(("INFINETFS_MIB_MODULE", "Inside Handle gateway stats parameter\n"));
    cout << "Inside handle gateway stats parameter...." << endl;
    if(!dStats) {
        return SNMPERR_GENERR;
    }
    GatewayStats *s = (GatewayStats *)dStats;
    switch(stats_parameter) {
      case GATEWAY_STATS_TOTAL_CALLS:
        sprintf(stats_parameter_value, s->m_total_gw_calls.c_str(), sizeof(s->m_total_gw_calls.c_str()));
        break;
      case GATEWAY_STATS_MAX_CALLS:
        sprintf(stats_parameter_value, s->m_max_gw_calls.c_str(), sizeof(s->m_max_gw_calls.c_str()));
        break;
      case GATEWAY_STATS_REJECTED_CALLS:
        sprintf(stats_parameter_value, s->m_rejected_gw_calls.c_str(), sizeof(s->m_rejected_gw_calls.c_str()));
        break;
      case GATEWAY_STATS_DROPPED_CALLS:
        sprintf(stats_parameter_value, s->m_dropped_gw_calls.c_str(), sizeof(s->m_dropped_gw_calls.c_str()));
        break;
      case GATEWAY_STATS_PARKED_DEVICE:
        sprintf(stats_parameter_value, s->m_parked_audio_device.c_str(), sizeof(s->m_parked_audio_device.c_str()));
        break;
      default:
        sprintf(stats_parameter_value, "-1", sizeof("-1"));
        break;
    }
    DEBUGMSGTL(("INFINETFS_MIB_MODULE","Mixer Stats Parameter %d: %s\n", stats_parameter, stats_parameter_value));
    cout << "gateway stats param " << stats_parameter << " --- " << stats_parameter_value << endl;
    snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR, (const u_char *)stats_parameter_value, (size_t)strlen(stats_parameter_value));
    return SNMP_ERR_NOERROR;
}

void GatewayStats::InitStatsTable()
{
    DEBUGMSGTL(("INFINETFS_MIB_MODULE", "Initalizing Gateway Stats Module\n"));
    cout << "Initalizing Gateway Stats Module --- " << endl;
    dStats = (void *)this;
    Stats::InitStatsTable();
    GENERATE_GATEWAY_STATS_PARAMETER_HANDLER(1, gwStatsTotalCalls);
    GENERATE_GATEWAY_STATS_PARAMETER_HANDLER(2, gwStatsMaxCalls);
    GENERATE_GATEWAY_STATS_PARAMETER_HANDLER(3, gwStatsRejectedCalls);
    GENERATE_GATEWAY_STATS_PARAMETER_HANDLER(4, gwStatsDroppedCalls);
    GENERATE_GATEWAY_STATS_PARAMETER_HANDLER(5, gwStatsParkedDevice);
   
    return;
}

