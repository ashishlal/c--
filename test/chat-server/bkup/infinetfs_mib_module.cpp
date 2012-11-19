/*
 * Filename: infinetfs_mib_module.c
 * Description: Module for Net-Snmp Returns routes when queried with dialstring
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

#include "infinetfs_mib_module.h"

static netsnmp_table_data_set *infinetfs_route_table_set;
static netsnmp_table_data_set *infinetfs_full_route_table_set;


static int
handle_mixer_parameter(netsnmp_mib_handler *handler,
                   netsnmp_handler_registration *reginfo,
                   netsnmp_agent_request_info *reqinfo,
                   netsnmp_request_info *requests)
{

    int mixer_parameter = requests->requestvb->name[requests->requestvb->name_length-2];
    int mixer_parameter_value;
    switch(mixer_parameter) {
      case MIXER_CAPACITY_COMPUTE_USAGE:
	mixer_parameter_value = get_cpu_usage();
	break;
      case MIXER_CAPACITY_COMPUTE_QUEUE_LENGTH:
	mixer_parameter_value = get_cpu_queue_length();
	break;
      case MIXER_CAPACITY_TOTAL_MEMORY:
	mixer_parameter_value = get_total_memory();
	break;
      case MIXER_CAPACITY_MEMORY_USAGE:
	mixer_parameter_value = get_memory_usage();
	break;
      case MIXER_CAPACITY_TOTAL_MIXERS:
	mixer_parameter_value = 10;
	break;
      case MIXER_CAPACITY_AVAILABLE_MIXERS:
	mixer_parameter_value = 5;
	break;
      default:
	mixer_parameter_value = -1;
	break;
    }
    DEBUGMSGTL(("INFINETFS_MIB_MODULE","Mixer Parameter %d: %d\n", mixer_parameter, mixer_parameter_value));
    snmp_set_var_typed_integer(requests->requestvb, ASN_INTEGER, mixer_parameter_value);
    return SNMP_ERR_NOERROR;
}

void init_infinetfs_mib_module(void) {

    netsnmp_table_row *row;
    int x;
    oid infinetfs_routes_oid[] = { 1, 3, 6, 1, 4, 1, 39282, 1, 1 };
    oid infinetfs_full_routes_oid[] = { 1, 3, 6, 1, 4, 1, 39282, 2, 1 };

    DEBUGMSGTL(("INFINETFS_MIB_MODULE", "Initalizing Module\n"));

    infinetfs_route_table_set =
      netsnmp_create_table_data_set("infinetfsRoutesTable");
    infinetfs_route_table_set->allow_creation = 1;
    netsnmp_table_dataset_add_index(infinetfs_route_table_set, ASN_OCTET_STR);
    netsnmp_table_set_multi_add_default_row(infinetfs_route_table_set,
					    2, ASN_OCTET_STR, 1, NULL, 0,
					    0);

    netsnmp_handler_registration *get_routes_handler =
      netsnmp_create_handler_registration("infinetfsRoutesTable",
					  infinetfs_routes_handler,
					  infinetfs_routes_oid,
					  OID_LENGTH(infinetfs_routes_oid),
					  HANDLER_CAN_RWRITE);
    netsnmp_register_table_data_set(get_routes_handler,
				    infinetfs_route_table_set,
				    NULL);

    row = netsnmp_create_table_data_row();
    netsnmp_table_row_add_index(row, ASN_OCTET_STR, "snmpconf",
                                strlen("snmpconf"));
    netsnmp_set_row_column(row, 2, ASN_OCTET_STR, "David Partain",
                           strlen("David Partain"));
    netsnmp_mark_row_column_writable(row, 2, 1);
    netsnmp_table_dataset_add_row(infinetfs_route_table_set, row);


    infinetfs_full_route_table_set =
      netsnmp_create_table_data_set("infinetfsFullRoutesTable");
    infinetfs_full_route_table_set->allow_creation = 1;

    netsnmp_table_dataset_add_index(infinetfs_full_route_table_set, ASN_INTEGER);
    netsnmp_table_set_multi_add_default_row(infinetfs_full_route_table_set,
					    2, ASN_OCTET_STR, 1, NULL, 0,
                                            3, ASN_INTEGER, 1, NULL, 0,
					    4, ASN_INTEGER, 1, NULL, 0,
					    5, ASN_INTEGER, 1, NULL, 0,
					    6, ASN_INTEGER, 1, NULL, 0,
                                            0 /* done */ );
    netsnmp_register_table_data_set(netsnmp_create_handler_registration
                                    ("infinetfsFullRoutesTable", NULL,
                                     infinetfs_full_routes_oid,
                                     OID_LENGTH(infinetfs_full_routes_oid),
                                     HANDLER_CAN_RWRITE), infinetfs_full_route_table_set, NULL);


    row = netsnmp_create_table_data_row();
    x=1;
    netsnmp_table_row_add_index(row, ASN_INTEGER, (char*)&x, sizeof(x));
    netsnmp_set_row_column(row, 2, ASN_OCTET_STR, "abcd", strlen("abcd"));
    x=2;
    netsnmp_set_row_column(row, 3, ASN_INTEGER, (char*)&x, sizeof(x));
    x=3;
    netsnmp_set_row_column(row, 4, ASN_INTEGER, (char*)&x, sizeof(x));
    x=4;
    netsnmp_set_row_column(row, 5, ASN_INTEGER, (char*)&x, sizeof(x));
    x=5;
    netsnmp_set_row_column(row, 6, ASN_INTEGER, (char*)&x, sizeof(x));
    netsnmp_table_dataset_add_row(infinetfs_full_route_table_set, row);

    row = netsnmp_create_table_data_row();
    x=2;
    netsnmp_table_row_add_index(row, ASN_INTEGER, (char*)&x, sizeof(x));
    netsnmp_set_row_column(row, 2, ASN_OCTET_STR, "wxyz", strlen("wxyz"));
    x=101;
    netsnmp_set_row_column(row, 3, ASN_INTEGER, (char*)&x, sizeof(x));
    x=102;
    netsnmp_set_row_column(row, 4, ASN_INTEGER, (char*)&x, sizeof(x));
    x=103;
    netsnmp_set_row_column(row, 5, ASN_INTEGER, (char*)&x, sizeof(x));
    x=104;
    netsnmp_set_row_column(row, 6, ASN_INTEGER, (char*)&x, sizeof(x));
    netsnmp_table_dataset_add_row(infinetfs_full_route_table_set, row);


    row = netsnmp_create_table_data_row();
    x=3;
    netsnmp_table_row_add_index(row, ASN_INTEGER, (char*)&x, sizeof(x));
    netsnmp_set_row_column(row, 2, ASN_OCTET_STR, "lmno", strlen("lmno"));
    x=201;
    netsnmp_set_row_column(row, 3, ASN_INTEGER, (char*)&x, sizeof(x));
    x=202;
    netsnmp_set_row_column(row, 4, ASN_INTEGER, (char*)&x, sizeof(x));
    x=203;
    netsnmp_set_row_column(row, 5, ASN_INTEGER, (char*)&x, sizeof(x));
    x=204;
    netsnmp_set_row_column(row, 6, ASN_INTEGER, (char*)&x, sizeof(x));
    netsnmp_table_dataset_add_row(infinetfs_full_route_table_set, row);

    GENERATE_MIXER_PARAMETER_HANDLER(1, mixerCapacityComputeUsage);
    GENERATE_MIXER_PARAMETER_HANDLER(2, mixerCapacityComputeQueueLength);
    GENERATE_MIXER_PARAMETER_HANDLER(3, mixerCapacityTotalMemory);
    GENERATE_MIXER_PARAMETER_HANDLER(4, mixerCapacityMemoryUsage);
    GENERATE_MIXER_PARAMETER_HANDLER(5, mixerCapacityTotalMixers);
    GENERATE_MIXER_PARAMETER_HANDLER(6, mixerCapacityAvailableMixers);

    DEBUGMSGTL(("INFINETFS_MIB_MODULE", "Done Initalizing.\n"));
}

