/*
 * Filename: IfsAgent.cpp
 * Description: AgentX class for MIB used in ATP
 * Author: Ashish Lal <ashish.lal@infinetfs.com>
 * Company: Infinet Financial Systems Ltd.
 * Created: 2012-09-05
 * Version: 1.0
 * Compiler: g++
 *
 * Copyright 2012 by Infinet Financial Systems Ltd. All Rights Reserved.
 */

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
# include <netinet/in.h>
# include <sys/time.h>
# include <time.h>
#include <sys/select.h>
#include <iostream>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "IfsAgent.hpp"

#include <signal.h>
#include <iostream>
#include <vector>
#include <sstream>

#include <boost/asio/placeholders.hpp>

oid             objid_enterprise[] = { 1, 3, 6, 1, 4, 1, 3, 1, 1 };
oid             objid_sysdescr[] = { 1, 3, 6, 1, 2, 1, 1, 1, 0 };
oid             objid_sysuptime[] = { 1, 3, 6, 1, 2, 1, 1, 3, 0 };
oid             objid_snmptrap[] = { 1, 3, 6, 1, 6, 3, 1, 1, 4, 1, 0 };
int             inform = 0;

using namespace std;
IfsAgent * NetSnmpLibResources::agent = nullptr;
NetSnmpLibResources::CommonStatsTableEntry NetSnmpLibResources::commonStatsTableEntry;

IfsAgent::IfsAgent(boost::asio::io_service& io_service):
    m_deviceTypeBeingMonitored(0),
    m_init(0),
    m_monitoring_agent_addr_port(string("")),
    m_agentx_loc(string("tcp:localhost:45705")), // make sure same setting is in snmpd.conf
    m_main_loop_timer(io_service)
{
        for(int i=0; i < 10; i++) {
            m_argv[i] = new char[256];
            switch(i) {
                case 0:
                    strcpy(m_argv[i], "snmptrap");
                    break;
                case 1:
                    strcpy(m_argv[i], "-v2c");
                    break;
                case 2:
                    strcpy(m_argv[i], "-c");
                    break;
                case 3:
                    strcpy(m_argv[i], "public");
                    break;
                case 4:
                    strcpy(m_argv[i], "192.168.5.73:162");
                    break;
                case 5:
                    strcpy(m_argv[i], "");
                    break;
                case 6:
                    strcpy(m_argv[i], "NET-INFINETFS-MIB::infinetfsError");
                    break;
                case 7:
                    strcpy(m_argv[i], "NET-INFINETFS-MIB::infinetfsError");
                    break;
                case 8:
                    strcpy(m_argv[i], "s");
                    break;
                case 9:
                    strcpy(m_argv[i], "snmptrap");
                    break;
            }
        }
        m_disable_snmp = false;
        NetSnmpLibResources::agent = this;
}

bool IfsAgent::initialise(int dev, string& host, string& atp_port, string& site,
        string& org, string& common_name )
{
    if(m_init) return true;
    m_deviceTypeBeingMonitored = dev;

    if(system("pidof snmpd > /dev/null")) {

        /* initialize mib code so the set works */
        set_atp_host_name(host);
        set_atp_port(atp_port);
        set_atp_site(site);
        set_atp_org(org);
        set_atp_cn(common_name);

        return true;
    }
    NetSnmpLibResources::initAgentX(common_name, m_agentx_loc);
    NetSnmpLibResources::initCommonStatsTable();
    set_atp_host_name(host);
    set_atp_port(atp_port);
    set_atp_site(site);
    set_atp_org(org);
    set_atp_cn(common_name);

    m_init = 1;

    boost::system::error_code success;
    mainloop_nonblocking(success);
    
    return true;
}

void IfsAgent::mainloop_nonblocking(const boost::system::error_code& error)
{
    if(!m_init) return;
    
    agent_check_and_process(0);  /* 0 == don't block ; 1 == block */
    
    m_main_loop_timer.expires_from_now(boost::posix_time::milliseconds(50));
    m_main_loop_timer.async_wait(boost::bind(
        &IfsAgent::mainloop_nonblocking,
      this, boost::asio::placeholders::error));
       
    return;
}

