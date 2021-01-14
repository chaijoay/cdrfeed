/*
**++
**  FACILITY    : CDR Feed Process
**
**  FILE NAME   : frm_cdr_feed.c
**
**  ABSTRACT    : Main Program for CDR Feed Process.
**
**  AUTHOR  : Palanichamy.M.
**
**  CREATE DATE : 5-Sep-1997
**
**  CURRENT VERSION NO : 12.1
**
**  LAST RELEASE DATE  : Sep-2016
**
*/

#include <dirent.h>     // for opendir, readdir function
#include <sys/stat.h>   // for lstat function
#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <fnmatch.h>

#include "frm_cdr_feed.h"

#include "frm_cdr_glob.h"
#include "frm_cdr_stru.h"
#include "frm_cdr_mapp.h"

#include "minIni.h"
#include "procsig.h"
#include "strlogutl.h"


extern int errno;
extern COM_RECORD com_buf;                  // global buffer for Common Structure Record

CHECK_RECORD chk_buf;                       // global buffer for Check Point Record
SNAP_INFO    glb_SnapInfo[NOF_CDR_PATH];    // global buffer for SnapInfo Records

char gszIniFullName[SIZE_INIREC];
char glb_CdrBck4IUM_Dir[SIZE_FNAME+1];


char gszMtxTypeName[NOF_MTX][10] = {
    "UNKNOWN", "SSP", "MSCPPS", "GSM", "SMC", "SMS", "USC", "GPR", "GSMBC", "PPSBC", "GSMTC", "PPSTC", "GSMCT", "PPSCT", "RBT", "CCUDR", "SMSFDA",
    "AWN", "AMF", "DPCGSM", "DPCAIS", "DPCTAC", "DPCGPR", "DPCSMS", "SDG", "AWNSMS", "AWNGPR", "DCB", "DCBCT", "OCSVPN", "AIN", "NRTRDE", "NRGGSN", "NRTSCP",
    "GSMTOP", "IMS", "RDC", "RDCSMS", "RBM"
};

// Splitting PPS AWN CDR - by Thanakorn on 07-Mar-2013
char glb_PpsType[NOF_PPSAWN_PATH][10+1] = {
    "AWN",
    "SMS",
    "GPRS"
};

int main(int argc, char **argv)
{
    int Updates = 0;
    int Can_Continue = 0;
    int Retry_Count = 0;
    int i = 0;

    char Snap_Fname[SIZE_FNAME];
    char Temp[SIZE_FNAME];
    char szToday[9];

    memset(glb_MsgText, 0x00, sizeof (glb_MsgText));
    memset(Snap_Fname, 0x00, sizeof (Snap_Fname));
    memset(Temp, 0x00, sizeof (Temp));
    memset(&glb_SnapInfo, 0x00, sizeof (glb_SnapInfo));
    memset(szToday, 0x00, sizeof(szToday));

    strcpy(szToday, getSysDTM(DTM_DATE_ONLY));

    // ----- Check -debug parameter -----
    glb_DbgFlag = FALSE;
    glb_CdrFile_Limit = 0;
    if ( argc > 1 ) {
        for ( i = 1; i < argc; i++ ) {
            if ( !strcmp(argv[i], "-debug") ) {
                glb_DbgFlag = TRUE;
                printf("Running in debug mode...\n");
            }
            /* Added Argument "-count=" as below - by Kawee on 23-Apr-2004 */
            if ( !strncmp(argv[i], "-count=", 7) ) {
                glb_CdrFile_Limit = atol(argv[i]+7);
                glb_CdrFile_Limit = ( glb_CdrFile_Limit>0 ? glb_CdrFile_Limit : 0 );
            }
        }
    }

    // ----- Initialize Cdr Feeder -----
    if ( Init_Main(argv[0]) ) {
        writeLog(LOG_INF, "------------------ exit %s ------------------", _APP_NAME_);
        Close_Main();
        return FAILURE;
    }

    // ----- Added Argument "-count=" as below - by Kawee on 23-Apr-2004 -----
    writeLog(LOG_INF, "ARGUMENT count -> %ld", glb_CdrFile_Limit);

    // ----- Load System Parameters from Ini File -----
    if ( Load_SysParameters(argv) ) {
        writeLog(LOG_INF, "------------------ exit %s ------------------", _APP_NAME_);
        Close_Main();
        return FAILURE;
    }

    // ----- Prepare Snapshot Info -----
    strcpy(glb_SnapInfo[CDR_LOCAL].sync_filedir, glb_Syncfile_Dir[CDR_LOCAL]);
    strcpy(glb_SnapInfo[CDR_LOCAL].sync_pattern, "*.syn");
    strcpy(glb_SnapInfo[CDR_NRTRDE].sync_filedir, glb_Syncfile_Dir[CDR_NRTRDE]);
    strcpy(glb_SnapInfo[CDR_NRTRDE].sync_pattern, "*.syn");

    // ----- Initialize Mapping Data Files Loading Statistics -----
    if ( Init_MapdStat() ) {
        writeLog(LOG_INF, "------------------ exit %s ------------------", _APP_NAME_);
        Close_Main();
        return FAILURE;
    }

    writeLog(LOG_INF, "load the following mapping file");
    // ----- Load Mapping Data Files into Memory -----
    if ( Load_MapdTables() ) {
        writeLog(LOG_INF, "------------------ exit %s ------------------", _APP_NAME_);
        Close_Main();
        return FAILURE;
    }

    // ----- Analyze the Previous Check Point File -----
    writeLog(LOG_INF, "analyzing for upnormal/half way process termination");

    switch ( Anz_Check_File() ) {
        case FAILURE :
            writeLog(LOG_INF, "------------------ exit %s ------------------", _APP_NAME_);
            Close_Main();
            return FAILURE;

        case ANZ_MAP_HALF :

            // ----- Start the Recovery Processing on CDR File -----
            Start_Recovery();
            // ----- check for shutdown signal during Recovery Processing -----
            if ( cGblShutdown ) {
                writeLog(LOG_INF, "shutdown signal received");
                writeLog(LOG_INF, "------------------ terminate %s ------------------", _APP_NAME_);
                Close_Main();
                return SUCCESS;
            }
            break;

        case ANZ_GO_AHEAD :
            break;
    }

    // ----- Log a message -----
    writeLog(LOG_INF, "continue normal processing");

    // ----- Prepare Snapfile Name -----
    sprintf(Snap_Fname, "%s/CDR_SnapXXXXXX", glb_Errlog_Dir);

    strcpy(Temp, Snap_Fname);
    mkstemp(Temp);
    if ( *Temp == '\0' ) {  /* mkstemp failed */
        writeLog(LOG_SYS, "mkstemp failed %s (%s)", Snap_Fname, strerror(errno));
        Close_Main();
        return FAILURE;
    }
    strcpy(Snap_Fname, Temp);

    Can_Continue = TRUE;
    Retry_Count = 0;
    glb_CdrFile_Count = 0;      /* Added Argument "-count=" - by Kawee on 23-Apr-2004 */

    while ( Can_Continue ) {
        // ----- check for shutdown signal -----
        if ( cGblShutdown ) {
            writeLog(LOG_INF, "shutdown signal received");
            writeLog(LOG_INF, "------------------ terminate %s ------------------", _APP_NAME_);
            Can_Continue = FALSE;
            break;
        }

        // ----- check for Mapping Data file(s) updates -----
        //
        // During runtime, Operator may rename the Mapping Data file(s) to create updated Mapping Data files
        //
        if ( !Check_MapdStat(&Updates) ) { /* success */
            if ( Updates ) {    /* Atleast one Mapping Data file found updated */
                // ----- Re-Load those updated Mapping Data File(s) into Memory -----
                writeLog(LOG_INF, "reload the following mapping data files");
                if ( Load_MapdTables() ) {
                    Can_Continue = FALSE;
                    break;
                }
            }
        }

        // ----- build the CDR snapshot file -----
        if ( !Bld_Snap_File(Snap_Fname) ) {    /* Build Success */
            Deb_Msg("frm_cdr_feed: Build success.");
            Retry_Count = 0;    /* reset */

            // ----- Process the Snapshot file -----
            if ( Proc_Snapshot(Snap_Fname) == FAILURE ) {    /* problem in processing */
                Can_Continue = FALSE;
                break;
            }

            // ----- Added Argument "-count=" as below - by Kawee on 23-Apr-2004 -----
            // ----- Check Number of processed CDR File Reach Limit then go to Sleep -----
            if ( glb_CdrFile_Limit > 0 && glb_CdrFile_Count >= glb_CdrFile_Limit ) {
                writeLog(LOG_INF, "reach file count (%ld) : sleep for %ld sec", glb_CdrFile_Count, glb_CdrFeed_Sleep);
                unlink(Snap_Fname);    /* clean-up the Snapshot file */
                glb_CdrFile_Count = 0;  /* Before go to sleep reset the CdrFile Counter */
                SnoozeSleep((unsigned int)glb_CdrFeed_Sleep);
            }
        }
        else {  // ----- Build Failed -----
            if ( errno == ENOENT ) {
                unlink(Snap_Fname);     /* clean-up the Snapshot file */
                Retry_Count++;
                if ( Retry_Count > 100 ) {
                    // ----- log message with frm after 100 tries -----
                    writeLog(LOG_INF, "no sync file in %s", glb_Syncfile_Dir[CDR_LOCAL]);
                    Retry_Count = 0;    /* reset */
                }
            }

            // ----- Sleep for specified seconds -----
            writeLog(LOG_INF, "sleep for %ld sec", glb_CdrFeed_Sleep);
            glb_CdrFile_Count = 0;  /* Before go to sleep reset the CdrFile Counter */
            SnoozeSleep((unsigned int)glb_CdrFeed_Sleep);
        }

        if ( strcmp(szToday, getSysDTM(DTM_DATE_ONLY)) ) {  // rotate log file
            strcpy(szToday, getSysDTM(DTM_DATE_ONLY));
            manageLogFile();
        }
    }   /* end while */

    Release_MapdTables();   /* Free Memory Tables */
    Close_Main();
    unlink(Snap_Fname);

    return SUCCESS;

}

/*
** Name     : Proc_Snapshot()
**
** Description  : Process Snapshot File.
**        Opens the specified Snapshot file.
**        Check for Shutdown Indicator after every sync file processed.
**                Reads Sync file names one by one untile eof.
**        Calls the Proc_SyncFile () to Read the Sync file.
**        Calls the Proc_CdrFile () to Process CDR file.
**        Closes the Snapshot file.
**
** Parameters   : char *Msg - INPUT - Pointer to Snapshot file name
**
** Return Value : SUCCESS - on Successful completion.
**        FAILURE - on processing Failures.
**        SHUT_SIGNAL_RCVD - on receipt of Shutdown Signal.
*/

int Proc_Snapshot(char *Snap_name)
{
    int  Updates = 0;
    int  Can_Process;
    FILE *snap_fp;
    char Temp[SIZE_FNAME];

    memset(glb_MsgText, 0x00, sizeof (glb_MsgText));
    memset(Temp, 0x00, sizeof (Temp));

    /* open the snapshot file */
    if ( (snap_fp = fopen(Snap_name, MODE_READ)) == NULL ) {
        writeLog(LOG_SYS, "cannot open snapshot %s (%s)", Snap_name, strerror(errno));
        return FAILURE;
    }

    /* read snapshot file and process until eof */
    Can_Process = TRUE;
    while ( Can_Process ) {

        /* check for shutdown indicator before start processing sync files */
        if ( cGblShutdown ) {
            fclose(snap_fp);   /* close the Snapshot file */
            unlink(Snap_name); /* clean-up the Snapshot file */
            return SHUT_SIGNAL_RCVD;
        }

        /* check for Mapping Data file(s) updates */
        /*
        ** During runtime, Operator may rename the Mapping Data file(s) to create updated Mapping Data files.
        */
        if ( !Check_MapdStat(&Updates) ) {  /* success */
            if ( Updates ) {  /* Atleast one Mapping Data file found updated */
                /* Re-Load those updated Mapping Data File(s) into Memory */
                if ( Load_MapdTables() ) {
                    return FAILURE;
                }
            }
        }

        /* read the (next) Sync File Name from the snapshot file */
        /* the sync file name present in the snapshot file contains path */
        /* Changed as below for using opendir while build snapshot - by Kawee on 02-Jan-2003 */
        if ( Get_Sync_Fname(snap_fp, &glb_CdrPathType, glb_Sync_File) ) {    /* End of Snapshort File */
            sprintf(glb_MsgText, "Proc_Snapshot : EOF on Snapshot.");
            Deb_Msg(glb_MsgText);
            Can_Process = FALSE;
            break;
        }
        sprintf(glb_MsgText, "\nProc_Snapshot: The Sync file name is = %s", glb_Sync_File);
        Deb_Msg(glb_MsgText);

        if ( Proc_SyncFile() ) {
            /* on error processing sync file */
            /* continue with remaining sync files present in Snapshot */
            continue;
        }

        /* Set the Process Status Flag to Indicate Normal Processing */
        glb_ProcStat = NORM_PROC;

        switch ( Proc_CdrFile() ) {
            case SUCCESS :  /* Successfull Processing */
                DoBackupCdr(glb_Cdr_File, glb_Sync_File);   /* Do Backup CDR and Sync file */
                unlink(glb_Sync_File); /* Remove the Sync File */
                unlink(glb_Cdr_File);  /* Remove the CDR File */

                /* Added Argument "-count=" as below - by Kawee on 23-Apr-2004 */
                glb_CdrFile_Count++;

                /* Check Number of processed CDR File Reach Limit then go to Sleep */
                if ( glb_CdrFile_Limit > 0 && glb_CdrFile_Count >= glb_CdrFile_Limit ) {
                    fclose(snap_fp);   /* Close the Snapshot file */
                    unlink(Snap_name); /* clean-up the Snapshot file */
                    Can_Process = FALSE;
                    return SUCCESS;
                }
                continue;       /* continue with remaining sync files present in Snapshot */

            case ERROR_FOPEN_FAIL :    /* These Errors can only occur on unix */
            case REJCDR_FOPEN_FAIL :   /* file system error etc.              */
            case CHK_FOPEN_FAIL :      /* cannot Continue CDR Feed Process     */
            case EALM_FOPEN_FAIL :
            /* case GSMUNK_FOPEN_FAIL : */
            case ISAM_FOPEN_FAIL :
            case EVENTCDR_FOPEN_FAIL :
            case EVENTSYNC_CREATE_FAIL :
                fclose(snap_fp);   /* Close the Snapshot file */
                unlink(Snap_name); /* clean-up the Snapshot file */
                Can_Process = FALSE;
                return FAILURE;

            case CDR_FOPEN_FAIL :
                /* rename the sync file to indicate error */
                sprintf(Temp, "%s_ERR", glb_Sync_File);
                rename(glb_Sync_File, Temp);
                /* continue with remaining sync files present in Snapshot */
                continue;

            case SHUT_SIGNAL_RCVD :
                fclose(snap_fp);   /* Close the Snapshot file */
                unlink(Snap_name); /* clean-up the Snapshot file */
                Can_Process = FALSE;
                return SHUT_SIGNAL_RCVD; /* Pass-on the return value */

            case FAILURE :
                /* in this cases the CDR Processing Failed */
                writeLog(LOG_ERR, "cdr processing failed %s", glb_Cdr_File);
                /* rename the sync file to indicate error */
                sprintf(Temp, "%s_ERR", glb_Sync_File);
                rename(glb_Sync_File , Temp);
                /* continue with remaining sync files present in Snapshot */
                continue;
        }
    } /* end while */

    fclose(snap_fp);   /* Close the Snapshot file */
    unlink(Snap_name);     /* clean-up the Snapshot file */

    return SUCCESS;

}

/*
** Name     : Proc_SyncFile()
**
** Description  : Process Sync File.
**        Opens the specified Sync file.
**                Reads Sync file content.
**        Validates Mtx Type.
**        Closes the Sync file.
**
** Parameters   : None.
**
** Return Value : SUCCESS - on Successful completion.
**        FAILURE - on any Error.
*/

