//
//
// FACILITY    : CDR Feed Process
//
// FILE NAME   : frm_cdr_glob.h
//
// ABSTRACT    : Global Definitions of CDR Feed Process
//
// AUTHOR  : Palanichamy.M.
//
// CREATE DATE : 5-Sep-1997
//
// CURRENT VERSION NO : 12.1
//
// LAST RELEASE DATE  : Sep-2016
//
//

#ifndef _FRM_CDR_GLOB_H_
#define _FRM_CDR_GLOB_H_

#ifdef  __cplusplus
    extern "C" {
#endif

#if	!defined(TRUE) || ((TRUE) != 1)
	#ifdef TRUE
		#undef TRUE
	#endif
	#define TRUE	1
#endif
#if	!defined(FALSE) || ((FALSE) != 0)
	#ifdef FALSE
		#undef FALSE
	#endif
	#define FALSE	0
#endif

#define SUCCESS			0
#define FAILURE			-1

#define SET_FALSE       '0'
#define SET_TRUE        '1'

#define YEAR_SINCE      1900    // Year since represented by Unix time_t type

#define SIZE_YYYYMMDD   8
#define SIZE_YYMMDD     6
#define SIZE_HHMMSS     6

#define SIZE_DATESTR    10      // YYYY/MM/DD format size
#define SIZE_TIMESTR    8       // HH:MM:SS format size

#define SIZE_NORMSTR    80
#define SIZE_FNAME      256
#define SIZE_MESSAGE    512
#define SIZE_COMMAND    512
#define SIZE_MTXTYPE    8

#define SIZE_OLDPHONENO     7       // old 7 digit phone numbers
#define SIZE_PHONENO        10      // currently AIS has 10 digit phone numbers
#define SIZE_MOBILE_NUM     16      // Max Size of FIT Service Number - Added by Kawee on 06-Feb-2007
#define SIZE_IMSI           17      // Exact size of IMSI Number
#define SIZE_IMEI           17      // Exact size of IMEI Number
#define SIZE_VAS_NAME       60      // Maximum size of VAS Name (Costcode Description) - Added by Kawee on 12-Mar-2007
#define SIZE_OPER           3       // Maximum size of Operator Network Name that feed to ERM - Added by Kawee on 18-Sep-2008
#define SIZE_MSC_NAME       20      // Maximum size of MSC Name (For CAMEL Migration ) - Added by Kawee on 11-Dec-2007

#define MAX_CDR_REPO_DIR    4

#define SPACE_CHAR          ' '
#define DIR_CHAR            '/'
#define DIR_STRG            "/"

#define EALM_TEMP_EXT       ".exntmp"
#define EALM_FILE_EXT       ".EXN"

#define CHARGE_TEMP_EXT     ".ccctmp"
#define CHARGE_FILE_EXT     ".CCC"

//
// Partitions Defined in FRM System
//
#define DEFAULT_PARTITION   1       // Default partition id
#define HOMESVC_PARTITION   1       // Home Services partition id
#define HOSTSVC_PARTITION   10      // Host Services partition id

#define AISSVC_PARTITION    1       // AIS Services Partition ID
#define DPCSVC_PARTITION    2       // DPC Services Partition ID

#define AWN_SVC_PARTITION       2
#define PREPAID_SVC_PARTITION   4
#define AIN_SVC_PARTITION       8

//
// Remark: IN FMS Prepaid and Postpaid CDR use the same partition for detect prepaid/postpaid mismatch.
// Thus, below Prepaid Partition is dummy value for use with EXN Feeder only
// - Kawee 26-Feb-2007
//
#define AWN_PARTITION           "2"
#define AIN_PARTITION           "4"
#define PREPAID_PARTITION       "8"

//
// MTX Type Constants
//
typedef enum {
    UNK_TYPE = 0,
    SSP_TYPE,           // Pre-paid type - rename from PPS to SSP on 22-Oct-2003
    MSCPPS_TYPE,        // PPS Network Calls in MSC CDR files
    GSM_TYPE,           // GSM type
    SMC_TYPE,           // SMC type (SSP SMS) - added for V9.2-1 on 15-Aug-2005 by Jetsada
    SMS_TYPE,           // SMS type (GSM SMS) - Split from GSM Type by Kawee on 07-Apr-2005
    USC_TYPE,           // USC type - Mobile-life CDRs
    GPRS_TYPE,          // GPRS type (GPRS Volumn Based) - added on 20-Sep-2002 by kawin
    GSMBC_TYPE,         // GSMBC type (GPRS Time Based bearier) - added on 20-May-2004 by Kawee
    PPSBC_TYPE,         // PPSBC type (GPRS Time Based bearier)- added on 20-May-2004 by Kawee
    GSMTC_TYPE,         // GSMTC type (MMS) - added on 08-Nov-2004 by Kawee
    PPSTC_TYPE,         // PPSTC type (MMS) - added on 08-Nov-2004 by Kawee
    GSMCT_TYPE,         // GSMCT type (CDG) - added on 02-Jun-2005 by Kawee
    PPSCT_TYPE,         // PPSCT type (CDG) - added on 02-Jun-2005 by Kawee
    RBT_TYPE,           // RBT type - added on 18-Jul-2005 by Kawee
    CCUDR_TYPE,         // CCUDR type - added on 27-August-2007 by Jetsada
    SMSFDA_TYPE,        // SMSFDA type (GSM SMS) - Split from GSM Type by Kawee on 07-Apr-2005
    AWN_TYPE,           // AWN type - by Thanakorn on Jan-2010
    AMF_TYPE,           // AMF type - by Thanakorn on 04-Aug-2010
    DPCGSM_TYPE,        // DPC - Own GSM Network CDR files
    DPCAIS_TYPE,        // DPC - AIS Network CDR files
    DPCTAC_TYPE,        // DPC - TAC Network CDR files
    DPCGPR_TYPE,        // DPC - GPRS
    DPCSMS_TYPE,        // DPC - SMS over GPRS
    SDG_TYPE,           // SDG type - by Thanakorn on 01-Mar-2012
    AWNSMS_TYPE,        // AWN SMS  type - by Thanakorn on Jan-2013
    AWNGPR_TYPE,        // AWN GPRS type - by Thanakorn on Jan-2013
    DCB_TYPE,           // GSMCT type (DBC for ) - added on 29-May-2014 by N.Thanakorn
    DCBCT_TYPE,         // GSMCT for DCB - added on 29-Apr-2019 by N.Thanakorn
    OCSVPN_TYPE,        // GSM type Maritime Mobile Terminated Call (MT) - added on 18-Nov-2014 by N.Thanakorn
    AIN_TYPE,           // AIN type - added on 29-Jan-2007 by Kawee
    NRTRDE_TYPE,        // NRTRDE type - added on 30-Jan-2008 by Kawee
    NRGGSN_TYPE,        // NRGGSN type - added on 26-Feb-2008 by Kawee
    NRTSCP_TYPE,        // Added by Thanakorn on 24-Sep-2012, Support FMS V11
    GSMTOP_TYPE,        // Added by Thanakorn on 05-Feb-2015, Support IRB file of GSM TopUp (file name prefix of 'good')
    IMS_TYPE,           // Added by Thanakorn on 28-Sep-2015, Support VoWIFI and VoLTE
    RDC_TYPE,           // Added by Thanakorn on 22-Jan-2016, Support National Roam with DTAC
    RDCSMS_TYPE,        // Added by Thanakorn on 22-Jan-2016, Support National Roam with DTAC
    RBM_TYPE,
    NOF_MTX
} ENUM_MTXTYPE_ID;

// Changes to support new fms v11 by Thanakorn on 09-Sep-2012
#ifdef __FRM_V12__
#define VOICE_EVENT_TYPE_IDD    5   // Voice Event Type (IDD Category just for categorize Priority Tube)
#define VOICE_EVENT_TYPE        6   // Voice Event Type
#define SMS_EVENT_TYPE          7   // SMS, MMS Event Type
#define VAS_EVENT_TYPE          8   // VAS Event Type (USC, CDG, RBT, SESSION, SCG, AMF)
#define DATA_EVENT_TYPE         9   // Data Event Type (3G, GPRS)
#define IR_EVENT_TYPE           10  // IR Event Type
#define FIXED_EVENT_TYPE        11  // Fixed Voice Event Type
#else
#define VOICE_EVENT_TYPE_IDD    0   // Voice Event Type (IDD Category just for categorize Priority Tube)
#define VOICE_EVENT_TYPE        1   // Voice Event Type
#define SMS_EVENT_TYPE          2   // SMS, MMS Event Type
#define VAS_EVENT_TYPE          3   // VAS Event Type (USC, CDG, RBT, SESSION, SCG, AMF)
#define DATA_EVENT_TYPE         4   // Data Event Type (3G, GPRS)
#define IR_EVENT_TYPE           5   // IR Event Type
#define FIXED_EVENT_TYPE        6   // Fixed Voice Event Type
#endif

#ifdef __FRM_V12__
#define CAT_TOTAL           0x00001
#define CAT_VOICE           0x00002
#define CAT_SMS             0x00004
#define CAT_MMS             0x00008
#define CAT_DATA            0x00010
#define CAT_VAS             0x00020
#define CAT_PREMIUM         0x00040
#define CAT_LOCAL           0x00080
#define CAT_IDD             0x00100
#define CAT_IR              0x00200
#define CAT_ONNET           0x00400
#define CAT_OFFNET          0x00800
#define CAT_OUTGOING        0x01000
#define CAT_INCOMING        0x02000
#define CAT_PACKAGE         0x04000     // This category use outside cdr_feed/ium_frontend. that process handle directly by IUM
#define CAT_APP_PURCHASE    0x08000
#define CAT_MARITIME        0x10000
#define CAT_INCOMP_CALL     0x20000     // incomplete call category for AIN - added by Thanakorn on Aug-2016
#else
#define CAT_TOTAL           0x0000001
#define CAT_VOICE           0x0000002
#define CAT_SMS             0x0000004
#define CAT_MMS             0x0000008
#define CAT_DATA            0x0000010
#define CAT_VAS             0x0000020
#define CAT_PREMIUM         0x0000040
#define CAT_LOCAL           0x0000080
#define CAT_IDD             0x0000100
#define CAT_IR              0x0000200
#define CAT_PACKAGE         0x0000400
#define CAT_APP_PURCHASE    0x0000800
#define CAT_MARITIME        0x0001000
#define CAT_FIXED_LINE      0x0002000
#define CAT_IR_ONNET        0x0004000
#define CAT_M2M             0x0008000
#define CAT_VAS_ONETIME     0x0010000       // CCT => 11
#define CAT_VAS_RECUR       0x0020000       // CCT => 10
#define CAT_VAS_SUBS        0x0040000       // CCT =>  9
#define CAT_VOIP            0x0080000
#define CAT_CHANGE_TYPE     0x0100000
#define CAT_CONTRACT_PHONE  0x0200000
#define CAT_NEW_REGISTER    0x0400000
#define CAT_PORT            0x0800000
#define CAT_SIM             0x1000000
#define CAT_SUSP_ORDER      0x2000000
#define CAT_TERMINATED      0x4000000
#endif
//
// FIT CDR Call Features Flag Name
// Each byte of FIT CDR Call Features field have different meaning as the following
//
// - Added by Kawee on 03-Jul-2003
//
// Changed to Bitmask as below by Jetsada on 15-Aug-2005
#define FEAT_POSTPAID_CALL  0x0000001
#define FEAT_PREPAID_CALL   0x0000002
#define FEAT_FORWARD        0x0000004
#define FEAT_XFER           0x0000008
#define FEAT_WAIT           0x0000010
#define FEAT_HOLD           0x0000020
#define FEAT_CONFERENCE     0x0000040
#define FEAT_3WAY           0x0000080
#define FEAT_MBOX_REC       0x0000100
#define FEAT_MBOX_RETR      0x0000200
#define FEAT_FEAT_SETUP     0x0000400
#define FEAT_BAR_IN         0x0000800
#define FEAT_BAR_OUT        0x0001000
#define FEAT_MALIC_CALLID   0x0002000
#define FEAT_COMPL_BUSY     0x0004000
#define FEAT_USER_USER_SIG  0x0008000
#define FEAT_CLOSE_USR_GRP  0x0010000
#define FEAT_NO_AIR         0x0020000
#define FEAT_SPEED_CALL     0x0040000
#define FEAT_FREE_SERV      0x0080000
#define FEAT_FAX            0x0100000
#define FEAT_DATA_XMIT      0x0200000
#define FEAT_IR             0x0400000       // Added by Kawee on 22-Oct-2007, Use this in future on FMS.V.10 Phase2
#define FEAT_IDD            0x0800000       // Added by Thanakorn on 27-Sep-2012, Use this in FMS V11
#define FEAT_THAI           0x1000000
#define FEAT_LOCAL          0x2000000

#ifdef __FRM_V12__
// Changes to support new fms v11 by Thanakorn on 09-Sep-2012
// Skip Detection Parameters - Input Mask
#define SKIP_ARCHIVE            0x0000001   // Skip Archiving
#define SKIP_DETECTION          0x0000002   // Skip All Detection
#define SKIP_PIPELINE           0x0000004   // Skip Pipeline
#define SKIP_ACCUM_PATTERN      0x0000008   // Skip Accum Pattern Check
#define SKIP_AUTHORIZED_FEATURE 0x0000010   // Skip AuthFeature Check
#define SKIP_BLACKLIST          0x0000020   // Skip Blacklist Check
#define SKIP_DEST               0x0000040   // Skip Destination Check
#define SKIP_DEST_USAGE         0x0000080   // Skip Destination Usage
#define SKIP_IMMEDIATE_ROAM     0x0000100   // Skip Immediate Roam Check
#define SKIP_UNKNOWN            0x0000200   // Skip Unknown Entity Check
#define SKIP_PREMATURE_CHANGE   0x0000400   // Skip Premature Change Check
#define SKIP_ROAM_NOHOME        0x0000800   // Skip Roam No Home Check
#define SKIP_INACTIVE_CHECK     0x0001000   // Skip Inactive Entity Check
#define SKIP_CONTACT            0x0002000   // Skip Contact Check
#define SKIP_PATTERN            0x0004000   // Skip Pattern Check
#define SKIP_SEQ_PATTERN        0x0008000   // Skip Sequence Pattern Check
#define SKIP_SUSP               0x0010000   // Skip Suspension Check
#define SKIP_THRESHOLD          0x0020000   // Skip Threshold Check
#define SKIP_USAGE_ACCUM        0x0040000   // Skip Usage Accumulation
#define SKIP_COLLISION          0x0080000   // Skip Collision Check
#define SKIP_VELOCITY           0x0100000   // Skip Velocity Check
#define SKIP_POST_PATTERN       0x0200000   // Skip Post Pattern Check
#define SKIP_RECENT_CHARGE      0x0400000   // Skip Recent Charge
#define SKIP_DUP_EVENT          0x0800000   // Skip Duplicate Event Check
#define SKIP_UPD_OLDEVENT       0x1000000   // Skip Update of Older Event in Collision/Velocity
#endif
//
// FIT EVENT CDR Service Type Values
// Value of FIT Service Type should be only one of the below value.
//
// - Added by Kawee on 27-Feb-2007
//
#define ST_HOME         "0"
#define ST_HOST         "1"
#define ST_ROAM         "2"

#define INCOMING        "1"
#define OUTGOING        "2"
#define _UNKNOWN        "15"

#define DEF_CELL_AREA   "00000"     // cell area for non-cell_id cdr, eg. SMS, FDA, USC, etc.
#define UNK_CELL_AREA   "TH001"     // cell area for un-map-able cell_id
//
// Unknown Value for Network Type, Direction, Deposition and Switch Type - Added by Kawee on 08-Jul-2003
//
#define TYPE_UNKNOWN    15

// Introduce NRTRDE CDR in different sync and data file directory - by Kawee on 12-Feb-2007
typedef enum {
    CDR_LOCAL = 0,      // Local CDR Data
    CDR_NRTRDE,         // NRTRDE CDR Data
    NOF_CDR_PATH
} ENUM_CDR_PATH;

// Splitting PPS AWN CDR - by Thanakorn on 07-Mar-2013
typedef enum {
    PPSAWN_AWN = 0,
    PPSAWN_SMS,
    PPSAWN_GPRS,
    NOF_PPSAWN_PATH
} ENUM_SPLT_PPSAWN;

//
// Check Indicator Constants
//
#define CHK_TIME_UPD        'T'     // Time Update
#define CHK_SHUT_UPD        'S'     // Shutdown Update
#define CHK_CMPL_UPD        'C'     // Completion Update

//
// File Open Modes
//
#define MODE_READ       "r"         // Open Read only
#define MODE_WRITE      "w"         // Open a New file for Writing
#define MODE_APPEND     "a"         // Open file for Append & Write
#define MODE_UPDATE     "r+"        // Open for Update
#define MODE_WUPDATE    "w+"        // Open a New file for Update
#define MODE_UAPPEND    "a+"        // Open file for Append & Update

//
// Constants used for Loading Memory Tables
//
#define FILE_STAT_ERROR     101     // File Status Error
#define FILE_OPEN_ERROR     102     // File Open Error
#define LOAD_TABLE_FULL     103     // Memory Not Enough
#define NO_CODE_LOADED      104     // No Code Loaded into Memory

//
// Constants for Check Point Analysis
//
#define ANZ_GO_AHEAD        201     // Go ahead
#define ANZ_MAP_HALF        202     // Mapping Halfway

//
// Process Status Constants
//
#define NORM_PROC           301     // Normal Processing
#define RECV_PROC           302     // Recovery Processing

//
// Error Constants for CDR Processing
//
#define CDR_FOPEN_FAIL          401
#define ISAM_FOPEN_FAIL         402
#define CHK_FOPEN_FAIL          403
#define ERROR_FOPEN_FAIL        404
#define REJCDR_FOPEN_FAIL       405
#define COMMAP_FOPEN_FAIL       406
#define EVENTCDR_FOPEN_FAIL     407
#define EALM_FOPEN_FAIL         408
#define CHARGE_FOPEN_FAIL       409
#define EVENTSYNC_CREATE_FAIL   410
#define DFMSMAP_FOPEN_FAIL      411
#define GSMUNK_FOPEN_FAIL       412
#define SHUT_SIGNAL_RCVD        450
#define PIPE_SIGNAL_RCVD        451

//
// Error Constants used for Parsing Country, Area Codes
//
#define PARSE_MUL_MATCH         501
#define PARSE_NOT_MATCH         502


//
// Global Variables
//
char cGblShutdown;              // Shutdown Indicator
char glb_DbgFlag;               // Debug Flag (TRUE - Debug Mode, FALSE - Production Mode)
char glb_PostpaidExnFlag;       // Postpaid EXN Create Flag (Y - Create, N - Don't Create) - added by kawin on 19-Mar-2004
char glb_PrepaidExnFlag;        // Prepaid EXN Create Flag (Y - Create, N - Don't Create) - added by kawin on 19-Mar-2004
char glb_AinExnFlag;            // AIN EXN Create Flag (Y - Create, N - Don't Create) - added by kawee on 29-Jan-2007
char glb_NrtExnFlag;            // NRTRDE EXN Create Flag (Y - Create, N - Don't Create) - added by kawee on 11-Feb-2008
char glb_CreateRejFlag;         // Create Reject File Flag (Y - Create, N - Don't Create) - added by Jetsada K. on 30-Aug-2005
char glb_UseFixRateFlag;        // Use Fix Rate File Flag (Y - Use Fix Rate, N - Use Costcode to lookup Rate) - added by Kawee on 04-Jun-2007
char glb_CdrBackFlag;           // Enable/Disable flag for backup cdr/sync file - added by Thanakorn on 15-Mar-2013
char glb_CdrBck4IUMFlag;        // Enable/Disable flag for backup cdr file for IUM - added by Thanakorn on 18-Mar-2013

// Migrated CAMEL V.5 - Added below by Kawee on 04-Jun-2007
char glb_Msc_Name[SIZE_MSC_NAME+1];             // stores MSC Name

char glb_Log_File[SIZE_FNAME];                  // stores Cdr Feeder Log File Name (including Path)

char glb_Check_File[SIZE_FNAME];                // stores Check Point File Name
char glb_Sync_File[SIZE_FNAME];                 // stores Sync File Name
char glb_Cdr_File[SIZE_FNAME];                  // stores CDR File Name
char glb_Error_File[SIZE_FNAME];                // stores Error File Name
char glb_Reject_File[SIZE_FNAME];               // stores CDR Reject File Name
char glb_Ignore_File[SIZE_FNAME];               // stores CDR Ignore File Name
char glb_EventCdr_File[SIZE_FNAME];             // stores FIT CDR File Name
char glb_PrepaidEAlmData_File[SIZE_FNAME];      // stores Prepaid External Alarm Data File Name
char glb_PostpaidEAlmData_File[SIZE_FNAME];     // stores Postpaid External Alarm Data File Name
char glb_AinEAlmData_File[SIZE_FNAME];          // stores AIN External Alarm Data File Name
char glb_NrtEAlmData_File[SIZE_FNAME];          // stores NRTRDE External Alarm Data File Name
char glb_ChargeData_File[SIZE_FNAME];           // stores Charge Data File Name
char glb_SvcIdSub_File[SIZE_FNAME];

char glb_MapData_Dir[SIZE_FNAME];               // stores Mapping Data Directory Path
char glb_Syncfile_Dir[NOF_CDR_PATH][SIZE_FNAME];// stores Sync File Directory Path
char glb_Cdrfile_Dir[NOF_CDR_PATH][SIZE_FNAME]; // stores CDR File Directory Path
char glb_Errlog_Dir[SIZE_FNAME];                // stores Error File Directory Path

// Now glb_ExnFile_Dir was changed into 2 names - added by kawin on 25-Nov-2002
char glb_PostpaidExnFile_Dir[SIZE_FNAME];       // stores Postpaid EXN File Directory Path
char glb_PrepaidExnFile_Dir[SIZE_FNAME];        // stores Prepaid EXN File Directory Path
char glb_AinExnFile_Dir[SIZE_FNAME];            // stores AIN EXN File Directory Path
char glb_NrtExnFile_Dir[SIZE_FNAME];            // stores NRTRDE EXN File Directory Path

// Merged CDR Repository to Common as below - by Kawee on 31-Jul-2003
char glb_CdrRepo_Dir[MAX_CDR_REPO_DIR][SIZE_FNAME];     // stores CDR Repository Path
char glb_OldCdrDate[SIZE_YYYYMMDD+1];                   // stores Old CDR Date in YYYYMMDD format - Added by Kawee on 26-Sep-2008
char glb_CdrBack_Dir[SIZE_FNAME+1];                     // backup path for cdr/sync file - Added by Thanakorn on 15-Mar-2013
//char glb_CdrBck4IUM_Dir[SIZE_FNAME+1];                  // backup path for cdr file for IUM - Added by Thanakorn on 18-Mar-2013
char glb_SvcIdSub_Dir[SIZE_FNAME+1];                    // path for Service_Id entity - Added by Thanakorn on 01-Apr-2020

long glb_CdrFile_Limit;             // stores Limit of CDR Files Count
long glb_CdrFeed_Sleep;             // stores CDR Feeder Process Sleep Time in Seconds
long glb_CdrSample_Size;            // stores CDR Sample Size (Records)

long glb_PostpaidMin_Duration;      // stores Postpaid CDR Minimum Duration (seconds)
long glb_PrepaidMin_Duration;       // stores Prepaid CDR Minimum Duration (seconds)
long glb_AinMin_Duration;           // stores AIN CDR Minimum Duration (seconds)
long glb_NrtMin_Duration;           // stores Nrt CDR Minimum Duration (seconds)
long glb_OldCdrDay;                 // stores Old CDR Day

int  glb_CdrPathType;               // store CDR Path Type
int  glb_ProcStat;                  // Process Status Flag (Recovery/Normal)
char glb_MtxType[SIZE_MTXTYPE+1];   // Indicates Mtx Type Name under processing (CDR_Feeder defined Type)
char glb_NfsType[SIZE_MTXTYPE+1];   // Indicates Mtx Type Name under processing (CDR type from source)
char glb_MsgText[SIZE_MESSAGE];     // Holds any Message during processing
int  glb_MtxInd;                    // Indicates Mtx Type under processing
int  glb_PartId;                    // Stores Partition ID during processing
int  glb_EventId;                   // Stores Event ID during processing - Added by Kawee on 04-Jun-2007
char glb_AisInstFlag;               // Stores AIS Installation Flag during processing - Added by Kawee on 13-Jan-2003
char glb_DpcInstFlag;               // Stores DPC Installation Flag during processing - Added by Kawee on 13-Jan-2003
char glb_12cInstFlag;               // Stores 1-2-Call Installation Flag during processing - Added by Kawee on 13-Jan-2003
char glb_AinInstFlag;               // Stores AIN Installation Flag during processing - Added by Kawee on 29-Jan-2007
char glb_NrtInstFlag;               // Stores NRTRDE Installation Flag during processing - Added by Kawee on 11-Feb-2008
int  glb_NofCdrRepo;                // Stores Number of Cdr Repository during processing - Added by Kawee on 31-Jul-2003
int  glb_CurrCdrRepo;               // Stores Current Cdr Repository during processing - Added by Kawee on 31-Jul-2003
char glb_EventSource[100+1];        // Stores Event Data Source of current Cdr file during processing - by Thanakorn on 09-Sep-2012, FMS V11

char glb_Enable_Splt;               // Global enable/disable flag for splitting cdr - by Thanakorn on 07-Mar-2013
char glb_SpltCdr_Dir[NOF_PPSAWN_PATH][SIZE_FNAME];      // stores splitted pps awn cdr paths
char glb_SpltCdr_Syn[NOF_PPSAWN_PATH][SIZE_FNAME];      // stores splitted pps awn syn paths
char glb_SpltType_Flg[NOF_PPSAWN_PATH];                 // enable/disable flag for splitting of each cdr type
int  glb_nFeedType;                     // Global parameter for check feeding type of BOS, MD or ALL
char glb_CurCDRDate[SIZE_YYYYMMDD+1];   // Global variable to hold current read record CDR's date (YYYYMMDD)
char glb_CurCDRTime[SIZE_HHMMSS+1];     // Global variable to hold current read record CDR's time (HH24SS)
char glb_SkipDetIrType[15];
int  glb_UseRatedCdr;
int  glb_GenSvcIdSub;

typedef enum {
    FD_ALL,
    FD_MD
} EN_FED_TYPE;

typedef enum {
    AL_DCB,
    AL_GPRS,
    AL_RBT,
    AL_SDG,
    AL_SMS,
    AL_USC,
    AL_VSCP,
    AL_VOICE,
    NOF_AL_TYPE
} EN_ALLOW_TYPE;

int  glb_AllowType[NOF_AL_TYPE];

unsigned long glb_CdrFile_Count;    // To Count Number of CDR Files Processed
unsigned long glb_ReadCtr;          // To Count Number of Records Read from a CDR File
unsigned long glb_IgnCtr;           // To Count Number of Regords Ignored due to X network Call
unsigned long glb_DuraCtr;          // To Count Number of Records Rejected due to less Duration
unsigned long glb_OldCtr;           // To Count Number of Records Rejected due to old Start Datetime
unsigned long glb_ErrCtr;           // To Count Number of Regords Error due to fields cannot be validated

unsigned long glb_MapCtr;           // To Count Number of Records Mapped to Call Buffer
unsigned long glb_SamCtr;           // To Count Number of Records Processed for Sample Size Tracking

unsigned long glb_EAlmPostpaidCtr;  // To Count Number of Record Write to Postpaid External Alarm File
unsigned long glb_EAlmPrepaidCtr;   // To Count Number of Record Write to Prepaid External Alarm File
unsigned long glb_EAlmAinCtr;       // To Count Number of Record Write to AIN External Alarm File
unsigned long glb_EAlmNrtCtr;       // To Count Number of Record Write to NRTRDE External Alarm File

int glb_SvcIdCnt;

//
// Global File pointers
//
FILE *glb_Inifp;                        // Global pointer to ini File

FILE *glb_Cdrfp;                        // Global pointer to CDR File
FILE *glb_Chkfp;                        // Global pointer to Check Point File

FILE *glb_Errfp;                        // Global pointer to Error File
FILE *glb_Ignfp;                        // Global pointer to Ingore File
FILE *glb_RCdrfp;                       // Global pointer to Rejected(Ignored data) CDRs File
FILE *glb_Eventcdrfp;                   // Global pointer to FIT CDR File
FILE *glb_PrepaidEAlmfp;                // Global pointer to Prepaid External Alarm Data File
FILE *glb_PostpaidEAlmfp;               // Global pointer to Postpaid External Alarm Data File
FILE *glb_AinEAlmfp;                    // Global pointer to AIN External Alarm Data File
FILE *glb_NrtEAlmfp;                    // Global pointer to NRTRDE External Alarm Data File
FILE *glb_PpsSpltfp[NOF_PPSAWN_PATH];   // Global pointer to Splitted PPS AWN CDR Data File
FILE *glb_SvcIdSubfp;                   // Global pointer to FIT CDR File

#ifdef  __cplusplus
    }
#endif

#endif // _FRM_CDR_GLOB_H_ //