void IfsAgent::shutdown()
{
    /* at shutdown time */
    if(m_init) {
        snmp_shutdown("IfsAgent");
        SOCK_CLEANUP;
    }
    for(int i = 0; i < 10; i++)
      if(m_argv[i])
          delete [] m_argv[i];
    return;
}

void IfsAgent::set_disable_snmp(string &disable)
{
    m_disable_snmp = false;
    if(disable == "1") {
        // snmp is enabled. Disable it
        m_disable_snmp = true;
    }
}

int IfsAgent::snmpTrap(int level, string & errorMsg)
{
    if(m_disable_snmp) {
        return 0;
    }
    if(m_monitoring_agent_addr_port == "") {
        return 0;
    }
    if((level != ATP_ERROR) && (level != ATP_FATAL)) {
        return 0;
    }
    // eg. snmptrap -v 2c -c public host "" UCD-NOTIFICATION-TEST-MIB::demoNotif SNMPv2-MIB::sysLocation.0 s "Just here"
    strcpy(m_argv[4], m_monitoring_agent_addr_port.c_str());
    strncpy(m_argv[9], errorMsg.c_str(), 256);
    if(level == 0) { // 0 == ATP_FATAL
        strcpy(m_argv[6], "NET-INFINETFS-MIB::infinetfsFatalError");
        strcpy(m_argv[7], "NET-INFINETFS-MIB::infinetfsFatalError");
    }
    return NetSnmpLibResources::snmpTrap(10, m_argv);
}

void NetSnmpLibResources::initCommonStatsTable()
{
    commonStatsTableEntry.commonStatsTableIndex = getpid();
    std::cout << "Inside initCommonStatsTable" << std::endl;
    oid oidAtpCommonStatsTable[] = { 1, 3, 6, 1, 4, 1, 39282, 3, 1, 1, commonStatsTableEntry.commonStatsTableIndex };
    size_t oid_len = OID_LENGTH(oidAtpCommonStatsTable);
    netsnmp_table_data_set *table_set;

    /* create the table structure itself */
    table_set = netsnmp_create_table_data_set("AtpCommonStatsTable");

    /* comment this out or delete if you don't support creation of new rows */
    table_set->allow_creation = 1;

    /***************************************************
     * Adding indexes
     */
    DEBUGMSGTL(("initialize_table_AtpCommonStatsTable",
                "adding indexes to table AtpCommonStatsTable\n"));
    netsnmp_table_set_add_indexes(table_set,
                           ASN_INTEGER,  
                           0);

    
    commonStatsTableEntry.host_name = "ashishpc";
    netsnmp_table_set_multi_add_default_row(table_set,
        1, ASN_INTEGER, 0,
        (void *)(&commonStatsTableEntry.commonStatsTableIndex), 
        sizeof(commonStatsTableEntry.commonStatsTableIndex),
        2, ASN_OCTET_STR, 0,
        NULL, 0,
        /* COLUMN_OCSTBHOSTAVINTERFACESTATUS, ASN_INTEGER, 0,
        NULL, 0, */
        0);
    
    /* registering the table with the master agent */
    /* note: if you don't need a subhandler to deal with any aspects
       of the request, change ocStbHostAVInterfaceTable_handler to "NULL" */
    netsnmp_register_table_data_set(
        netsnmp_create_handler_registration("AtpCommonStatsTable", 
        MyTable_handler,
        oidAtpCommonStatsTable,
        oid_len,
        HANDLER_CAN_RONLY),
        table_set, NULL);
    netsnmp_table_row *row;
    row = netsnmp_create_table_data_row();

    netsnmp_table_row_add_index(row, ASN_INTEGER, 
        &commonStatsTableEntry.commonStatsTableIndex,
        sizeof(commonStatsTableEntry.commonStatsTableIndex));


    /*
     * add the row to the table 
     */
    netsnmp_table_dataset_add_row(table_set, row);

    /*
     * Finally, this actually allows the "add_row" token it the
     * * snmpd.conf file to add rows to this table.
     * * Example snmpd.conf line:
     * *   add_row netSnmpIETFWGTable eos "Glenn Waters" "Dale Francisco"
     */
    netsnmp_register_auto_data_table(table_set, NULL);
}