int Proc_SyncFile()
{
    FILE *sync_fp;
    SYNC_RECORD sync_buf;
    char sync_content[SIZE_SYNCREC+1];
    char Temp[SIZE_FNAME];
    int i;
    char    *pstr;

    memset(glb_MsgText, 0x00, sizeof(glb_MsgText));
    memset(&sync_buf, 0x00, sizeof(sync_buf));
    memset(sync_content, 0x00, sizeof(sync_content));
    memset(Temp, 0x00, sizeof(Temp));
    memset(glb_Msc_Name, 0x00, sizeof(glb_Msc_Name)); /* Migrated CAMEL V.5 - Added below by Kawee on 04-Jun-2007 */
    memset(glb_NfsType, 0x00, sizeof(glb_NfsType));

    if ( glb_CdrPathType == CDR_NRTRDE ) {
        strcpy(sync_buf.file_name, Remove_Path(glb_Sync_File));
        if ( (pstr = (char *)strstr(sync_buf.file_name, ".syn")) != NULL ) {
            *pstr = '\0';
            strcat(sync_buf.file_name, ".dat");
        }
        else {
            strcpy(sync_buf.file_name, "");
        }
        strcpy(sync_buf.time_stamp, "");
        if ( !strncmp(sync_buf.file_name, "GGSN", 4) ) {
            strcpy(sync_buf.mtx_id, "NRGGSN");
            strcpy(sync_buf.mtx_type, "NRGGSN");
        }
        else if ( !strncmp(sync_buf.file_name, "SCP", 3) ) {
            strcpy(sync_buf.mtx_id, "NRTSCP");
            strcpy(sync_buf.mtx_type, "NRTSCP");
        }
        else {
            strcpy(sync_buf.mtx_id, "NRTRDE");
            strcpy(sync_buf.mtx_type, "NRTRDE");
        }
        strcpy(glb_NfsType, "NRTRDE");
    }
    else {
        /* now open the sync file */
        if ( (sync_fp = fopen(glb_Sync_File, MODE_READ)) == NULL ) {
            writeLog(LOG_SYS, "cannot open syncfile %s (%s)", glb_Sync_File, strerror(errno));
            return FAILURE;
        }

        /* read the sync file content */
        if ( !fgets((char *)sync_content, SIZE_SYNCREC, sync_fp) ) {
            /* No Sync record found */
            writeLog(LOG_INF, "syncfile empty %s", glb_Sync_File);

            /* rename the sync file to indicate error */
            fclose(sync_fp);
            sprintf(Temp, "%s_ERR", glb_Sync_File);
            rename(glb_Sync_File , Temp);

            return FAILURE;
        }
        /* now close the sync file */
        fclose(sync_fp);
#if 0
        strncpy(sync_buf.mtx_id, sync_content, 20);
        strncpy(sync_buf.mtx_type, sync_content+20, 6);     /* Resized mtx_id to 20 Chars - by Kawee on 28-Mar-2003 */
        strncpy(sync_buf.file_name, sync_content+26, 80);
        strncpy(sync_buf.time_stamp, sync_content+106, 19);
        strncpy(glb_NfsType, sync_content+125, 6);
#endif
        int last_idx = strlen(sync_content)-1;
        char last_chr = sync_content[last_idx];
        if ( last_chr == '\n' || last_chr == '\r' ) {
            sync_content[last_idx] = 0;
        }

        if ( strstr(sync_content, "|") != NULL ) {
            getTokenItem(sync_content, 1, '|', sync_buf.mtx_id);
            getTokenItem(sync_content, 2, '|', sync_buf.mtx_type);
            getTokenItem(sync_content, 3, '|', sync_buf.file_name);
            getTokenItem(sync_content, 6, '|', sync_buf.time_stamp);
            getTokenItem(sync_content, 7, '|', glb_NfsType);
        }
        else {
            strncpy(sync_buf.mtx_id, sync_content, 20);
            strncpy(sync_buf.mtx_type, sync_content+20, 6);     /* Resized mtx_id to 20 Chars - by Kawee on 28-Mar-2003 */
            strncpy(sync_buf.file_name, sync_content+26, 80);
            strncpy(sync_buf.time_stamp, sync_content+107, 19);
            strncpy(glb_NfsType, sync_content+126, 6);
        }
    }

    /* Trim Trailing Spaces on String Fields */
    Trim_Str(sync_buf.mtx_id);
    Trim_Str(sync_buf.mtx_type);
    Trim_Str(sync_buf.file_name);
    Trim_Str(sync_buf.time_stamp);
    memset(glb_EventSource, 0x00, sizeof(glb_EventSource));
    strcpy(glb_EventSource, sync_buf.mtx_type);

    sprintf(glb_MsgText, "Proc_SyncFile: file_name=%s, mtx_id=%s, mtx_type=%s, time_stamp=%s", sync_buf.file_name, sync_buf.mtx_id, sync_buf.mtx_type, sync_buf.time_stamp);
/* writeLog(LOG_INF, "Proc_SyncFile: file_name=%s, mtx_id=%s, mtx_type=%s, time_stamp=%s", sync_buf.file_name, sync_buf.mtx_id, sync_buf.mtx_type, sync_buf.time_stamp); */
    Deb_Msg(glb_MsgText);

    /* validate the mtx_id */
    if ( !strncmp(sync_buf.mtx_id, "AIS.", 4) ) {
        glb_PartId = AWN_SVC_PARTITION;    /* Set Default value as Postpaid Service Partition */
        strncpy(glb_Msc_Name, sync_buf.file_name+4, SIZE_MSC_NAME);    /* Migrated CAMEL V.5 - Added below by Kawee on 11-Dec-2007 */
        /* Find End of MSC name and replance will NULL */
        for ( i = 0; i < SIZE_MSC_NAME; i++ ) {
            if ( glb_Msc_Name[i] == '_' ) {
                glb_Msc_Name[i] = '\0';
                break;
            }
        }

        /* validate the mtx_type */
        if ( !strncmp(sync_buf.mtx_type, "SMC", 3) )    /* SMC Type (SMC) */
            glb_MtxInd = SMC_TYPE;          /* added by Jetsada on 15-Aug-2005 */

        else if ( !strncmp(sync_buf.mtx_type, "GSMBC", 5) )     /* GSMBC Type (GSM GPRS time based bearier) */
            glb_MtxInd = GSMBC_TYPE;        /* added by Kawee on 20-May-2004 */

        else if ( !strncmp(sync_buf.mtx_type, "PPSBC", 5) )     /* PPSBC Type (PPS GPRS time based bearier) */
            glb_MtxInd = PPSBC_TYPE;        /* added by Kawee on 20-May-2004 */

        else if ( !strncmp(sync_buf.mtx_type, "GSMTC", 5) )    /* GSMTC Type (GSM MMS) */
            glb_MtxInd = GSMTC_TYPE;        /* added by Kawee on 05-Nov-2004 */

        else if ( !strncmp(sync_buf.mtx_type, "PPSTC", 5) )     /* PPSTC Type (PPS MMS) */
            glb_MtxInd = PPSTC_TYPE;        /* added by Kawee on 05-Nov-2004 */

        else if ( !strncmp(sync_buf.mtx_type, "GSMCT", 5) )     /* GSMCT Type (CDG) */
            glb_MtxInd = GSMCT_TYPE;        /* added by Kawee on 02-Jun-2005 */

        else if ( !strncmp(sync_buf.mtx_type, "PPSCT", 5) )     /* PPSCT Type (CDG) */
            glb_MtxInd = PPSCT_TYPE;        /* added by Kawee on 23-Aug-2005 */

        else if ( !strncmp(sync_buf.mtx_type, "GSMCC", 5) )     /* added CCUDR Type (SESSION) - by Jetsada on 14-Sep-2007 */
            glb_MtxInd = CCUDR_TYPE;

        else if ( !strncmp(sync_buf.mtx_type, "PPSCC", 5) )     /* added CCUDR Type (SESSION) - by Jetsada on 14-Sep-2007 */
            glb_MtxInd = CCUDR_TYPE;

        else if ( !strncmp(sync_buf.mtx_type, "GSMTOP", 6) )    /* GSM TopUp */
                glb_MtxInd = GSMTOP_TYPE;       /* added by N.Thanakorn on 05-Feb-2015 */

        else if ( !strncmp(sync_buf.mtx_type, "GSM", 3) )
            glb_MtxInd = GSM_TYPE;

        else if ( !strncmp(sync_buf.mtx_type, "SMS", 3) ) {     /* split SMS from GSM Type - by Kawee on 07-Apr-2005 */
            // if ( !strncmp(glb_Msc_Name, "FDA", 3) )
            if ( strstr(glb_Msc_Name, "FDA") != NULL )  // Change by Thanakorn on 08-Jan-2016
                glb_MtxInd = SMSFDA_TYPE;
            else
                glb_MtxInd = SMS_TYPE;
        }

        else if ( !strncmp(sync_buf.mtx_type, "SSP", 3) )
            glb_MtxInd = SSP_TYPE;

        else if ( !strncmp(sync_buf.mtx_type, "USC", 3) )
            glb_MtxInd = USC_TYPE;

        else if ( !strncmp(sync_buf.mtx_type, "GPR", 3) )       /* added GPRS Type - by Kawin on 02-Oct-2002 */
            glb_MtxInd = GPRS_TYPE;

        else if ( !strncmp(sync_buf.mtx_type, "RBT", 3) )       /* added RBT Type - by Kawee on 18-Jul-2005 */
            glb_MtxInd = RBT_TYPE;

        else if ( !strncmp(sync_buf.mtx_type, "AWNGPR", 6) )    /* added AWN GPRS Type by Thanakorn on Jan-2013 */
            glb_MtxInd = AWNGPR_TYPE;

        else if ( !strncmp (sync_buf.mtx_type, "AWN", 3) )      /* added AWN Type by Thanakorn on Jan-2010 */
            glb_MtxInd = AWN_TYPE;

        else if ( !strncmp(sync_buf.mtx_type, "DPCGPR", 6) )    /* added DPC GPRS Type by Thanakorn on 19-Feb-2011 */
            glb_MtxInd = DPCGPR_TYPE;

        else if ( !strncmp(sync_buf.mtx_type, "AMF", 3) )       /* added AMF Type by Thanakorn on Aug-2011 */
            glb_MtxInd = AMF_TYPE;

        else if ( !strncmp(sync_buf.mtx_type, "SDG", 3) )       /* added SDG Type by Thanakorn on Mar-2012 */
            glb_MtxInd = SDG_TYPE;

        else if ( !strncmp(sync_buf.mtx_type, "DCBCT", 5) )     /* GSMCT Type for DCB */
            glb_MtxInd = DCBCT_TYPE;        /* added by N.Thankorn on 29-Apr-2019 */

        else if ( !strncmp(sync_buf.mtx_type, "DCB", 3) )       /* GSMCT Type (DCB) */
            glb_MtxInd = DCB_TYPE;          /* added by N.Thankorn on 20-May-2014 */

		else if ( !strncmp(sync_buf.mtx_type, "OCSVPN", 6) )    /* GSM type Maritime MT (Mobile Terminated Call)*/
			glb_MtxInd = OCSVPN_TYPE;       /* added by N.Thanakorn on 18-Nov-2014 */

        else if ( !strncmp(sync_buf.mtx_type, "IMS", 3) )       /* IMS type VoWIFI, VoLTE (4G voice) */
            glb_MtxInd = IMS_TYPE;

        else if ( !strncmp(sync_buf.mtx_type, "RDCSMS", 6) )    /* National Roam with DTAC */
            glb_MtxInd = RDCSMS_TYPE;

        else if ( !strncmp(sync_buf.mtx_type, "RDC", 3) )       /* National Roam with DTAC */
            glb_MtxInd = RDC_TYPE;

        else if ( !strncmp(sync_buf.mtx_type, "RBMET6", 6) )
            glb_MtxInd = GSMTOP_TYPE;

        else if ( !strncmp(sync_buf.mtx_type, "RBMET1", 6) )
            glb_MtxInd = RBM_TYPE;

        else {
            /* Unknown type */
            writeLog(LOG_WRN, "unknown mtx type %s in %s", sync_buf.mtx_type, glb_Sync_File);
            /* rename the sync file to indicate error */
            sprintf(Temp, "%s_ERR", glb_Sync_File);
            rename(glb_Sync_File , Temp);
            return FAILURE;
        }
    }
    else if ( !strncmp (sync_buf.mtx_id, "DPC.", 4) ) {
        glb_PartId = AWN_SVC_PARTITION;    /* POSTPAID Service Partition */
        strncpy(glb_Msc_Name, sync_buf.file_name+4, SIZE_MSC_NAME);     /* Migrated CAMEL V.5 - Added below by Kawee on 11-Dec-2007 */
        /* Find End of MSC name and replance will NULL */
        for ( i = 0; i < SIZE_MSC_NAME; i++ ) {
            if ( glb_Msc_Name[i] == '_' ) {
                glb_Msc_Name[i] = '\0';
                break;
            }
        }

        /* Identify mtx_type */
        if ( !strncmp(sync_buf.mtx_id, "DPC.AIS", 7) )          /* DPC - AIS Network CDR file */
            glb_MtxInd = DPCAIS_TYPE;
        else if ( !strncmp(sync_buf.mtx_id, "DPC.T_TAC", 7) )   /* DPC - TAC Network CDR file */
            glb_MtxInd = DPCTAC_TYPE;
        else if ( !strncmp(sync_buf.mtx_type, "AMF", 3) )       /* added AMF Type by Thanakorn on Aug-2011 */
            glb_MtxInd = AMF_TYPE;
        else if ( !strncmp(sync_buf.mtx_type, "SDG", 3) )       /* added SDG Type by Thanakorn on Mar-2012 */
            glb_MtxInd = SDG_TYPE;
        else {  /* others */
            if ( !strncmp(sync_buf.mtx_type, "DPCGPR", 6) )     /* added DPC GPRS Type by Thanakorn on 19-Feb-2011 */
                glb_MtxInd = DPCGPR_TYPE;
            else
                glb_MtxInd = DPCGSM_TYPE;       /* DPC - Own GSM Network CDR file */
        }
    }
    else if ( !strncmp(sync_buf.mtx_id, "AIN", 3) ) {
        glb_PartId = AIN_SVC_PARTITION;         /* AIN Service Partition */

        /* validate the mtx_type */
        if ( !strncmp(sync_buf.mtx_type, "AIN", 3) )
            glb_MtxInd = AIN_TYPE;
    }
    else if ( !strncmp(sync_buf.mtx_id, "NRTRDE", 6) ) {
        glb_PartId = AWN_SVC_PARTITION;    /* POSTPAID Service Partition */

        /* validate the mtx_type */
        if ( !strncmp(sync_buf.mtx_type, "NRTRDE", 6) )
            glb_MtxInd = NRTRDE_TYPE;
    }
    else if ( !strncmp (sync_buf.mtx_id, "NRGGSN", 6)) {
        glb_PartId = AWN_SVC_PARTITION;        /* POSTPAID Service Partition */

        /* validate the mtx_type */
        if ( !strncmp(sync_buf.mtx_type, "NRGGSN", 6) )
            glb_MtxInd = NRGGSN_TYPE;
    }
    else if ( !strncmp(sync_buf.mtx_id, "NRTSCP", 6) ) {
        glb_PartId = AWN_SVC_PARTITION;        /* POSTPAID Service Partition */

        /* validate the mtx_type */
        if ( !strncmp(sync_buf.mtx_type, "NRTSCP", 6) )
            glb_MtxInd = NRTSCP_TYPE;
    }
    else {
        glb_PartId = AWN_SVC_PARTITION;    /* Set Default value as Postpaid Service Partition */
        strncpy(glb_Msc_Name, sync_buf.file_name+4, SIZE_MSC_NAME);    /* Migrated CAMEL V.5 - Added below by Kawee on 11-Dec-2007 */
        /* Find End of MSC name and replance will NULL */
        for ( i = 0; i < SIZE_MSC_NAME; i++ ) {
            if ( glb_Msc_Name[i] == '_' ) {
                glb_Msc_Name[i] = '\0';
                break;
            }
        }

        /* validate the mtx_type */
        if ( !strncmp(sync_buf.mtx_type, "SMC", 3) )    /* SMC Type (SMC) */
            glb_MtxInd = SMC_TYPE;          /* added by Jetsada on 15-Aug-2005 */

        else if ( !strncmp(sync_buf.mtx_type, "GSMBC", 5) )     /* GSMBC Type (GSM GPRS time based bearier) */
            glb_MtxInd = GSMBC_TYPE;        /* added by Kawee on 20-May-2004 */

        else if ( !strncmp(sync_buf.mtx_type, "PPSBC", 5) )     /* PPSBC Type (PPS GPRS time based bearier) */
            glb_MtxInd = PPSBC_TYPE;        /* added by Kawee on 20-May-2004 */

        else if ( !strncmp(sync_buf.mtx_type, "GSMTC", 5) )    /* GSMTC Type (GSM MMS) */
            glb_MtxInd = GSMTC_TYPE;        /* added by Kawee on 05-Nov-2004 */

        else if ( !strncmp(sync_buf.mtx_type, "PPSTC", 5) )     /* PPSTC Type (PPS MMS) */
            glb_MtxInd = PPSTC_TYPE;        /* added by Kawee on 05-Nov-2004 */

        else if ( !strncmp(sync_buf.mtx_type, "GSMCT", 5) )     /* GSMCT Type (CDG) */
            glb_MtxInd = GSMCT_TYPE;        /* added by Kawee on 02-Jun-2005 */

        else if ( !strncmp(sync_buf.mtx_type, "PPSCT", 5) )     /* PPSCT Type (CDG) */
            glb_MtxInd = PPSCT_TYPE;        /* added by Kawee on 23-Aug-2005 */

        else if ( !strncmp(sync_buf.mtx_type, "GSMCC", 5) )     /* added CCUDR Type (SESSION) - by Jetsada on 14-Sep-2007 */
            glb_MtxInd = CCUDR_TYPE;

        else if ( !strncmp(sync_buf.mtx_type, "PPSCC", 5) )     /* added CCUDR Type (SESSION) - by Jetsada on 14-Sep-2007 */
            glb_MtxInd = CCUDR_TYPE;

        else if ( !strncmp(sync_buf.mtx_type, "GSMTOP", 6) )    /* GSM TopUp */
                glb_MtxInd = GSMTOP_TYPE;       /* added by N.Thanakorn on 05-Feb-2015 */

        else if ( !strncmp(sync_buf.mtx_type, "GSM", 3) )
            glb_MtxInd = GSM_TYPE;

        else if ( !strncmp(sync_buf.mtx_type, "SMS", 3) ) {     /* split SMS from GSM Type - by Kawee on 07-Apr-2005 */
            // if ( !strncmp(glb_Msc_Name, "FDA", 3) )
            if ( strstr(glb_Msc_Name, "FDA") != NULL )  // Change by Thanakorn on 08-Jan-2016
                glb_MtxInd = SMSFDA_TYPE;
            else
                glb_MtxInd = SMS_TYPE;
        }

        else if ( !strncmp(sync_buf.mtx_type, "SSP", 3) )
            glb_MtxInd = SSP_TYPE;

        else if ( !strncmp(sync_buf.mtx_type, "USC", 3) )
            glb_MtxInd = USC_TYPE;

        else if ( !strncmp(sync_buf.mtx_type, "GPR", 3) )       /* added GPRS Type - by Kawin on 02-Oct-2002 */
            glb_MtxInd = GPRS_TYPE;

        else if ( !strncmp(sync_buf.mtx_type, "RBT", 3) )       /* added RBT Type - by Kawee on 18-Jul-2005 */
            glb_MtxInd = RBT_TYPE;

        else if ( !strncmp(sync_buf.mtx_type, "AWNGPR", 6) )    /* added AWN GPRS Type by Thanakorn on Jan-2013 */
            glb_MtxInd = AWNGPR_TYPE;

        else if ( !strncmp (sync_buf.mtx_type, "AWN", 3) )      /* added AWN Type by Thanakorn on Jan-2010 */
            glb_MtxInd = AWN_TYPE;

        else if ( !strncmp(sync_buf.mtx_type, "DPCGPR", 6) )    /* added DPC GPRS Type by Thanakorn on 19-Feb-2011 */
            glb_MtxInd = DPCGPR_TYPE;

        else if ( !strncmp(sync_buf.mtx_type, "AMF", 3) )       /* added AMF Type by Thanakorn on Aug-2011 */
            glb_MtxInd = AMF_TYPE;

        else if ( !strncmp(sync_buf.mtx_type, "SDG", 3) )       /* added SDG Type by Thanakorn on Mar-2012 */
            glb_MtxInd = SDG_TYPE;

        else if ( !strncmp(sync_buf.mtx_type, "DCBCT", 5) )     /* GSMCT Type for DCB */
            glb_MtxInd = DCBCT_TYPE;        /* added by N.Thankorn on 29-Apr-2019 */

        else if ( !strncmp(sync_buf.mtx_type, "DCB", 3) )       /* GSMCT Type (DCB) */
            glb_MtxInd = DCB_TYPE;          /* added by N.Thankorn on 20-May-2014 */

		else if ( !strncmp(sync_buf.mtx_type, "OCSVPN", 6) )    /* GSM type Maritime MT (Mobile Terminated Call)*/
			glb_MtxInd = OCSVPN_TYPE;       /* added by N.Thanakorn on 18-Nov-2014 */

        else if ( !strncmp(sync_buf.mtx_type, "IMS", 3) )       /* IMS type VoWIFI, VoLTE (4G voice) */
            glb_MtxInd = IMS_TYPE;

        else if ( !strncmp(sync_buf.mtx_type, "RDCSMS", 6) )    /* National Roam with DTAC */
            glb_MtxInd = RDCSMS_TYPE;

        else if ( !strncmp(sync_buf.mtx_type, "RDC", 3) )       /* National Roam with DTAC */
            glb_MtxInd = RDC_TYPE;

        else if ( !strncmp(sync_buf.mtx_type, "RBMET6", 6) )
            glb_MtxInd = GSMTOP_TYPE;

        else if ( !strncmp(sync_buf.mtx_type, "RBMET1", 6) )
            glb_MtxInd = RBM_TYPE;

        else {
            /* Unknown type */
            writeLog(LOG_WRN, "unknown mtx type %s in %s", sync_buf.mtx_type, glb_Sync_File);
            /* rename the sync file to indicate error */
            sprintf(Temp, "%s_ERR", glb_Sync_File);
            rename(glb_Sync_File , Temp);
            return FAILURE;
        }
#if 0
        writeLog(LOG_WRN, "unknown mtx id %s in %s", sync_buf.mtx_id, glb_Sync_File);
        /* rename the sync file to indicate error */
        sprintf(Temp, "%s_ERR", glb_Sync_File);
        rename(glb_Sync_File, Temp);
        return FAILURE;
#endif
    }

    strcpy(glb_Cdr_File, sync_buf.file_name);

    return SUCCESS;
}

/*
** Name     : Proc_CdrFile()
**
** Description  : Process CDR File.
**        Open CDR file, GSMISAM file, Check Point file, Map Error file, CDR Reject file,
**        Common Mapping Result file, Gsm Unknown Subscriber Log file,
**        External Alarm Data file, Charge Data File.
**        Calls Cont_Mapping() to Continue Processing on CDR file.
**                Close all the opened files.
**
** Parameters   : None.
**
** Return Value : Return Value of Cont_Mapping() - on completion.
**        CDR_FOPEN_FAIL - on CDR file open failure.
**        ISAM_FOPEN_FAIL - on GSMISAM file open failure.
**        CHK_FOPEN_FAIL - on Check Point file open failure.
**        ERROR_FOPEN_FAIL - on Error file open failure.
**        REJCDR_FOPEN_FAIL - on CDR Reject file open failure.
**        GSMUNK_FOPEN_FAIL - on Unknown Subscriber Log file open failure.
*/

