/*
 * Filename: infinet_mib_module.cpp
 * Description: ATP MIB Classes
 * Author: Ashish Lal <ashish.lal@infinetfs.com>
 * Company: Infinet Financial Systems Ltd.
 * Created: 2012-09-05
 * Version: 1.0
 * Compiler: g++
 *
 * Copyright 2012 by Infinet Financial Systems Ltd. All Rights Reserved.
 */
#include "IfsMibModule.hpp"
#include "../Log/AtpLog.hpp"

void *stats= NULL;
void *dStats = NULL;

CStats::CStats():
    m_host_name(string("")),
    m_atp_port(string("")),
    m_org(string("")),
    m_site(string("")),
    m_CN(string("")),
    m_load_factor(string("")),
    m_mib_polling(true)
{};

CStats::CStats(string host, string port, string org, string site, string CN, string load_factor):
    m_host_name(host),
    m_atp_port(port),
    m_org(org),
    m_site(site),
    m_CN(CN),
    m_load_factor(load_factor),
    m_mib_polling(true)
{};

int 
HandleStatsParameter(netsnmp_mib_handler *handler,
                   netsnmp_handler_registration *reginfo,
                   netsnmp_agent_request_info *reqinfo,
                   netsnmp_request_info *requests)
{

    oid stats_parameter = requests->requestvb->name[requests->requestvb->name_length-2];
    char stats_parameter_value[1024];
    string stats_default("-1");

    CStats *s = (dStats) ? (CStats *)dStats:(CStats *)stats;
    if(!(s->m_mib_polling)) {
        return SNMPERR_GENERR;
    }
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
      case STATS_LOAD_FACTOR:
          sprintf(stats_parameter_value, s->m_load_factor.c_str(), sizeof(s->m_load_factor.c_str()));
          break;
      default:
          sprintf(stats_parameter_value, stats_default.c_str(), (size_t)(stats_default.c_str()));
          break;
    }
    snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR, (const u_char *)stats_parameter_value, (size_t)strlen(stats_parameter_value));
    return SNMP_ERR_NOERROR;
}

void CStats::InitStatsTable()
{
    if (!dStats) {
        stats = (void *)this;
    }
    ATP_LOG_INFO("IfsMibModule", "stats CN: " << get_CN());
    GENERATE_STATS_PARAMETER_HANDLER(1, "statsHostName");
    GENERATE_STATS_PARAMETER_HANDLER(2, "statsAtpPort");
    GENERATE_STATS_PARAMETER_HANDLER(3, "statsOrg");
    GENERATE_STATS_PARAMETER_HANDLER(4, "statsSite");
    GENERATE_STATS_PARAMETER_HANDLER(5, "statsCN");

    return;
}

