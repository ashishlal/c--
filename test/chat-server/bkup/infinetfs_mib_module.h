/*
 * Filename: infinetfs_mib_module.h
 * Description: Header file for Net-Snmp Module 
 * Author: Mandar Joshi <mandar.joshi@infinetfs.com>
 * Company: Infinet Financial Systems Ltd.
 * Created: 2011-12-30
 * Version: 1.0
 * Compiler: gcc
 *
 * Copyright 2011 by Infinet Financial Systems Ltd. All Rights Reserved.
 *
 * Date        Author           Change
 * 2011-12-30  Mandar Joshi     Initial version
 *
 */
#ifndef INFINETFS_MIB_MODULE_H
#define INFINETFS_MIB_MODULE_H

#ifdef __cplusplus 
extern "C" {
#endif

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

void            init_infinetfs_mib_module(void);
void            shutdown_infinetfs_mib_module(void);
Netsnmp_Node_Handler infinetfs_routes_handler;
static int handle_mixer_parameter(netsnmp_mib_handler *handler,
				  netsnmp_handler_registration *reginfo,
				  netsnmp_agent_request_info *reqinfo,
				  netsnmp_request_info *requests);
int get_process_output(char* command, char* output);
double time_since_epoch();
int get_cpu_usage();
int get_total_memory();
int get_memory_usage();
int get_cpu_queue_length();

enum MixerParameters {MIXER_CAPACITY_COMPUTE_USAGE = 1, MIXER_CAPACITY_COMPUTE_QUEUE_LENGTH, MIXER_CAPACITY_TOTAL_MEMORY, MIXER_CAPACITY_MEMORY_USAGE, MIXER_CAPACITY_TOTAL_MIXERS, MIXER_CAPACITY_AVAILABLE_MIXERS};

#define GENERATE_MIXER_PARAMETER_HANDLER(index,name_oid) {\
      oid name_oid[] = { 1, 3, 6, 1, 4, 1, 39282, 3, index };\
      netsnmp_register_scalar(\
            netsnmp_create_handler_registration(\
                "infinetfsMixers/name_oid", handle_mixer_parameter,\
		name_oid, OID_LENGTH(name_oid),\
                HANDLER_CAN_RONLY));\
    }

#ifdef __cplusplus 
}
#endif

#endif