int Proc_CdrFile()
{

    int rv = 0;

    memset(glb_MsgText, 0x00, sizeof (glb_MsgText));

    /* Added below by Kawee on 26-Sep-2008 */
    AddDate(glb_OldCdrDate, sizeof(glb_OldCdrDate), NULL, NULL, glb_OldCdrDay*-1);

    /* Open CDR File */
    if ( Open_Cdr_File(MODE_READ) )  /* open for ready only */
        return CDR_FOPEN_FAIL;

    /* Open Check Point File */
    if ( Open_Check_File(MODE_WUPDATE) ) {   /* create a file for update */
        fclose(glb_Cdrfp);
        return CHK_FOPEN_FAIL;
    }

    /* Open Error File */
    /* write mode on Normal Processing - Append mode on Recovery Processing */
    if ( Open_Error_File(glb_ProcStat == NORM_PROC ? MODE_WRITE : MODE_APPEND) ) {
        fclose(glb_Cdrfp);
        fclose(glb_Chkfp);
        return ERROR_FOPEN_FAIL;
    }

    /* Open CDR Reject File */
    /* write mode on Normal Processing - Append mode on Recovery Processing */
    if ( Open_Reject_File(glb_ProcStat == NORM_PROC ? MODE_WRITE : MODE_APPEND) ) {
        fclose(glb_Cdrfp);
        fclose(glb_Chkfp);
        fclose(glb_Errfp);
        fclose(glb_Ignfp);
        return REJCDR_FOPEN_FAIL;
    }

    /* Open EVENT CDR File */
    if ( Open_EventCdr_File(MODE_WRITE) ) {  /* Alway Write mode */
        fclose(glb_Cdrfp);
        fclose(glb_Chkfp);
        fclose(glb_Errfp);
        fclose(glb_RCdrfp);
        fclose(glb_Ignfp);
        return EVENTCDR_FOPEN_FAIL;
    }


    if ( Open_SvcIdSub_File(MODE_WRITE) ) {
        fclose(glb_Cdrfp);
        fclose(glb_Chkfp);
        fclose(glb_Errfp);
        fclose(glb_RCdrfp);
        fclose(glb_Ignfp);
        fclose(glb_Eventcdrfp);
        return EVENTCDR_FOPEN_FAIL;
    }

    /* Open External Alarm Data File */
    if ( Open_EAlmData_File(MODE_WRITE) ) {  /* Always write mode */
        fclose(glb_Cdrfp);
        fclose(glb_Chkfp);
        fclose(glb_Errfp);
        fclose(glb_RCdrfp);
        fclose(glb_Ignfp);
        fclose(glb_Eventcdrfp);
        return EALM_FOPEN_FAIL;
    }

    rv = Cont_Mapping();

    /* Close all the opened files */
    fclose(glb_Cdrfp);
    fclose(glb_Chkfp);
    fclose(glb_Errfp);
    fclose(glb_RCdrfp);
    fclose(glb_Ignfp);
    fclose(glb_Eventcdrfp);
    Close_Split_Pps();      /* Added by Thanakorn on 07-Mar-2013 */

    /* Add Condition Check for Close file when Create Exn File Flag is Enabled - Changed as below by Kweee on 25-Oct-2005 */
    if ( (glb_AisInstFlag || glb_DpcInstFlag) && glb_PostpaidExnFlag == 'Y' )     /* Create EXN for Postpaid Service */
        fclose(glb_PostpaidEAlmfp);
    if ( glb_12cInstFlag && glb_PrepaidExnFlag == 'Y' )               /* Create EXN for Prepaid Service */
        fclose(glb_PrepaidEAlmfp);
    if ( glb_AinInstFlag && glb_AinExnFlag == 'Y' )                   /* Create EXN for AIN Service */
        fclose(glb_AinEAlmfp);
    if ( glb_NrtInstFlag && glb_NrtExnFlag == 'Y' )                   /* Create EXN for NRTRDE Service */
        fclose(glb_NrtEAlmfp);

    if ( glb_SvcIdSubfp != NULL ) {
        fclose(glb_SvcIdSubfp);
    }
    glb_SvcIdSubfp = NULL;
    if ( glb_GenSvcIdSub && glb_SvcIdCnt > 0 ) {
        char cmd[SIZE_FNAME];
        sprintf(cmd, "sort -u %s > %s.sort", glb_SvcIdSub_File, glb_SvcIdSub_File);
        system(cmd);
        sprintf(cmd, "mv %s.sort %s.DAT", glb_SvcIdSub_File, glb_SvcIdSub_File);
        system(cmd);
    }
    unlink(glb_SvcIdSub_File);

    /* Check if CDRs mapped from this file - chamy */
    if ( glb_MapCtr ) {     /* At least one CDR is mapped from this file */

        // switch ( glb_MtxInd ) {
        //     case GPRS_TYPE:
        //     case GSMBC_TYPE:    // Added GSMBC Type (GPRS Time Based bearier) by Kawee on 20-May-2004
        //     case PPSBC_TYPE:    // Added PPSBC Type (GPRS Time Based bearier) by Kawee on 20-May-2004
        //     case USC_TYPE:
        //     case GSMTC_TYPE:    // Added GSMTC Type (MMS) by Kawee on 05-Nov-2004
        //     case PPSTC_TYPE:    // Added PPSTC Type (MMS) by Kawee on 05-Nov-2004
        //     case GSMCT_TYPE:    // Added GSMCT Type (CDG) by Kawee on 02-Jun-2005
        //     case PPSCT_TYPE:    // Added PPSCT Type (CDG) by Kawee on 02-Jun-2005
        //     case RBT_TYPE:      // Added RBT Type by Kawee on 18-Jul-2005
        //     case NRGGSN_TYPE:   // Added NRGGSN Type by Kawee on 26-Feb-2008
        //     case DPCGPR_TYPE:   // Added AWN Type by Thanakorn on 19-Feb-2011
        //     case AMF_TYPE:      // Added AMF Type by Thanakorn on 16-Aug-2011
        //     case SDG_TYPE:      // Added SDG Type by Thanakorn on 01-Mar-2012
        //     case DCB_TYPE:      // Added DCB Type by Thanakorn on 20-May-2104
        //     case CCUDR_TYPE:    // Added CCUDR Type (SESSION) by Jetsada on 27-August-2007
        //         //
        //         // GPRS_TYPE or GSMBC_TYPE or PPSBC_TYPE or GSMTC_TYPE or PPSTC_TYPE or
        //         // GSMCT_TYPE or PPSCT_TYPE, RBT_TYPE, CCUDR_TYPE, DPCGPR, DPCSMS,
        //         // NRGGSN_TYPE
        //         // Create only Event CDR file,
        //         // Do not create EXN file, remove opened the EXN file.
        //         //
        //         if ( Create_EventSync() )        // Create Sync File for Event CDR File
        //             return EVENTSYNC_CREATE_FAIL;
        //
        //         if ( (glb_AisInstFlag || glb_DpcInstFlag) && glb_PostpaidExnFlag == 'Y' )
        //             unlink(glb_PostpaidEAlmData_File); // remove the Temporary External Alarm Data file
        //
        //         if ( glb_12cInstFlag && glb_PrepaidExnFlag == 'Y' )
        //             unlink(glb_PrepaidEAlmData_File);  // remove the Temporary External Alarm Data file
        //
        //         if ( glb_AinInstFlag && glb_AinExnFlag == 'Y' )
        //             unlink(glb_AinEAlmData_File);      // remove the Temporary External Alarm Data file
        //
        //         if ( glb_NrtInstFlag && glb_NrtExnFlag == 'Y' )
        //             unlink(glb_NrtEAlmData_File);      // remove the Temporary External Alarm Data file
        //
        //         break;
        //
        //     default:
        //         //
        //         // SSP_TYPE or GSM_TYPE or SMC_TYPE or SMS_TYPE or SMSFDA_TYPE or DPC_GSM or AIN_TYPE or NRTRDE_TYPE
        //         // Create Event CDR file, and then create EXN file.
        //         //
        //         if ( Create_EventSync() )        // Create Sync File for EVENT CDR File
        //             return EVENTSYNC_CREATE_FAIL;
        //
        //         // Check EXN flag - added by kawin on 19-Mar-2004
        //         // Create EXN for Postpaid Service
        //         if ( (glb_AisInstFlag || glb_DpcInstFlag) && glb_PostpaidExnFlag == 'Y' ) {
        //             if ( glb_EAlmPostpaidCtr ) {  // Have Written Record
        //                 // Rename the Temporary External Alarm Data file to final
        //                 Rename_ExtFile(glb_PostpaidEAlmData_File, EALM_TEMP_EXT, EALM_FILE_EXT);
        //             }
        //             else {
        //                 // remove zero byte Temporary External Alarm Data file
        //                 unlink(glb_PostpaidEAlmData_File);
        //             }
        //         }
        //
        //         // Create EXN for Prepaid Service
        //         if ( glb_12cInstFlag && glb_PrepaidExnFlag == 'Y' ) {
        //             if ( glb_EAlmPrepaidCtr ) {   // Have Written Record
        //                 // Rename the Temporary External Alarm Data file to final
        //                 Rename_ExtFile(glb_PrepaidEAlmData_File, EALM_TEMP_EXT, EALM_FILE_EXT);
        //             }
        //             else {
        //                 // remove zero byte Temporary External Alarm Data file
        //                 unlink(glb_PrepaidEAlmData_File);
        //             }
        //         }
        //
        //         // Create EXN for AIN Service
        //         if ( glb_AinInstFlag && glb_AinExnFlag == 'Y' ) {
        //             if ( glb_EAlmAinCtr ) {   // Have Written Record
        //                 // Rename the Temporary External Alarm Data file to final
        //                 Rename_ExtFile(glb_AinEAlmData_File, EALM_TEMP_EXT, EALM_FILE_EXT);
        //             }
        //             else {
        //                 // remove zero byte Temporary External Alarm Data file
        //                 unlink(glb_AinEAlmData_File);
        //             }
        //         }
        //
        //         // Create EXN for NRTRDE Service
        //         if ( glb_NrtInstFlag && glb_NrtExnFlag == 'Y' ) {
        //             if ( glb_EAlmNrtCtr ) {   // Have Written Record
        //                 // Rename the Temporary External Alarm Data file to final
        //                 Rename_ExtFile(glb_NrtEAlmData_File, EALM_TEMP_EXT, EALM_FILE_EXT);
        //             }
        //             else {
        //                 // remove zero byte Temporary External Alarm Data file
        //                 unlink(glb_NrtEAlmData_File);
        //             }
        //         }
        // }

        if ( Create_EventSync() ) { // Create Sync File for EVENT CDR File
            return EVENTSYNC_CREATE_FAIL;
        }

        // Create EXN for Postpaid Service
        if ( (glb_AisInstFlag || glb_DpcInstFlag) && glb_PostpaidExnFlag == 'Y' ) {
            if ( glb_EAlmPostpaidCtr ) {  // Have Written Record
                // Rename the Temporary External Alarm Data file to final
                Rename_ExtFile(glb_PostpaidEAlmData_File, EALM_TEMP_EXT, EALM_FILE_EXT);
            }
            else {
                // remove zero byte Temporary External Alarm Data file
                unlink(glb_PostpaidEAlmData_File);
            }
        }

        // Create EXN for Prepaid Service
        if ( glb_12cInstFlag && glb_PrepaidExnFlag == 'Y' ) {
            if ( glb_EAlmPrepaidCtr ) {   // Have Written Record
                Rename_ExtFile(glb_PrepaidEAlmData_File, EALM_TEMP_EXT, EALM_FILE_EXT);
            }
            else {
                unlink(glb_PrepaidEAlmData_File);
            }
        }

        // Create EXN for AIN Service
        if ( glb_AinInstFlag && glb_AinExnFlag == 'Y' ) {
            if ( glb_EAlmAinCtr ) {   // Have Written Record
                Rename_ExtFile(glb_AinEAlmData_File, EALM_TEMP_EXT, EALM_FILE_EXT);
            }
            else {
                unlink(glb_AinEAlmData_File);
            }
        }

        // Create EXN for NRTRDE Service
        if ( glb_NrtInstFlag && glb_NrtExnFlag == 'Y' ) {
            if ( glb_EAlmNrtCtr ) {   // Have Written Record
                Rename_ExtFile(glb_NrtEAlmData_File, EALM_TEMP_EXT, EALM_FILE_EXT);
            }
            else {
                unlink(glb_NrtEAlmData_File);
            }
        }
    }
    else {  /* No CDRs mapped from this file */
        unlink(glb_EventCdr_File); /* remove the Event CDR file */

        /* Check EXN flag - added by kawin on 19-Mar-2004 */
        if ( (glb_AisInstFlag || glb_DpcInstFlag) && glb_PostpaidExnFlag == 'Y' ) {
            unlink(glb_PostpaidEAlmData_File); /* remove the Temporary External Alarm Data file */
        }
        if ( glb_12cInstFlag && glb_PrepaidExnFlag == 'Y' ) {
            unlink(glb_PrepaidEAlmData_File);  /* remove the Temporary External Alarm Data file */
        }

        if ( glb_AinInstFlag && glb_AinExnFlag == 'Y' ) {
            unlink(glb_AinEAlmData_File);      /* remove the Temporary External Alarm Data file */
        }

        if ( glb_NrtInstFlag && glb_NrtExnFlag == 'Y' ) {
            unlink(glb_NrtEAlmData_File);      /* remove the Temporary External Alarm Data file */
        }

    }

    /* Remove Reject File or Not, added by Jetsada K. on 30-Aug-2005 */
    if ( glb_CreateRejFlag == 'N' ) {
        unlink(glb_Reject_File);
    }

    return rv;

}

/*
** Name     : Cont_Mapping()
**
** Description  : Continue Processing CDR File.
**        Skips Records from input CDR file if required.
**        Updates Check Point File as and when required.
**        Reads Records from input CDR file until eof.
**        Depending on Requirement and Mtx Type, calls the Following Functions :
**          Read_Gsm_Cdr (), Gsm_To_Com()
**          Read_Nok_Cdr (), Nok_To_Com()
**          Read_Ssp_Cdr (), Ssp_To_Com(), Com_To_Fit ()
**          Read_Gprs_Cdr (), Gprs_To_Fit ()
**        Writes the Mapped CDR information into Fit File.
**        Check for Shutdown Indicator after every CDR processed.
**
** Parameters   : None.
**
** Return Value : SUCCESS - on Successful completion.
**        FAILURE - on any Error.
**        SHUT_SIGNAL_RCVD - on receipt of Shutdown Signal.
*/

int Cont_Mapping()
{
    int  Eof;
    char szRecover[SIZE_NORMSTR];

    memset(glb_MsgText, 0x00, sizeof(glb_MsgText));
    memset(szRecover, 0x00, sizeof(szRecover));

    /* Note : All the Required files are open. */

    glb_ReadCtr = 0;        /* Reset the CDR Read Counter */
    glb_IgnCtr  = 0;        /* Reset the X call Ignored Counter */
    glb_DuraCtr = 0;        /* Reset the less Duration Counter */
    glb_OldCtr  = 0;        /* Reset the Old Start Datetime Counter */ /* Added by Kawee on 29-Sep-2008 */
    glb_MapCtr  = 0;        /* Reset the Mappped Counter */
    glb_SamCtr  = 0;        /* Reset the CDR Sample Tracking Counter */
    glb_ErrCtr  = 0;        /* Reset the Invalid CDR Counter */
    glb_SvcIdCnt = 0;

    strcpy(glb_MtxType, gszMtxTypeName[glb_MtxInd]);

    if ( glb_ProcStat == RECV_PROC ) {
        sprintf(szRecover, "(recover@%ld)", chk_buf.cdr_offset);
        writeLog(LOG_INF, "recovery offset %ld on %s cdr %s", chk_buf.cdr_offset, glb_Cdr_File, glb_MtxType);
    }
    sprintf(glb_MsgText, "[SUM] %sinput= %s svc_nfs= %s svc_cdr= %s dt_nfs= %s dt_fed= %s",
            szRecover, glb_Cdr_File, glb_NfsType, glb_MtxType, getFileTimeM(glb_Cdr_File, "%Y%m%d%H%M%S"), getSysDTM(DTM_DATE_TIME));

    Deb_Msg(glb_MsgText);

    /* Print in the Mapping Error File */
    fprintf(glb_Errfp, "%s\n", glb_MsgText);
    fprintf(glb_Ignfp, "%s\n", glb_MsgText);

    /* Determine the USC CDR Size here */
    Determine_Cdr_Size(glb_MtxInd);

    /* Seek to the required Offset for Recovery Processing */
    if ( glb_ProcStat == RECV_PROC ) {
        if ( fseek(glb_Cdrfp, chk_buf.cdr_offset, SEEK_SET) ) {
            writeLog(LOG_SYS, "cannot set offset %ld on %s (%s)", chk_buf.cdr_offset, glb_Cdr_File, strerror(errno));
            return FAILURE;
        }
    }

    /* Update the Check Point File before Starting Mapping */
    Upd_Check_File(CHK_TIME_UPD);
long lLine = 0;

    Eof = FALSE;
    while ( !Eof ) {
lLine++;

        /* check for shutdown indicator Before (Next) CDR Read */
        if ( cGblShutdown ) {
            /* Update the Check Point File to indicate Shutdown */
            Upd_Check_File(CHK_SHUT_UPD);

            /* writeLog(LOG_INF, "shutdown on file %s", glb_Cdr_File); */
            writeLog(LOG_INF, "shutdown on file %s, offset %ld, record %ld, maprec %ld, glb_UseRatedCdr %d", glb_Cdr_File, chk_buf.cdr_offset,glb_ReadCtr, glb_MapCtr, glb_UseRatedCdr);

            Print_Stat();      /* Print in the Mapping Error File */
            return SHUT_SIGNAL_RCVD;
        }

        if ( glb_MtxInd == GSM_TYPE ) {
            if ( Read_Gsm_Cdr() ) {     /* read one GSM CDR from file */
                Eof = TRUE;
                break;                  /* eof */
            }

            /* Map GSM to Common structure */
            /* Migrated CAMEL V.5 - Changed as below by Kawee on 04-Jun-2007 */
            if ( Gsm_To_Event() )       /* mapping failed on this CDR */
                continue;               /* contiune mapping with other CDRs */

        }
        /* --------------------------------
        Added by N.Thanakorn on 18-Nov-2014
        to support GSM type Maritime MT
        -------------------------------- */
        if ( glb_MtxInd == OCSVPN_TYPE ) {
            if ( Read_OcsVpn_Cdr() ) {
                Eof = TRUE;
                break;
            }

            if ( Awn_To_Event() )
                continue;               /* contiune mapping with other CDRs */
        }

        /* Split SMS type from GSM type - by Kawee on 07-Apr-2005 */
        if ( glb_MtxInd == SMS_TYPE || glb_MtxInd == SMSFDA_TYPE ) {
            if ( Read_Sms_Cdr()) {      /* read one SMS CDR from file */
                Eof = TRUE;
                break;                  /* eof */
            }

            /* Map SMS to Common structure */
            if ( Sms_To_Sms_Event() )   /* mapping failed on this CDR */
                continue;               /* contiune mapping with other CDRs */

            /* Maintain the GSMISAM file only for GSM_TYPE, SMS_TYPE cdrs */
        }

        if ( glb_MtxInd == SMC_TYPE ) {
            if ( Read_Smc_Cdr() ) {     /* read one SMC CDR from file */
                Eof = TRUE;
                break;                  /* eof */
            }

            /* Map SMC to Common structure */
            if ( Smc_To_Sms_Event() )   /* mapping failed on this CDR */
                continue;               /* continue mapping with other CDRs */
        }

        if ( glb_MtxInd == SSP_TYPE ) {
            /* read one SSP CDR from file */
            if ( Read_Ssp_Cdr() ) {
                Eof = TRUE;
                break;                  /* eof */
            }

            if ( Ssp_To_Voice_Event() ) /* mapping failed on this CDR */
                continue;               /* contiune mapping with other CDRs */
        }

        if ( glb_MtxInd == USC_TYPE ) {
            if ( Read_Usc_Cdr() ) {     /* read one USC CDR from file */
                Eof = TRUE;
                break;                  /* eof */
            }

            if ( Usc_To_Usc_Event() )   /* mapping failed on this CDR */
                continue;               /* contiune mapping with other CDRs */
        }

        /* added GPRS_TYPE by Kawin on 02-Oct-2002 */
        if ( glb_MtxInd == GPRS_TYPE ) {
            if ( Read_Gprs_Cdr() ) {    /* read one GPRS CDR from file */
                Eof = TRUE;
                break;                  /* eof */
            }

            if ( Gprs_To_Data_Event() ) /* mapping failed on this CDR */
                continue;               /* contiune mapping with other CDRs */
        }

        /* added DPCGPR_TYPE by Thanakorn on 19-Feb-2011 */
        if ( glb_MtxInd == DPCGPR_TYPE ) {
            if ( Read_Gprs_Cdr() ) {    /* read one GPRS CDR from file */
                Eof = TRUE;
                break;                  /* eof */
            }

            if ( DpcGprs_To_Data_Event() )  /* mapping failed on this CDR */
                continue;                   /* contiune mapping with other CDRs */
        }
        /* added GSMBC_TYPE and PPSBC_TYPE by Kawee on 20-May-2004 */
        if ( glb_MtxInd == GSMBC_TYPE || glb_MtxInd == PPSBC_TYPE ) {
            if ( Read_Gprst_Cdr() ) {       /* read one GPRST CDR from file */
                Eof = TRUE;
                break;                      /* eof */
            }

            if ( Gprst_To_Data_Event() )    /* mapping failed on this CDR */
                continue;                   /* contiune mapping with other CDRs */
        }

        /* added RBT_TYPE by Kawee on 18-Jul-2005 */
        if ( glb_MtxInd == RBT_TYPE ) {
            if ( Read_Rbt_Cdr() ) {         /* read one RBT CDR from file */
                Eof = TRUE;
                break;                      /* eof */
            }

            if ( Rbt_To_Rbt_Event() )       /* mapping failed on this CDR */
                continue;                   /* contiune mapping with other CDRs */
        }

        /* added GSMTC_TYPE and PPSTC_TYPE by Kawee on 05-Nov-2004 */
        if ( glb_MtxInd == GSMTC_TYPE || glb_MtxInd == PPSTC_TYPE ) {
            if ( Read_Mms_Cdr() ) {         /* read one MMS CDR from file */
                Eof = TRUE;
                break;                      /* eof */
            }

            if ( Mms_To_Mms_Event() )       /* mapping failed on this CDR */
                continue;                   /* contiune mapping with other CDRs */
        }

        /* added GSMCT_TYPE and PPSCT_TYPE by Kawee on 02-Jun-2005 */
        if ( glb_MtxInd == GSMCT_TYPE || glb_MtxInd == PPSCT_TYPE ) {
            if ( Read_Cdg_Cdr() ) {         /* read one MMS CDR from file */
                Eof = TRUE;
                break;                      /* eof */
            }

            if ( Cdg_To_Cdg_Event() )       /* mapping failed on this CDR */
                continue;                   /* contiune mapping with other CDRs */
        }
        /* added CCUDR_TYPE by Jetsada on 27-August-2007 */
        if ( glb_MtxInd == CCUDR_TYPE ) {
            if ( Read_Ccudr_Cdr() ) {       /* read one CCUDR CDR from file */
                Eof = TRUE;
                break;                      /* eof */
            }

            if ( Ccudr_To_Session_Event() ) /* mapping failed on this CDR */
                continue;                   /* contiune mapping with other CDRs */
        }
        /* For DPC Services */
        /* New CDR Format - Merged DPCAIS with DPCGSM - by Kawee on 12-Mar-2003 */
        if ( glb_MtxInd == DPCGSM_TYPE ) {
            if ( Read_DpcGsm_Cdr() ) {      /* read one DPCGSM CDR from file */
                Eof = TRUE;
                break;                      /* eof */
            }
            /* Map DPCGSM to Common structure */
            if ( DpcGsm_To_Event() )        /* mapping failed on this CDR */
                continue;                   /* continue mapping with other CDRs */
        }

        /* Introduce AIN CDR Type - Added below by Kawee on 29-Jan-2007 */
        /* For AIN Services */
        if ( glb_MtxInd == AIN_TYPE ) {
            /* read one AIN CDR from file */
            if ( Read_Ain_Cdr() ) {
                Eof = TRUE;
                break;                      /* eof */
            }

            if ( Ain_To_Voice_Event() )     /* mapping failed on this CDR */
                continue;                   /* contiune mapping with other CDRs */
        }

        /* Introduce NRTRDE CDR Type - Added below by Kawee on 12-Feb-2008 */
        /* For NRTRDE Services */
        if ( glb_MtxInd == NRTRDE_TYPE || glb_MtxInd == NRGGSN_TYPE ) {
            /* read one NRTRDE CDR from file */
            if ( Read_Nrtrde_Cdr() ) {
                Eof = TRUE;
                break;                      /* eof */
            }

            if ( Nrtrde_To_IR_Event() )     /* mapping failed on this CDR */
                continue;                   /* contiune mapping with other CDRs */
        }

        /* For NRTRDE SCP Services - Added on 24-Sep-2012 by Thanakorn, Support FMS V11 */
        if ( glb_MtxInd == NRTSCP_TYPE ) {
            /* read one NRTRDE SCP CDR from file */
            if ( Read_IrScp_Cdr() ) {
                Eof = TRUE;
                break;                      /* eof */
            }

            if ( IrScp_To_IR_Event() )      /* mapping failed on this CDR */
                continue;                   /* contiune mapping with other CDRs */
        }

        if ( glb_MtxInd == AWN_TYPE || glb_MtxInd == AWNSMS_TYPE ) {

            if ( Read_Awn_Cdr() ) {         /* read one GSM CDR from file */
                Eof = TRUE;
                break;                      /* eof */
            }

            if ( Awn_To_Event() )           /* mapping failed on this CDR */
                continue;                   /* contiune mapping with other CDRs */
        }
/*
        if ( glb_MtxInd == AWNSMS_TYPE) {
            if ( Read_Sms_Cdr() ) {      // read one SMS CDR from file
                Eof = TRUE;
                break;                  // eof
            }

            // Map SMS to Common structure
            if ( Sms_To_Sms_Event() )    // mapping failed on this CDR
                continue;               // contiune mapping with other CDRs
        }
*/
        if ( glb_MtxInd == AWNGPR_TYPE ) {
            if ( Read_Gprs_Cdr() ) {    /* read one GPRS CDR from file */
                Eof = TRUE;
                break;                  /* eof */
            }

            if ( Gprs_To_Data_Event() ) /* mapping failed on this CDR */
                continue;               /* contiune mapping with other CDRs */
        }

        if ( glb_MtxInd == AMF_TYPE ) {
            if ( Read_Amf_Cdr() ) {     /* read one CDR from file */
                Eof = TRUE;
                break;                  /* eof */
            }

            if ( Amf_To_Event() )       /* mapping failed on this CDR */
                continue;               /* contiune mapping with other CDRs */
        }

        if ( glb_MtxInd == SDG_TYPE ) {
            if ( Read_Sdg_Cdr() ) {     /* read one CDR from file */
                Eof = TRUE;
                break;                  /* eof */
            }

            if ( Sdg_To_Event() )       /* mapping failed on this CDR */
                continue;               /* contiune mapping with other CDRs */
        }

        /* added DCB_TYPE by N.Thanakorn on 20-May-2014 */
        if ( glb_MtxInd == DCB_TYPE || glb_MtxInd == DCBCT_TYPE ) {
            if ( Read_Cdg_Cdr() ) {     /* Same format as CDG CDR from file */
                Eof = TRUE;
                break;                  /* eof */
            }

            if ( Cdg_To_Dcb_Event() )   /* mapping failed on this CDR */
                continue;               /* contiune mapping with other CDRs */
        }

        /* --------------------------------
        Added by N.Thanakorn on 5-Feb-2015
        to support GSM TopUp
        -------------------------------- */
        if ( glb_MtxInd == GSMTOP_TYPE ) {
/* writeLog(LOG_SYS, "xxx# line %ld before Read_GsmTopUp_Cdr, glb_UseRatedCdr %d", lLine, glb_UseRatedCdr); */
            if ( Read_GsmTopUp_Cdr() ) {
/* writeLog(LOG_SYS, "xxx# line %ld after Read_GsmTopUp_Cdr and fail, glb_UseRatedCdr %d", lLine, glb_UseRatedCdr); */
                Eof = TRUE;
                break;
            }
/* writeLog(LOG_SYS, "xxx# line %ld before GsmTopUp_To_Event, glb_UseRatedCdr %d", lLine, glb_UseRatedCdr); */

            if ( GsmTopUp_To_Event() )
                continue;           /* contiune mapping with other CDRs */
        }

        /* --------------------------------
        Added by N.Thanakorn on 28-Sep-2015
        to support 4G VoWIFI, VoLTE
        -------------------------------- */
        if ( glb_MtxInd == IMS_TYPE ) {
            if ( Read_Ims_Cdr() ) {
                Eof = TRUE;
                break;
            }

            if ( Ims_To_Voice_Event() )
                continue;
        }

        /* --------------------------------
        Added by N.Thanakorn on 22-Jan-2016
        to National Roam with DTAC
        -------------------------------- */
        if ( glb_MtxInd == RDC_TYPE ) {
            if ( Read_Rdc_Cdr() ) {
                Eof = TRUE;
                break;
            }

            if ( Rdc_To_Voice_Event() )
                continue;
        }

        if ( glb_MtxInd == RDCSMS_TYPE ) {
            if ( Read_RdcSms_Cdr() ) {
                Eof = TRUE;
                break;
            }

            if ( Rdc_To_Sms_Event() )
                continue;
        }
if ( glb_MtxInd == RBM_TYPE ) {
/* writeLog(LOG_SYS, "xxx# line %ld glb_MtxInd = RBM_TYPE, glb_UseRatedCdr %d", lLine, glb_UseRatedCdr); */
}
        if ( glb_MtxInd == RBM_TYPE && glb_UseRatedCdr == 1 ) {
/* writeLog(LOG_SYS, "xxx# line %ld before Read_Rbm_Cdr, glb_UseRatedCdr %d", lLine, glb_UseRatedCdr); */
            if ( Read_Rbm_Cdr() ) {
/* writeLog(LOG_SYS, "xxx# line %ld after Read_Rbm_Cdr and fail, glb_UseRatedCdr %d", lLine, glb_UseRatedCdr); */
                Eof = TRUE;
                break;
            }

/* writeLog(LOG_SYS, "xxx# line %ld before Rbm_To_Voice_Event, glb_UseRatedCdr %d", lLine, glb_UseRatedCdr); */
            if ( Rbm_To_Voice_Event() )
                continue;
        }

        Write_Event_Record();
        Wrt_Exn_Data();
        glb_MapCtr++;           // Count Mapped records
    }   /* end while */

    /* Update the Check Point File to indicate Mapping Completed */
    Upd_Check_File(CHK_CMPL_UPD);

    writeLog(LOG_INF, "output for external alarm POS:%d  PRE:%d  AIN:%d  NRT:%d", glb_EAlmPostpaidCtr, glb_EAlmPrepaidCtr, glb_EAlmAinCtr, glb_EAlmNrtCtr);

    sprintf(glb_MsgText, (glb_ProcStat == NORM_PROC ? "completed on %s" : "recovery completed on %s"), glb_Cdr_File);
    writeLog(LOG_INF, glb_MsgText);

    /* Log a message */
    if ( glb_ProcStat == RECV_PROC ) {
        /*
        ** only on Recovery process log message.
        ** This is done to reduce number of messages in error log
        */
        writeLog(LOG_INF, glb_MsgText);
    }
    Print_Stat();   /* Print in the Mapping Error File */

    return SUCCESS;

}

