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

#include "map_util.h"
static int gnNofMapTable;
static char gszErrStr[SIZE_ITEM_M];

void setNofMapTable(int nof_tab)
{
    gnNofMapTable = nof_tab;
    memset(gszErrStr, 0x00, sizeof(gszErrStr));
}

int getNofMapTable()
{
    return gnNofMapTable;
}

//
// Name         : loadVaryLenTable ()
//
// Description  : Loads Data Fields from Variable Field Length File into Memory Table.
//                Opens the Variable Field Length File.
//                Counts valid records and (Re)Allocates required memory to load those Records.
//                Determine each loading field size.
//                Reads the Records from Variable Field Length file until end of file.
//                  Skips Comment Records.
//                  Parses and copies fields.
//                  Adds to the Memory Table.
//                Closes the Variable Field Lendth File.
//                Sorts the Memory table by Specfic field depend on table type.
//
// Parameters   : int    idx - INPUT - Index of Variable Field Length Table in Memory Statistics.
//
// Return Value : SUCCESS         - on Successful completion.
//                FILE_OPEN_ERROR - on File open Error.
//                LOAD_TABLE_FULL - on Failure to allocate memory.
//                NO_CODE_LOADED  - on No Valid Code Loaded into memory.
//
int loadVaryLenTable(MAPD_STAT *pstMapdStat)
{
    FILE    *fp;
    char    szInput[SIZE_BUFF];
    char    *pstr=NULL;
    char    *pchData;
    char    *apchFld[MAX_LOAD_MAP_FIELD];
    char    **ppchHead=NULL;
    char    cComment;
    char    cDelimiter;
    int     aiSizeFld[MAX_LOAD_MAP_FIELD];
    int     iFldCnt, iFldLen, iRecCnt;
    int     iSizeHead, iSizeHeadRec, iSizeData, iSizeRec;
    int     iMinInputFld, iNofLoadFld;
    int     i, j;
    int     iTmp = 0;
    char    *pchTemp=NULL;        // Local Pointer to Variable Length Table

    if ( pstMapdStat->vl_info.iNofLoadFld <= 0 )
        return NO_CODE_LOADED;

    if ( pstMapdStat->vl_info.iNofLoadFld > MAX_LOAD_MAP_FIELD )
        return NO_CODE_LOADED;

    // Prepare Variable before Read Data
    cComment = pstMapdStat->vl_info.cComment;
    cDelimiter = pstMapdStat->vl_info.cDelimiter;
    iNofLoadFld = pstMapdStat->vl_info.iNofLoadFld;
    iMinInputFld = 0;
    for ( i = 0; i < iNofLoadFld; i++ ) {
        if ( iMinInputFld < pstMapdStat->vl_info.aiLoadFldId[i] ) {
            iMinInputFld = pstMapdStat->vl_info.aiLoadFldId[i];
        }
    }

    // open input Data file
    if ( (fp = fopen(pstMapdStat->szName, "r")) ==NULL )
        return FILE_OPEN_ERROR;

    // Count Valid Number of Records
    memset(aiSizeFld, 0x00, sizeof (aiSizeFld));
    iRecCnt = 0;
    while ( fgets ((char *)szInput, SIZE_BUFF, fp) != NULL ) {
        if ( *szInput == cComment )     // Comment Line not counted
            continue;

        iFldCnt = 0;
        while ( TRUE ) {
            if ( !iFldCnt )
                pstr = getStrTok(szInput, cDelimiter);
            else
                pstr = getStrTok((char *)NULL, cDelimiter);

            if ( !pstr )    // No more Field
                break;

            iFldCnt++;
            trimStr(pstr);
            iFldLen = strlen(pstr);

            // Find the Max Field Length
            for ( i = 0; i < iNofLoadFld; i++ ) {
                if ( iFldCnt == pstMapdStat->vl_info.aiLoadFldId[i] ) {
                    if ( iFldLen > aiSizeFld[i] ) {
                        aiSizeFld[i] = iFldLen;
                    }
                }
            }
        }
        if ( iFldCnt < iMinInputFld )
            continue;

        iRecCnt++;
    }
    if ( !iRecCnt )     // no valid records found
        return (NO_CODE_LOADED);

    // Calculate the memory size required
    iSizeHeadRec = sizeof(char *) * iNofLoadFld;
    iSizeHead = iSizeHeadRec * iRecCnt;
    iSizeRec = 0;
    for ( i = 0; i < iNofLoadFld; i++ ) {
        iSizeRec += aiSizeFld[i] + 1;
    }
    iSizeData = iRecCnt * iSizeRec;
    iTmp = iSizeHead + iSizeData;

    pchTemp = (char *)(*(pstMapdStat->ppTableHead));

    // (Re)Allocate memory required for all Records
    if ( !pstMapdStat->iNumLoads )      // First time loading
        pchTemp = (char *)malloc(iTmp);
    else    // Not First time
        pchTemp = (char *)realloc(pchTemp, iTmp);

    if ( pchTemp == (char *)NULL ) {
        return (LOAD_TABLE_FULL);
    }

    memset(pchTemp, 0x00, iTmp);
    pchData = ((char *)pchTemp) + iSizeHead;
    ppchHead = (char **)pchTemp;
    for ( i = 0; i<iRecCnt; i++ ) {
        pstr = pchData + (iSizeRec * i);
        for ( j = 0; j < iNofLoadFld; j++ ) {
            if ( j != 0 ) {
                pstr += aiSizeFld[j-1] + 1;
            }
            ppchHead[i * iNofLoadFld + j] = pstr;
        }
    }

    // Save the newly allocated memory Pointer
    *(pstMapdStat->ppTableHead) = (void *)pchTemp;

    pstMapdStat->iTotalRec = 0;     // Total Reting Element
    fseek (fp, 0, SEEK_SET);        // re-wind the file pointer

    iRecCnt = 0;
    while ( TRUE ) {
        // Initialize pchTemp buffers
        memset(szInput, 0x00, sizeof (szInput));

        // read one record
        if ( !fgets ((char *)szInput, SIZE_BUFF, fp) )   // eof
            break;

        if ( *szInput == cComment ) // Comment Line
            continue;               // No need to Parse

        iFldCnt = 0;
        while ( TRUE ) {
            if ( !iFldCnt )
                pstr = getStrTok(szInput, cDelimiter);
            else
                pstr = getStrTok((char *)NULL, cDelimiter);

            if ( !pstr )    // No more Field
                break;

            iFldCnt++;
            trimStr(pstr);

            // Keep the pointer to data
            for ( i = 0; i < iNofLoadFld; i++ ) {
                if ( iFldCnt == pstMapdStat->vl_info.aiLoadFldId[i] ) {
                    apchFld[i] = pstr;
                }
            }
        }
        if ( iFldCnt < iMinInputFld )
            continue;

        for ( i = 0; i < iNofLoadFld; i++ ) {
            strcpy(ppchHead[iRecCnt * iNofLoadFld + i], apchFld[i]);
        }
        iRecCnt++;
        pstMapdStat->iTotalRec++;

    }   // end of while
    fclose (fp);

    if ( !pstMapdStat->iTotalRec )      // no record loaded
        return NO_CODE_LOADED;


    // Reset the Local Pointer to the first record
    if ( pstMapdStat->pCmpFunc != NULL ) {
        pchTemp = (char *)(*(pstMapdStat->ppTableHead));
        qsort(pchTemp, pstMapdStat->iTotalRec, sizeof(char *) * iNofLoadFld, pstMapdStat->pCmpFunc);
    }

    pstMapdStat->cLoadStat = TRUE;  // successfully loaded
    pstMapdStat->iNumLoads++;       // count this loading

#ifdef SHOW_LOAD
if ( idx == 1 ) {
    for ( iTmp = 0; iTmp < pstMapdStat->iTotalRec; iTmp++ ) {
        for ( i = 0; i < iNofLoadFld-1; i++ ) {
            printf ("%s|",ppchHead[iTmp * iNofLoadFld + i]);
        }
        printf("%s\n",ppchHead[iTmp * iNofLoadFld + i]);
    }
}
#endif

    return SUCCESS;
}

