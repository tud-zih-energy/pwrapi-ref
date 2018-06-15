/*
 * Copyright 2014 Sandia Corporation. Under the terms of Contract
 * DE-AC04-94AL85000, there is a non-exclusive license for use of this work
 * by or on behalf of the U.S. Government. Export of this program may require
 * a license from the United States Government.
 *
 * This file is part of the Power API Prototype software package. For license
 * information, see the LICENSE file in the top level directory of the
 * distribution.
*/

#include <pwr_hdeemdev.h>
#include <pwr_dev.h>


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <pthread.h>
#include <hdeem.h>
#include <freeipmi/freeipmi.h>


static int rc;
static int hdeemdev_verbose = 1;
static const char * delimiter = ";";
static char *  cpystring;

static plugin_devops_t devops = {
    .open         = pwr_hdeemdev_open,
    .close        = pwr_hdeemdev_close,
    .read         = pwr_hdeemdev_read,
    /*.time       = pwr_hdeemdev_time,*/
    .clear        = pwr_hdeemdev_clear,
    .private_data = 0x0
};



int stringhash(const char* word)
{
    unsigned int hash = 0;
    int i;
    for (i = 0 ; word[i] != '\0' ; i++)
    {
        hash = 31*hash + word[i];
    }
    return hash;
}



plugin_devops_t *pwr_hdeemdev_init(const char *initstr) {
    plugin_devops_t *dev = malloc( sizeof(plugin_devops_t) );
    *dev = devops;
    dev->private_data = malloc( sizeof(hdeem_bmc_data_t) );
    bzero( dev->private_data, sizeof(hdeem_bmc_data_t) );
    hdeem_status_t  readStatus;
    hdeem_bmc_data_t * bmc = dev->private_data;
    cpystring=malloc(sizeof(char)*strlen(initstr));
    strcpy(cpystring,initstr);
    if(hdeemdev_verbose) printf("initstr is: %s\n",initstr);
    if(!strcmp(initstr,"inband")) {
        bmc->host=NULL;
        bmc->password=NULL;
        bmc->user=NULL;
        if(hdeemdev_verbose)printf("plugin configured for inbandconnection, host, password, user is set to nullptr\n");
    } else {
        bmc->host=strdup(strtok(cpystring,delimiter));
        bmc->user=strdup(strtok(NULL,delimiter));
        bmc->password=strdup(strtok(NULL,delimiter));
        if(hdeemdev_verbose) printf("host is: '%s' user is '%s' password is '%s'\n",bmc->host,bmc->user,bmc->password);
    }
    rc=hdeem_init(bmc);
    hdeem_check_status(bmc,&readStatus);
    if(IsFpgaPolling(readStatus.status)) {
        hdeem_stop(bmc);
        hdeem_clear(bmc);
        hdeem_start(bmc);
    } else {
        hdeem_start(bmc);
    }
    if(hdeemdev_verbose)printf("hdeem initialized!\n");
    if(rc) {
        printf("hdeem_init() failed!\n");
    }
    return dev;
}

int pwr_hdeemdev_final( plugin_devops_t *dev )
{
    if( hdeemdev_verbose )
        printf( "Info: finaling PWR HDEEM device\n" );
    hdeem_bmc_data_t * bmc = dev->private_data;
    hdeem_close(bmc);
    free(dev->private_data);
    free(dev);
    return 0;
}

hdeemFdInfo_t * pwr_hdeemdev_open(plugin_devops_t *dev, const char *openstr ) {
    int i;
    if(hdeemdev_verbose) {
        printf("openstr is: '%s'\n",openstr);
    }
    hdeemFdInfo_t * fd;
    fd=malloc(sizeof(hdeemFdInfo_t));
    switch(stringhash(openstr)) { //openstr to integer hash, for switch() case
    case _BLADE:
        fd->sensor = BLADE;
        break;
    case _CPU0:
        fd->sensor = CPU0;
        break;
    case _CPU1:
        fd->sensor = CPU1;
        break;
    case _DDR_AB:
        fd->sensor = DDR_AB;
        break;
    case _DDR_CD:
        fd->sensor = DDR_CD;
        break;
    case _DDR_EF:
        fd->sensor = DDR_EF;
        break;
    case _DDR_GH:
        fd->sensor = DDR_GH;
        break;
    default:
        fprintf(stderr,"Warning: unknown hdeem sensor requested: %s\n",openstr);
        return NULL;
    }
    fd->bmc=dev->private_data;
    strcpy(fd->name,openstr);
    strcpy(fd->node,fd->bmc->host);
    return fd;
}