/*-----------------------------------------------------------------------------------------------------------------------*/

/*
** Name     : Load_SysParameters ()
**
** Description  : Loads required System Parameters from INI file.
**        Checks access permissions on Required Directories.
**        Prepares System Environment for the CFP.
**
** Parameters   : NONE
**
** Return Value : SUCCESS - on Successful completion
**        FAILURE - on any error.
**
*/

int Load_SysParameters(char *argv[])
{
    char Temp[SIZE_INIREC+1];
    char Temp2[SIZE_INIREC+1];
    int  Idx;
    char *pch;

    memset(Temp, 0x00, sizeof(Temp));
    memset(Temp2, 0x00, sizeof(Temp2));

    /* Get Installation Name */     /* Added by Kawee on 13-Jan-2003 */
    if ( Get_Parameter("INSTALLATION_NAME", Temp) ) {
        writeLog(LOG_ERR, "cannot get INSTALLATION_NAME parameter from %s", gszIniFullName);
        ( glb_Inifp != NULL ? fclose(glb_Inifp) : 0 );
        glb_Inifp = NULL;

        return FAILURE;
    }

    /* Convert to uppercase */
    StrUpr(Temp);

    /* Set Installation Flag */ /* Added by Kawee on 13-Jan-2003 */
    glb_AisInstFlag = FALSE;
    glb_DpcInstFlag = FALSE;
    glb_12cInstFlag = FALSE;
    glb_AinInstFlag = FALSE;    /* Introduce AIN CDR Type - Added by Kawee on 29-Jan-2007 */
    glb_NrtInstFlag = FALSE;    /* Introduce NRTRDE CDR Type - Added by Kawee on 11-Feb-2008 */

    strcpy(Temp2, Temp);
    Idx = 0;
    while ( 1 ) {
        if ( !Idx )
            pch = strtok(Temp2, "+");
        else
            pch = strtok((char *)NULL, "+");

        if ( !pch )     /* No More Field */
            break;
        Trim_Str(pch);

        if ( !strcmp (pch, "AIS") ) {
            glb_AisInstFlag = TRUE;
        }
        else if ( !strcmp (pch, "DPC") ) {
            glb_DpcInstFlag = TRUE;
        }
        else if ( !strcmp (pch, "12C") ) {
            glb_12cInstFlag = TRUE;
        }
        else if ( !strcmp (pch, "AIN") ) {
            glb_AinInstFlag = TRUE;
        }
        else if ( !strcmp (pch, "NRT") ) {
            glb_NrtInstFlag = TRUE;
        }
        else if ( !strcmp (Temp, "ALL") ) {
            glb_AisInstFlag = TRUE;
            glb_DpcInstFlag = TRUE;
            glb_12cInstFlag = TRUE;
            glb_AinInstFlag = TRUE;
            glb_NrtInstFlag = TRUE;
        }
        else {
            writeLog(LOG_ERR, "unknown INSTALLATION_NAME -> %s", Temp);
            ( glb_Inifp != NULL ? fclose(glb_Inifp) : 0 );
        glb_Inifp = NULL;
            return FAILURE;
        }
        Idx++;
    }
    writeLog(LOG_INF, "INSTALLATION_NAME -> %s", Temp);

    writeLog(LOG_INF, "AIS_INSTALLATION -> %s", ( glb_AisInstFlag ? "TRUE" : "FALSE" ));
    writeLog(LOG_INF, "DPC_INSTALLATION -> %s", ( glb_DpcInstFlag ? "TRUE" : "FALSE" ));
    writeLog(LOG_INF, "12C_INSTALLATION -> %s", ( glb_12cInstFlag ? "TRUE" : "FALSE" ));
    writeLog(LOG_INF, "AIN_INSTALLATION -> %s", ( glb_AinInstFlag ? "TRUE" : "FALSE" ));
    writeLog(LOG_INF, "NRT_INSTALLATION -> %s", ( glb_NrtInstFlag ? "TRUE" : "FALSE" ));

    // ----- Get CDR Feed Process Sleep Time -----
    if ( Get_Parameter("SLEEP_SECONDS", Temp) ) {
        writeLog(LOG_WRN, "cannot get SLEEP_SECONDS from %s", gszIniFullName);
        glb_CdrFeed_Sleep = 0;
    }
    else {
        glb_CdrFeed_Sleep = atoi(Temp);
    }

    // ----- Check if Sleep time is set -----
    if ( glb_CdrFeed_Sleep <= 0 ) {
        writeLog(LOG_INF, "SLEEP_SECONDS -> %ld, set to 300 sec", glb_CdrFeed_Sleep);
        glb_CdrFeed_Sleep = 300;    /* Set 300 Seconds as default */
    }
    else {
        writeLog(LOG_INF, "SLEEP_SECONDS -> %ld", glb_CdrFeed_Sleep);
    }

    // ----- Get CDR Feed Sample Size -----
    if ( Get_Parameter("CDR_SAMPLE_SIZE", Temp) ) {
        writeLog(LOG_WRN, "cannot get CDR_SAMPLE_SIZE from %s", gszIniFullName);
        glb_CdrSample_Size = 0;
    }
    else {
        glb_CdrSample_Size = atoi(Temp);
    }

    // ----- Check if Sample Size is set -----
    if ( glb_CdrSample_Size <= 0 ) {
        writeLog(LOG_INF, "CDR_SAMPLE_SIZE -> %ld, set to 1000 rec", glb_CdrSample_Size);
        glb_CdrSample_Size = 1000;  /* Set 1000 Records as default */
    }
    else {
        writeLog(LOG_INF, "CDR_SAMPLE_SIZE -> %ld", glb_CdrSample_Size);
    }

    // ----- Get Map Data directory path -----
    if ( Get_Parameter("DAT_FILE_DIR", glb_MapData_Dir) ) {
        writeLog(LOG_ERR, "cannot get DAT_FILE_DIR from %s", gszIniFullName);
        ( (glb_Inifp == NULL) ? fclose(glb_Inifp) : 0 );
                glb_Inifp = NULL;
        return FAILURE;
    }

    // ----- Check if Map Data File path exists and permissions -----
    if ( access(glb_MapData_Dir, (F_OK|R_OK|W_OK|X_OK)) ) {
        writeLog(LOG_SYS, "cannot access mapping data dir %s (%s)", glb_MapData_Dir, strerror(errno));
        ( (glb_Inifp == NULL) ? fclose(glb_Inifp) : 0 );
                glb_Inifp = NULL;
        return FAILURE;
    }
    writeLog(LOG_INF, "DAT_FILE_DIR -> %s", glb_MapData_Dir);

    // ----- Get Sync File Directory -----
    if ( Get_Parameter("SYN_FILE_DIR", glb_Syncfile_Dir[CDR_LOCAL]) ) {
        writeLog(LOG_ERR, "cannot get SYN_FILE_DIR from %s", gszIniFullName);
        ( glb_Inifp != NULL ? fclose(glb_Inifp) : 0 );
        glb_Inifp = NULL;
        return FAILURE;
    }
    writeLog(LOG_INF, "SYN_FILE_DIR -> %s", glb_Syncfile_Dir[CDR_LOCAL]);

    // ----- Check if Sync File path exists and permissions -----
    if ( access(glb_Syncfile_Dir[CDR_LOCAL], (F_OK|R_OK|W_OK|X_OK)) ) {
        writeLog(LOG_SYS, "cannot access sync file dir %s (%s)", glb_Syncfile_Dir[CDR_LOCAL], strerror(errno));
        ( glb_Inifp != NULL ? fclose(glb_Inifp) : 0 );
        glb_Inifp = NULL;
        return FAILURE;
    }

    // ----- Get CDR File Directory -----
    if ( Get_Parameter("CDR_FILE_DIR", glb_Cdrfile_Dir[CDR_LOCAL]) ) {
        writeLog(LOG_ERR, "cannot get CDR_FILE_DIR from %s", gszIniFullName);
        ( glb_Inifp != NULL ? fclose(glb_Inifp) : 0 );
        glb_Inifp = NULL;
        return FAILURE;
    }
    writeLog(LOG_INF, "CDR_FILE_DIR -> %s", glb_Cdrfile_Dir[CDR_LOCAL]);

    // ----- Check if CDR File path exists and permissions -----
    if ( access(glb_Cdrfile_Dir[CDR_LOCAL], (F_OK|R_OK|W_OK|X_OK)) ) {
        writeLog(LOG_SYS, "cannot access cdr file dir %s (%s)", glb_Cdrfile_Dir[CDR_LOCAL], strerror(errno));
        ( glb_Inifp != NULL ? fclose(glb_Inifp) : 0 );
        glb_Inifp = NULL;
        return FAILURE;
    }

    // ----- Get Error Log Directory -----
    if ( Get_Parameter("ERR_FILE_DIR", glb_Errlog_Dir) ) {
        writeLog(LOG_ERR, "cannot get ERR_FILE_DIR from %s", gszIniFullName);
        ( glb_Inifp != NULL ? fclose(glb_Inifp) : 0 );
        glb_Inifp = NULL;
        return FAILURE;
    }
    writeLog(LOG_INF, "ERR_FILE_DIR -> %s", glb_Errlog_Dir);

    // ----- Check if Error Log path exists and permissions -----
    if ( access(glb_Errlog_Dir, (F_OK|R_OK|W_OK|X_OK)) ) {
        writeLog(LOG_SYS, "cannot access error file dir %s (%s)", glb_Errlog_Dir, strerror(errno));
        ( glb_Inifp != NULL ? fclose(glb_Inifp) : 0 );
        glb_Inifp = NULL;
        return FAILURE;
    }

    // ----- Get Number of CDR Repository Directory for EVENT -----
    if ( Get_Parameter("NOF_CDR_REPO", Temp) ) {
        writeLog(LOG_WRN, "cannot get NOF_CDR_REPO from %s", gszIniFullName);
        glb_NofCdrRepo = 0;
    }
    else {
        glb_NofCdrRepo = atoi(Temp);
    }

    // ----- Check if Number of CDR Repository Directory is set -----
    if ( glb_NofCdrRepo <= 0 ) {
        writeLog(LOG_INF, "NOF_CDR_REPO -> %d, set to 1", glb_NofCdrRepo);
        glb_NofCdrRepo = 1;     /* Set 1 as minimum default */
    }
    else if ( glb_NofCdrRepo > MAX_CDR_REPO_DIR) {
        writeLog(LOG_WRN, "NOF_CDR_REPO -> %d, set to %d", glb_NofCdrRepo, MAX_CDR_REPO_DIR);
        glb_NofCdrRepo = MAX_CDR_REPO_DIR;      /* Set MAX_CDR_REPO_DIR as maximum default */
    }
    else {
        writeLog(LOG_INF, "NOF_CDR_REPO -> %d", glb_NofCdrRepo);
    }

    // ----- Get CDR Repository Directory -----
    for ( Idx = 0; Idx < glb_NofCdrRepo; Idx++ ) {
        sprintf(Temp, "CDR_REPO_%d_DIR", Idx);
        if ( Get_Parameter (Temp, glb_CdrRepo_Dir[Idx]) ) {
            writeLog(LOG_ERR, "cannot get %s from %s", Temp, gszIniFullName);
            ( glb_Inifp != NULL ? fclose(glb_Inifp) : 0 );
        glb_Inifp = NULL;
            return FAILURE;
        }
        writeLog(LOG_INF, "%s -> %s", Temp, glb_CdrRepo_Dir[Idx]);

        // ----- Check if Error Log path exists and permissions -----
        if ( access(glb_CdrRepo_Dir[Idx], (F_OK|R_OK|W_OK|X_OK)) ) {
            writeLog(LOG_SYS, "cannot access cdr repo id%d of %s (%s)", Idx, glb_CdrRepo_Dir[Idx], strerror(errno));
            ( glb_Inifp != NULL ? fclose(glb_Inifp) : 0 );
        glb_Inifp = NULL;
            return FAILURE;
        }
    }
    glb_CurrCdrRepo = -1;   // Set Initial Value of Current CDR Repository default is -1 for start at set 0.

    // ----- Get CREATE REJECT FILE Flag ----- added by Jetsada K. on 30-Aug-2005 -----
    if ( Get_Parameter("CREATE_REJ_FILE", Temp) ) {
        writeLog(LOG_WRN, "cannot get CREATE_REJ_FILE from %s", gszIniFullName);
        glb_CreateRejFlag = '0';
    }
    else {
        glb_CreateRejFlag = *Temp;
    }

    // ----- Check if POSTPAID EXN Creation Flag is set -----
    if ( glb_CreateRejFlag != 'y' && glb_CreateRejFlag != 'Y' && glb_CreateRejFlag != 'n' && glb_CreateRejFlag != 'N' ) {
        writeLog(LOG_INF, "CREATE_REJ_FILE -> %c, set to 'N'.", glb_CreateRejFlag);
        glb_CreateRejFlag = 'N';            /* Set 'N' as default */
    }
    else {
        if ( glb_CreateRejFlag == 'y' )
            glb_CreateRejFlag = 'Y';        /* Set to Upper Case */
        else if ( glb_CreateRejFlag == 'n' )
            glb_CreateRejFlag = 'N';        /* Set to Upper Case */

        writeLog(LOG_INF, "CREATE_REJ_FILE -> %c", glb_CreateRejFlag);
    }

    // ----- Load Installation Specific System Parameter ----- Added by Kawee on 13-Jan-2003 -----
    if ( glb_AisInstFlag || glb_DpcInstFlag ) {
        if ( Load_Postpaid_SysParam() ) {
            ( glb_Inifp != NULL ? fclose(glb_Inifp) : 0 );
        glb_Inifp = NULL;
            return FAILURE;
        }
    }
    if ( glb_12cInstFlag ) {
        if ( Load_Prepaid_SysParam() ) {
            ( glb_Inifp != NULL ? fclose(glb_Inifp) : 0 );
        glb_Inifp = NULL;
            return FAILURE;
        }
    }
    if ( glb_AinInstFlag ) {
        if ( Load_Ain_SysParam() ) {
            ( glb_Inifp != NULL ? fclose(glb_Inifp) : 0 );
        glb_Inifp = NULL;
            return FAILURE;
        }
    }
    if ( glb_NrtInstFlag ) {
        if ( Load_Nrtrde_SysParam() ) {
            ( glb_Inifp != NULL ? fclose(glb_Inifp) : 0 );
        glb_Inifp = NULL;
            return FAILURE;
        }
    }

    // ----- Since AIN Min Duration is used in all Partitions - Moved from Load_Ain_SysParam function by Kawee on 09-Feb-2007 -----
    // ----- Get AIN Minimum Duration (Seconds) -----
    if ( Get_Parameter("AIN_MIN_DURATION", Temp) ) {
        writeLog(LOG_WRN, "cannot get AIN_MIN_DURATION from %s", gszIniFullName);
        glb_AinMin_Duration = -1;
    }
    else {
        glb_AinMin_Duration = atoi(Temp);
    }

    // ----- Check if CDR Minimum Duration (Seconds) is set -----
    if ( glb_AinMin_Duration < 0 ) {
        writeLog(LOG_INF, "AIN_MIN_DURATION -> %ld, set to 4 sec", glb_AinMin_Duration);
        glb_AinMin_Duration = 4;    /* Set 4 Seconds as default */
    }
    else {
        writeLog(LOG_INF, "AIN_MIN_DURATION -> %ld", glb_AinMin_Duration);
    }

    // ----- Change Concept to Calculate Charge to Fix Rate - Added below by Kawee on 19-Sep-2008 -----
    // ----- Get USE_FIX_RATE Flag -----
    if ( Get_Parameter("USE_FIX_RATE", Temp) ) {
        writeLog(LOG_ERR, "cannot get USE_FIX_RATE from %s", gszIniFullName);
        ( glb_Inifp != NULL ? fclose(glb_Inifp) : 0 );
        glb_Inifp = NULL;
        return FAILURE;
    }
    glb_UseFixRateFlag = *Temp;

    // ----- Check if POSTPAID EXN Creation Flag is set -----
    if ( glb_UseFixRateFlag != 'y' && glb_UseFixRateFlag != 'Y' && glb_UseFixRateFlag != 'n' && glb_UseFixRateFlag != 'N' ) {
        writeLog(LOG_INF, "USE_FIX_RATE -> %c, set to 'N'.", glb_UseFixRateFlag);
        glb_UseFixRateFlag = 'N';           /* Set 'N' as default */
    }
    else {
        if ( glb_UseFixRateFlag == 'y' )
            glb_UseFixRateFlag = 'Y';       /* Set to Upper Case */
        else if ( glb_UseFixRateFlag == 'n' )
            glb_UseFixRateFlag = 'N';       /* Set to Upper Case */

        writeLog(LOG_INF, "USE_FIX_RATE -> %c", glb_UseFixRateFlag);
    }

    // ----- Added check CDR when CDR older than OLD_CDR_DAY - by Kawee on 26-Sep-2008 -----
    // ----- Get Old CDR Day -----
    if ( Get_Parameter("OLD_CDR_DAY", Temp) ) {
        writeLog(LOG_WRN, "cannot get OLD_CDR_DAY from %s", gszIniFullName);
        glb_OldCdrDay = 0;
    }
    else {
        glb_OldCdrDay = atoi(Temp);
    }

    // ----- Check if CDR Minimum Duration (Seconds) is set -----
    if ( glb_OldCdrDay < 3 ) {
        writeLog(LOG_WRN, "OLD_CDR_DAY -> %ld, set to 3 days", glb_OldCdrDay);
        glb_OldCdrDay = 3;  /* Set 3 Days as default */
    }
    else {
        writeLog(LOG_INF, "OLD_CDR_DAY -> %ld", glb_OldCdrDay);
    }

    /* Check if Splitting PPSAWN CDR is enable or disable - by Thanakorn on 07-Mar-2013 */
    if ( Get_Parameter("SPLIT_AWNPPS_CDR", Temp) ) {
        writeLog(LOG_WRN, "cannot get SPLIT_AWNPPS_CDR from %s, set to 'N'.", gszIniFullName);
        glb_Enable_Splt = 'N';
    }
    else {
        glb_Enable_Splt = *Temp;
        if ( glb_Enable_Splt != 'y' && glb_Enable_Splt != 'Y' && glb_Enable_Splt != 'n' && glb_Enable_Splt != 'N' ) {
            writeLog(LOG_WRN, "SPLIT_AWNPPS_CDR -> %c, set to 'N'.", glb_Enable_Splt);
            glb_Enable_Splt = 'N';            /* Set 'N' as default */
        }
        else {
            if ( glb_Enable_Splt == 'y' )
                glb_Enable_Splt = 'Y';        /* Set to Upper Case */
            else if ( glb_Enable_Splt == 'n' )
                glb_Enable_Splt = 'N';        /* Set to Upper Case */

            writeLog(LOG_INF, "SPLIT_AWNPPS_CDR -> %c", glb_Enable_Splt);
        }
    }

    if ( glb_Enable_Splt == 'Y' ) {
        if ( Load_SpltPPS_SysParam() ) {
            ( glb_Inifp != NULL ? fclose(glb_Inifp) : 0 );
        glb_Inifp = NULL;
            return FAILURE;
        }
    }

    // ----- Check if enable/disable back cdr/sync file - CDR data before processing by CDR FEEDER -----
    if ( Get_Parameter("BACKUP_FED_CDR", Temp) ) {
        writeLog(LOG_WRN, "cannot get BACKUP_FED_CDR from %s", gszIniFullName);
        glb_CdrBackFlag = 'N';
    }
    else {
        glb_CdrBackFlag = *Temp;
    }

    if ( glb_CdrBackFlag == 'y' || glb_CdrBackFlag == 'Y' ) {
        glb_CdrBackFlag = 'Y';
        writeLog(LOG_INF, "BACKUP_FED_CDR -> %c", glb_CdrBackFlag);
        if ( Get_Parameter("BACKUP_CDR_PATH", glb_CdrBack_Dir) ) {
            writeLog(LOG_ERR, "cannot get BACKUP_CDR_PATH from %s", gszIniFullName);
            ( glb_Inifp != NULL ? fclose(glb_Inifp) : 0 );
        glb_Inifp = NULL;
            return FAILURE;
        }
        if ( access(glb_CdrBack_Dir, (F_OK|R_OK|W_OK|X_OK)) ) {
            writeLog(LOG_SYS, "cannot access BACKUP_CDR_PATH dir %s (%s)", glb_CdrBack_Dir, strerror(errno));
            ( glb_Inifp != NULL ? fclose(glb_Inifp) : 0 );
        glb_Inifp = NULL;
            return FAILURE;
        }
        writeLog(LOG_INF, "BACKUP_CDR_PATH -> %s", glb_CdrBack_Dir);
    }
    else {
        writeLog(LOG_INF, "BACKUP_FED_CDR -> %c, set to 'N'.", glb_CdrBackFlag);
        glb_CdrBackFlag = 'N';
    }

    // ----- Check if enable/disable backup cdr file for IUM - CDR data after processing by CDR FEEDER -----
    if ( Get_Parameter("BACKUP_IUM_CDR", Temp) ) {
        writeLog(LOG_WRN, "cannot get BACKUP_IUM_CDR from %s", gszIniFullName);
        glb_CdrBck4IUMFlag = 'N';
    }
    else {
        glb_CdrBck4IUMFlag = *Temp;
    }

    if ( glb_CdrBck4IUMFlag == 'y' || glb_CdrBck4IUMFlag == 'Y' ) {
        glb_CdrBck4IUMFlag = 'Y';
        writeLog(LOG_INF, "BACKUP_IUM_CDR -> %c", glb_CdrBck4IUMFlag);
        memset(glb_CdrBck4IUM_Dir, 0x00, sizeof(glb_CdrBck4IUM_Dir));
        if ( Get_Parameter("BACKUP_IUM_CDR_PATH", glb_CdrBck4IUM_Dir) ) {
            writeLog(LOG_ERR, "cannot get BACKUP_IUM_CDR_PATH from %s", gszIniFullName);
            ( glb_Inifp != NULL ? fclose(glb_Inifp) : 0 );
        glb_Inifp = NULL;
            return FAILURE;
        }
        if ( access(glb_CdrBck4IUM_Dir, (F_OK|R_OK|W_OK|X_OK)) ) {
            writeLog(LOG_SYS, "cannot access BACKUP_IUM_CDR_PATH dir %s (%s)", glb_CdrBck4IUM_Dir, strerror(errno));
            ( glb_Inifp != NULL ? fclose(glb_Inifp) : 0 );
        glb_Inifp = NULL;
            return FAILURE;
        }
        writeLog(LOG_INF, "BACKUP_IUM_CDR_PATH -> %s", glb_CdrBck4IUM_Dir);
    }
    else {
        writeLog(LOG_WRN, "BACKUP_IUM_CDR -> %c, set to 'N'.", glb_CdrBck4IUMFlag);
        glb_CdrBck4IUMFlag = 'N';
    }

    // ----- Check Feeding Type - by Thanakorn on 29-Aug-2013 -----
    if ( Get_Parameter("FEED_TYPE", Temp) ) {
        writeLog(LOG_WRN, "cannot get FEED_TYPE from %s, set to 'MD'.", gszIniFullName);
        glb_nFeedType = FD_MD;
    }
    else {
        if ( strncasecmp(Temp, "ALL", 3) == 0 ) {
            writeLog(LOG_INF, "FEED_TYPE -> '%s'", Temp);
            glb_nFeedType = FD_ALL;
        }
        else if ( strncasecmp(Temp, "MD", 2) == 0 ) {
            writeLog(LOG_INF, "FEED_TYPE -> '%s'", Temp);
            glb_nFeedType = FD_MD;
        }
        else {
            writeLog(LOG_WRN, "FEED_TYPE -> '%s', set to 'MD'", Temp);
            glb_nFeedType = FD_MD;
        }
    }

    // ----- Check allow specific cdr type to feed even they are BOS or MD - By Thanakorn on 09-Feb-2016 -----
    memset(glb_AllowType, 0x00, sizeof(int)*NOF_AL_TYPE);
    if ( Get_Parameter("ALLOW_TYPE", Temp) ) {
        writeLog(LOG_WRN, "cannot get ALLOW_TYPE from %s, set to 'NONE'", gszIniFullName);
    }
    else {
        writeLog(LOG_INF, "ALLOW_TYPE(Feed BOS and MD) -> %s", Temp);
        if ( strstr(Temp, "DCB") != NULL ) {
            glb_AllowType[AL_DCB] = 1;
        }
        if ( strstr(Temp, "GPRS") != NULL ) {
            glb_AllowType[AL_GPRS] = 1;
        }
        if ( strstr(Temp, "RBT") != NULL ) {
            glb_AllowType[AL_RBT] = 1;
        }
        if ( strstr(Temp, "SDG") != NULL ) {
            glb_AllowType[AL_SDG] = 1;
        }
        if ( strstr(Temp, "SMS") != NULL ) {
            glb_AllowType[AL_SMS] = 1;
        }
        if ( strstr(Temp, "USC") != NULL ) {
            glb_AllowType[AL_USC] = 1;
        }
        if ( strstr(Temp, "VSCP") != NULL ) {
            glb_AllowType[AL_VSCP] = 1;
        }
        if ( strstr(Temp, "VOICE") != NULL ) {
            glb_AllowType[AL_VOICE] = 1;
        }
    }
    memset(glb_SkipDetIrType, 0x00, sizeof(glb_SkipDetIrType));
    if ( Get_Parameter("IR_SKIP_DETECT_THRESHOLD", Temp) ) {
        writeLog(LOG_WRN, "cannot get IR_SKIP_DETECT_THRESHOLD from %s, no skip detection", gszIniFullName);
    }
    else {
        strcpy(glb_SkipDetIrType, Temp);
        writeLog(LOG_INF, "IR_SKIP_DETECT_THRESHOLD -> %s", glb_SkipDetIrType);
    }

    glb_UseRatedCdr = 0;
    if ( Get_Parameter("USE_RATED_RBM_CDR", Temp) ) {
        writeLog(LOG_WRN, "cannot get USE_RATED_RBM_CDR from %s, rating as usual", gszIniFullName);
    }
    else {
        if ( *Temp == 'Y' || *Temp == 'y' ) {
            glb_UseRatedCdr = 1;
            writeLog(LOG_INF, "USE_RATED_RBM_CDR -> Y");
        }
        else {
            writeLog(LOG_INF, "USE_RATED_RBM_CDR -> N");
        }
    }

    glb_GenSvcIdSub = 0;
    if ( Get_Parameter("CREATE_SERVICE_ID_SUBS", Temp) ) {
        writeLog(LOG_WRN, "cannot get CREATE_SERVICE_ID_SUBS from %s, no service_id file will be created", gszIniFullName);
    }
    else {
        if ( *Temp == 'Y' || *Temp == 'y' ) {
            glb_GenSvcIdSub = 1;
            writeLog(LOG_INF, "CREATE_SERVICE_ID_SUBS -> Y");
            if ( Get_Parameter("SERVICE_ID_SUBS_OUTPUT", glb_SvcIdSub_Dir) ) {
                writeLog(LOG_ERR, "cannot get SERVICE_ID_SUBS_OUTPUT from %s", gszIniFullName);
                ( glb_Inifp != NULL ? fclose(glb_Inifp) : 0 );
                glb_Inifp = NULL;
                return FAILURE;
            }   // IV|SvcId|Desc|CpId output for service_id entity
            if ( access(glb_SvcIdSub_Dir, (F_OK|R_OK|W_OK|X_OK)) ) {
                writeLog(LOG_SYS, "cannot access SERVICE_ID_SUBS_OUTPUT dir %s (%s)", glb_SvcIdSub_Dir, strerror(errno));
                ( glb_Inifp != NULL ? fclose(glb_Inifp) : 0 );
                glb_Inifp = NULL;
                return FAILURE;
            }
            writeLog(LOG_INF, "SERVICE_ID_SUBS_OUTPUT -> %s", glb_SvcIdSub_Dir);
        }
        else {
            writeLog(LOG_INF, "CREATE_SERVICE_ID_SUBS -> N");
        }
    }


    ( glb_Inifp != NULL ? fclose(glb_Inifp) : 0 );
    glb_Inifp = NULL;
    return SUCCESS;

}

