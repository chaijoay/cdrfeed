/*
**++
**  FACILITY    : CDR Feed Process
**
**  FILE NAME   : frm_cdr_feed.h
**
**  ABSTRACT    : Include file for frm_cdr_feed.c program for Base HP FMS Ver 9.2-1
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

#ifndef _FRM_CDR_FEED_H_
#define _FRM_CDR_FEED_H_

#ifdef  __cplusplus
    extern "C" {
#endif

/* Resized mtx_id in sync file from 99 to 109 - by Kawee on 28-Mar-2003 */
#define SIZE_SYNCREC        320                 /* size of Sync Record as created by Duplicate Check Program */

#define _APP_NAME_          "frm_cdr_feed"
#define _APP_VERS_          "14.0"
#define _APP_FOR_           "FRM 14.1-0"
#define _INI_SUF_           ".ini"
/*
** Structure of CDR Sync Record buffer (native to CDR Feeder).
** Note : All the character array field size includes null size also
*/
typedef struct sync_record {
    /* Resized mtx_id from 10 to 20 Chars - by Kawee on 22-Oct-2003 */
    char mtx_id [20+1];         /* for example AXE92C, CBI1, MTX9D etc */
    char mtx_type [10+1];        /* GSM, SSP, USC, GPS */
    char file_name [100+1];      /* Filename without path */
    char time_stamp [20+1];     /* Time stamp in YYYY-MM-DD HH:MM:SS */
} SYNC_RECORD;

// char gszMtxTypeName[NOF_MTX][8] = {
//     "UNKNOWN", "SSP", "MSCPPS", "GSM", "SMC", "SMS", "USC", "GPR", "GSMBC", "PPSBC", "GSMTC", "PPSTC", "GSMCT", "PPSCT", "RBT", "CCUDR", "SMSFDA",
//     "AWN", "AMF", "DPCGSM", "DPCAIS", "DPCTAC", "DPCGPR", "DPCSMS", "SDG", "AWNSMS", "AWNGPR", "DCB", "OCSVPN", "AIN", "NRTRDE", "NRGGSN", "NRTSCP",
//     "GSMTOP", "IMS", "RDC", "RDCSMS"
// };


/*
** Structure of Check Point File Record
*/
typedef struct check_record {
    char proc_date[SIZE_YYYYMMDD+1];   /* Processing Date YYYYMMDD */
    char proc_time[SIZE_HHMMSS+1];     /* Checkpoint Update Time HHMMSS */
    char cdr_filename[SIZE_FNAME];     /* Current CDR file name under processing (with path) */
    char sync_filename[SIZE_FNAME];    /* Current Sync file name under processing (with path) */
    int  mtx_ind;                       /* Current Mtx Type Indicator */
    long cdr_offset;                    /* Current Offset - processing completed */
    char check_ind;                     /* Check Point Update Indicator */
} CHECK_RECORD;

/*
** Structure of Check Point File Record
*/
typedef struct snap_info {
    char sync_filedir[SIZE_FNAME];     /* Sync file directory to be created snapshot */
    char sync_pattern[SIZE_FNAME];     /* Sync file pattern */
} SNAP_INFO;

/*
** Table of Contents of Functions used in frm_cdr_feed.c
*/
int     Init_Main(const char *avg0);
void    Close_Main();

int     Handle_Signal(char *);
void    CatchAndCont(int);
void    CatchAndExit(int);
void    SnoozeSleep(unsigned int);

int     Load_SysParameters(char *argv[]);
int     Load_Postpaid_SysParam();          /* Added by Kawee on 13-Jan-2003 */
int     Load_Prepaid_SysParam();           /* Added by Kawee on 13-Jan-2003 */
int     Load_Ain_SysParam();               /* Added by Kawee on 29-Jan-2007 */
int     Load_Nrtrde_SysParam();            /* Added by Kawee on 11-Feb-2008 */
int     Load_SpltPPS_SysParam();           /* Added by Thanakorn on 08-Mar-2013 */
int     Get_Parameter(char *, char *);
void    Trim_IniRec(char *);

int     Anz_Check_File();
int     Bld_Snap_File(char *);
int     IsMatchFileType (char *, char *);           /* Added by Kawee on 02-Jan-2003 */

int     Get_Sync_Fname(FILE *, int *, char *);     /* Added by Kawee on 02-Jan-2003 */ /* Changed by Kawee on 12-Feb-2008 */
int     Proc_Snapshot(char *);
int     Proc_SyncFile();
int     Start_Recovery();
int     Proc_CdrFile();
int     Cont_Mapping();

int     Open_Cdr_File(char *);
int     Open_Check_File(char *);
int     Open_Error_File(char *);
int     Open_Reject_File(char *);
int     Open_EventCdr_File(char *);
int     Open_EAlmData_File(char *);
int     Open_SvcIdSub_File(char *);
void    Upd_Check_File(char);
void    Append_Slash(char *);
char    *Remove_Path(char *);
int     Rename_ExtFile(char *, char *, char *);
void    Print_Stat();
void    Deb_Msg(char *);
int     Create_EventSync();
int     Sort_EventOutput(char *file_input);

char    *StrUpr(char *);                                        /* Added by Kawee on 13-Jan-2003 */
int     AddDate(char *, int, const char *, const char *, int);  /* Added by Kawee on 26-Sep-2008 */
void    DoBackupCdr(char *cdrfile, char *synfile);              /* Added by Thanakorn on 15-Mar-2013 */
void    DoBackupCdr4IUM(char *iumfile);                         /* Added by Thanakorn on 18-Mar-2013 */

char    *getTokenItem(const char *str, int fno, char sep, char *out);

#ifdef  __cplusplus
    }
#endif

#endif /* _FRM_CDR_FEED_H_ */