void shutdown_infinetfs_mib_module(void) {

#if 0
  netsnmp_delete_table_data_set(infinetfs_route_table_set);
  infinetfs_route_table_set = NULL;

  netsnmp_delete_table_data_set(infinetfs_full_route_table_set);
  infinetfs_full_route_table_set = NULL;
#else
  netsnmp_table_dataset_delete_all_data(infinetfs_route_table_set->default_row);
  infinetfs_route_table_set = NULL;

  netsnmp_table_dataset_delete_all_data(infinetfs_full_route_table_set->default_row);
  infinetfs_full_route_table_set = NULL;

#endif

}

int infinetfs_routes_handler(netsnmp_mib_handler *handler,
		       netsnmp_handler_registration *reginfo,
		       netsnmp_agent_request_info *reqinfo,
		       netsnmp_request_info *requests) {

  unsigned char infinetfs_route_string[1024] = "", tempstring[100];
  int dialstring;
  netsnmp_table_request_info *table_info;
  netsnmp_request_info *request;
  switch (reqinfo->mode) {
    case MODE_GET:
    case MODE_GETNEXT:
	table_info = netsnmp_extract_table_info(requests);
	sprintf((char *)infinetfs_route_string, "%s: 2, 3, 4, 5;%s: 2, 3, 4, 5", table_info->indexes->val.string, table_info->indexes->val.string);
	snmp_set_var_typed_value(requests->requestvb,
			       ASN_OCTET_STR,
			       (u_char *)&infinetfs_route_string,
			       strlen((char *)infinetfs_route_string));
      break;
  }
  return SNMP_ERR_NOERROR;
}