/* Added below function for load parameter of POSTPAID Installation - by Kawee on 13-Jan-2003 */
int Load_Postpaid_SysParam()
{
    char Temp[SIZE_INIREC+1];

    // ----- Get Postpaid Minimum Duration (Seconds) -----
    if ( Get_Parameter("POSTPAID_MIN_DURATION", Temp) ) {
        writeLog(LOG_WRN, "cannot get POSTPAID_MIN_DURATION from %s", gszIniFullName);
        glb_PostpaidMin_Duration = -1;
    }
    else {
        glb_PostpaidMin_Duration = atoi(Temp);
    }

    // ----- Check if CDR Minimum Duration (Seconds) is set -----
    if ( glb_PostpaidMin_Duration < 0 ) {
        writeLog(LOG_INF, "POSTPAID_MIN_DURATION -> %ld, set to 4 sec", glb_PostpaidMin_Duration);
        glb_PostpaidMin_Duration = 4;   /* Set 4 Seconds as default */
    }
    else {
        writeLog(LOG_INF, "POSTPAID_MIN_DURATION -> %ld", glb_PostpaidMin_Duration);
    }

    // ----- Get POSTPAID EXN Creation Flag ----- added by kawin on 19-Mar-2004 -----
    if ( Get_Parameter("POSTPAID_CREATE_EXN", Temp) ) {
        writeLog(LOG_WRN, "cannot get POSTPAID_CREATE_EXN from %s", gszIniFullName);
        glb_PostpaidExnFlag = '0';
    }
    else {
        glb_PostpaidExnFlag = *Temp;
    }

    // ----- Check if POSTPAID EXN Creation Flag is set -----
    if ( glb_PostpaidExnFlag != 'y' && glb_PostpaidExnFlag != 'Y' && glb_PostpaidExnFlag != 'n' && glb_PostpaidExnFlag != 'N' ) {
        writeLog(LOG_WRN, "POSTPAID_CREATE_EXN -> %c, set to 'N'.", glb_PostpaidExnFlag);
        glb_PostpaidExnFlag = 'N';          /* Set 'N' as default */
    }
    else {
        if ( glb_PostpaidExnFlag == 'y' )
            glb_PostpaidExnFlag = 'Y';      /* Set to Upper Case */
        else if ( glb_PostpaidExnFlag == 'n' )
            glb_PostpaidExnFlag = 'N';      /* Set to Upper Case */

        writeLog(LOG_INF, "POSTPAID_CREATE_EXN -> %c", glb_PostpaidExnFlag);
    }

    // ----- Get POSTPAID EXN File Directory if required -----
    if ( glb_PostpaidExnFlag == 'Y' ) {
        if ( Get_Parameter("POSTPAID_EXN_FILE_DIR", glb_PostpaidExnFile_Dir) ) {
            writeLog(LOG_ERR, "cannot get POSTPAID_EXN_FILE_DIR from %s", gszIniFullName);
            ( glb_Inifp != NULL ? fclose(glb_Inifp) : 0 );
        glb_Inifp = NULL;
            return FAILURE;
        }
        writeLog(LOG_INF, "POSTPAID_EXN_FILE_DIR -> %s", glb_PostpaidExnFile_Dir);

        // ----- Check if Error Log path exists and permissions -----
        if ( access(glb_PostpaidExnFile_Dir, (F_OK|R_OK|W_OK|X_OK)) ) {
            writeLog(LOG_SYS, "cannot access postpaid exn file dir %s (%s)", glb_PostpaidExnFile_Dir, strerror(errno));
            ( glb_Inifp != NULL ? fclose(glb_Inifp) : 0 );
        glb_Inifp = NULL;
            return FAILURE;
        }
    }
    return SUCCESS;
}

/* Added below function for load parameter of Prepaid Installation - by Kawee on 13-Jan-2003 */
int Load_Prepaid_SysParam()
{
    char Temp[SIZE_INIREC+1];

    // ----- Get Prepaid Minimum Duration (Seconds) -----
    if ( Get_Parameter("PREPAID_MIN_DURATION", Temp) ) {
        writeLog(LOG_WRN, "cannot get PREPAID_MIN_DURATION from %s", gszIniFullName);
        glb_PrepaidMin_Duration = -1;
    }
    else {
        glb_PrepaidMin_Duration = atoi(Temp);
    }

    // ----- Check if CDR Minimum Duration (Seconds) is set -----
    if ( glb_PrepaidMin_Duration < 0 ) {
        writeLog(LOG_INF, "PREPAID_MIN_DURATION -> %ld, set to 4 sec", glb_PrepaidMin_Duration);
        glb_PrepaidMin_Duration = 4;    /* Set 4 Seconds as default */
    }
    else {
        writeLog(LOG_INF, "PREPAID_MIN_DURATION -> %ld", glb_PrepaidMin_Duration);
    }

    // ----- Get PREPAID EXN Creation Flag ----- added by kawin on 19-Mar-2004 -----
    if ( Get_Parameter("PREPAID_CREATE_EXN", Temp)) {
        writeLog(LOG_WRN, "cannot get PREPAID_CREATE_EXN from %s", gszIniFullName);
        glb_PrepaidExnFlag = '0';
    }
    else {
        glb_PrepaidExnFlag = *Temp;
    }

    // ----- Check if PREPAID EXN Creation Flag is set -----
    if ( glb_PrepaidExnFlag != 'y' && glb_PrepaidExnFlag != 'Y' && glb_PrepaidExnFlag != 'n' && glb_PrepaidExnFlag != 'N' ) {
        writeLog(LOG_WRN, "PREPAID_CREATE_EXN -> %c, set to 'N'.", glb_PrepaidExnFlag);
        glb_PrepaidExnFlag = 'N';           /* Set 'N' as default */
    }
    else {
        if ( glb_PrepaidExnFlag == 'y' )
            glb_PrepaidExnFlag = 'Y';       /* Set to Upper Case */
        else if ( glb_PrepaidExnFlag == 'n' )
            glb_PrepaidExnFlag = 'N';       /* Set to Upper Case */

        writeLog(LOG_INF, "PREPAID_CREATE_EXN -> %c", glb_PrepaidExnFlag);
    }

    // ----- Get PREPAID EXN File Directory if required -----
    if ( glb_PrepaidExnFlag == 'Y' ) {
        if ( Get_Parameter("PREPAID_EXN_FILE_DIR", glb_PrepaidExnFile_Dir) ) {
            writeLog(LOG_ERR, "cannot get PREPAID_EXN_FILE_DIR dir from %s", gszIniFullName);
            ( glb_Inifp != NULL ? fclose(glb_Inifp) : 0 );
        glb_Inifp = NULL;
            return FAILURE;
        }
        writeLog(LOG_INF, "PREPAID_EXN_FILE_DIR -> %s", glb_PrepaidExnFile_Dir);

        // ----- Check if Error Log path exists and permissions -----
        if ( access(glb_PrepaidExnFile_Dir, (F_OK|R_OK|W_OK|X_OK)) ) {
            writeLog(LOG_SYS, "cannot access prepaid exn file dir %s (%s)", glb_PrepaidExnFile_Dir, strerror(errno));
            ( glb_Inifp != NULL ? fclose(glb_Inifp) : 0 );
        glb_Inifp = NULL;
            return FAILURE;
        }
    }
    return SUCCESS;
}

// ----- Introduce AIN CDR Type - Added below by Kawee on 29-Jan-2007 -----
int Load_Ain_SysParam()
{
    char Temp[SIZE_INIREC+1];

    // ----- Get AIN EXN Creation Flag -----
    if ( Get_Parameter("AIN_CREATE_EXN", Temp) ) {
        writeLog(LOG_WRN, "cannot get AIN_CREATE_EXN from %s", gszIniFullName);
        glb_AinExnFlag = '0';
    }
    else {
        glb_AinExnFlag = *Temp;
    }

    // ----- Check if AIN EXN Creation Flag is set -----
    if ( glb_AinExnFlag != 'y' && glb_AinExnFlag != 'Y' && glb_AinExnFlag != 'n' && glb_AinExnFlag != 'N' ) {
        writeLog(LOG_WRN, "AIN_CREATE_EXN -> %c, set to 'N'.", glb_AinExnFlag);
        glb_AinExnFlag = 'N';           /* Set 'N' as default */
    }
    else {
        if ( glb_AinExnFlag == 'y' )
            glb_AinExnFlag = 'Y';       /* Set to Upper Case */
        else if ( glb_AinExnFlag == 'n' )
            glb_AinExnFlag = 'N';       /* Set to Upper Case */

        writeLog(LOG_INF, "AIN_CREATE_EXN -> %c", glb_AinExnFlag);
    }

    // ----- Get AIN EXN File Directory if required -----
    if ( glb_AinExnFlag == 'Y' ) {
        if ( Get_Parameter("AIN_EXN_FILE_DIR", glb_AinExnFile_Dir) ) {
            writeLog(LOG_ERR, "cannot get AIN_EXN_FILE_DIR from %s", gszIniFullName);
            ( glb_Inifp != NULL ? fclose(glb_Inifp) : 0 );
        glb_Inifp = NULL;
            return FAILURE;
        }
        writeLog(LOG_INF, "AIN_EXN_FILE_DIR -> %s", glb_AinExnFile_Dir);

        // ----- Check if Error Log path exists and permissions -----
        if ( access(glb_AinExnFile_Dir, (F_OK|R_OK|W_OK|X_OK)) ) {
            writeLog(LOG_SYS, "cannot access ain exn file dir %s (%s)", glb_AinExnFile_Dir, strerror(errno));
            ( glb_Inifp != NULL ? fclose(glb_Inifp) : 0 );
        glb_Inifp = NULL;
            return FAILURE;
        }
    }
    return SUCCESS;
}

// ----- Introduce NRTRDE CDR Type - Added below by Kawee on 11-Feb-2008 -----
int Load_Nrtrde_SysParam()
{
    char Temp[SIZE_INIREC+1];
    memset(Temp, 0x00, sizeof(Temp));

    // ----- Get NRTRDE Sync File Directory -----
    if ( Get_Parameter("NRTRDE_SYN_FILE_DIR", glb_Syncfile_Dir[CDR_NRTRDE]) ) {
        writeLog(LOG_ERR, "cannot get NRTRDE_SYN_FILE_DIR from %s", gszIniFullName);
        ( glb_Inifp != NULL ? fclose(glb_Inifp) : 0 );
        glb_Inifp = NULL;
        return FAILURE;
    }
    writeLog(LOG_INF, "NRTRDE_SYN_FILE_DIR -> %s", glb_Syncfile_Dir[CDR_NRTRDE]);

    // ----- Check if Sync File path exists and permissions -----
    if ( access(glb_Syncfile_Dir[CDR_NRTRDE], (F_OK|R_OK|W_OK|X_OK)) ) {
        writeLog(LOG_SYS, "cannot access nrtrde sync file dir %s (%s)", glb_Syncfile_Dir[CDR_NRTRDE], strerror(errno));
        ( glb_Inifp != NULL ? fclose(glb_Inifp) : 0 );
        glb_Inifp = NULL;
        return FAILURE;
    }

    // ----- Get NRTRDE CDR File Directory -----
    if ( Get_Parameter("NRTRDE_CDR_FILE_DIR", glb_Cdrfile_Dir[CDR_NRTRDE]) ) {
        writeLog(LOG_ERR, "cannot get NRTRDE_CDR_FILE_DIR from %s", gszIniFullName);
        ( glb_Inifp != NULL ? fclose(glb_Inifp) : 0 );
        glb_Inifp = NULL;
        return FAILURE;
    }
    writeLog(LOG_INF, "NRTRDE_CDR_FILE_DIR -> %s", glb_Cdrfile_Dir[CDR_NRTRDE]);

    // ----- Check if CDR File path exists and permissions -----
    if ( access(glb_Cdrfile_Dir[CDR_NRTRDE], (F_OK|R_OK|W_OK|X_OK)) ) {
        writeLog(LOG_SYS, "cannot access nrtrde cdr file dir %s (%s)", glb_Cdrfile_Dir[CDR_NRTRDE], strerror(errno));
        ( glb_Inifp != NULL ? fclose(glb_Inifp) : 0 );
        glb_Inifp = NULL;
        return FAILURE;
    }

    // ----- Get NRTRDE EXN Creation Flag -----
    if ( Get_Parameter("NRTRDE_CREATE_EXN", Temp) ) {
        writeLog(LOG_WRN, "cannot get NRTRDE_CREATE_EXN from %s", gszIniFullName);
        glb_NrtExnFlag = '0';
    }
    else {
        glb_NrtExnFlag = *Temp;
    }

    // ----- Check if NRTRDE EXN Creation Flag is set -----
    if ( glb_NrtExnFlag != 'y' && glb_NrtExnFlag != 'Y' && glb_NrtExnFlag != 'n' && glb_NrtExnFlag != 'N' ) {
        writeLog(LOG_WRN, "NRTRDE_CREATE_EXN -> %c, set to 'N'.", glb_NrtExnFlag);
        glb_NrtExnFlag = 'N';           /* Set 'N' as default */
    }
    else {
        if ( glb_NrtExnFlag == 'y' )
            glb_NrtExnFlag = 'Y';       /* Set to Upper Case */
        else if ( glb_NrtExnFlag == 'n' )
            glb_NrtExnFlag = 'N';       /* Set to Upper Case */

        writeLog(LOG_INF, "NRTRDE_CREATE_EXN -> %c", glb_NrtExnFlag);
    }

    // ----- Get NRTRDE EXN File Directory if required -----
    if ( glb_NrtExnFlag == 'Y') {
        if ( Get_Parameter("NRTRDE_EXN_FILE_DIR", glb_NrtExnFile_Dir) ) {
            writeLog(LOG_ERR, "cannot get NRTRDE_EXN_FILE_DIR from %s", gszIniFullName);
            ( glb_Inifp != NULL ? fclose(glb_Inifp) : 0 );
        glb_Inifp = NULL;
            return FAILURE;
        }
        writeLog(LOG_INF, "NRTRDE_EXN_FILE_DIR -> %s", glb_NrtExnFile_Dir);

        // ----- Check if Error Log path exists and permissions -----
        if ( access(glb_NrtExnFile_Dir, (F_OK|R_OK|W_OK|X_OK))) {
            writeLog(LOG_SYS, "cannot access nrtrde exn file dir %s (%s)", glb_NrtExnFile_Dir, strerror(errno));
            ( glb_Inifp != NULL ? fclose(glb_Inifp) : 0 );
        glb_Inifp = NULL;
            return FAILURE;
        }
    }
    return SUCCESS;
}