// 
// Name         : chkMapdStat ()
// 
// Description  : Check for update file timestamp of required Mapping Data Table from $POST_MAP_DIR.
// 
// Parameters   : char *Msg - OUTPUT - Pointer to Error Message.
//                int *piUpd - OUTPUT - Number of file to be required update.
// 
// Return Value : On Successful completion (>=0)
//                On FAILURE               (-1)
// 
// 
int chkMapdStat(MAPD_STAT *pstMapdStat, int *piUpd)
{
    int idx = 0;
    int Upd = 0;
    struct stat stStat;

    for ( idx = 0; idx < gnNofMapTable; idx++ ) {
        // Check only when load flag is true
        if ( !pstMapdStat[idx].cLoadFlag ) {
            continue;   // No need to load, continue with other tables
        }

        // get last modified time of this file
        memset(&stStat, 0x00, sizeof (struct stat));
        if ( stat(pstMapdStat[idx].szName, &stStat) ) {     // error reading file status
            sprintf(gszErrStr, "unable to read %s (%s)", pstMapdStat[idx].szName, strerror(errno));
            return FAILED;
        }

        if ( stStat.st_mtime > pstMapdStat[idx].lLastModTime ) {    // modification time not same
            pstMapdStat[idx].lCurrModTime = stStat.st_mtime;        // save the file Time as Current Try
            pstMapdStat[idx].cLoadStat = FALSE;                     // (re)set status - to be loaded
            Upd++;
        }
    }
    strcpy(gszErrStr, "successfully checked status of mapping data tables");
    return Upd;
}

