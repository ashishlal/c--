#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include "infinetfs_mib_module.hpp"

#include <signal.h>
#include <iostream>

/* #include <nstAgentSubagentObject.h> */
extern void init_infinetfs_mib_module(void);

static int keep_running;

RETSIGTYPE
stop_server(int a) {
    keep_running = 0;
}

/* int main (int argc, char **argv) */
int init_ifs_agent()
{
  int agentx_subagent=1; /* change this if you want to be a SNMP master agent */
  int background = 0; /* change this if you want to run in the background */
  int syslog = 0; /* change this if you want to use syslog */

  std::cout << ".......1........." << std::endl;
  /* print log errors to syslog or stderr */
  if (syslog)
    snmp_enable_calllog();
  else
    snmp_enable_stderrlog();
 
  /* we're an agentx subagent? */
  if (agentx_subagent) {
    /* make us a agentx client. */
    netsnmp_ds_set_boolean(NETSNMP_DS_APPLICATION_ID, NETSNMP_DS_AGENT_ROLE, 1);
  }

  /* run in background, if requested */
  if (background && netsnmp_daemonize(1, !syslog))
      exit(1);

  /* initialize tcpip, if necessary */
  SOCK_STARTUP;

  /* initialize the agent library */
  init_agent("infinetfs_agent");

  /* initialize mib code here */
#if 0
  IfsMib <GatewayStats> *ifsGwMib  = new IfsMib<GatewayStats>;
  ifsGwMib->InitStatsTable();
#else
  IfsMib<GatewayStats > *ifsMib = IfsMib<GatewayStats>::getInstance();
#endif

  std::cout << ".......2........." << std::endl;
  /* example-demon will be used to read example-demon.conf files. */
  init_snmp("infinetfs_agent");

  /* If we're going to be a snmp master agent, initial the ports */
  if (!agentx_subagent)
    init_master_agent();  /* open the port to listen on (defaults to udp:161) */

  /* In case we recevie a request to stop (kill -TERM or kill -INT) */
  keep_running = 1;
  signal(SIGTERM, stop_server);
  signal(SIGINT, stop_server);

  snmp_log(LOG_INFO,"infinetfs_agent is up and running.\n");
  return 0;
}

int ifs_mainloop_nonblocking()
{
  /* your main loop here... */
  /* while(keep_running) { */
    /* if you use select(), see snmp_select_info() in snmp_api(3) */
    /*     --- OR ---  */
    agent_check_and_process(0); /* 0 == don't block ; 1 == block */
  /* } */
}

int ifs_shutdown()
{
  /* at shutdown time */
  snmp_shutdown("example-demon");
  SOCK_CLEANUP;

  return 0;
}