int Load_SpltPPS_SysParam()
{
    char Temp[SIZE_INIREC+1];
    char Temp2[SIZE_INIREC+1];
    char *pch = NULL;
    int  Idx;

    memset(Temp, 0x00, sizeof(Temp));
    memset(Temp2, 0x00, sizeof(Temp2));
    memset(glb_SpltType_Flg, FALSE, sizeof(glb_SpltType_Flg));

    // ----- Get type list of PPS CDR to be splitted -----
    if ( Get_Parameter("SPLIT_AWNPPS_TYPE", Temp) ) {
        writeLog(LOG_ERR, "cannot get SPLIT_AWNPPS_TYPE from %s", gszIniFullName);
        ( glb_Inifp != NULL ? fclose(glb_Inifp) : 0 );
        glb_Inifp = NULL;
        return FAILURE;
    }

    strcpy(Temp2, Temp);
    Idx = 0;
    while ( 1 ) {
        if ( !Idx )
            pch = strtok(Temp2, "+");
        else
            pch = strtok((char *)NULL, "+");

        if ( !pch )     /* No More Field */
            break;
        Trim_Str(pch);

        if ( !strcmp (pch, "AWN") ) {
            glb_SpltType_Flg[PPSAWN_AWN] = TRUE;
        }
        else if ( !strcmp (pch, "SMS") ) {
            glb_SpltType_Flg[PPSAWN_SMS] = TRUE;
        }
        else if ( !strcmp (pch, "GPRS") ) {
            glb_SpltType_Flg[PPSAWN_GPRS] = TRUE;
        }
        else {
            writeLog(LOG_ERR, "unknown splitting type -> %s", Temp);
            ( glb_Inifp != NULL ? fclose(glb_Inifp) : 0 );
        glb_Inifp = NULL;
            return FAILURE;
        }
        Idx++;
    }

    // ----- Get PPS CDR Directories -----
    for ( Idx = 0; Idx < NOF_PPSAWN_PATH; Idx++ ) {
        glb_PpsSpltfp[Idx] = NULL;
        if ( glb_SpltType_Flg[Idx] == TRUE ) {
            sprintf(Temp, "SPLIT_AWNPPS_DIR_%s", glb_PpsType[Idx]);
            if ( Get_Parameter(Temp, glb_SpltCdr_Dir[Idx]) ) {
                writeLog(LOG_ERR, "cannot get SPLIT_AWNPPS_DIR_%s from %s", glb_PpsType[Idx], gszIniFullName);
                ( glb_Inifp != NULL ? fclose(glb_Inifp) : 0 );
        glb_Inifp = NULL;
                return FAILURE;
            }
            writeLog(LOG_INF, "%s -> %s", Temp, glb_SpltCdr_Dir[Idx]);

            // ----- Check if Error Log path exists and permissions -----
            if ( access(glb_SpltCdr_Dir[Idx], (F_OK|R_OK|W_OK|X_OK)) ) {
                writeLog(LOG_SYS, "cannot access SPLIT_AWNPPS_DIR_%d dir %s (%s)", Idx, glb_SpltCdr_Dir[Idx], strerror(errno));
                ( glb_Inifp != NULL ? fclose(glb_Inifp) : 0 );
                glb_Inifp = NULL;
                return FAILURE;
            }

            sprintf(Temp, "SPLIT_AWNPPS_SYN_%s", glb_PpsType[Idx]);
            if ( Get_Parameter(Temp, glb_SpltCdr_Syn[Idx]) ) {
                writeLog(LOG_ERR, "cannot get SPLIT_AWNPPS_SYN_%s from %s", glb_PpsType[Idx], gszIniFullName);
                ( glb_Inifp != NULL ? fclose(glb_Inifp) : 0 );
        glb_Inifp = NULL;
                return FAILURE;
            }
            writeLog(LOG_INF, "%s -> %s", Temp, glb_SpltCdr_Syn[Idx]);

            // ----- Check if Error Log path exists and permissions -----
            if ( access(glb_SpltCdr_Syn[Idx], (F_OK|R_OK|W_OK|X_OK)) ) {
                writeLog(LOG_SYS, "cannot access SPLIT_AWNPPS_SYN_%d dir %s (%s)", Idx, glb_SpltCdr_Syn[Idx], strerror(errno));
                ( glb_Inifp != NULL ? fclose(glb_Inifp) : 0 );
        glb_Inifp = NULL;
                return FAILURE;
            }
        }
    }
    return SUCCESS;

}

int Get_Parameter(char *Param, char *Value)
{
    char sRawIniRec[SIZE_INIREC+1];
    char sParamToken[SIZE_INIREC+1];
    char *pstr=NULL;

    rewind(glb_Inifp);

    while ( fgets(sRawIniRec, SIZE_INIREC, glb_Inifp) != NULL ) {   /* while not end of file */

        Trim_IniRec(sRawIniRec);
        if ( *sRawIniRec != '\0' ) {

            /* Parse the fields (delimiter is =) */
            if ( (pstr = strtok(sRawIniRec, "=" )) != NULL ) {

                strcpy(sParamToken, pstr);  /* Parameter Token */
                Trim_Str(sParamToken);

                if ( !strcmp (sParamToken, Param) ) {   /* It is required Parameter */
                    if ( (pstr = strtok((char *)NULL, "=" )) != NULL ) {
                            strcpy(Value, pstr);   /* Copy the Parameter Value */
                            return SUCCESS;
                    }
                }
            }
        }
    }   /* end while end of file */
    return FAILURE;

}

void Trim_IniRec(char *buff)
{
    int i=0, j=0;
    int len = strlen (buff);

    while ( len && *(buff+i) != 0x00 && *(buff+i) != 0x3b && *(buff+i) != 0x0a && i < SIZE_INIREC ) {
        if ( *(buff+i) != 0x20 && *(buff+i) != 0x09) {   /* ignore space and tab char */
            *(buff+j) = *(buff+i);          /* copy - it is valid character */
            j++;
        }
        i++;
    }
    *(buff+j) = '\0';
}

/*
** Name     : Anz_Check_File ()
**
** Description  : Analyze Check Point File.
**        Opens & Reads the Check Point file.
**        Closes the Check Point file.
**        Analyzes the Previous Status.
**        If Recovery Processing required, locates the corresponding CDR file.
**
** Parameters   : None.
**
** Return Value : ANZ_GO_AHEAD - if Recovery not required.
**        ANZ_MAP_HALF - if Recovery Required.
**        FAILURE - on any Error.
*/

int Anz_Check_File()
{

    char Temp[SIZE_FNAME];
    int  rv = 0;

    memset(glb_MsgText, 0x00, sizeof (glb_MsgText));
    memset(Temp, 0x00, sizeof (Temp));

    sleep(5);   /* intentional delay 5 secs. */

    /* Prepare Check Point File Name */
    sprintf(glb_Check_File, "%s/CDR_CHKPT.DAT", glb_MapData_Dir);

    /* Check if Check Point File exists */
    if ( access(glb_Check_File, F_OK) )
        return ANZ_GO_AHEAD;

    /* open Check Point File */
    if ( (glb_Chkfp = fopen(glb_Check_File, MODE_READ)) == NULL ) {
        writeLog(LOG_SYS, "cannot open check point file %s (%s)", glb_Check_File, strerror(errno));
        return FAILURE;
    }

    /* read Check Point File */
    memset(&chk_buf, 0x00, sizeof (chk_buf));
    rv = fread((char *)&chk_buf, sizeof(chk_buf), 1, glb_Chkfp);

    /* close Check Point File */
    fclose(glb_Chkfp);

    if ( rv != 1 ) {  /* Check Record not exists */
        writeLog(LOG_WRN, "check point file empty %s", glb_Check_File);
        unlink(glb_Check_File);    /* clean-up check point file */
        return ANZ_GO_AHEAD;
    }

    /* if CDR Mapping was Completed */
    if ( chk_buf.check_ind == 'C' ) {
        unlink(glb_Check_File);    /* clean-up check point file */
        return ANZ_GO_AHEAD;
    }

    /* if CDR Mapping was not yet started */
    if ( chk_buf.cdr_offset == 0L ) {
        unlink(glb_Check_File);    /* clean-up check point file */
        return ANZ_GO_AHEAD;
    }

    /*
    ** Now we conclude that the Mapping Process was Half Way before Shutdown
    */

    glb_MtxInd = chk_buf.mtx_ind;
    /*
    ** Migrated to HP-FMS V.9.2-1
    ** Setting all CDR Type to Postpaid Partition as default
    ** Except AIN CDR Type Setting to AIN Partition as default
    ** Only SSP CDR is feeded for both Prepaid and Postpaid
    ** for SSP CDR the "glb_PartId" was set for each CDR Record
    **
    ** - Kawee 29-Jan-2007
    */
    if ( chk_buf.mtx_ind == AIN_TYPE )
        glb_PartId = AIN_SVC_PARTITION;     /* AIN Service Partition */
    else
        glb_PartId = AWN_SVC_PARTITION;    /* POSTPAID (AIS,DPC and NRT) Service Partition */

    if ( chk_buf.mtx_ind == NRTRDE_TYPE || chk_buf.mtx_ind == NRGGSN_TYPE || chk_buf.mtx_ind == NRTSCP_TYPE )
        glb_CdrPathType = CDR_NRTRDE;       /* NRTRDE CDR Path */
    else
        glb_CdrPathType = CDR_LOCAL;        /* LOCAL CDR Path */

    strcpy(glb_Cdr_File, chk_buf.cdr_filename);    /* cdr file name is with path now */
    strcat(glb_Sync_File, chk_buf.sync_filename);  /* sync file name is with path now */

    /* check if the CDR file present */
    if ( access(glb_Cdr_File, F_OK) ) { /* Not present */

        writeLog(LOG_SYS, "cannot locate cdr file %s (%s)", glb_Cdr_File, strerror(errno));
        /* rename the Check Point file to indicate error */
        sprintf(Temp, "%s_ERR", glb_Check_File);
        rename(glb_Check_File, Temp);

        /* check if the Sync file present */
        if ( !access(glb_Sync_File, F_OK) ) {   /* present */
            /* rename the Sync File also to indicate error */
            sprintf(Temp, "%s_ERR", glb_Sync_File);
            rename(glb_Sync_File, Temp);
        }
        return ANZ_GO_AHEAD;
    }
    return ANZ_MAP_HALF;

}

/*
** Name     : Start_Recovery ()
**
** Description  : Start Recovery Processing on Specified CDR File.
**        Calls the Proc_CdrFile() to Process the CDR file.
**
** Parameters   : None.
**
** Return Value : Always SUCCESS is returned.
*/

int Start_Recovery()
{

    char Temp[SIZE_FNAME];

    memset(glb_MsgText, 0x00, sizeof (glb_MsgText));
    memset(Temp, 0x00, sizeof (Temp));

    /* Set the Process Status Flag to Indicate Recovery Processing */
    glb_ProcStat = RECV_PROC;

    switch ( Proc_CdrFile() ) {
    case SUCCESS :
        /* Successfull Processing */
        DoBackupCdr(glb_Cdr_File,glb_Sync_File); /* Do Backup CDR and Sync file */
        unlink(glb_Sync_File); /* Remove the Sync File */
        unlink(glb_Cdr_File);  /* Remove the CDR File */
        break;
    case SHUT_SIGNAL_RCVD :
        break;
    default :
        writeLog(LOG_ERR, "recovery processing failed %s", glb_Cdr_File);
        /* rename the sync file to indicate error */
        sprintf(Temp, "%s_ERR", glb_Sync_File);
        rename(glb_Sync_File , Temp);
        /* also rename the Check Point file to indicate error */
        fclose(glb_Chkfp);
        sprintf(Temp, "%s_ERR", glb_Check_File);
        rename(glb_Check_File, Temp);
        break;
    }
    return SUCCESS;

}

/*
** Name     : Bld_Snap_File ()
**
** Description  : Builds Snap File matching the pattern found in location sort by time.
**
** Parameters   : char *location   - INPUT - Specifies the location to be searched.
**                char *pattern    - INPUT - Specifies the Sync File Pattern to be searched.
**                char *snap_fname - INPUT - Specifies the Snap File Name including path.
**
** Return Value : SUCCESS - On Successful Completion
**                FAILURE - When Snapshot File Creation fails
**              errno indicates the cause of error
**
** - Changed function for Build Snapshot by Using Open directory - by Kawee on 02-Jan-2003
*/
int Bld_Snap_File(char *snap_fname)
{
    DIR     *p_dir;
    int     file_cnt = 0, rv=0, path_cnt=0;
    char    str_path[SIZE_FNAME+1];
    char    tempfile[SIZE_FNAME+1];
    char    cmd_string[SIZE_COMMAND];
    FILE    *fp_tempfile = NULL;
    struct dirent *p_dirent;
    struct stat   stat_buf;

    memset(glb_MsgText, 0x00, sizeof (glb_MsgText));
    memset(cmd_string, 0x00, sizeof (cmd_string));

    /* check the parameters */
    if ( *snap_fname == '\0' )
        return FAILURE;

    /* Create Temporary file name */
    memset(tempfile, 0x00, sizeof(tempfile));
    sprintf(tempfile, "%s/TempListFile_XXXXXX", glb_Errlog_Dir);
    mkstemp(tempfile);
    if ( tempfile[0] == '\0' ) {
        writeLog(LOG_SYS, "cannot create temporary file %s (%s)", tempfile, strerror(errno));
        return FAILURE;
    }

    /* Open temporary file name for keep current matching file entry */
    if ( (fp_tempfile = fopen(tempfile, "wb")) == NULL ) {
        writeLog(LOG_SYS, "cannot open temporary file %s (%s)", tempfile, strerror(errno));
        return FAILURE;
    }

    /* We assume valid sync info are passed */
    file_cnt = 0;
    for ( path_cnt=0; path_cnt < NOF_CDR_PATH; path_cnt++ ) {
        if ( !glb_NrtInstFlag && (path_cnt == CDR_NRTRDE) )
            continue;
        else if ( !glb_AisInstFlag && !glb_DpcInstFlag && !glb_12cInstFlag && (path_cnt == CDR_LOCAL) )
            continue;

        /* Open directory */
        p_dir = opendir(glb_SnapInfo[path_cnt].sync_filedir);
        if ( p_dir == NULL ) {    /* cannot open directory */
            writeLog(LOG_WRN, "cannot Open directory %s", glb_SnapInfo[path_cnt].sync_filedir);
            fclose(fp_tempfile);
            fp_tempfile = NULL;
            unlink(tempfile);
            return FAILURE;
        }

        /* Read the directory and build the matching file list */
        while ( (p_dirent = readdir(p_dir)) != NULL ) {
            /* file name match with file type */
            if ( IsMatchFileType(p_dirent->d_name, glb_SnapInfo[path_cnt].sync_pattern) ) {
                memset(str_path, 0x00, sizeof(str_path));
                memset(&stat_buf, 0x00, sizeof(stat_buf));
                sprintf(str_path, "%s/%s", glb_SnapInfo[path_cnt].sync_filedir, p_dirent->d_name);
                rv = lstat(str_path, &stat_buf);
                if ( !rv ) {  /* complete get file stat */
                    /* write the time, filename */
                    fprintf(fp_tempfile, "%012ld %d %s\n", stat_buf.st_mtime, path_cnt, str_path);
                    file_cnt++;
                }
            }
        }
        closedir(p_dir);   /*  close the directory stream. */
    }
    fclose(fp_tempfile);
    fp_tempfile = NULL;

    if ( !file_cnt ) {    /* No more Sync file */
        unlink(tempfile);
        errno = ENOENT;
        return FAILURE;
    }

    /* sort by date-time field by call shell command */
    sprintf(cmd_string, "sort -k 1,1 %s > %s 2>/dev/null", tempfile, snap_fname);
    Deb_Msg(cmd_string);

    rv = system(cmd_string);
    if ( rv != 0 ) {
        sprintf(glb_MsgText, "cannot sort file %s", tempfile );
        Deb_Msg(glb_MsgText);
        unlink(tempfile);
        return FAILURE;
    }

    unlink(tempfile);  /* Remove the tmporary file */
    return SUCCESS;

}

/* Added function below for Changed Build Snapshot concept by Using Open directory - by Kawee on 02-Jan-2003 */
/*
** Name     : IsMatchFileType ()
**
** Description  : Matching pattern with filename.
**
** Parameters   : char *filename - INPUT - Specifies the target filename.
**                char *pattern  - INPUT - Specifies the File Pattern to be matching.
**
** Return Value : TRUE  - On Filename match with pattern
**                FALSE - When Filename does not match
*/
int IsMatchFileType (char* filename, char* pattern)
{
    int len_filename = 0, len_pattern = 0;
    char    *pch_pattern;

    if(pattern[0] == '*')
        pch_pattern = pattern+1;
    else
        pch_pattern = pattern;

    len_filename = strlen (filename);
    len_pattern  = strlen (pch_pattern);

    if( len_filename < (len_pattern + 3) )
        return FALSE;

    /* Check Invalid File Pattern */
    if ( fnmatch(pattern, filename, 0))
        return FALSE;

    /* Added below for skip files of other installation - by Kawee on 13-Jan-2003 */

    /* Feed All AIS Files when include AIS Installation */
    if ( glb_AisInstFlag && !strncmp(filename, "AIS", 3) )
        return TRUE;

    /* Feed All DPC Files when include DPC Installation */
    if ( glb_DpcInstFlag &&
         (!strncmp(filename, "DPC", 3) || !strncmp(filename, "AIS.SSP", 7)))
        return TRUE;

    /* Feed only SSP CDR File when include 12C Installation */
    if ( glb_12cInstFlag && !strncmp(filename, "AIS.SSP", 7) ) {
        return TRUE;
    }

    /* Introduce AIN CDR Type - Added below by Kawee on 29-Jan-2007 */
    /* Feed only AIN CDR File when include AIN Installation */
    if ( glb_AinInstFlag && !strncmp(filename, "AIN", 3) ) {
        return TRUE;
    }

    /* Introduce NRTRDE CDR Type - Added below by Kawee on 11-Feb-2008 */
    /* Feed only NRTRDE CDR File when include NRTRDE Installation */
    if ( glb_NrtInstFlag && (!strncmp(filename, "NRTRDE", 6) || !strncmp(filename, "GGSN", 4) || !strncmp(filename, "SCP", 3))) {
        return TRUE;
    }

    if ( glb_AisInstFlag && !strncmp(filename, "AWN", 3) )
        return TRUE;

    return FALSE;
}

/* Added function below for Changed Build Snapshot concept by Using Open directory - by Kawee on 02-Jan-2003 */
/*
** Name     : Get_Sync_Fname ()
**
** Description  : Get CDR Sync Filename from Opened Snapshot file.
**
** Parameters   : FILE *fp_snap  - INPUT - Specifies the opened Snapshot file pointer.
**                int  *piCdrPathType - OUTPUT - Pointer to integer for get CDR Path Type.
**                char *sSynFile - OUTPUT - Pointer to buffer for get Sync filename.
**
** Return Value : SUCCESS - On Have more Sync Filename in Snapshot file.
**                FAILURE - When End of Snapshot file.
*/
int Get_Sync_Fname (FILE *fp_snap, int *piCdrPathType, char *sSynFile)
{

    char stime[13];          /* Time string */

    memset(stime, 0x00, sizeof(stime));

    if ( fscanf(fp_snap, "%s%d%s", stime, piCdrPathType, sSynFile ) == EOF )    /* End of Snapshot file */
        return FAILURE;
    else
        return SUCCESS;

}

/************************************************************************************************/

/*
** Name     : Open_Cdr_File()
**
** Description  : Prefixes the CDR Directory path to the Filename.
**        Opens the CDR File.
**
** Parameters   : char *Mode   - INPUT - Specifies the File Opening mode.
**
** Return Value : SUCCESS      - on Successful File Open.
**        FAILURE      - on any Error.
*/

int Open_Cdr_File(char *Mode)
{
    char Temp[SIZE_FNAME];

    memset(glb_MsgText, 0x00, sizeof (glb_MsgText));
    memset(Temp, 0x00, sizeof (Temp));
    glb_Cdrfp = NULL;

    /*
    ** Note - glb_Cdr_File is already filed-in as follows:
    ** for Normal Processing, the path name is not present - so add path to CDR File name
    ** for Recovery Processing, the Path name is already present and so no need to add path
    */

    if ( glb_ProcStat == NORM_PROC ) {
        sprintf(Temp, "%s/%s", glb_Cdrfile_Dir[glb_CdrPathType], glb_Cdr_File);
        strcpy(glb_Cdr_File, Temp);
    }

    sprintf(glb_MsgText, "Open_Cdr_File: File = %s", glb_Cdr_File);
/* writeLog(LOG_SYS, "Open_Cdr_File: File = %s, glb_UseRatedCdr %d", glb_Cdr_File, glb_UseRatedCdr); */
    Deb_Msg(glb_MsgText);

    /* open CDR File */
    if ( (glb_Cdrfp = fopen(glb_Cdr_File, Mode)) == NULL ) {
        writeLog(LOG_SYS, "cannot open cdr file %s (%s)", glb_Cdr_File, strerror(errno));
        return FAILURE;
    }
    return SUCCESS;

}

/*
** Name     : Open_Check_File()
**
** Description  : Opens the Check Point File.
**
** Parameters   : char *Mode   - INPUT - Specifies the File Opening mode.
**
** Return Value : SUCCESS      - on Successful File Open.
**        FAILURE      - on any Error.
*/