int NetSnmpLibResources::MyTable_handler( netsnmp_mib_handler *handler, 
            netsnmp_handler_registration *reginfo, 
            netsnmp_agent_request_info *reqinfo, 
            netsnmp_request_info *requests )
{
    std::cout << "Inside MyTable_handler" << std::endl;
    DEBUGMSGTL("Inside MyTable_handler");
    if ( reqinfo->mode != MODE_GET ) return SNMP_ERR_NOERROR;
    for ( netsnmp_request_info *request = requests; request; request = request->next )
    {
        NetSnmpLibResources::CommonStatsTableEntry *table_entry  = 
            (CommonStatsTableEntry *)netsnmp_tdata_extract_entry( request );
        netsnmp_table_request_info *table_info = netsnmp_extract_table_info( request );

    std::cout << "table_entry = " << table_entry << std::endl;
     //   if ( table_entry == NULL ) { netsnmp_set_request_error( reqinfo, request, SNMP_NOSUCHINSTANCE); continue; }

    std::cout << "table_info->column = " << table_info->colnum << std::endl;
        switch ( table_info->colnum )
        {
            // ...this is similar to non-table situations, eg:
            case 2:
                {
                char stats_parameter_value[1024];

                IfsAgent *s = NetSnmpLibResources::agent;
                if(s->get_disable_snmp()) {
                    return SNMPERR_GENERR;
                }
                unsigned int stats_parameter = 1;
                sprintf(stats_parameter_value, 
                    s->get_common_stat(stats_parameter).c_str(), 
                    sizeof(s->get_common_stat(stats_parameter).c_str()));

                snmp_set_var_typed_value(requests->requestvb, 
                    ASN_OCTET_STR, 
                    (const u_char *)stats_parameter_value, 
                    (size_t)strlen(stats_parameter_value));
                }
                break;
            // ...
            default: netsnmp_set_request_error( reqinfo, request, SNMP_NOSUCHOBJECT );
        }
    }
    return SNMP_ERR_NOERROR;
}
bool NetSnmpLibResources::initAgentX(string &common_name, string &agentx_loc)
{
    netsnmp_log_handler *logh;
    logh = netsnmp_register_loghandler(NETSNMP_LOGHANDLER_FILE, 3);

    if (logh)
    {
        // Dump spurious warnings/error messages from snmp library into syslog
        snmp_enable_syslog();
    }
     snmp_set_do_debugging(1);

    /* make us a agentx client. */
    netsnmp_ds_set_boolean(NETSNMP_DS_APPLICATION_ID, 
        NETSNMP_DS_AGENT_ROLE, 1);
    netsnmp_ds_set_int(NETSNMP_DS_APPLICATION_ID, 
        NETSNMP_DS_AGENT_AGENTX_PING_INTERVAL, 120);
    netsnmp_ds_set_string(NETSNMP_DS_APPLICATION_ID, 
        NETSNMP_DS_AGENT_X_SOCKET, 
        agentx_loc.c_str());
    netsnmp_ds_set_boolean(NETSNMP_DS_APPLICATION_ID, 
        NETSNMP_DS_AGENT_VERBOSE, 0);
    
    /* initialize tcpip, if necessary */
    SOCK_STARTUP;

    /* initialize the agent library */
    init_agent("IfsAgent");
    init_snmp("IfsAgent");

    GENERATE_AGENT_COMMON_STATS_PARAMETER_HANDLER(1, common_name, "statsHostName");
    GENERATE_AGENT_COMMON_STATS_PARAMETER_HANDLER(2, common_name, "statsAtpPort");
    GENERATE_AGENT_COMMON_STATS_PARAMETER_HANDLER(3, common_name, "statsOrg");
    GENERATE_AGENT_COMMON_STATS_PARAMETER_HANDLER(4, common_name, "statsSite");
    GENERATE_AGENT_COMMON_STATS_PARAMETER_HANDLER(5, common_name, "statsCN");

    GENERATE_AGENT_MIXER_STATS_PARAMETER_HANDLER(1, common_name, "mixerStatsNumMixes");
    GENERATE_AGENT_MIXER_STATS_PARAMETER_HANDLER(2, common_name, "mixerStatsMaxMixes");

    GENERATE_AGENT_TURRET_STATS_PARAMETER_HANDLER(1, common_name, "turretStatsTotalCalls");
    GENERATE_AGENT_TURRET_STATS_PARAMETER_HANDLER(2, common_name, "turretStatsTotalRejectedCalls");
    GENERATE_AGENT_TURRET_STATS_PARAMETER_HANDLER(3, common_name, "turretStatsTotalDroppedCalls");
    GENERATE_AGENT_TURRET_STATS_PARAMETER_HANDLER(4, common_name, "turretStatsCurrentCalls");
    GENERATE_AGENT_TURRET_STATS_PARAMETER_HANDLER(5, common_name, "turretStatsAverageCallDuration");
    GENERATE_AGENT_TURRET_STATS_PARAMETER_HANDLER(6, common_name, "turretStatsLongestCallDuration");

    GENERATE_AGENT_GATEWAY_STATS_PARAMETER_HANDLER(1, common_name, "gwStatsTotalCalls");
    GENERATE_AGENT_GATEWAY_STATS_PARAMETER_HANDLER(2, common_name, "gwStatsCurrentCalls");
    GENERATE_AGENT_GATEWAY_STATS_PARAMETER_HANDLER(3, common_name, "gwStatsMaxCalls");
    GENERATE_AGENT_GATEWAY_STATS_PARAMETER_HANDLER(4, common_name, "gwStatsTotalIncomingCalls");
    GENERATE_AGENT_GATEWAY_STATS_PARAMETER_HANDLER(5, common_name, "gwStatsTotalOutgoingCalls");
    GENERATE_AGENT_GATEWAY_STATS_PARAMETER_HANDLER(6, common_name, "gwStatsRejectedCalls");
    GENERATE_AGENT_GATEWAY_STATS_PARAMETER_HANDLER(7, common_name, "gwStatsDroppedCalls");
    GENERATE_AGENT_GATEWAY_STATS_PARAMETER_HANDLER(8, common_name, "gwAverageCallDuration");
    GENERATE_AGENT_GATEWAY_STATS_PARAMETER_HANDLER(9, common_name, "gwLongestCallDuration");

    return true;
}