int InfinetfsStatsHandler(netsnmp_mib_handler *handler,
                       netsnmp_handler_registration *reginfo,
                       netsnmp_agent_request_info *reqinfo,
                       netsnmp_request_info *requests) 
{
    unsigned char infinetfs_stats_string[1024] = "";
    netsnmp_table_request_info *table_info;

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

MixerStats::MixerStats():
    CStats(),
    m_num_mixes(0),
    m_max_mixes(0) {};

MixerStats::MixerStats(string host, string port, string org, string site, string CN):
    CStats(host, port, org, site, CN, ""),
    m_num_mixes(0),
    m_max_mixes(0) {};

int 
HandleMixerStatsParameter(netsnmp_mib_handler *handler,
                   netsnmp_handler_registration *reginfo,
                   netsnmp_agent_request_info *reqinfo,
                   netsnmp_request_info *requests)
{

    oid stats_parameter = requests->requestvb->name[requests->requestvb->name_length-2];
    unsigned int stats_parameter_value=0;

    if(!dStats) {
        return SNMPERR_GENERR;
    }
    MixerStats *s = (MixerStats *)dStats;
    if(!(s->get_mib_polling())) {
        return SNMPERR_GENERR;
    }
    switch(stats_parameter) {
      case MIXER_STATS_NUM_MIXES:
          stats_parameter_value = s->m_num_mixes;
          break;
      case MIXER_STATS_MAX_MIXES:
          stats_parameter_value = s->m_max_mixes;
          break;
      default:
          break;
    }
    snmp_set_var_typed_value(requests->requestvb, ASN_COUNTER,
                             (const u_char *)&stats_parameter_value,
                             sizeof(stats_parameter_value));
    return SNMP_ERR_NOERROR;
}

void MixerStats::InitStatsTable()
{
    dStats = (void *)this;
    CStats::InitStatsTable();
    GENERATE_MIXER_STATS_PARAMETER_HANDLER(1, "mixerStatsNumMixes");
    GENERATE_MIXER_STATS_PARAMETER_HANDLER(2, "mixerStatsMaxMixes");

    return;
}

TurretStats::TurretStats():
    CStats(),
    m_total_turret_calls(0) ,
    m_total_turret_rejected_calls(0),
    m_total_turret_dropped_calls(0),
    m_turret_current_calls(0),
    m_turret_average_call_duration(0),
    m_turret_longest_call_duration(0)
{
};

TurretStats::TurretStats(string host, string port, string org, string site, string CN):
    CStats(host, port, org, site, CN, ""),
    m_total_turret_calls(0) ,
    m_total_turret_rejected_calls(0),
    m_total_turret_dropped_calls(0),
    m_turret_current_calls(0),
    m_turret_average_call_duration(0),
    m_turret_longest_call_duration(0)
{
};

int
HandleTurretStatsParameter(netsnmp_mib_handler *handler,
                   netsnmp_handler_registration *reginfo,
                   netsnmp_agent_request_info *reqinfo,
                   netsnmp_request_info *requests)
{

    oid stats_parameter = requests->requestvb->name[requests->requestvb->name_length-2];
    unsigned int stats_parameter_value = 0;

    if(!dStats) {
        return SNMPERR_GENERR;
    }
    TurretStats *s = (TurretStats *)dStats;
    if(!(s->get_mib_polling())) {
        return SNMPERR_GENERR;
    }
    
    switch(stats_parameter) {
      case TURRET_STATS_TOTAL_CALLS:
          stats_parameter_value = s->m_total_turret_calls;
          break;
      case TURRET_STATS_TOTAL_REJECTED_CALLS:
          stats_parameter_value = s->m_total_turret_rejected_calls;
          break;
      case TURRET_STATS_TOTAL_DROPPED_CALLS:
          stats_parameter_value = s->m_total_turret_dropped_calls;
          break;
      case TURRET_STATS_CURRENT_CALLS:
          ATP_LOG_INFO("IfsAgent", "-----Inside HandleTurretStatsParameter----");
          stats_parameter_value = s->m_turret_current_calls;
          break;
      case TURRET_STATS_AVERAGE_CALL_DURATION:
          stats_parameter_value = s->m_turret_average_call_duration;
          break;
      case TURRET_STATS_LONGEST_CALL_DURATION:
          stats_parameter_value = s->m_turret_longest_call_duration;
          break;
      default:
          break;
    }

    snmp_set_var_typed_value(requests->requestvb, ASN_COUNTER,
                             (const u_char *)&stats_parameter_value,
                             sizeof(stats_parameter_value));
    return SNMP_ERR_NOERROR;
}

void TurretStats::InitStatsTable()
{
    dStats = (void *)this;
    CStats::InitStatsTable();
    GENERATE_TURRET_STATS_PARAMETER_HANDLER(1, "turretStatsTotalCalls");
    GENERATE_TURRET_STATS_PARAMETER_HANDLER(2, "turretStatsTotalRejectedCalls");
    GENERATE_TURRET_STATS_PARAMETER_HANDLER(3, "turretStatsTotalDroppedCalls");
    GENERATE_TURRET_STATS_PARAMETER_HANDLER(4, "turretStatsCurrentCalls");
    GENERATE_TURRET_STATS_PARAMETER_HANDLER(5, "turretStatsAverageCallDuration");
    GENERATE_TURRET_STATS_PARAMETER_HANDLER(6, "turretStatsLongestCallDuration");
    return;
}

GatewayStats::GatewayStats():
    CStats(),
    m_total_gw_calls(0),
    m_current_gw_calls(0),
    m_max_gw_calls(0),
    m_total_gw_incoming_calls(0),
    m_total_gw_outgoing_calls(0),
    m_rejected_gw_calls(0),
    m_dropped_gw_calls(0),
    m_gw_average_call_duration(0),
    m_gw_longest_call_duration(0)
{
};

GatewayStats::GatewayStats(string host, string port, string org, string site, string CN):
    CStats(host, port, org, site, CN, ""),
    m_total_gw_calls(0),
    m_current_gw_calls(0),
    m_max_gw_calls(0),
    m_total_gw_incoming_calls(0),
    m_total_gw_outgoing_calls(0),
    m_rejected_gw_calls(0),
    m_dropped_gw_calls(0),
    m_gw_average_call_duration(0),
    m_gw_longest_call_duration(0)
{
};

int 
HandleGatewayStatsParameter(netsnmp_mib_handler *handler,
                   netsnmp_handler_registration *reginfo,
                   netsnmp_agent_request_info *reqinfo,
                   netsnmp_request_info *requests)
{

    oid stats_parameter = requests->requestvb->name[requests->requestvb->name_length-2];
    unsigned int stats_parameter_value = 0;

    if(!dStats) {
        return SNMPERR_GENERR;
    }
    GatewayStats *s = (GatewayStats *)dStats;
    if(!(s->get_mib_polling())) {
        ATP_LOG_INFO("IfsMibModule", "Mib polling: " << s->get_mib_polling());
        return SNMPERR_GENERR;
    }
    switch(stats_parameter) {
      case GATEWAY_STATS_TOTAL_CALLS:
          stats_parameter_value = s->m_total_gw_calls;
          break;
      case GATEWAY_STATS_CURRENT_CALLS:
          stats_parameter_value = s->m_current_gw_calls;
          break;
      case GATEWAY_STATS_MAX_CALLS:
          stats_parameter_value = s->m_max_gw_calls;
          break;
      case GATEWAY_STATS_TOTAL_OUTGOING_CALLS:
          stats_parameter_value = s->m_total_gw_outgoing_calls;
          break;
      case GATEWAY_STATS_TOTAL_INCOMING_CALLS:
          stats_parameter_value = s->m_total_gw_incoming_calls;
          break;
      case GATEWAY_STATS_REJECTED_CALLS:
          stats_parameter_value = s->m_rejected_gw_calls;
          break;
      case GATEWAY_STATS_DROPPED_CALLS:
          stats_parameter_value = s->m_dropped_gw_calls;
          break;
      case GATEWAY_STATS_AVERAGE_CALL_DURATION:
          stats_parameter_value = s->m_gw_average_call_duration;
          break;
      case GATEWAY_STATS_LONGEST_CALL_DURATION:
          stats_parameter_value = s->m_gw_longest_call_duration;
          break;
      default:
          break;
    }
    snmp_set_var_typed_value(requests->requestvb, ASN_COUNTER,
                             (const u_char *)&stats_parameter_value,
                             sizeof(stats_parameter_value));
    return SNMP_ERR_NOERROR;
}

void GatewayStats::InitStatsTable()
{
    dStats = (void *)this;
    CStats::InitStatsTable();
    GENERATE_GATEWAY_STATS_PARAMETER_HANDLER(1, "gwStatsTotalCalls");
    GENERATE_GATEWAY_STATS_PARAMETER_HANDLER(2, "gwStatsCurrentCalls");
    GENERATE_GATEWAY_STATS_PARAMETER_HANDLER(3, "gwStatsMaxCalls");
    GENERATE_GATEWAY_STATS_PARAMETER_HANDLER(4, "gwStatsTotalIncomingCalls");
    GENERATE_GATEWAY_STATS_PARAMETER_HANDLER(5, "gwStatsTotalOutgoingCalls");
    GENERATE_GATEWAY_STATS_PARAMETER_HANDLER(6, "gwStatsRejectedCalls");
    GENERATE_GATEWAY_STATS_PARAMETER_HANDLER(7, "gwStatsDroppedCalls");
    GENERATE_GATEWAY_STATS_PARAMETER_HANDLER(8, "gwAverageCallDuration");
    GENERATE_GATEWAY_STATS_PARAMETER_HANDLER(9, "gwLongestCallDuration");
    return;
}