//
// Name         : loadMapdStat ()
//
// Description  : Loads required Mapping Data Table from $POST_MAP_DIR.
//                Calls appropriate Loading Functions to Load Mapping Table.
//
// Parameters   : char *Msg - OUTPUT - Pointer to Error Message
//
// Return Value : SUCCESS - on Successful completion
//                FAILURE - on any error.
//
int loadMapdTables(MAPD_STAT *pstMapdStat)
{
    int idx, result = SUCCESS;
    
    for ( idx = 0; idx < gnNofMapTable; idx++ ) {
        // if load flag is false then skip load
        if ( !pstMapdStat[idx].cLoadFlag ) {
            continue;
        }
        // if already load then skip load
        if ( pstMapdStat[idx].cLoadStat ) {
            continue;
        }
        
        switch ( pstMapdStat[idx].pLoadFunc(&pstMapdStat[idx]) ) {
            case SUCCESS:
                pstMapdStat[idx].lLastModTime = pstMapdStat[idx].lCurrModTime;  // save the file Time
                pstMapdStat[idx].lCurrModTime = 0;                              // reset current try Time
                pstMapdStat[idx].cLoadStat = TRUE;                              // successfully loaded
                sprintf(gszErrStr, "loaded %d recs from %s", pstMapdStat[idx].iTotalRec, pstMapdStat[idx].szName);
                break;
            case FILE_OPEN_ERROR:
                sprintf(gszErrStr, "unable to open %s (%s)", pstMapdStat[idx].szName, strerror(errno));
                if (!pstMapdStat[idx].iNumLoads) {      // Trying to load for the First time
                    result = FAILED;
                }
                break;  // we ignore this error and continue with the already loaded codes

            case NO_CODE_LOADED:
                sprintf(gszErrStr, "no code loaded from %s ", pstMapdStat[idx].szName);
                if ( !pstMapdStat[idx].iNumLoads ) {    // Trying to load for the First time
                    result = FAILED;
                }
                break;  // we ignore this error and continue with the already loaded codes

            case LOAD_TABLE_FULL:
                sprintf(gszErrStr, "not enough memory for %s (%s)", pstMapdStat[idx].szName, strerror(errno));
                result = FAILED;
                break;

            default:
                sprintf(gszErrStr, "load mapd table fail %s", pstMapdStat[idx].szName);
                result = FAILED;
                break;
        }
        if ( result != SUCCESS ) {
            break;
        }
    }

    if ( gcDebugFlag && result == SUCCESS ) {
        for ( idx = 0; idx < gnNofMapTable; idx++ ) {
            fprintf(stderr, "%d -> %-45s\t%-10ld\t%-10ld\t%-5s\t%-5s\t%d\t%d\n", idx, pstMapdStat[idx].szName,
                    pstMapdStat[idx].lLastModTime, pstMapdStat[idx].lCurrModTime,
                    ((pstMapdStat[idx].cLoadFlag) ? "TRUE" : "FALSE"),
                    ((pstMapdStat[idx].cLoadStat) ? "TRUE" : "FALSE"),
                    pstMapdStat[idx].iTotalRec, pstMapdStat[idx].iNumLoads);
        }
    }
    return result;

}

//
// Name         : freeMapdTables ()
//
// Description  : Release and free memory of all Mapping table.
//
// Parameters   : None
//
// Return Value : None
//
void freeMapdTables (MAPD_STAT *pstMapdStat)
{
    int idx = 0;
    for ( idx = 0; idx < gnNofMapTable; idx++ ) {
        if ( pstMapdStat[idx].cLoadFlag ) {
            free(*(pstMapdStat[idx].ppTableHead));
            pstMapdStat[idx].iTotalRec = 0;    // Reset
        }
    }
}


char *getErrStr()
{
    return gszErrStr;
}
