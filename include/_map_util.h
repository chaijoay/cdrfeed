//
//   © Copyright 2005 Hewlett-Packard Development Company, L.P.
//
//   Restricted Rights:
//   Confidential computer software.  Valid license from Hewlett-Packard
//   required for possession, use or copying.  Consistent with FAR 12.211
//   and 12.212, Commercial Computer Software, Computer Software
//   Documentation, and Technical Data for Commercial Items are licensed to
//   the U.S. Government under vendor's standard commercial license.
//
//   This software contains proprietary information which is protected by
//   copyright.  All rights are reserved.  No part of this software may be
//   photocopied, reproduced or translated to another language without the
//   prior written consent of Hewlett-Packard Company.
//

#ifndef __MAP_UTIL_H__
#define __MAP_UTIL_H__

#include "strlogutl.h"
#include "glb_str_def.h"

#define MAX_LOAD_MAP_FIELD      32

#define FILE_STAT_ERROR         101     // File Status Error
#define FILE_OPEN_ERROR         102     // File Open Error
#define LOAD_TABLE_FULL         103     // Memory Not Enough
#define NO_CODE_LOADED          104     // No Code Loaded into Memory

typedef struct mapd_stat {
    char    szName[SIZE_FNAME];         // Mapping Data File Name including path or Table Name
    time_t  lLastModTime;               // Last Known Successful File time
    time_t  lCurrModTime;               // Current File time - Trying to load
    int     (*pLoadFunc)(struct mapd_stat *);           // Pointer to Loading Function
    int     (*pCmpFunc)(const void *, const void *);    // Pointer to Compare Function
    char    cLoadFlag;                  // Loading Flag : Installation dependent (based on INSTALLATION_NAME in INI FILE)
                                        //      FALSE -> Do not load for this installation
                                        //      TRUE -> Load for this installation
                                        //
    char    cLoadStat;                  // Loading Status : FALSE -> To be loaded, TRUE -> Already loaded
    int     iTotalRec;                  // Total Records Loaded
    int     iNumLoads;                  // Number of Loadings : 0 -> Not yet loaded, Non-Zero -> loaded n times
    void    **ppTableHead;              // Pointer to Head Table Pointer */     /* Added by Kawee on 22-Jun-2007
    struct {
        char cComment;                  // Comment Character
        char cDelimiter;                // Delimiter Character
        int  iNofLoadFld;               // Number of Field to be load
        int  aiLoadFldId[MAX_LOAD_MAP_FIELD];    // Array of Loaded Field Id
    } vl_info;
} MAPD_STAT;

#ifdef  __cplusplus
extern "C" {
#endif

void    setNofMapTable(int nof_tab);
int     getNofMapTable();
int     loadVaryLenTable(MAPD_STAT *pstMapdStat);
int     chkMapdStat(MAPD_STAT *pstMapdStat);
int     loadMapdTables(MAPD_STAT *pstMapdStat);
void    freeMapdTables(MAPD_STAT *pstMapdStat);
char    *getErrStr();

#ifdef  __cplusplus
}
#endif

#endif /* __MAP_UTIL_H__ */
