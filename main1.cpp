/***************************************************************************/
/*                                                                         */
/* 234218 Data DSs 1, Winter 2019-2020                                     */
/* Homework : Wet 1                                                        */
/*                                                                         */
/***************************************************************************/

/***************************************************************************/
/*                                                                         */
/* File Name : main1.cpp                                                   */
/*                                                                         */
/* Holds the "int main()" function and the parser of the shell's           */
/* command line.                                                           */
/***************************************************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library1.h"

#ifdef __cplusplus
extern "C" {
#endif

/* The command's strings */
typedef enum {
    NONE_CMD = -2,
    COMMENT_CMD = -1,
    INIT_CMD = 0,
	ADDDATACENTER_CMD = 1,
	REMOVEDATACENTER_CMD = 2,
	REQUESTSERVER_CMD = 3,
	FREESERVER_CMD = 4,
	GETDATACENTERSBYOS_CMD = 5,
    QUIT_CMD = 6
} commandType;

static const int numActions = 9;
static const char *commandStr[] = {
        "Init",
        "AddDataCenter",
        "RemoveDataCenter",
        "RequestServer",
        "FreeServer",
        "GetDataCentersByOS",
        "Quit" };

static const char* ReturnValToStr(int val) {
    switch (val) {
        case SUCCESS:
            return "SUCCESS";
        case ALLOCATION_ERROR:
            return "ALLOCATION_ERROR";
        case FAILURE:
            return "FAILURE";
        case INVALID_INPUT:
            return "INVALID_INPUT";
        default:
            return "";
    }
}

/* we assume maximum string size is not longer than 256  */
#define MAX_STRING_INPUT_SIZE (255)
#define MAX_BUFFER_SIZE       (255)

#define StrCmp(Src1,Src2) ( strncmp((Src1),(Src2),strlen(Src1)) == 0 )

typedef enum {
    error_free, error
} errorType;
static errorType parser(const char* const command);

#define ValidateRead(read_parameters,required_parameters,ErrorString,ErrorParams) \
if ( (read_parameters)!=(required_parameters) ) { printf(ErrorString, ErrorParams); return error; }

static bool isInit = false;

/***************************************************************************/
/* main                                                                    */
/***************************************************************************/

int main(int argc, const char**argv) {

    char buffer[MAX_STRING_INPUT_SIZE];

    // Reading commands
    while (fgets(buffer, MAX_STRING_INPUT_SIZE, stdin) != NULL) {
        fflush(stdout);
        if (parser(buffer) == error)
            break;
    };
    return 0;
}

/***************************************************************************/
/* Command Checker                                                         */
/***************************************************************************/

static commandType CheckCommand(const char* const command,
                                const char** const command_arg) {
    if (command == NULL || strlen(command) == 0 || StrCmp("\n", command))
        return (NONE_CMD);
    if (StrCmp("#", command)) {
        if (strlen(command) > 1)
            printf("%s", command);
        return (COMMENT_CMD);
    };
    for (int index = 0; index < numActions; index++) {
        if (StrCmp(commandStr[index], command)) {
            *command_arg = command + strlen(commandStr[index]) + 1;
            return ((commandType)index);
        };
    };
    return (NONE_CMD);
}

/***************************************************************************/
/* Commands Functions                                                      */
/***************************************************************************/

static errorType OnInit(void** DS, const char* const command);
static errorType OnAddDataCenter(void* DS, const char* const command);
static errorType OnRemoveDataCenter(void* DS, const char* const command);
static errorType OnRequestServer(void* DS, const char* const command);
static errorType OnFreeServer(void* DS, const char* const command);
static errorType OnGetDataCentersByOS(void* DS, const char* const command);
static errorType OnQuit(void** DS, const char* const command);

/***************************************************************************/
/* Parser                                                                  */
/***************************************************************************/

static errorType parser(const char* const command) {
    static void *DS = NULL; /* The general data structure */
    const char* command_args = NULL;
    errorType rtn_val = error;

    commandType command_val = CheckCommand(command, &command_args);

    switch (command_val) {

        case (INIT_CMD):
            rtn_val = OnInit(&DS, command_args);
            break;
        case (ADDDATACENTER_CMD):
            rtn_val = OnAddDataCenter(DS, command_args);
            break;
        case (REMOVEDATACENTER_CMD):
            rtn_val = OnRemoveDataCenter(DS, command_args);
            break;
        case (REQUESTSERVER_CMD):
            rtn_val = OnRequestServer(DS, command_args);
            break;
        case (FREESERVER_CMD):
            rtn_val = OnFreeServer(DS, command_args);
            break;
        case (GETDATACENTERSBYOS_CMD):
            rtn_val = OnGetDataCentersByOS(DS, command_args);
            break;
        case (QUIT_CMD):
            rtn_val = OnQuit(&DS, command_args);
            break;

        case (COMMENT_CMD):
            rtn_val = error_free;
            break;
        case (NONE_CMD):
            rtn_val = error;
            break;
        default:
            assert(false);
            break;
    };
    return (rtn_val);
}