int pwr_hdeemdev_close( hdeemFdInfo_t * fd )
{
    hdeem_stop(fd->bmc);
    free(fd);
    return 0;
}

int pwr_hdeemdev_clear( hdeemFdInfo_t * fd )
{
    rc=hdeem_clear(fd->bmc);
    if(rc) {
        printf("hdeem_clear() failed!\n");
    }
    return 0;
}

int pwr_hdeemdev_read( hdeemFdInfo_t *fd, PWR_AttrName attr, void *value, unsigned int len, PWR_Time *timestamp )
{
    hdeem_stats_reading_t  stats;
    hdeem_global_reading_t  globalStats;
    if( len != sizeof(unsigned long long) ) {
        printf( "Error: value field size of %u incorrect, should be %ld\n", len, sizeof(unsigned long long) );
        return 1;
    }
    switch( attr ) {
    case PWR_ATTR_POWER: // !!! SLOW !!!
        do {
            rc=hdeem_get_global(fd->bmc,&globalStats);
            if(rc) {
                fprintf(stderr,"hdeem_get_global(%p,%p) failed! Resetting...\n",fd->bmc,&globalStats);
                hdeem_data_free(&globalStats);
                hdeem_stop(fd->bmc);
                hdeem_clear(fd->bmc);
                hdeem_start(fd->bmc);
            }
        } while(!globalStats.nb_blade_values);
        if(fd->sensor==BLADE) {
            if(&globalStats.blade_power->value[0]==NULL) {
                fprintf(stderr,"Error: No Data in hdeem globalStats struct!\n");
                *((double *)value) = -1;
                hdeem_data_free(&globalStats);
                break;
            } else {
                *((double *)value) = globalStats.blade_power->value[0];
                hdeem_data_free(&globalStats);
                break;
            }
        } else {
            if(&globalStats.vr_power->value[fd->sensor]==NULL) {
                fprintf(stderr,"Error: No Data in hdeem globalStats struct!\n");
                *((double *)value) = -1;
                break;
            } else {
                *((double *)value) = globalStats.vr_power->value[fd->sensor];
                break;
            }
        }
    case PWR_ATTR_ENERGY:
        rc=hdeem_get_stats(fd->bmc,&stats);
        if(rc) {
            fprintf(stderr,"hdeem_get_stats(%p,%p) failed!\n",fd->bmc,&stats);
            hdeem_stats_free(&stats);
            break;
        }
        if(fd->sensor==BLADE) {
            if(&stats.energy_blade_values.value[0]==NULL) {
                fprintf(stderr,"Error: No Data in hdeem stats struct!\n");
                *((double *)value) = -1;
                hdeem_stats_free(&stats);
                break;
            } else {
                *((double *)value) = stats.energy_blade_values.value[0];
                hdeem_stats_free(&stats);
                break;
            }
        } else {
            if(&stats.energy_vr_values.value[fd->sensor]==NULL) {
                fprintf(stderr,"Error: No Data in hdeem stats struct!\n");
                *((double *)value) = -1;
                hdeem_stats_free(&stats);
                break;
            } else {
                *((double *)value) = stats.energy_vr_values.value[fd->sensor];
                hdeem_stats_free(&stats);
                break;
            }
        }
    default:
        fprintf(stderr,"Warning: unknown PWR reading attr (%u, requested)\n", attr );
        break;
    }
    struct timeval tv;
    gettimeofday( &tv, NULL );
    *timestamp = tv.tv_sec*1000000000ULL + tv.tv_usec*1000;
    if( hdeemdev_verbose ) {
        printf( "Info: reading of type %d at sensor %s on node %s at time %llu with value %f\n",
                attr,fd->name,fd->bmc->host,*(unsigned long long *)timestamp, *((double *)value) );
    }
    return 0;
}

static plugin_dev_t dev = {
    .init   = pwr_hdeemdev_init,
    .final  = pwr_hdeemdev_final,
};

plugin_dev_t* getDev() {
    return &dev;
}