int Open_Check_File(char *Mode)
{

    memset(glb_MsgText, 0x00, sizeof (glb_MsgText));
    glb_Chkfp = NULL;

    /* Note - Check Point File Name is already prepared in main () */

    sprintf(glb_MsgText, "Open_Check_File: File = %s", glb_Check_File);
    Deb_Msg(glb_MsgText);

    /* open Check Point File */
    if ( (glb_Chkfp = fopen(glb_Check_File, Mode)) == NULL ) {
        writeLog(LOG_SYS, "cannot open check point file %s (%s)", glb_Check_File, strerror(errno));
        return FAILURE;
    }
    return SUCCESS;

}

/*
** Name         : Open_Error_File()
**
** Description  : Prefixes the Error Log Directory path to the Filename.
**                Opens Error File.
**                This function open 2 files, Error File and Ignore File
**                Error File is intended to keep info of particular CDR that is error (mapping failed or invalid data)
**                Ignore File is intended to keep info of particular CDR that is ingore (rejected cdr based on customer's condition)
**                Both Error and Ignore File does not keep full CDR record but only some information
**
** Parameters   : char *Mode   - INPUT - Specifies the File Opening mode.
**
** Return Value : SUCCESS      - on Successful File Open.
**        FAILURE      - on any Error.
*/

int Open_Error_File(char *Mode)
{
    char szPath[SIZE_FNAME+1];

    memset(glb_MsgText, 0x00, sizeof (glb_MsgText));
    memset(szPath, 0x00, sizeof(szPath));

    glb_Errfp = NULL;
    glb_Ignfp = NULL;

    sprintf(szPath, "%s/%s", glb_Errlog_Dir, getSysDTM(DTM_DATE_ONLY));
    if ( access(szPath, F_OK) != 0 ) {
        if ( mkdir(szPath, 0777) != 0 ) {
            writeLog(LOG_SYS, "cannot create dir for %s (%s)", szPath, strerror(errno));
            return FAILURE;
        }
    }

    /* Prepare Error File Name */
    sprintf(glb_Error_File, "%s/%s", szPath, (char *)Remove_Path(glb_Cdr_File));
    if ( strcmp(glb_Error_File + strlen(glb_Error_File) - 4, ".n00") )  /* Always Create Error File with the Extension .n00 */
        strcat(glb_Error_File, ".dat");
    strcat(glb_Error_File, ".ERR");

    sprintf(glb_MsgText, "Open_Error_File: File = %s", glb_Error_File);
    Deb_Msg(glb_MsgText);

    /* open CDR Mapping Error File */
    if ( (glb_Errfp = fopen(glb_Error_File, Mode)) == NULL ) {
        writeLog(LOG_SYS, "cannot open error file %s (%s)", glb_Error_File, strerror(errno));
        return FAILURE;
    }
    //fprintf(glb_Errfp, "====\n");

    /* Prepare Ignore data File Name */
    sprintf(glb_Ignore_File, "%s/%s", szPath, (char *)Remove_Path(glb_Cdr_File));
    if ( strcmp(glb_Ignore_File + strlen(glb_Ignore_File) - 4, ".n00")) /* Always Create Ignore File with the Extension .n00 */
        strcat(glb_Ignore_File, ".dat");
    strcat(glb_Ignore_File, ".IGN");

    sprintf(glb_MsgText, "Open_Error_File: File = %s", glb_Ignore_File);
    Deb_Msg(glb_MsgText);

    /* open Ignore data File*/
    if ( (glb_Ignfp = fopen(glb_Ignore_File, Mode)) == NULL ) {
        writeLog(LOG_SYS, "cannot open ignore file %s (%s)", glb_Ignore_File, strerror(errno));
        fclose(glb_Errfp);
        return FAILURE;
    }
    //fprintf(glb_Ignfp, "====\n");
    return SUCCESS;

}

/*
** Name         : Open_Reject_File()
**
** Description  : Prefixes the Error Log Directory path to the Filename.
**                Opens Rejected CDR File.
**                Reject File is intended to keep on CDR record(full record) of that is error or ignore
**                in order to later data verification.
**
** Parameters   : char *Mode   - INPUT - Specifies the File Opening mode.
**
** Return Value : SUCCESS      - on Successful File Open.
**        FAILURE      - on any Error.
*/

int Open_Reject_File(char *Mode)
{

    memset(glb_MsgText, 0x00, sizeof (glb_MsgText));
    glb_RCdrfp = NULL;

    /* Prepare Rejected(Ignore) CDR File Name */
    sprintf(glb_Reject_File, "%s/%s", glb_Errlog_Dir, (char *)Remove_Path(glb_Cdr_File));
    if ( strcmp(glb_Reject_File + strlen(glb_Reject_File) - 4, ".n00") ) /* Always Create Reject File with the Extension .n00 */
        strcat(glb_Reject_File, ".dat");
    strcat(glb_Reject_File, ".REJ");

    sprintf(glb_MsgText, "Open_Reject_File: File = %s", glb_Reject_File);
    Deb_Msg(glb_MsgText);

    /* open Rejected(Ignored CDR) CDR File */
    if ( (glb_RCdrfp = fopen(glb_Reject_File, Mode)) == NULL ) {
        writeLog(LOG_SYS, "cannot open cdr reject(for error data) file %s (%s)", glb_Reject_File, strerror(errno));
        return FAILURE;
    }
    return SUCCESS;

}

/*
** Name     : Open_EventCdr_File()
**
** Description  : Prefixes the CDR Repository Directory path, Date and Time to the Filename.
**        Opens EVENT CDR File.
**
** Parameters   : char *Mode   - INPUT - Specifies the File Opening mode.
**
** Return Value : SUCCESS      - on Successful File Open.
**        FAILURE      - on any Error.
*/
int Open_EventCdr_File(char *Mode)
{
    //char fname[1024];
    char tmp_fname[256], *p = NULL;
    memset(glb_MsgText, 0x00, sizeof (glb_MsgText));
    memset(tmp_fname, 0x00, sizeof(tmp_fname));

    glb_Eventcdrfp = NULL;
    strcpy(tmp_fname, Remove_Path(glb_Cdr_File));
    p = strrstr(tmp_fname, ".dat");
    p = 0;

    /* Prepare EVENT CDR File Name */
    glb_CurrCdrRepo = (glb_CurrCdrRepo + 1) % glb_NofCdrRepo;

    sprintf(glb_EventCdr_File, "%s/cdr.%s_%s.dat.tmp", glb_CdrRepo_Dir[glb_CurrCdrRepo], getSysDTM(DTM_DATE_TIME), tmp_fname);
//printf("dbg: cdr input file \"%s\" -> ", glb_EventCdr_File);
    //char *p = strrstr(glb_EventCdr_File, ".n00");
    //if ( strcmp(glb_EventCdr_File + strlen(glb_EventCdr_File) - 4, ".n00") )
    //    strcat(glb_EventCdr_File, ".dat");
    //*p = 0;
    //strcat(glb_EventCdr_File, ".dat.tmp");
//printf("\"%s\"\n", glb_EventCdr_File);
    sprintf(glb_MsgText, "Open_EventCdr_File: File = %s", glb_EventCdr_File);
    Deb_Msg(glb_MsgText);

    /* Open EVENT CDR File */
    if ( (glb_Eventcdrfp = fopen(glb_EventCdr_File, Mode)) == NULL ) {
        writeLog(LOG_SYS, "cannot open event cdr file %s (%s)", glb_EventCdr_File, strerror(errno));
        return FAILURE;
    }
    return SUCCESS;

}

int Open_SvcIdSub_File(char *Mode)
{
    memset(glb_MsgText, 0x00, sizeof (glb_MsgText));
    glb_SvcIdSubfp = NULL;

    sprintf(glb_SvcIdSub_File, "%s/SUBS_%s_%s", glb_SvcIdSub_Dir, getSysDTM(DTM_DATE_TIME), Remove_Path(glb_Cdr_File));
    sprintf(glb_MsgText, "Open_SvcIdSub_File: File = %s", glb_SvcIdSub_File);
    Deb_Msg(glb_MsgText);

    /* Open EVENT CDR File */
    if ( (glb_SvcIdSubfp = fopen(glb_SvcIdSub_File, Mode)) == NULL ) {
        writeLog(LOG_SYS, "cannot open service_id entity file %s (%s)", glb_SvcIdSub_File, strerror(errno));
        return FAILURE;
    }
    return SUCCESS;
}
/*
** Name     : Open_EAlmData_File()
**
** Description  : Prefixes the CDR Directory path to the Filename.
**        Opens External Alarm Data File.
**
** Parameters   : char *Mode   - INPUT - Specifies the File Opening mode.
**
** Return Value : SUCCESS      - on Successful File Open.
**        FAILURE      - on any Error.
*/
int Open_EAlmData_File(char *Mode)
{

    char Temp[SIZE_FNAME];

    memset(glb_MsgText, 0x00, sizeof (glb_MsgText));
    memset(Temp, 0x00, sizeof (Temp));
    glb_PrepaidEAlmfp = NULL;
    glb_PostpaidEAlmfp = NULL;
    glb_AinEAlmfp = NULL;
    glb_NrtEAlmfp = NULL;

    /* Reset Counter */
    glb_EAlmPostpaidCtr = 0;
    glb_EAlmPrepaidCtr = 0;
    glb_EAlmAinCtr = 0;
    glb_EAlmNrtCtr = 0;

    /* Check EXN flag - added by kawin on 19-Mar-2004 */
    if ( (glb_AisInstFlag || glb_DpcInstFlag) && glb_PostpaidExnFlag == 'Y' ) {   /* Create EXN for Postpaid Service */
        /* Prepare Postpaid or Prepaid External Alarm Data File Name */ /* added by kawin on 25-Nov-2002 */
        sprintf(glb_PostpaidEAlmData_File, "%s/%s", glb_PostpaidExnFile_Dir, (char *)Remove_Path(glb_Cdr_File));
        if ( strcmp(glb_PostpaidEAlmData_File + strlen(glb_PostpaidEAlmData_File) - 4, ".n00") )    /* Always Create Alarm File with the Extension .n00 */
            strcat(glb_PostpaidEAlmData_File, ".dat");

        /*
        ** Unique name creation format is changed for example:
        ** Old name XXXXXX_AIS.TLC1.00082238.n00.EXN is changed to AIS.TLC1.00082238.n00_XXXXXX.EXN
        **  - Chamy on 14-Jan-2002
        */
        if ( glb_ProcStat == RECV_PROC ) {
            /* on recovery processing we open Actual name + Unique name */
            strcat(glb_PostpaidEAlmData_File, "_XXXXXX");
            strcpy(Temp, glb_PostpaidEAlmData_File);
            mkstemp(Temp); /* Make Unique name */

            /* check if mkstemp fails */
            if ( *Temp == '\0' ) {
                writeLog(LOG_SYS, "mkstemp failed for exn %s (%s)", glb_PostpaidEAlmData_File);
                return FAILURE;
            }
            strcpy(glb_PostpaidEAlmData_File, Temp);
        }
        strcat(glb_PostpaidEAlmData_File, EALM_TEMP_EXT);   /* First write to a Temporary External Alarm file */

        sprintf(glb_MsgText, "Open_EAlmData_File: File = %s", glb_PostpaidEAlmData_File);
        Deb_Msg(glb_MsgText);

        /* open CDR Mapping Error File */
        if ( (glb_PostpaidEAlmfp = fopen(glb_PostpaidEAlmData_File, Mode)) == NULL ) {
            writeLog(LOG_SYS, "cannot open external alarm data file %s (%s)", glb_PostpaidEAlmData_File, strerror(errno));
            return FAILURE;
        }
    }

    /* Check EXN flag - added by Kawee on 28-Dec-2005 */
    if ( glb_12cInstFlag && glb_PrepaidExnFlag == 'Y' ) {   /* Create EXN for Prepaid Service */
        /* Prepare Prepaid or Prepaid External Alarm Data File Name */  /* added by kawin on 25-Nov-2002 */
        sprintf(glb_PrepaidEAlmData_File, "%s/%s", glb_PrepaidExnFile_Dir, (char *)Remove_Path(glb_Cdr_File));
        if ( strcmp(glb_PrepaidEAlmData_File + strlen(glb_PrepaidEAlmData_File) - 4, ".n00") )  /* Always Create Alarm File with the Extension .n00 */
            strcat(glb_PrepaidEAlmData_File, ".dat");

        /*
        ** Unique name creation format is changed for example:
        ** Old name XXXXXX_AIS.TLC1.00082238.n00.EXN is changed to AIS.TLC1.00082238.n00_XXXXXX.EXN
        **  - Chamy on 14-Jan-2002
        */
        if ( glb_ProcStat == RECV_PROC ) {
            /* on recovery processing we open Actual name + Unique name */
            strcat(glb_PrepaidEAlmData_File, "_XXXXXX");
            strcpy(Temp, glb_PrepaidEAlmData_File);
            mkstemp(Temp); /* Make Unique name */

            /* check if mkstemp fails */
            if ( *Temp == '\0' ) {
                writeLog(LOG_SYS, "mkstemp failed for exn %s (%s)", glb_PrepaidEAlmData_File, strerror(errno));
                return FAILURE;
            }
            strcpy(glb_PrepaidEAlmData_File, Temp);
        }
        strcat(glb_PrepaidEAlmData_File, EALM_TEMP_EXT);   /* First write to a Temporary External Alarm file */

        sprintf(glb_MsgText, "Open_EAlmData_File: File = %s", glb_PrepaidEAlmData_File);
        Deb_Msg(glb_MsgText);

        /* open CDR Mapping Error File */
        if ( (glb_PrepaidEAlmfp = fopen(glb_PrepaidEAlmData_File, Mode)) == NULL ) {
            writeLog(LOG_SYS, "cannot open external alarm data file %s (%s)", glb_PrepaidEAlmData_File, strerror(errno));
            return FAILURE;
        }
    }

    /* Introduce AIN CDR Type - Added below by Kawee on 29-Jan-2007 */
    if ( glb_AinInstFlag && glb_AinExnFlag == 'Y' ) {   /* Create EXN for AIN Service */
        /* Prepare AIN External Alarm Data File Name */
        sprintf(glb_AinEAlmData_File, "%s/%s", glb_AinExnFile_Dir, (char *)Remove_Path(glb_Cdr_File));
        /* Always Create Alarm File with the Extension .n00 */
        if ( strcmp(glb_AinEAlmData_File + strlen(glb_AinEAlmData_File) - 4, ".n00") )
            strcat(glb_AinEAlmData_File, ".dat");

        /*
        ** Unique name creation format is changed for example:
        ** Old name XXXXXX_AIS.TLC1.00082238.n00.EXN is changed to AIS.TLC1.00082238.n00_XXXXXX.EXN
        **  - Chamy on 14-Jan-2002
        */
        if ( glb_ProcStat == RECV_PROC ) {
            /* on recovery processing we open Actual name + Unique name */
            strcat(glb_AinEAlmData_File, "_XXXXXX");
            strcpy(Temp, glb_AinEAlmData_File);
            mkstemp(Temp); /* Make Unique name */

            /* check if mkstemp fails */
            if ( *Temp == '\0' ) {
                writeLog(LOG_SYS, "mkstemp failed for exn %s (%s)", glb_AinEAlmData_File, strerror(errno));
                return FAILURE;
            }
            strcpy(glb_AinEAlmData_File, Temp);
        }
        strcat(glb_AinEAlmData_File, EALM_TEMP_EXT);   /* First write to a Temporary External Alarm file */

        sprintf(glb_MsgText, "Open_EAlmData_File: File = %s", glb_AinEAlmData_File);
        Deb_Msg(glb_MsgText);

        /* open CDR Mapping Error File */
        if ( (glb_AinEAlmfp = fopen(glb_AinEAlmData_File, Mode)) == NULL ) {
            writeLog(LOG_SYS, "cannot open external alarm data file %s (%s)", glb_AinEAlmData_File, strerror(errno));
            return FAILURE;
        }
    }

    /* Introduce NRTRDE CDR Type - Added below by Kawee on 11-Feb-2008 */
    if ( glb_NrtInstFlag && glb_NrtExnFlag == 'Y' ) {     /* Create EXN for NRTRDE Service */
        /* Prepare NRTRDE External Alarm Data File Name */
        sprintf(glb_NrtEAlmData_File, "%s/%s", glb_NrtExnFile_Dir, (char *)Remove_Path(glb_Cdr_File));
        /* Always Create Alarm File with the Extension .n00 */
        if ( strcmp(glb_NrtEAlmData_File + strlen(glb_NrtEAlmData_File) - 4, ".n00") )
            strcat(glb_NrtEAlmData_File, ".dat");

        /*
        ** Unique name creation format is changed for example:
        ** Old name XXXXXX_AIS.TLC1.00082238.n00.EXN is changed to AIS.TLC1.00082238.n00_XXXXXX.EXN
        **  - Chamy on 14-Jan-2002
        */
        if ( glb_ProcStat == RECV_PROC ) {
            /* on recovery processing we open Actual name + Unique name */
            strcat(glb_NrtEAlmData_File, "_XXXXXX");
            strcpy(Temp, glb_NrtEAlmData_File);
            mkstemp(Temp); /* Make Unique name */

            /* check if mkstemp fails */
            if ( *Temp == '\0' ) {
                writeLog(LOG_SYS, "mkstemp failed for exn %s (%s)", glb_NrtEAlmData_File, strerror(errno));
                return FAILURE;
            }
            strcpy(glb_NrtEAlmData_File, Temp);
        }
        strcat(glb_NrtEAlmData_File, EALM_TEMP_EXT);   /* First write to a Temporary External Alarm file */

        sprintf(glb_MsgText, "Open_EAlmData_File: File = %s", glb_NrtEAlmData_File);
        Deb_Msg(glb_MsgText);

        /* open CDR Mapping Error File */
        if ( (glb_NrtEAlmfp = fopen(glb_NrtEAlmData_File, Mode)) == NULL ) {
            writeLog(LOG_SYS, "cannot open external alarm data file %s (%s)", glb_NrtEAlmData_File, strerror(errno));
            return FAILURE;
        }
    }
    return SUCCESS;

}

/************************************************************************************************/


/*
** Name     : Upd_Check_File ()
**
** Description  : Prepares the Check Point Buffer.
**        Calls getSysDTM() to get the Current System Date.
**        Calls getSysDTM() to get the Current System Time.
**        Updates the Check Point File.
**
** Parameters   : None
**
** Return Value : void
*/

void Upd_Check_File(char Chk_ind)
{

    rewind(glb_Chkfp);

    /* Prepare Check Point Buffer */
    strcpy(chk_buf.proc_date, getSysDTM(DTM_DATE_ONLY));
    strcpy(chk_buf.proc_time, getSysDTM(DTM_TIME_ONLY));
    strcpy(chk_buf.cdr_filename, glb_Cdr_File);
    strcpy(chk_buf.sync_filename, glb_Sync_File);
    chk_buf.cdr_offset = (long)ftell(glb_Cdrfp);

    chk_buf.mtx_ind = glb_MtxInd;

    chk_buf.check_ind = Chk_ind;

    /* write the buffer to file */
    fwrite(&chk_buf, sizeof(chk_buf), 1, glb_Chkfp);

    /* sync the changes to permanent storage */
    fflush(glb_Chkfp);

}

/*
** Name     : Append_Slash ()
**
** Description  : Appends / to a path, if it does not end with one.
**
** Parameters   : char *path  - INPUT/OUTPUT - path to check and append.
**
** Return Value : void
*/

void Append_Slash (char *path)
{
    register size_t last=strlen((char *)path)-1;

    if(path[last]!=DIR_CHAR)
        strcat((char *)path, DIR_STRG);
}


/*
** Name     : Remove_Path()
**
** Description  : Removes the Directory Path if present.
**
** Parameters   : char *Pname - INPUT - Filname including Path (read only).
**                If NULL is passed, Nothing is done.
**
** Return Value : character pointer to the resultant filename.
**
*/

char *Remove_Path(char *Pname)
{
    char *pstr;

    if ( Pname != NULL ) {
        if ( (pstr = (char *)strrchr(Pname, DIR_CHAR)) != NULL )
            return ((char *)++pstr);
        else
            return Pname;
    }
    return Pname;
}