static errorType OnInit(void** DS, const char* const command) {
    if (isInit) {
        printf("init was already called.\n");
        return (error_free);
    };
    isInit = true;

    ValidateRead(sscanf(command, ""), 0, "%s failed.\n", commandStr[INIT_CMD]);
    *DS = Init();

    if (*DS == NULL) {
        printf("init failed.\n");
        return error;
    };

    printf("init done.\n");
    return error_free;
}

static errorType OnAddDataCenter(void* DS, const char* const command) {
    int dataCenterID, numOfServers;
    ValidateRead(sscanf(command, "%d %d", &dataCenterID, &numOfServers), 2, "%s failed.\n", commandStr[ADDDATACENTER_CMD]);
    StatusType res = AddDataCenter(DS, dataCenterID, numOfServers);

    if (res != SUCCESS) {
        printf("%s: %s\n", commandStr[ADDDATACENTER_CMD], ReturnValToStr(res));
        return error_free;
    }

    printf("%s: %s\n", commandStr[ADDDATACENTER_CMD], ReturnValToStr(res));
    return error_free;
}

static errorType OnRemoveDataCenter(void* DS, const char* const command) {
    int dataCenterID;
    ValidateRead(sscanf(command, "%d", &dataCenterID), 1, "%s failed.\n", commandStr[REMOVEDATACENTER_CMD]);
    StatusType res = RemoveDataCenter(DS, dataCenterID);

    if (res != SUCCESS) {
        printf("%s: %s\n", commandStr[REMOVEDATACENTER_CMD], ReturnValToStr(res));
        return error_free;
    }

    printf("%s: %s\n", commandStr[REMOVEDATACENTER_CMD], ReturnValToStr(res));
    return error_free;
}

static errorType OnRequestServer(void* DS, const char* const command) {
    int dataCenterID, serverID, os, assignedID;
    ValidateRead(sscanf(command, "%d %d %d", &dataCenterID, &serverID, &os), 3, "%s failed.\n", commandStr[REQUESTSERVER_CMD]);
    StatusType res = RequestServer(DS, dataCenterID, serverID, os, &assignedID);

    if (res != SUCCESS) {
        printf("%s: %s\n", commandStr[REQUESTSERVER_CMD], ReturnValToStr(res));
        return error_free;
    }

    printf("%s: %d\n", commandStr[REQUESTSERVER_CMD], assignedID);
    return error_free;
}

static errorType OnFreeServer(void* DS, const char* const command) {
    int dataCenterID, serverID;
    ValidateRead(sscanf(command, "%d %d", &dataCenterID, &serverID), 2, "%s failed.\n", commandStr[FREESERVER_CMD]);
    StatusType res = FreeServer(DS, dataCenterID, serverID);

    if (res != SUCCESS) {
        printf("%s: %s\n", commandStr[FREESERVER_CMD], ReturnValToStr(res));
        return error_free;
    }

    printf("%s: %s\n", commandStr[FREESERVER_CMD], ReturnValToStr(res));
    return error_free;
}

static errorType OnGetDataCentersByOS(void* DS, const char* const command) {
    int os, numOfDataCenters;
    int *dataCenters;
    ValidateRead(sscanf(command, "%d", &os), 1, "%s failed.\n", commandStr[GETDATACENTERSBYOS_CMD]);
    StatusType res = GetDataCentersByOS(DS, os, &dataCenters, &numOfDataCenters);

    if (res != SUCCESS) {
        printf("%s: %s\n", commandStr[GETDATACENTERSBYOS_CMD], ReturnValToStr(res));
        return error_free;
    }

    printf("%s: %s\n", commandStr[GETDATACENTERSBYOS_CMD], ReturnValToStr(res));

    for (int i = 0; i < numOfDataCenters; i++)
    {
        printf("%d\n", dataCenters[i]);
    }

    printf("--End of data center list--\n");

    free(dataCenters);
    return error_free;
}

static errorType OnQuit(void** DS, const char* const command) {
    Quit(DS);
    if (*DS != NULL) {
        printf("quit failed.\n");
        return error;
    };

    isInit = false;
    printf("quit done.\n");
    return error_free;
}

#ifdef __cplusplus
}
#endif
