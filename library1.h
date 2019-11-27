/****************************************************************************/
/*                                                                          */
/* This file contains the interface functions                               */
/* you should use for the wet ex 1                                          */
/*                                                                          */
/****************************************************************************/

/****************************************************************************/
/*                                                                          */
/* File Name : library1.h                                                   */
/*                                                                          */
/****************************************************************************/

#ifndef _234218_WET1
#define _234218_WET1

#ifdef __cplusplus
extern "C" {
#endif

/* Return Values
 * ----------------------------------- */
typedef enum {
    SUCCESS = 0,
    FAILURE = -1,
    ALLOCATION_ERROR = -2,
    INVALID_INPUT = -3
} StatusType;


void *Init();

StatusType AddDataCenter(void *DS, int dataCenterID, int numOfServers);

StatusType RemoveDataCenter(void *DS, int dataCenterID);

StatusType RequestServer(void *DS, int dataCenterID, int serverID, int os, int *assignedID);

StatusType FreeServer(void *DS, int dataCenterID, int serverID);

StatusType GetDataCentersByOS(void *DS, int os, int **dataCenters, int* numOfDataCenters);

void Quit(void** DS);

#ifdef __cplusplus
}
#endif

#endif    /*_234218_WET1 */