int NetSnmpLibResources::snmp_input(int operation,
           netsnmp_session * session,
           int reqid, netsnmp_pdu *pdu, void *magic)
{
    return 1;
}

void NetSnmpLibResources::optProc(int argc, char *const *argv, int opt)
{
    switch (opt) {
        case 'C':
            while (*optarg) {
                switch (*optarg++) {
                    case 'i':
                        inform = 1;
                        break;
                    default:
                        fprintf(stderr,
                                "Unknown flag passed to -C: %c\n", optarg[-1]);
                        exit(1);
                }
            }
            break;
    }
    return;
}

int NetSnmpLibResources::snmpTrap(int argc, char **argv)
{
        // eg. snmptrap -v 2c -c public host "" UCD-NOTIFICATION-TEST-MIB::demoNotif SNMPv2-MIB::sysLocation.0 s "Just here"
        netsnmp_session session, *ss;
        netsnmp_pdu    *pdu, *response;
        oid             name[MAX_OID_LEN];
        size_t          name_length;
        int             arg;
        int             status;
        char           *trap = nullptr;
        int             exitval = 0;

        putenv(strdup("POSIXLY_CORRECT=1"));
        switch (arg = snmp_parse_args(argc, (char **)(argv), &session, "C:", optProc)) {
        case NETSNMP_PARSE_ARGS_ERROR:
            goto freeres;
            exit(1);
        case NETSNMP_PARSE_ARGS_SUCCESS_EXIT:
            goto freeres;
            exit(0);
        case NETSNMP_PARSE_ARGS_ERROR_USAGE:
            goto freeres;
            exit(1);
        default:
            break;
        }
        SOCK_STARTUP;
        session.callback = snmp_input;
        session.callback_magic = nullptr;
        /*
         * setup the local engineID which may be for either or both of the
         * contextEngineID and/or the securityEngineID.
         */
        setup_engineID(nullptr, nullptr);
        /* if we don't have a contextEngineID set via command line
         *     argume*nts, use our internal engineID as the context. */
        if (session.contextEngineIDLen == 0 ||
            session.contextEngineID == nullptr) {
            session.contextEngineID =
            snmpv3_generate_engineID(&session.contextEngineIDLen);
        }
        if (session.version == SNMP_VERSION_3 && !inform) {
            /*
             * pick our own engineID
             */
            if (session.securityEngineIDLen == 0 ||
                session.securityEngineID == nullptr) {
                    session.securityEngineID =
                    snmpv3_generate_engineID(&session.securityEngineIDLen);
                }

                /*
                 * set boots and time, which will cause problems if this
                 * machine ever reboots and a remote trap receiver has cached our
                 * boots and time...  I'll cause a not-in-time-window report to
                 * be sent back to this machine.
                 */
                if (session.engineBoots == 0)
                    session.engineBoots = 1;
                if (session.engineTime == 0)    /* not really correct, */
                    session.engineTime = (u_int)(get_uptime());  /* but it'll work. Sort of. */
        }
        ss = snmp_add(&session,
                     netsnmp_transport_open_client("snmptrap", 
                     session.peername),
                    nullptr, nullptr);
        if (ss == nullptr) {
            /*
             * diagnose netsnmp_transport_open_client and snmp_add errors with
             * the input netsnmp_session pointer
             */
            snmp_sess_perror("snmptrap", &session);
            SOCK_CLEANUP;
            goto freeres;
            exit(1);
        }
        {
            long            sysuptime;
            char            csysuptime[20];
            pdu = snmp_pdu_create(inform ? SNMP_MSG_INFORM : SNMP_MSG_TRAP2);
            if ( !pdu ) {
                fprintf(stderr, "Failed to create notification PDU\n");
                SOCK_CLEANUP;
                goto freeres;
                exit(1);
            }
            if (arg == argc) {
                fprintf(stderr, "Missing up-time parameter\n");
                SOCK_CLEANUP;
                goto freeres;
                exit(1);
            }
            trap = argv[arg];
            if(1) {
                sysuptime = get_uptime();
                sprintf(csysuptime, "%ld", sysuptime);
                trap = csysuptime;
            }
            snmp_add_var(pdu, objid_sysuptime,
                         sizeof(objid_sysuptime) / sizeof(oid), 't', trap);
            if (++arg == argc) {
                fprintf(stderr, "Missing trap-oid parameter\n");
                SOCK_CLEANUP;
                goto freeres;
                exit(1);
            }
            if (snmp_add_var(pdu, objid_snmptrap, sizeof(objid_snmptrap) / sizeof(oid),'o', argv[arg]) != 0) {
                snmp_perror(argv[arg]);
                SOCK_CLEANUP;
                goto freeres;
                exit(1);
            }
        }
        arg++;
        while (arg < argc) {
            arg += 3;
            if (arg > argc) {
                fprintf(stderr, "%s: Missing type/value for variable\n",
                        argv[arg - 3]);
                SOCK_CLEANUP;
                goto freeres;
                exit(1);
            }
            name_length = MAX_OID_LEN;
            if (!snmp_parse_oid(argv[arg - 3], name, &name_length)) {
                snmp_perror(argv[arg - 3]);
                SOCK_CLEANUP;
                goto freeres;
                exit(1);
            }
            // int snmp_add_var (netsnmp_pdu *pdu, const oid *name, size_t name_length, char type, const char *value)
            if (snmp_add_var(pdu, name, name_length, argv[arg - 2][0],argv[arg - 1]) != 0) {
                snmp_perror(argv[arg - 3]);
                SOCK_CLEANUP;
                goto freeres;
                exit(1);
            }
        }
        if (inform)
            status = snmp_synch_response(ss, pdu, &response);
        else
            status = snmp_send(ss, pdu) == 0;
        if (status) {
            snmp_sess_perror(inform ? "snmpinform" : "snmptrap", ss);
            if (!inform)
                snmp_free_pdu(pdu);
            exitval = 1;
        } else if (inform)
            snmp_free_pdu(response);
        snmp_close(ss);
        snmp_shutdown("snmpapp");
        SOCK_CLEANUP;
        goto freeres;
        return exitval;
freeres:
        return 0;
}

