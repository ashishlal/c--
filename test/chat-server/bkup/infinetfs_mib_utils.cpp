/*
 * Filename: infinetfs_mib_utils.c
 * Description: Utilities used by the Infinetfs MIB Module
 * Author: Mandar Joshi <mandar.joshi@infinetfs.com>
 * Company: Infinet Financial Systems Ltd.
 * Created: 2012-01-07
 * Version: 1.0
 * Compiler: gcc
 *
 * Copyright 2011 by Infinet Financial Systems Ltd. All Rights Reserved.
 *
 * Date        Author           Change
 * 2011-12-30  Mandar Joshi     Initial version
 *
 */

#include <sys/sysinfo.h>

#include "infinetfs_mib_module.h"

int get_process_output(char* command, char* output) {
  FILE *fp;
  int len, read_len;
  fp = popen(command, "r");
  fseek(fp,0,SEEK_END);
  len=ftell(fp);
  len = len>99?99:len;
  rewind(fp);
  read_len = fread(output, 1, len, fp);
  pclose(fp);
  return read_len;
}

double time_since_epoch() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec + tv.tv_usec * 0.000001 ;
}

int get_cpu_usage() {
  FILE *fp;
  double t1, t2, t;
  char label[4];
  int val1, val2, val3, val4, val5, val6;
  float cpu_usage;

  t1 = time_since_epoch();
  fp = fopen("/proc/stat", "r");
  fscanf(fp,"%s\t%d\t%d\t%d\n", label, &val1, &val2, &val3);
  fclose(fp);

  usleep(100000);

  t2 = time_since_epoch();
  fp = fopen("/proc/stat", "r");
  fscanf(fp,"%s\t%d\t%d\t%d\n", label, &val4, &val5, &val6);
  fclose(fp);

  t = (val4 + val5 + val6)-(val1 + val2 + val3);

  cpu_usage = (t / ((t2-t1) * 100)) * 100;
  return  (int)cpu_usage;
}

int get_total_memory() {
  struct sysinfo info;
  sysinfo(&info);
  return info.totalram*info.mem_unit/(1024*1024);
}

int get_memory_usage() {
  struct sysinfo info;
  sysinfo(&info);
  return (info.totalram-info.freeram)*info.mem_unit/(1024*1024);
}

int get_cpu_queue_length() {
  //   vmstat|tail -1|cut -d" " -f2
  char process_output[100];
  memset(process_output, 0, 100);
  get_process_output((char *)"vmstat|tail -1|cut -d\" \" -f2", (char *)process_output);
  int queue_len = atoi(process_output);
  return queue_len;
}