int Rename_ExtFile(char *Fname, char *Oext, char *Next)
{

    int rv = 0;
    char *pstr = '\0';
    char *New_Fname = (char*)strdup(Fname);

    if ( Fname != NULL && Oext != NULL && Next != NULL ) {
        if ( (pstr = (char *)strstr(New_Fname, Oext)) != NULL ) {
            *pstr = '\0';
            strcat(New_Fname, Next);
        }
    }
    rv = rename(Fname, New_Fname);
    chmod(New_Fname, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    free(New_Fname);
    return rv;

}

void Print_Stat()
{

    char szDTM[SIZE_YYYYMMDD+SIZE_HHMMSS+1];
    unsigned long OthCtr = glb_ReadCtr - glb_MapCtr - glb_DuraCtr - glb_IgnCtr - glb_OldCtr - glb_ErrCtr;

    memset(szDTM, 0x00, sizeof(szDTM));
    sprintf(szDTM, "%s-%s", getSysDTM(DTM_DATE_ONLY), getSysDTM(DTM_TIME_ONLY));

    fprintf(glb_Errfp, "[INF] %s @ %s\n", glb_MsgText, szDTM);
    fprintf(glb_Ignfp, "[INF] %s @ %s\n", glb_MsgText, szDTM);

    memset(glb_MsgText, 0x00, sizeof(glb_MsgText));
    sprintf(glb_MsgText, "[SUM] total= %ld map= %ld err= %ld ign= %ld (old=%ld dur=%ld oth=%ld)\n",
            glb_ReadCtr, glb_MapCtr, glb_ErrCtr, (glb_IgnCtr+glb_DuraCtr+glb_OldCtr), glb_OldCtr, glb_DuraCtr, OthCtr);

    fprintf(glb_Errfp, glb_MsgText);
    fprintf(glb_Ignfp, glb_MsgText);

}

int Sort_EventOutput(char *file_input)
{

    char cmd_string[SIZE_COMMAND+SIZE_COMMAND];
    char file_sort[SIZE_FNAME];
    char n00_extension[SIZE_FNAME];
    char *p = NULL;

    strcpy(n00_extension, file_input);
    p = strrstr(n00_extension, ".tmp");
    *p = 0;

    memset(cmd_string, 0x00, sizeof(cmd_string));
    sprintf(file_sort, "%s.sort", n00_extension);
#if 0
    if ( glb_EventId == SMS_EVENT_TYPE )
        sprintf(cmd_string, "sort -T '%s' -t'|' -k 3,3 -k 6,6 '%s' > '%s' 2>/dev/null", glb_CdrBck4IUM_Dir, file_input, file_sort);
    else if ( glb_EventId == VOICE_EVENT_TYPE || glb_EventId == FIXED_EVENT_TYPE || glb_EventId == VOICE_EVENT_TYPE_IDD )
        sprintf(cmd_string, "sort -T '%s' -t'|' -k 3,3 -k 7,7 '%s' > '%s' 2>/dev/null", glb_CdrBck4IUM_Dir, file_input, file_sort);
    else if ( glb_EventId == VAS_EVENT_TYPE )
        sprintf(cmd_string, "sort -T '%s' -t'|' -k 3,3 -k 5,5 '%s' > '%s' 2>/dev/null", glb_CdrBck4IUM_Dir, file_input, file_sort);
    else if ( glb_EventId == DATA_EVENT_TYPE )
        sprintf(cmd_string, "sort -T '%s' -t'|' -k 3,3 -k 5,5 '%s' > '%s' 2>/dev/null", glb_CdrBck4IUM_Dir, file_input, file_sort);
    else if ( glb_EventId == IR_EVENT_TYPE )
        sprintf(cmd_string, "sort -T '%s' -t'|' -k 3,3 -k 6,6 '%s' > '%s' 2>/dev/null", glb_CdrBck4IUM_Dir, file_input, file_sort);
#endif

    sprintf(cmd_string, "sort -T '%s' -t'|' -k 3,3 -k 11,11 '%s' > '%s' 2>/dev/null", glb_CdrBck4IUM_Dir, file_input, file_sort);

/* printf("dbg: sort \"%s\"\n", cmd_string); */
    // first; write event to -> 'cdr_YMDHMS_org.n00.tmp'
    // second; sort 'cdr_YMDHMS_org.n00.tmp' to -> 'cdr_YMDHMS_org.n00.sort'
    // finally; mv 'cdr_YMDHMS_org.n00.sort' to -> 'cdr_YMDHMS_org.n00.tmp'
    if ( system(cmd_string) == 0 ) {   // Sort success
        memset(cmd_string, 0x00, sizeof(cmd_string));
        sprintf(cmd_string, "mv %s %s", file_sort, file_input);
/* printf("dbg: move sort: \"%s\"\n", cmd_string); */
        system(cmd_string);
        return SUCCESS;
    }
    else {  // Sort failed
        unlink(file_sort);
        return FAILURE;
    }

}

/* Create Sync File for EVENT CDR File */
int Create_EventSync()
{

    /* char cmd_string[SIZE_MESSAGE]; - chamy 13-Jul-2004 */
    char cmd_string[SIZE_COMMAND];
    char n00_extension[SIZE_FNAME];
    char *p = NULL;

    memset(glb_MsgText, 0x00, sizeof (glb_MsgText));
    memset(cmd_string, 0x00, sizeof (cmd_string));

    strcpy(n00_extension, glb_EventCdr_File);
    p = strrstr(n00_extension, ".tmp");
    *p = 0;
//printf("dbg: input file name: %s\n", glb_EventCdr_File);
    Sort_EventOutput(glb_EventCdr_File);

    DoBackupCdr4IUM(glb_EventCdr_File);

    sprintf(cmd_string, "mv %s %s", glb_EventCdr_File, n00_extension);
//printf("dbg: move file to be finished \"%s\"\n\n", cmd_string);
    if ( system(cmd_string) )  /* Non-Zero */
        return FAILURE;

    sprintf(glb_MsgText, "Create_EventSync : Command string is '%s'", cmd_string);
    Deb_Msg(glb_MsgText);

    return SUCCESS;

}

int Init_Main (const char *avg0)
{

    char Error[SIZE_MESSAGE];
    char Log_Dir[SIZE_FNAME];
    char appPath[SIZE_INIREC+1];
    char temp[SIZE_INIREC+1];
    int  nResult;

            glb_Inifp = NULL;
    memset(Log_Dir, 0x00, sizeof (Log_Dir));
    memset(glb_Log_File, 0x00, sizeof (glb_Log_File));

    strcpy(appPath, avg0);
    char *p = strrchr(appPath, '/');
    *p = '\0';

    memset(gszIniFullName, 0x00, sizeof(gszIniFullName));
    sprintf(gszIniFullName, "%s/%s%s", appPath, _APP_NAME_, _INI_SUF_);

    /* Open Ini File */
    if ( (glb_Inifp = fopen(gszIniFullName, MODE_READ)) == NULL ) {
        //writeLog(LOG_SYS, "cannot open %s (%s)", gszIniFullName, strerror(errno));
        fprintf(stderr, "cannot open %s (%s)", gszIniFullName, strerror(errno));
        return FAILURE;
    }

    if ( Get_Parameter("LOG_DIR", temp) ) {
        if ( (char *)getenv("LOGS") ) {
            strcpy(Log_Dir, (char *)getenv("LOGS"));
            // check if that path exists
            if ( access(Log_Dir, (F_OK|R_OK|W_OK|X_OK))) {  // Not exists
                strcpy(Log_Dir, "./");
            }
        }
        else {
            strcpy(Log_Dir, "./");
        }
    }
    else {
        strcpy(Log_Dir, temp);
    }

    if ( glb_DbgFlag ) {
        nResult = startLogging(Log_Dir, _APP_NAME_, LOG_DBG);
    }
    else {
        nResult = startLogging(Log_Dir, _APP_NAME_, LOG_INF);
    }

    if ( nResult != SUCCESS ) {
        return nResult;
    }

    // Log startup message
    writeLog(LOG_INF, "------------------ Start CDR Feeder ------------------");
    writeLog(LOG_INF, "%s v.%s, for %s", _APP_NAME_, APP_VERSION, _APP_FOR_);

    // Setup Signal Handling
    if ( Handle_Signal(Error) ) {
        writeLog(LOG_SYS, Error);
        return FAILURE;
    }
    return SUCCESS;

}

void Close_Main ()
{

    stopLogging();

}

int Handle_Signal(char *sError)
{

    struct sigaction    Action_Abort, Action_Cont;
    char            sErr[SIZE_MESSAGE];

    memset(sErr, 0x00, sizeof (sErr));

    /* Define structure for a Continue Signal Handler */
    sigemptyset ( &Action_Cont.sa_mask );
    Action_Cont.sa_flags = 0;
    Action_Cont.sa_handler = CatchAndCont;

    /* Define structure for an Abort Signal Handler */
    sigemptyset ( &Action_Abort.sa_mask );
    Action_Abort.sa_flags = 0;
    Action_Abort.sa_handler = CatchAndExit;

    /* Trap the following signals before Termination */
    if ( (sigaction (SIGABRT, &Action_Abort, NULL) == -1 && strcpy(sErr, "SIGABRT")) ||
         //(sigaction (SIGEMT,  &Action_Abort, NULL) == -1 && strcpy(sErr, "SIGEMT" )) ||
         (sigaction (SIGFPE,  &Action_Abort, NULL) == -1 && strcpy(sErr, "SIGFPE" )) ||
         /* (sigaction (SIGSEGV, &Action_Abort, NULL) == -1 && strcpy(sErr, "SIGSEGV")) || */
         (sigaction (SIGPIPE, &Action_Abort, NULL) == -1 && strcpy(sErr, "SIGPIPE")) ||
         (sigaction (SIGALRM, &Action_Abort, NULL) == -1 && strcpy(sErr, "SIGALRM")) ) {

            sprintf(sError, "Handle_Signal: errno=%d %s on signal %s",  errno, strerror(errno), sErr);
            return FAILURE;
    }

    /* Setup Continue Signal Handler */
    /* Migrated to HPUX as below - by Kawee on 01-Jul-2003 */
    if ( /* (sigaction (SIGINFO, &Action_Cont, NULL) == -1 && strcpy(sErr, "SIGINFO")) || */
         (sigaction (SIGUSR2, &Action_Cont, NULL) == -1 && strcpy(sErr, "SIGUSR2")) ||
         (sigaction (SIGHUP,  &Action_Cont, NULL) == -1 && strcpy(sErr, "SIGHUP" )) ||
         (sigaction (SIGINT,  &Action_Cont, NULL) == -1 && strcpy(sErr, "SIGINT" )) ||
         (sigaction (SIGQUIT, &Action_Cont, NULL) == -1 && strcpy(sErr, "SIGQUIT")) ||
         (sigaction (SIGILL,  &Action_Cont, NULL) == -1 && strcpy(sErr, "SIGILL" )) ||
         (sigaction (SIGTRAP, &Action_Cont, NULL) == -1 && strcpy(sErr, "SIGTRAP")) ||
         (sigaction (SIGTERM, &Action_Cont, NULL) == -1 && strcpy(sErr, "SIGTERM")) ||
         (sigaction (SIGUSR1, &Action_Cont, NULL) == -1 && strcpy(sErr, "SIGUSR1")) ) {
            sprintf(sError, "Handle_Signal: errno=%d %s on signal %s",  errno, strerror(errno), sErr);
        return FAILURE;
    }

    return SUCCESS;
}

void    CatchAndCont (int signal)
{
    char    sTemp[SIZE_MESSAGE];

    /* Migrated to HPUX as below - by Kawee on 01-Jul-2003 */
    /* if ( signal == SIGINFO) */
    if ( signal == SIGTERM)
        cGblShutdown = TRUE;

    sprintf(sTemp, "frm_cdr_feed: Received Signal (%d)", signal);
    Deb_Msg(sTemp);
}

void    CatchAndExit (int signal)
{
    char    sTemp[SIZE_MESSAGE];

    /* will add later */

    sprintf(sTemp, "Received signal (%d)\n", signal);
    Deb_Msg(sTemp);
    return;
}

/*
********************************************************************************
** Function Name:  SnoozeSleep(unsigned int iSleepSec)
**
** Description  : This routine makes the module sleep for the specified
**        hibernation period. In actuality, the process wakes up
**        every few seconds to check for a  shutdown request.
**        The routine returns once it has slept long enough or
**        if a shutdown is received.
**
** Parameters: iSleepSec - The requested hibernation period, in seconds
**
** Return Value : None
**
********************************************************************************
*/

void    SnoozeSleep(unsigned int iSleepSec)
{
    unsigned int    iSnoozeTime = 15;       /* Sleep for 15 seconds at a time */
    double      dTimeLeft;
    time_t      tStartTime, tCurrentTime;

    /* Check for a shutdown request */
    if ( cGblShutdown )
        return;

    /* Set up sleep variables */
    if ( iSleepSec < iSnoozeTime)
        iSnoozeTime = iSleepSec;

    tStartTime = time( NULL );

    /*
    ** Sleep for 15 seconds at a time.  Each time we wake up, check for a
    ** shutdown message.  If there was none, see if we should wake up yet.
    */
    do {
        sleep( iSnoozeTime );

        /* Check for a shutdown request */
        if ( cGblShutdown )
            return;

        tCurrentTime = time( NULL );
        dTimeLeft = difftime( tCurrentTime, tStartTime );

    } while ( dTimeLeft < iSleepSec );

}  /* end SnoozeSleep */

/*
** Name     : Deb_Msg()
**
** Description  : Sends the Message to the Standard Error device if required.
**
** Parameters   : char *Msg - INPUT - Message to display.
**
** Return Value : void
*/

void Deb_Msg(char *Msg)
{
    if ( glb_DbgFlag ) {       /* Debug Mode */
        fprintf(stderr, Msg);
        fprintf(stderr, "\n");
        fflush(stderr);
    }   /* End Debug */

}

void Get_MtxType(int mtxind, char* mtxtype)
{
    switch ( mtxind ) {
        case GSM_TYPE:
            strcpy(mtxtype, "GSM");
            break;
        case SMS_TYPE:     /* Split SMS type from GSM type - by Kawee on 07-Apr-2005 */
            strcpy(mtxtype, "SMS");
            break;
        case SMSFDA_TYPE:  /* Split SMSFDA type from SMS type - by Kawee on 24-Oct-2008 */
            strcpy(mtxtype, "SMSFDA");
            break;
        case SMC_TYPE:      /* Added SMC type for V9.2-1 - by Jetsada on 23-Aug-2005 */
            strcpy(mtxtype, "SMC");
            break;
        case SSP_TYPE:
            strcpy(mtxtype, "SSP");
            break;
        case USC_TYPE:
            strcpy(mtxtype, "USC");
            break;
        case GPRS_TYPE:     /* GPRS Type added by Kawin on 02-Oct-2002 */
            strcpy(mtxtype, "GPR");
            break;
        case DPCGPR_TYPE:
            strcpy(mtxtype, "DPCGPR");
            break;
        case DPCSMS_TYPE:
            strcpy(mtxtype, "DPCSMS");
            break;
        case GSMBC_TYPE:    /* GSMBC Type added by Kawee on 20-May-2004 */
            strcpy(mtxtype, "GSMBC");
            break;
        case PPSBC_TYPE:    /* PPSBC Type added by Kawee on 20-May-2004 */
            strcpy(mtxtype, "PPSBC");
            break;
        case GSMTC_TYPE:    /* GSMTC Type added by Kawee on 05-Nov-2004 */
            strcpy(mtxtype, "GSMTC");
            break;
        case PPSTC_TYPE:    /* PPSTC Type added by Kawee on 05-Nov-2004 */
            strcpy(mtxtype, "PPSTC");
            break;
        case GSMCT_TYPE:    /* GSMCT Type added by Kawee on 02-Jun-2005 */
            strcpy(mtxtype, "GSMCT");
            break;
        case PPSCT_TYPE:    /* PPSCT Type added by Kawee on 02-Jun-2005 */
            strcpy(mtxtype, "PPSCT");
            break;
        case RBT_TYPE:      /* RBT Type added by Kawee on 18-Jul-2005 */
            strcpy(mtxtype, "RBT");
            break;
        case CCUDR_TYPE:    /* CCUDR Type added by Jetsada on 27-August-2007 */
            strcpy(mtxtype, "CCUDR");
            break;
        case DPCGSM_TYPE:
            strcpy(mtxtype, "DPCGSM");
            break;
        case DPCAIS_TYPE:
            strcpy(mtxtype, "DPCAIS");
            break;
        case DPCTAC_TYPE:
            strcpy(mtxtype, "DPCTAC");
            break;
        case AIN_TYPE:
            strcpy(mtxtype, "AIN");
            break;
        case NRTRDE_TYPE:
            strcpy(mtxtype, "NRTRDE");
            break;
        case NRGGSN_TYPE:
            strcpy(mtxtype, "NRGGSN");
            break;
        case AWN_TYPE:
            strcpy(mtxtype, "AWN");
            break;
        case AMF_TYPE:
            strcpy(mtxtype, "AMF");
            break;
        case SDG_TYPE:
            strcpy(mtxtype, "SDG");
            break;
        case NRTSCP_TYPE:
            strcpy(mtxtype, "NRTSCP");
            break;
        case AWNSMS_TYPE:
            strcpy(mtxtype, "AWNSMS");
            break;
        case AWNGPR_TYPE:
            strcpy(mtxtype, "AWNGPR");
            break;
        case DCB_TYPE:
            strcpy(mtxtype, "DCB");
            break;
        case OCSVPN_TYPE:       /* GSM type Maritime MT added by N.Thanakorn on 18-Nov-2014 */
            strcpy(mtxtype, "OCSVPN");
            break;
        case GSMTOP_TYPE:       /* GSM TopUp added by N.Thanakorn on 5-Feb-2015 */
            strcpy(mtxtype, "GSMTOP");
            break;
        case IMS_TYPE:          /* 4G LTE (VoLTE and VoWIFI) */
            strcpy(mtxtype, "IMS");
            break;
        case RDC_TYPE:          /* National Roam with DTAC */
            strcpy(mtxtype, "RDC");
            break;
        case RDCSMS_TYPE:          /* National Roam with DTAC */
            strcpy(mtxtype, "RDCSMS");
            break;
        case RBM_TYPE:
            strcpy(mtxtype, "RBM");
            break;
        default :
            strcpy(mtxtype, "UNKNOWN");
            break;
    }
    /* printf ("mtxtype = %s\n", mtxtype); */
}

char *StrUpr(char *str)
{
    const int asc_diff = -32;   /* 'a'=97, 'A'=65 ==> 'A'-'a' is -32 (decimal) */
        int idx, slen;
        if(!str)
                return(NULL);
        slen = strlen(str);
        for(idx=0; idx<slen; idx++) {
                if((str[idx] >= 'a') && (str[idx] <= 'z')) {
                        str[idx] += asc_diff;
                }
        }
        return(str);
}

int AddDate(char *szOutputDate, int iOutputSize, const char *szDateFormat, const char *szInputDate, int iDateOffset)
{

    time_t      lTime;
    struct tm   tmBuf, *ptm;
    char        szTemp[5];
    size_t      rv;

    if ( szInputDate == '\0' ) {  /* Don't Have Specific Input Date */
        time(&lTime);
        ptm = localtime(&lTime);
    }
    else {
        memset(&tmBuf, 0x00, sizeof(tmBuf));
        memcpy(szTemp, szInputDate, 4);
        szTemp[4]='\0';
        tmBuf.tm_year = atoi(szTemp)-1900;
        memcpy(szTemp, szInputDate+4, 2);
        szTemp[2]='\0';
        tmBuf.tm_mon = atoi(szTemp)-1;
        memcpy(szTemp, szInputDate+6, 2);
        szTemp[2]='\0';
        tmBuf.tm_mday = atoi(szTemp);
        ptm = &tmBuf;
        tmBuf.tm_hour = 0;
        tmBuf.tm_min  = 0;
        tmBuf.tm_sec  = 0;
    }

    ptm->tm_mday = ptm->tm_mday + iDateOffset;
    lTime = mktime(ptm);
    ptm =localtime(&lTime);

    if ( szDateFormat == '\0' ) {
        rv = strftime(szOutputDate, iOutputSize, "%Y%m%d", ptm);
    }
    else {
        rv = strftime(szOutputDate, iOutputSize, szDateFormat, ptm);
    }
    if ( !rv ) {  /* Buffer Too small */
        return FAILURE;
    }
    return SUCCESS;

}

void DoBackupCdr(char *cdrfile, char *synfile)
{
    struct stat fstat;
    struct tm   *ptm;
    time_t __st_mtime = 0;
    char   fullpath[SIZE_FNAME+1], yymmddH_dir[11+1], cmd[2*SIZE_FNAME];

    if ( glb_CdrBackFlag == 'N' )
        return;

    memset(&fstat, 0x00, sizeof(struct stat));
    memset(yymmddH_dir, 0x00, sizeof(yymmddH_dir));
    memset(fullpath, 0x00, sizeof(fullpath));
    memset(cmd, 0x00, sizeof(cmd));

    __st_mtime = time(NULL);
    if ( stat(cdrfile, &fstat) == 0 ) { /* stat successful */
        __st_mtime = fstat.st_mtime;
    }

    ptm = localtime(&__st_mtime);
    strftime(yymmddH_dir, sizeof(yymmddH_dir), "%Y%m%d_%H", ptm);

    sprintf(fullpath, "%s/%s", glb_CdrBack_Dir, yymmddH_dir);
    if ( access(fullpath, (F_OK|R_OK|W_OK|X_OK)) ) {
        if ( mkdir(fullpath, 0777) != 0 ) {
            writeLog(LOG_SYS, "cannot create dir for %s (%s) - cannot backup cdr file", fullpath, strerror(errno));
            return;
        }
    }
    sprintf(cmd, "cp -p %s %s", synfile, fullpath);
    system(cmd);
    sprintf(cmd, "cp -p %s %s", cdrfile, fullpath);
    system(cmd);
    sprintf(cmd, "gzip %s/%s", fullpath, Remove_Path(cdrfile));
    system(cmd);
}

void DoBackupCdr4IUM(char *iumfile)
{
    static int _count_ = 0;
    struct stat fstat;
    struct tm   *ptm;
    time_t __st_mtime;
    char   fullpath[SIZE_FNAME+SIZE_FNAME], yymmddH_dir[11+1], cmd[2*SIZE_FNAME];
    char   *p = NULL, n00_extension[SIZE_FNAME];
    char   ymd[20];

    if ( glb_CdrBck4IUMFlag == 'N' )
        return;

    memset(&fstat, 0x00, sizeof(struct stat));
    memset(yymmddH_dir, 0x00, sizeof(yymmddH_dir));
    memset(fullpath, 0x00, sizeof(fullpath));
    memset(cmd, 0x00, sizeof(cmd));
    memset(n00_extension, 0x00, sizeof(n00_extension));

    __st_mtime = time(NULL);
    if ( stat(iumfile, &fstat) == 0 ) { /* stat successful */
        __st_mtime = fstat.st_mtime;
    }

    ptm = localtime(&__st_mtime);
    strftime(yymmddH_dir, sizeof(yymmddH_dir), "%Y%m%d_%H", ptm);

    memset(ymd, 0x00, sizeof(ymd));
    strftime(ymd, sizeof(ymd), "%Y%m%d %H%M%S", ptm);
    sprintf(fullpath, "%s/%s", glb_CdrBck4IUM_Dir, yymmddH_dir);

//printf("%d %s: glb_CdrBck4IUM_Dir(%s) yymmddH_dir(%s) fullpath(%s) [%s]\n", ++_count_, ymd, glb_CdrBck4IUM_Dir, yymmddH_dir, fullpath, glb_Cdr_File);

    if ( access(fullpath, (F_OK|R_OK|W_OK|X_OK)) ) {
        if ( mkdir(fullpath, 0777) != 0 ) {
            writeLog(LOG_SYS, "cannot create dir for %s (%s) - cannot backup ium-cdr file", fullpath, strerror(errno));
            return;
        }
    }
    /* cdr event is written to file_name.n00.tmp before rename to file_name.n00 */
    strcpy(n00_extension, iumfile);
    p = strrstr(n00_extension, ".tmp");
    *p = 0;

    sprintf(cmd, "cp -p %s %s/%s", iumfile, fullpath, Remove_Path(n00_extension));
/* printf("dbg: bck up \"%s\"\n", cmd); */
    system(cmd);
    sprintf(cmd, "gzip %s/%s", fullpath, Remove_Path(n00_extension));
/* printf("dbg: compress \"%s\"\n", cmd); */
    system(cmd);

}

char *getTokenItem(const char *str, int fno, char sep, char *out)
{
    int i = 0, cur = 1, start = 0, is_copied = 0;

    if ( fno <= 0 ) {
        return (char*)str;
    }

    while ( *(str+i) != '\0' ) {
        if ( *(str+i) == sep ) {
            if ( cur == fno ) {
                if ( i == start ) {
                    strcpy(out, "");
                }
                else {
                    strncpy(out, str+start, (i-start));
                }
                is_copied = 1;
                break;
            }
            cur++;
            start=i+1;
        }
        i++;
    }
    if ( *out == 0 || is_copied == 0 ) {
        if ( i == start ) {
            strcpy(out, "");
        }
        else {
            strncpy(out, str+start, (i-start));
        }
    }
    return out;
}

/*- End -*/