int NetSnmpLibResources::HandleAgentCommonStatsParameter(
    netsnmp_mib_handler *handler,
    netsnmp_handler_registration *reginfo,
    netsnmp_agent_request_info *reqinfo,
    netsnmp_request_info *requests)
{
    oid stats_parameter = requests->requestvb->name[requests->requestvb->name_length-2];
    char stats_parameter_value[1024];

    IfsAgent *s = NetSnmpLibResources::agent;
    if(s->get_disable_snmp()) {
        return SNMPERR_GENERR;
    }
    sprintf(stats_parameter_value, 
        s->get_common_stat((unsigned int)(stats_parameter)).c_str(), 
        sizeof(s->get_common_stat((unsigned int)(stats_parameter)).c_str()));

    snmp_set_var_typed_value(requests->requestvb, 
        ASN_OCTET_STR, 
        (const u_char *)stats_parameter_value, 
        (size_t)strlen(stats_parameter_value));
    return SNMP_ERR_NOERROR;
}

int NetSnmpLibResources::HandleAgentMixerStatsParameter(
    netsnmp_mib_handler *handler,
    netsnmp_handler_registration *reginfo,
    netsnmp_agent_request_info *reqinfo,
    netsnmp_request_info *requests)
{
    oid stats_parameter = requests->requestvb->name[requests->requestvb->name_length-2];
    unsigned int stats_parameter_value = 0;

    IfsAgent *s = NetSnmpLibResources::agent;
    if(s->get_disable_snmp()) {
        return SNMPERR_GENERR;
    }
    stats_parameter_value = 
        (unsigned int)(s->get_stat((unsigned int)(stats_parameter)+STAT_END_COMMON_STATS));

    snmp_set_var_typed_value(
        requests->requestvb, ASN_COUNTER,
        (const u_char *)&stats_parameter_value,
        sizeof(stats_parameter_value));
    return SNMP_ERR_NOERROR;
}

int NetSnmpLibResources::HandleAgentTurretStatsParameter(
    netsnmp_mib_handler *handler,
    netsnmp_handler_registration *reginfo,
    netsnmp_agent_request_info *reqinfo,
    netsnmp_request_info *requests)
{
    oid stats_parameter = requests->requestvb->name[requests->requestvb->name_length-2];
    unsigned int stats_parameter_value = 0;

    IfsAgent *s = NetSnmpLibResources::agent;
    if(s->get_disable_snmp()) {
        return SNMPERR_GENERR;
    }
    stats_parameter_value = 
        (unsigned int)(s->get_stat((unsigned int)(stats_parameter)+STAT_END_MIXER_STATS));

    snmp_set_var_typed_value(
        requests->requestvb, ASN_COUNTER,
        (const u_char *)&stats_parameter_value,
        sizeof(stats_parameter_value));
    return SNMP_ERR_NOERROR;
}

int NetSnmpLibResources::HandleAgentGatewayStatsParameter(
    netsnmp_mib_handler *handler,
    netsnmp_handler_registration *reginfo,
    netsnmp_agent_request_info *reqinfo,
    netsnmp_request_info *requests)
{
    oid stats_parameter = requests->requestvb->name[requests->requestvb->name_length-2];
    unsigned int stats_parameter_value = 0;

    IfsAgent *s = NetSnmpLibResources::agent;
    if(s->get_disable_snmp()) {
        return SNMPERR_GENERR;
    }
    stats_parameter_value = (unsigned int)(s->get_stat((unsigned int)(stats_parameter)+STAT_END_TURRET_STATS));

    snmp_set_var_typed_value(
        requests->requestvb, ASN_COUNTER,
        (const u_char *)&stats_parameter_value,
        sizeof(stats_parameter_value));
    return SNMP_ERR_NOERROR;
}
