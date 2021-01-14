/*
**++
**  FACILITY    : CDR Mapping
**
**  FILE NAME   : frm_cdr_mapp.h
**
**  ABSTRACT    : Structure of all the CDR Mapping Master Formats.
**        All the character array field size includes null size also
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

#ifndef _FRM_CDR_MAPP_H_
#define _FRM_CDR_MAPP_H_

#ifdef  __cplusplus
    extern "C" {
#endif

/*
** size of Raw CDR Records
*/
/* Changed CDR Format to New MD CDR Format - by Kawee on 20-Oct-2003 */
#define SIZE_GSMRAWOLD      272+1       /* 264 + 8 + 1 for Newline - Null not included (8 - Bno extended) */
#define SIZE_GSMRAW         291+1       /* 272 + 19 + 1 for Newline - Null not included (19 - 4G Cell Info) */

#define SIZE_SMSGPR         264+6+1     /* 264 + + 6 for new added field for only SMS + 1 for Newline - Null not included */
/* Resize SMS and SMC Raw size from 268 to 274 by Kawee on 20-Jun-2006 */
#define SIZE_SMSRAWOLD      324         /* 323 + 1 for Newline - Null not included */ /* Added by Thanakorn on Aug-2013 */
#define SIZE_SMSRAW         327         /* 326 + 1 for Newline - Null not included */ /* Added by Thanakorn on Aug-2013 */
#define SIZE_SMCRAWOLD      304         /* 303 + 1 for Newline - Null not included */ /* Added by Jetsada on 15-Aug-2005 */
#define SIZE_SMCRAW         324         /* 323 + 1 for Newline - Null not included */ /* Added by Thanakorn on Oct-2011 */
#define SIZE_SSPRAWOLD      272+1       /* 264 + 8 + 1 for Newline - Null not included (8 - Bno extended)*/
#define SIZE_SSPRAW         291+1       /* 272 + 19 + 1 for Newline - Null not included (19 - 4G Cell Info) */

/* Changed USC CDR Format to New MD CDR Format - by Kawee on 20-Oct-2003 */
#define SIZE_USCRAWOLD      223         /* 222 + 1 for Newline - Null not included */
#define SIZE_USCRAW         226         /* 225 + 1 for Newline - Null not included */

#define SIZE_DPCGSMRAWOLD   279+1       /* 279 + 1 for Newline - Null not included */
#define SIZE_DPCGSMRAW      287+1       /* 287 + 1 for Newline - Null not included (8 - Bno extended)*/
#define SIZE_GPRSRAWOLD     338         /* 337 + 1 for Newline - Null not included */
#define SIZE_GPRSRAW        339         /* Resize 337 + 1(for Newline - Null not included) to 338 + 1 by Thanakorn on 11-Jan-2010 */
#define SIZE_MSCPPSRAWOLD   264+1       /* 264 + 1 for Newline - Null not included */
#define SIZE_MSCPPSRAW      272+1       /* 272 + 1 for Newline - Null not included (8 - Bno extended)*/
#define SIZE_GPRSTRAWOLD    527         /* 526 + 1 by Thanakorn on 04-Jan-2010 */
#define SIZE_GPRSTRAW       1024        /* 1023 + 1 by Thanakorn on 18-Dec-2012 (this type is variable length record) */
#define SIZE_MMSRAWOLD      375         /* 375 + 1 for Newline - Null not included */   /* Resize 353+1 to 374+1 by Kawee on 31-Aug-2007 */
#define SIZE_MMSRAW         527         /* 526 + 1 for Newline - Null not included by Thanakorn on 04-Jan-2010 */
#define SIZE_INPUT          1520        /* 1520 - Null not included */  /* added by Kawee on 02-Jun-2005 */ /* extend size from 1024 to 1520 by Thanakorn on 25-Feb-2010 */
#define SIZE_DPCSMSGPRRAW   266         /* 265 + 1 for Newline - Null not included */

#define NUMFLD_GPRSTOLD     37          /* Changed from 28 to 37 by Thanakorn on Jan 2010 */
#define NUMFLD_GPRST        42          /* Changed from 37 to 42 by Thanakorn on Sep 2011 to Support 3G */
#define NUMFLD_MMSOLD       36          /* Changed from 35 to 36 by Kawee on 31-Aug-2007 */
#define NUMFLD_MMS          44          /* Changed from 36 to 44 by Thanakorn on 01-Jan-2010 */
#define NUMFLD_AIN          37          /* Introduce AIN CDR Type - Added by Kawee on 29-Jan-2007 */
#define NUMFLD_WLAN         21          /* Introduce WLAN CDR Type - Added by Kawee on 02-May-2007 */
#define MAX_READ_SIZE       1024

/*
** Constants used for Loading Memory Tables
*/
/* Changed as below by Kawee on 02-Jun-2005 */
#define SIZE_LOADREC        1024        /* max size of Records from Mapping Tables ($DATADIR/ *.DAT files) */


#define BASE64              64
static const char glb_sBASE64[BASE64+1] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz@$";

typedef enum {
    IDX_IDD_TABLE = 0,                  // Index of International Rate Table in Loading Statistics
    IDX_NPBN_TABLE,                     // Index of Number Port by Nature Table in Loading Statistics - Added by Kawee on 14-Jun-2004
    IDX_BILLREG_TABLE,                  // Index of Bill Register Table in Loading Statistics - Added by Kawee on 18-Nov-2004
    IDX_ADJGRP_TABLE,                   // Index of Adjacent Group Table in Loading Statistics - Added by Kawee on 18-Nov-2004
    IDX_BILLAREA_TABLE,                 // Index of Bill Area Table in Loading Statistics - Added by Kawee on 18-Nov-2004
    IDX_BEARER_TABLE,                   // Index of Bearer Table in Loading Statistics - Added by Kawee on 02-Jun-2004
    IDX_COSTCODE_TABLE,                 // Index of Cost Code Table in Loading Statistics - Added by Kawee on 02-Jun-2004
    // IDX_COSTBAND_TABLE,              // Index of Cost Band Table in Loading Statistics - Added by Kawee on 02-Jun-2004
    // IDX_RATING_TARIFF_TABLE,         // Index of Rating Tariff Table in Loading Statistics - Added by Kawee on 02-Jun-2004
    IDX_RATING_ELEMENT_TABLE,           // Index of Rating Element Table in Loading Statistics - Added by Kawee on 02-Jun-2004
    IDX_COSTGROUP_XREF_TABLE,           // Index of Costgroup Xref Table in Loading Statistics - Added by Jetsada on 21-Jul-2005
    IDX_SERVICE_KEY_TABLE,              // Index of Service Key Table in Loading Statistics - Added by Jetsada on 21-Jul-2005
    IDX_PPS_DESC_TABLE,                 // Index of PPS Desc Table in Loading Statistics - Added by Jetsada on 15-Aug-2005
    // IDX_FMAP_TABLE,                  // Index of Feature Mapping Table in Loading Statistics - not used now will be used in future
    IDX_AIN_COSTCODE_TABLE,             // Index of AIN Costcode Table in Load Statistics - Added by Kawee on 29-Jan-2007
    IDX_AIN_OPER_CARRIER_MASTER_TABLE,  // Index of AIN Operator Carrier Master Table in Load Statistics - Added by Kawee on 29-Jan-2007
    IDX_AIN_RATING_TARIFF_ELEMENT_TABLE,// Index of AIN Rating Tariff Element Table in Load Statistics - Added by Kawee on 29-Jan-2007
    IDX_AIN_ROUTE_MASTER_TABLE,         // Index of AIN Route Master Table in Load Statistics - Added by Kawee on 29-Jan-2007
    IDX_AIN_SUBTYPE_MASTER_TABLE,       // Index of AIN Subscriber Type Master Table in Load Statistics - Added by Kawee on 29-Jan-2007
    IDX_AIN_E1_MAPPING_TABLE,           // Index of AIN E1 Mapping Table in Load Statistics - Added by Jetsada on 17-Sep-2007
    IDX_AIN_BASIC_TIME_RATE_TABLE,      // Index of AIN Basic Time Rate Table in Load Statistics - Added by Thanakorn on 03-Mar-2010
    IDX_WLAN_PACKAGE_TABLE,             // Index of WLAN PACKAGE Table in Load Statistics - Added by Jetsada on 10-Oct-2007
    IDX_IODC_TABLE,                     // Index of International Operator Direct Connection Table in Load Statistics - Added by Kawee on 04-Dec-2007
    IDX_IMSI_TABLE,                     // Index of IMSI Mapping Table in Load Statistics - Added by Kawee on 06-Feb-2008
    IDX_IMSI_VSO_TABLE,                 // Index of IMSI VSO Mapping Table in Load Statistics - Added by Kawee on 15-Aug-2008
    IDX_FIX_RATE_TABLE,                 // Index of FIX RATE Mapping Table in Load Statistics - Added by Kawee on 19-Sep-2008
    IDX_REJ_ORIG_ADDR_TABLE,            // Index of REJ ORIG ADDR Mapping Table in Load Statistics - Added by Wiraphot on 07-Jan-2009
    IDX_PLMN_CODE_TABLE,                // PLMN Code for Mapping Cell Area - Added by Thanakorn on 26-Nov-2009
    IDX_APN_TABLE,                      // APN table for Mapping Access Point Name NI - Added by Watthikorn.T on 24-May-2010
    IDX_3G_MAP_TABLE,                   // 3G/2G Service Mapping table - Added by Thanakorn on Sep-2011
    IDX_TOLL_FREE_TABLE,                // Toll-free table used to set skip_detec_unauthorize - Added by Thannakorn on Nov-2012
    IDX_ONE_LOVE_TABLE,                 // One Love table list used to set charge to be zero if a specific ano calll to a specific bno list - Added by Thanakorn on Sep-2013
    IDX_BOS_IMEI_TABLE,                 // IMEI table list - Added by Thanakorn on 13-Nov-2013
    IDX_COSTCODE_BNO_TABLE,             // Cost code Bno - Added by Thanakorn on 23-Apr-2014
    IDX_USMAP_TABLE,                    // USMAP table for vas description - Added by Thanakorn on Sep-2016
    IDX_GEO_TABLE,                      // GEO_CELL table for geo mapping (CELL_GEO.DAT) - Added by Thanakorn on Mar-2020
    IDX_CELLAREA_TABLE,                 // GEO_CELL table for geo mapping (CELLAREA_GEO.DAT) - Added by Thanakorn on Mar-2020
    IDX_VASGROUP_TABLE,
    NOF_MAPDFILES
} IDX_MAPD_TABLES;

typedef enum {
    SKIP_DETECTION=0,
    SKIP_ANCESTORS,
    SKIP_ANCESTORSIFNOSIB,
    SKIP_PARENT,
    SKIP_PARENTIFNOSIB,
    SKIP_PIPELINE,
    SKIP_ARCHIVETOORACLE,
    SKIP_ARCHIVETOVERTICA,
    SKIP_HISTORY,
    SKIP_COLLISION,
    SKIP_VELOCITY,
    SKIP_DUPEVENT,
    SKIP_UPDOLDEVENT,
    SKIP_BLACKLIST,
    SKIP_USAGEACCUM,
    SKIP_RECENTCHARGE,
    SKIP_DESTUSAGE,
    SKIP_THRESHOLD,
    SKIP_DEST,
    SKIP_SUSP,
    SKIP_AUTHORIZEDFEATURE,
    SKIP_PATTERN,
    SKIP_ACCUMPATTERN,
    SKIP_SEQPATTERN,
    SKIP_IMMEDIATEROAM,
    SKIP_UNKNOWN,
    SKIP_PREMATURECHANGE,
    SKIP_ROAMNOHOME,
    SKIP_INACTIVECHECK,
    SKIP_CONTACT,
    SKIP_REALTIMEANALYSIS,
    SKIP_UTILITYMODULE,
    SKIP_EVENTPARTITIONOVERRIDE,
    SKIP_POSTPATTERN,
    NOF_SKIP
} IDX_SKIP_DET_TABLES;

#define SIZE_NET        3   /* Max Size of Network Code in Mapping Tables */
#define SIZE_NPA        6   /* Max Size of Numbering Plan in Mapping Tables (for 9 digits, changed from 4 - 6)*/
#define SIZE_ZID        3   /* Max Size of Zone Id in Mapping Tables */
#define SIZE_CID        8   /* Max Size of Cell Id in Mapping Tables */
#define SIZE_ZTYPE      2   /* Max Size of Zone Type in Mapping Tables */
#define SIZE_UNIT       2   /* Max Size of Unit Code in Mapping Tables */

#define SIZE_CAREA      5   /* Actual size of Cell Area */
#define SIZE_CSET       5   /* Actual size of Cell Set */
#define SIZE_CELL       5   /* Actual size of Cell */

#define SIZE_IDD        4   /* Actual size of (IDD) country code */
#define SIZE_NDD        5   /* Actual size of (NDD) area code */

#define SIZE_SCAT       3   /* Service Category Id */
#define SIZE_CONT       3   /* Content Id */
#define SIZE_SVC        6   /* Service Id */

#define SIZE_LDD        6   /* Max LDD Number (Country Code) */
#define SIZE_FEAT       4   /* Actual size of Feature Code */

#define SIZE_IODC       24  /* IODC B-Number Prefix size for IODC Table */      /* Added by Kawee on 04-Dec-2007 */

#define SIZE_SVKCDR     6

#define SIZE_COSTCODE       52      /* Max Size of Cost Code in Mapping Tables */       /* Added by Kawee on 10-Jan-2002 */
#define SIZE_COSTBAND_ID    9       /* Max Size of Cost Band Id in Mapping Tables */    /* Added by Kawee on 10-Jan-2002 */
/* Resize from 3 byte to 32 - by Kawee on 16-Nov-2007 */
/* Added below by Jetsada on 29-Jul-2005 */
#define SIZE_COSTCODE_NAME  32      /* Max Size of Cost Code Name in Mapping Tables (Approximate) */
#define SIZE_COSTGROUP_ID   16      /* Max Size of Cost Group Id in Mapping Tables (Approximate) */ /* Added by Jetsada on 26-Jul-2005 */
#define SIZE_VASTYPE        1       /* Max Size of VAS Type in Mapping Tables */        /* Added by Kawee on 17-Jan-2002 */

#define SIZE_TARIFF         4       /* Actual size of DPC Tariff Code */

#define SIZE_PROVCODE       3       /* Size Province Code */    /* Introduce Adjcent Area - Added by Kawee on 18-Nov-2004 */
#define SIZE_ADJGRP         2       /* Size Adjcent Group Code */   /* Introduce Adjcent Area - Added by Kawee on 18-Nov-2004 */

#define SIZE_DATETIME       14      /* Size of date & time in YYYYMMDDHHMMSS format */  /* Added by Kawee on 14-Jun-2004 */

#define SIZE_SERVICE        16      /* Max Size of Service in SERVICE_KEY.DAT (Approximate) */  /* Added by Jetsada on 26-Jul-2005 */

#define SIZE_RATING_TARIFF_ID       3   /* Max Size of Rating Tariff Id in RatingElement Table (Approximate) */ /* Added by Jetsada on 26-Jul-2005 */
#define SIZE_EVENT_CLASS_ID         2   /* Max Size of Event Class Id in RatingElement Table (Approximate) */ /* Added by Jetsada on 28-Jul-2005 */
#define SIZE_TIME_RATE_ID           3   /* Max Size of Time Rate Id in RatingElement Table (Approximate) */ /* Added by Kawee on 04-Jan-2007 */
#define SIZE_CHARGE_SEGMENT_ID      2   /* Max Size of Charge Segment Id in RatingElement Table (Approximate) */ /* Added by Kawee on 04-Jan-2007 */
#define SIZE_RATING_CATALOGUE_ID    1   /* Max Size of Rating Catalogue Id in RatingElement Table (Approximate) */ /* Added by Jetsada on 28-Jul-2005 */
#define SIZE_CHARGE_RATE            21  /* Size for Keep long Value */ /* Added by Jetsada on 26-Jul-2005 */
#define SIZE_FIXED_CHARGE           21  /* Size for Keep long Value */ /* Added by Jetsada on 26-Jul-2005 */
#define SIZE_UNIT_DURATION          21  /* Size for Keep long Value */ /* Added by Kawee on 04-Jan-2007 */

#define SIZE_ACTUAL_REC_TYPE        2   /* Max Size of Actual Record Type (Approximate) */ /* Added by Jetsada on 15-Aug-2005 */

/* Introduce AIN CDR Type - Added below by Kawee on 29-Jan-2007 */
#define SIZE_OPER_CARRIER_ID        4
#define SIZE_OPER_SHORTNAME         50
#define SIZE_OPER_SINGLE_BILL_FLAG  1
#define SIZE_AIN_DESTCODE           50
#define SIZE_AIN_SUBTYPE            3
#define SIZE_AIN_SUBCARRIER         10
#define SIZE_AIN_SUBPREFIX          6
#define SIZE_AIN_SOURCE_NAME        30
#define SIZE_AIN_TARGET_NAME        30

#define IDDACC_CODE         "001"       /* Normal IDD Access Code */
#define EPHACC_CODE         "009"       /* E-Phone Access Code */
#define TOTIDDACC_CODE      "007"       /* TOT International Access Code */
#define TOTVOIPACC_CODE     "008"       /* TOT VOIP International Access Code */    /* Added by Kawee on 07-Sep-2004 */
#define TOTMALAYACC_CODE    "002"       /* TOT VOIP International Access Code (Call to Malaysia) */
#define AINIDDACC_CODE      "005"       /* AIN International Access Code */     /* Added by Kawee on 15-Nov-2006 */
#define AIN2NDACC_CODE      "0050"      /* AIN 2nd Product International Access Code */ /* Added by Kawee on 25-Aug-2008 */
#define AIN3RDACC_CODE      "00500"     /* AIN 3rd Product International Access Code */ /* Added by Kawee on 11-Nov-2009 */

#define AWNIDDACC_COD1      "003"       /* AIN International Access Code */     /* Added by Kawee on 15-Nov-2006 */
#define AWNIDDACC_COD2      "0030"      /* AIN 2nd Product International Access Code */ /* Added by Kawee on 25-Aug-2008 */
#define AWNIDDACC_COD3      "00300"     /* AIN 3rd Product International Access Code */ /* Added by Kawee on 11-Nov-2009 */

#define OASST1_CODE         "001800"    /* Operator Assisted Call 1 */
#define OASST2_CODE         "001999"    /* Operator Assisted Call 2 */
#define LDDACC1_CODE        "C0"        /* LDD Access Code */
#define LDDACC2_CODE        "#0"        /* LDD Access Code */
#define LCACC_CODE          "C999"      /* Long Call Access Code */ /* Added by Kawee on 04-Dec-2007 */
#define VPNACC_CODE         "00333"     /* VPN Access Code */

#define IDDACC_LEN          3           /* Actual length of Normal IDD Access Code */
#define EPHACC_LEN          3           /* Actual length of E-Phone Access Code */
#define TOTIDDACC_LEN       3           /* Actual length of TOT International Access Code */
#define TOTVOIPACC_LEN      3           /* Actual length of TOT VOIP International Access Code */   /* Added by Kawee on 07-Sep-2004 */
#define TOTMALAYACC_LEN     3           /* Actual length of TOT VOIP International Access Code (Call to Malaysia) */ /* Added by Kawee on 08-Sep-2005 */
#define AINIDDACC_LEN       3           /* Actual length of AIN International Access Code */    /* Added by Kawee on 04-Jan-2007 */
#define AIN2NDACC_LEN       4           /* Actual length of AIN 2nd Product International Access Code *//* Added by Kawee on 25-Aug-2008 */
#define AIN3RDACC_LEN       5           /* Actual length of AIN 3rd Product International Access Code *//* Added by Kawee on 11-Nov-2009 */
#define OASST1_LEN          6           /* Actual length of Operator Asissted Call 1 */
#define OASST2_LEN          6           /* Actual length of Operator Asissted Call 2 */
#define LDDACC1_LEN         2           /* Actual length of LDD Access Code */
#define LDDACC2_LEN         2           /* Actual length of LDD Access Code */
#define LCACC_LEN           4           /* Actual length of Long Call Access Code */    /* Added by Kawee on 04-Dec-2007 */
#define VPNACC_LEN          5           /* Actual length of VPN Access Code */

/* Introduce CDG Transaction. Added below by Kawee on 02-Jun-2005 */
#define MAX_LOAD_MAP_FIELD  32

/*
** global variables & buffers used only in cust_cdr_mapp.c
*/
/* Remove condition to check case 00760, 007855, 007856, 00795 - Changed as below by Kawee on 02-May-2007 */
// int glb_tac = 21;       /* global variable - total number of Area Codes (See Below the AREA_TABLE) */
// int glb_tSSRec = 0;     /* global variable to count total SS Records */

/* Changed as below by Kawee on 22-Feb-2007 */
// char    glb_Eindex[50+SIZE_MTXTYPE+1]; [> To construct Error Index Display (rectype-imsi-date-time: mtxtype:) <]
char    glb_Eindex[50+SIZE_MTXTYPE+100+1]; /* To construct Error Index Display (rectype-imsi-date-time: mtxtype:) */

//
// Structure to hold Mapping Data Files Loading Statistics
//
typedef struct mapd_stat {
    char    file_name[SIZE_FNAME]; // Mapping Data File Name including path
    time_t  lmod_time;              // Last Known Successful File time
    time_t  cmod_time;              // Current File time - Trying to load
    int (*load_func)(int);          // Pointer to Loading Function
    char    load_flag;              // Loading Flag : Installation dependent (based on INSTALLATION_NAME in CDRFEED.INI)
                                    //  FALSE -> Do not load for this installation
                                    //  TRUE -> Load for this installation
                                    //  - Added by Kawee on 13-Jan-2003
                                    //
    char    load_stat;              // Loading Status : FALSE -> To be loaded, TRUE -> Already loaded
    int     tot_recs;               // Total Records Loaded
    int     num_loads;              // Number of Loadings : 0 -> Not yet loaded, Non-Zero -> loaded n times
    void    **pptable_head;         // Pointer to Head Table Pointer */ /* Added by Kawee on 22-Jun-2007
    //
    // Introduce CDG Transaction. Added Information for Variable Field Length Mapping Table as below.
    //  - by Kawee on 02-Jun-2005
    //
    struct {
        char    cComment;           // Comment Character
        char    cDelimiter;         // Delimiter Character
        int     iNofLoadFld;        // Number of Field to be load
        int     aiLoadFldId[MAX_LOAD_MAP_FIELD];    // Array of Loaded Field Id
    } vl_info;
} MAPD_STAT;
MAPD_STAT glb_MapdStat[NOF_MAPDFILES];


/*
** Structure of One Record in Memory Mobile Network Table. Two separate Memory Mobile Network Table, one each AIS and DPC.
** Data from $DATADIR/MOB_NETWORK.DAT is loaded into this table (for AIS).
** Data from $DATADIR/DPC_MOB_NETWORK.DAT is loaded into this table (for DPC).
** Size (Number of Records) in this Memory Table is decided at run-time.
** This table is used to find (Binary Search) Network Type Origin/Destination for Rating a CDR.
** Input data record format is : Number Plan#Network
*/
typedef struct mnet_table {
    char    nplan[SIZE_NPA+1];     /* Numbering Plan */
    char    network[SIZE_NET+1];   /* Network Type */
} MNET_TABLE;
//MNET_TABLE *glb_MnetHead=NULL;      /* global pointer to AIS MOB_NETWORK Table */
//MNET_TABLE *glb_DpcMnetHead=NULL;   /* global pointer to DPC MOB_NETWORK Table */


/*
** Structure of One Record in Memory Number Port Table
** Data from $DATADIR/NUM_PORT.DAT is loaded into this table.
** Size (Number of Records) in this Memory Table is decided at run-time.
** This table is used to find (Binary Search) Network Type Origin/Destination of Ported Numbers.
** Input data record format is : Network#Ported Number
*/
typedef struct nport_table {
    char    nport[SIZE_PHONENO+1]; /* Number Ported */
    char    network[SIZE_NET+1];   /* Network Type */
} NPORT_TABLE;
//NPORT_TABLE *glb_NportHead=NULL;    /* global pointer to NUM_PORT Table */

/*
** Structure of One Record in Memory Number Port by Nature Table
** Data from $DATADIR/NPBN.DAT is loaded into this table.
** Size (Number of Records) in this Memory Table is decided at run-time.
** This table is used to find (Binary Search) Network Type Origin/Destination of Ported Numbers.
** Input data record format is : Ported Number#Start Effective Date#End Effective Date#Last Renew Date
*/
typedef struct npbn_table {
    char    mob_number[SIZE_PHONENO+1];    /* Added 1 more char for fix load NPBN Table problem - by Kawee on 30-Nov-2006 */
    char    start_eff_date[SIZE_DATETIME+1];
    char    end_eff_date[SIZE_DATETIME+1];
    char    network[SIZE_NET+1];       /* Added Network Field on 02-Nov-2004 */
} NPBN_TABLE;
//NPBN_TABLE *glb_NpbnHead=NULL;      /* global pointer to NPBN Table */

/*
** Structure of One Record in Memory Mobile Station Roaming Number Table
** Data from $DATADIR/MSRN.DAT is loaded into this table.
** Size (Number of Records) in this Memory Table is decided at run-time.
** This table is used to find out the roaming network and cell (home only), during mapping to rating a CDR.
** Input data record format is : Network#Cell#Msrn From#Msrn To
*/
typedef struct msrn_table {
    char    network[SIZE_NET+1];   /* Network Type */
    char    cell[SIZE_CELL+1]; /* Cell*/
    long    msrnfrom;       /* Msrn From */
    long    msrnto;         /* Msrn To */
} MSRN_TABLE;
//MSRN_TABLE *glb_MsrnHead=NULL;      /* global pointer to Mobile Station Roaming Number Table */


/*
** Structure of One Record in Memory Zone Table. Two separate Memory Zone Table, one each AIS and DPC.
** Data from $DATADIR/ZONE.DAT is loaded into this table (for AIS).
** Data from $DATADIR/DPC_ZONE.DAT is loaded into this table (for DPC).
** Size (Number of Records) in this Memory Table is decided at run-time.
** This table is used to find (Binary Search) the cell_ida and cell_idb for Rating a CDR.
**
** To improve the binary search speed, Network Type and Cell Id are combined to form a single key
** Input data record format is : Network#Zone ID#Cell ID
*/
typedef struct zone_table {
    char    netcell[SIZE_NET+SIZE_CID+1];  /* Network Type + Cell Id (combined key) */
    char    zoneid[SIZE_ZID+1];        /* Zone Id */
} ZONE_TABLE;
//ZONE_TABLE *glb_ZoneHead=NULL;          /* global pointer to AIS Zone Table */
//ZONE_TABLE *glb_DpcZoneHead=NULL;       /* global pointer to DPC Zone Table */


/*
** Strcture of One Record in Memory Zone Mapping Table. Two separate Memory Zone Mapping Table, one each AIS and DPC.
** Data from $DATADIR/ZMAPPING.DAT is loaded into this table (for AIS).
** Data from $DATADIR/DPC_ZMAPPING.DAT is loaded into this table (for DPC).
** Size (Number of Records) in this Memory Table is decided at run-time.
** This table is used to find (Binary Search) the zone_type for Rating a CDR.
**
** To improve the binary search speed, Network Type, Zone Id A, Zone Id B are combined to form a single key
** Input data record format is : Network#Zone Id A#Zone Id B#Zone Type
*/
typedef struct zmap_table {
    char    netzoneab[SIZE_NET+SIZE_ZID+SIZE_ZID+1];   /* Network Type + Zone Id-A + Zone Id-B (combined key) */
    char    zonetype[SIZE_ZTYPE+1];            /* Zone Type */
} ZMAP_TABLE;
// ZMAP_TABLE *glb_ZmapHead=NULL;                  /* global pointer to AIS Zone Mapping Table */
// ZMAP_TABLE *glb_DpcZmapHead=NULL;               /* global pointer to DPC Zone Mapping Table */


/*
** Structure of One Record in Memory National Rate Table
** Data from $DATADIR/NATL_RATE.DAT is loaded into this table.
** Size (Number of Records) in this Memory Table is decided at run-time.
** This table is used to find out the rate for a zone_type during Rating a CDR.
** Input data record format is : Zone Type#Description#Rate Per Minute
**   Description is ignored while loading.
*/
typedef struct nrate_table {
    char    zonetype[SIZE_ZTYPE+1];    /* Zone Type */
    double  rate_pm;            /* Rate per Minute */
} NRATE_TABLE;
// NRATE_TABLE *glb_NrateHead=NULL;        /* global pointer to AIS National Rate Table */
// NRATE_TABLE *glb_DpcNrateHead=NULL;     /* global pointer to DPC National Rate Table */


/*
** Structure of One Record in Memory International Rate Table
** Data from $DATADIR/INTL_RATE.DAT is loaded into this table.
** This table is used to parse the country code from the dialed digits and rate during Rating a CDR.
** Input data record format is : Idd Code#Country Name#IDD Rate Per Minute#E-Phone Rate per Minute
**   Country Name is ignored while loading.
*/
typedef struct idd_table {
    char    idd_code[SIZE_IDD+1];  /* Country Code */
    double  idd_rate_pm;        /* Rate per Minute */
    double  eph_rate_pm;        /* E-Phone - Rate per Minute - added on 08-Oct-2001 chamy */
    double  tot_intl_rate_pm;   /* TOT Intl' - Rate per Minute - added on 24-Aug-2004 Kawee */
    double  tot_voip_rate_pm;   /* TOT Intl' VOIP - Rate per Minute - added on 07-Sep-2004 Kawee */
} IDD_TABLE;
// IDD_TABLE *glb_IddHead=NULL;        /* global pointer to International Rate Table */

/*
** Structure of Memory Area Codes Table
*/
typedef struct area_table {
    int     size;
    char    area_code[SIZE_NDD+1]; /* Area Code */
} AREA_TABLE;

/*
** Structure of 3G/2G Mapping Table
*/
typedef struct service3G_table {
    char   *szSpeedtype;
    char   *szRat;
    char   *szServpackid;
    char   *szBearer_id;
    char   *szRating;
    char   *szDescName;
} SERVICE3G_TABLE;
// SERVICE3G_TABLE *glb_3GHead=NULL;       /* global pointer to 3G/2G Service Table */

/*
** Note : When new Area codes are added to the following list, Don't forget to change the
**        value defined in glb_tac above.  - chamy
*/
// AREA_TABLE area_tab[] = {
//     { 2, "02"  },  /* BKK */
//     /* { 2, "09"  },  MALAYSIA old - Removed by Kawee on 29-Oct-2003 */
//     { 3, "032" },  /* PKN/PBI/RBR */
//     { 3, "034" },  /* KRI/NPT/SKM/SKN */
//     { 3, "035" },  /* SPB/AYA/ATG */
//     { 3, "036" },  /* LRI/SBR/SRI */
//     { 3, "037" },  /* PRI/NYK/SKW */
//     { 3, "038" },  /* CBI/CCO/RYG */
//     { 3, "039" },  /* CTI/TRT */
//     { 3, "042" },  /* UDN/NKI/SNK/NPM/MDH/LEI/NBP */
//     { 3, "043" },  /* KKN/MKM/RET/KSN */
//     { 3, "044" },  /* NMA/BRM/CPM/SRN */
//     { 3, "045" },  /* UBN/SSK/YST/ACR */
//     { 3, "053" },  /* CRI/CMI/LPN/MSN */
//     { 3, "054" },  /* LPG/PYO/PRE/NAN */
//     { 3, "055" },  /* PLK/STI/KPT/TAK/UTT */
//     { 3, "056" },  /* NSN/PCT/PBN/UTI/CNT */
//     { 3, "073" },  /* YLA/PTN/NWT */
//     { 3, "074" },  /* SKA/PLG/STN */
//     { 3, "075" },  /* NRT/TRG/KBI */
//     { 3, "076" },  /* PKT/PNA */
//     { 3, "077" },  /* SNI/CPN/RNG */
//     /* { 3, "007" },  LAOS - Removed by Kawee on 29-Oct-2003 */
//     /* { 5, "00760" },  MALAYSIA new - Removed by Kawee on 02-May-2007 */
//     /* { 5, "00785" },  LAOS/CAMBODIA new - Removed by Kawee on 02-May-2007 */
//     /* { 5, "00795" },  MYANMAR new -  Removed by Kawee on 02-May-2007 */
//     { 0, "" }
// };

// /* Fixed IVR Mapping problem - Added below by Kawee on 18-Apr-2007 */
// char ivracc_tab[3][4] = {
//     "*",
//     "900",
//     "B"
// };
/*
** Structure of One Record in Memory Cell Mapping Table
** Data from $DATADIR/CELL_MAP.DAT is loaded into this table.
** Size (Number of Records) in this Memory Table is decided at run-time.
** This table is used to find out the originating cell, cell set and cell area of a CDR, during mapping to DFMS Buffer.
** Input data record format is : Network#Cell Area#Cell Set#Cell Id
*/
typedef struct cmap_table {
    char    network[SIZE_NET+1];       /* Network Type */
    char    cell_area[SIZE_CAREA+1];   /* Cell Area */
    char    cell_set[SIZE_CSET+1];     /* Cell Set */
    char    cell[SIZE_CELL+1];         /* Cell*/
} CMAP_TABLE;
// CMAP_TABLE *glb_CmapHead=NULL;          /* global pointer to Cell Map Table */

/*
** Structure of One Record in Memory Service Rate Table
** Data from $DATADIR/SERVICE_RATE.DAT is loaded into this table.
** Size (Number of Records) in this Memory Table is decided at run-time.
** This table is used to find out the rate for Mobile-Life CDR during mapping.
** Input data record format is :
**     Category_id#Content_id#Bearer#Service_id#Abbreviation#Desctiption#Charge_type#Transaction_value#Content_value
**     Abbreviation and Description fields are ignored while loading.
*/
typedef struct srate_table {
    char    scat_id[SIZE_SCAT+1];      /* Service Category Id */
    char    cont_id[SIZE_CONT+1];      /* Content Id */
    char    bearer;                     /* Bearer Code */
    char    service_id[SIZE_SVC+1];    /* Service Id */
    int     charge_type;                /* Charge Type */
    double  txn_value;                  /* Transaction Value */
    double  cont_value;                 /* Content Value */
} SRATE_TABLE;
// SRATE_TABLE *glb_SrateHead=NULL;        /* global pointer to Service Rate Table */

/*
** Structure of One Record in Memory Feature Mapping Table
** Data from $DATADIR/FEATURE_MAPPING.DAT is loaded into this table.
** Size (Number of Records) in this Memory Table is decided at run-time.
** This table is used to find out cell_idb for Local Direct Dial (LDD) calls during mapping.
** Input data record format is : Network#LDD Number#Cell Id#Feature Code
*/
typedef struct fmap_table {
    char    network[SIZE_NET+1];       /* Network Type */
    char    ldd_num[SIZE_LDD+1];       /* LDD Number */
    char    cell[SIZE_CELL+1];         /* Cell Id */
    char    feature[SIZE_FEAT+1];      /* Feature Code */
} FMAP_TABLE;
// FMAP_TABLE *glb_FmapHead=NULL;      /* global pointer to Feature Mapping Table */

/* Added below by Kawee on 13-Jan-2003 */
/*
** Structure of One Record in Memory VAS Mapping Table (exclusive for AIS).
** Data from $DATADIR/VAS.DAT is loaded into this table.
** Size (Number of Records) in this Memory Table is decided at run-time.
** This table is used to find (Binary Search) cost band id of cost code.
** Input data record format is : Cost Code#Cost Band Id#Cost Band Name
*/
typedef struct vas_table {
    char    vas_key[SIZE_VASTYPE+SIZE_COSTCODE+1];      /* Vastype + Cost Code (combined key) */
    char    cost_band_id[SIZE_COSTBAND_ID+1];      /* Cost Band Id */
} VAS_TABLE;
// VAS_TABLE *glb_VasHead=NULL;        /* global pointer to VAS Mapping Table */

/*
** Structure of One Record in Memory Customer Tariff Table. Two separate Memory Mobile Network Table, one each AIS and DPC.
** Data from $DATADIR/TARIFF.DAT is loaded into this table (for AIS).
** Data from $DATADIR/DPC_CUST_TARIFF.DAT is loaded into this table (for DPC).
** Size (Number of Records) in this Memory Table is decided at run-time.
** This table is used to find (Binary Search) tariff code of phone_no.
** Input data record format is : Phone Number#Tariff Code
*/
typedef struct cust_tariff_table {
    char    phoneno[SIZE_PHONENO+1];   /* Phone Number */
    char    tariff[SIZE_TARIFF+1];     /* Tariff Code */
} CUST_TARIFF_TABLE;
// CUST_TARIFF_TABLE *glb_CustTariffHead=NULL; /* global pointer to AIS CUST_TARIFF Table */
// CUST_TARIFF_TABLE *glb_DpcCustTariffHead=NULL;  /* global pointer to DPC CUST_TARIFF Table */

/*
** Strcture of One Record in Memory Tariff Rating Table (exclusive for DPC).
** Data from $DATADIR/DPC_TARIFF_RATE.DAT is loaded into this table.
** Size (Number of Records) in this Memory Table is decided at run-time.
** This table is used to find (Binary Search) the tariff_rate for Rating a CDR.
**
** To improve the binary search speed, Network Type, Tariff, Zone Type are combined to form a single key
** Input data record format is : Network#Tariff Code#Zone Type#Unit Code#ActualRate
*/
typedef struct tariff_rate_table {
    char    tariff_key[SIZE_NET+SIZE_TARIFF+SIZE_ZTYPE+SIZE_UNIT+1];   /* Network Type + Tariff Code + Zone Type + Unit Code (combined key) */
    double  rate_pm;                        /* Rating */
} TARIFF_RATE_TABLE;
// TARIFF_RATE_TABLE *glb_TariffRateHead=NULL;             /* global pointer to Zone Mapping Table */

/* Added TEMPPHONE Table for NMT Phone that send to fixed - on 25-Apr-2002 by Kawee */
/*
** Structure of One Record in Memory Temporary Phone Table (for NMT Phone that send to repair).
** Data from $DATADIR/TEMP_PHONE.DAT is loaded into this table.
** Size (Number of Records) in this Memory Table is decided at run-time.
** This table is used to find (Binary Search) existing of phone_no.
** Input data record format is : Phone Number
*/
typedef struct temp_phone_table {
    char    phoneno[SIZE_PHONENO+1];   /* Phone Number */
} TEMP_PHONE_TABLE;
// TEMP_PHONE_TABLE *glb_TempPhoneHead=NULL;   /* global pointer to Temp Phone Table */

/* Introduce Adjacent Area Detection. Added BILL_REG Table - by Kawee on 18-Nov-2004 */
/*
** Structure of One Record in Memory Temporary Bill Register Table.
** Data from $DATADIR/BILL_REG.DAT is loaded into this table.
** Size (Number of Records) in this Memory Table is decided at run-time.
** This table is used to find (Binary Search) existing of Msisdn (Phone Number).
** Input data record format is fixed length : Msisdn (11 digits) + Billing Province (3 digits) + 1 new line character
*/
typedef struct bill_reg_table {
    char    msisdn[SIZE_PHONENO+2+1];  /* Msisdn */
    char    bill_prov[SIZE_PROVCODE+1];    /* Billing Province */
} BILL_REG_TABLE;
// BILL_REG_TABLE *glb_BillRegHead=NULL;       /* global pointer to Bill Register Table */

/* Introduce Adjacent Area Detection. Added ADJ_GRP Table - by Kawee on 18-Nov-2004 */
/*
** Structure of One Record in Memory Adjacent Group Table.
** Data from $DATADIR/ADJ_GRP.DAT is loaded into this table.
** Size (Number of Records) in this Memory Table is decided at run-time.
** This table is used to find (Binary Search) existing of phone_no.
** Input data record format is : Billing Province#Adjcent Group
*/
typedef struct adj_grp_table {
    char    bill_prov[SIZE_PROVCODE+1];    /* Billing Province */
    char    adj_grp[SIZE_ADJGRP+1];    /* Adjcent Group */
} ADJ_GRP_TABLE;
// ADJ_GRP_TABLE *glb_AdjGrpHead=NULL;     /* global pointer to Adjcent Group Table */

/* Introduce Adjacent Area Detection. Added BILL_AREA.DAT Table - by Kawee on 18-Nov-2004 */
/*
** Structure of One Record in Memory Billing Area Code Table.
** Data from $DATADIR/BILL_AREA.DAT is loaded into this table.
** Size (Number of Records) in this Memory Table is decided at run-time.
** This table is used to find (Binary Search) existing of phone_no.
** Input data record format is : Cell Area#Billing Province
*/
typedef struct bill_area_table {
    char    cell_area[SIZE_CAREA+1];   /* Cell Area */
    char    bill_prov[SIZE_PROVCODE+1];    /* Billing Province */
} BILL_AREA_TABLE;
// BILL_AREA_TABLE *glb_BillAreaHead=NULL;     /* global pointer to Bill Area Table */

/* Introduce CDG Transaction. Added BEARER.DAT Table - by Kawee on 02-Jun-2005 */
/*
** Structure of One Record in Memory BEARER Code Table.
** Data from $DATADIR/BEARER.DAT is loaded into this table.
** Size (Number of Records) in this Memory Table is decided at run-time.
** This table is used to find (Binary Search) existing of phone_no.
** Input data record format is : Sgwid|Bearer|Description
*/
typedef struct bearer_table {
    char    *pchSgwid;          /* Sgwid */
    char    *pchBearer;         /* Bearer */
} BEARER_TABLE;
// BEARER_TABLE *glb_BearerHead=NULL;      /* global pointer to Bearer Table */

/* Introduce CDG Transaction. Added COSTCODE.DAT Table - by Kawee on 02-Jun-2005 */
/*
** Structure of One Record in Memory Cost Code Table.
** Data from $DATADIR/COSTCODE.DAT is loaded into this table.
** Size (Number of Records) in this Memory Table is decided at run-time.
** This table is used to find (Binary Search) existing of costcode.
** Input data record format is : Record_Type|Banding_Model_Id|CostCode|Start_Date|Rating_Catalogue_Id|End_Date|Costcode_Name|CostCode_Desc|Costgroup_Id|Cost_Band_Id
*/
typedef struct costcode_table {
    char    *banding_model_id;      /* Field 2: Banding Model Id */
    char    *costcode;              /* Field 3: Cost Code */
    char    *costcode_name;         /* Field 7: Cost Code Name */
    char    *costcode_desc;         /* Field 8: Cost Code Description */    /* Add Vas_Name field for USC and CDG Event Type - by Kawee on 12-Mar-2007 */
    char    *costgroup_id;          /* Field 9: Cost Group Id */
    char    *cost_band_id;          /* Field 10: Cost Band Id */
} COSTCODE_TABLE;
// COSTCODE_TABLE *glb_CostCodeHead=NULL;      /* global pointer to Cost Code Table */

/* Introduce CDG Transaction. Added COSTBAND.DAT Table - by Kawee on 02-Jun-2005 */
/*
** Structure of One Record in Memory Cost Band Table.
** Data from $DATADIR/COSTBAND.DAT is loaded into this table.
** Size (Number of Records) in this Memory Table is decided at run-time.
** This table is used to find (Binary Search) existing of costcode.
** Input data record format is : Record_Type|CostBand_id|RatingCatalogue_Id|Cost_Band_Name|Cost_Band_Desc|Rating_Tariff_Type_Id
*/
typedef struct costband_table {
    char    *cost_band_id;          /* Cost Band Id */
    char    *rate_cat_id;           /* Rating Catelogue Id */
    char    *rate_tariff_type_id;       /* Rating Tariff Type Id */
} COSTBAND_TABLE;
// COSTBAND_TABLE *glb_CostBandHead=NULL;      /* global pointer to Cost Band Table */

/* Introduce CDG Transaction. Added RATINGTARIFF.DAT Table - by Kawee on 02-Jun-2005 */
/*
** Structure of One Record in Memory Rating Tariff Table.
** Data from $DATADIR/RATINGTARIFF.DAT is loaded into this table.
** Size (Number of Records) in this Memory Table is decided at run-time.
** This table is used to find (Binary Search) existing of rating_catalogue_id.
** Input data record format is :
** Record_Type|Rating_Tariff_Id|Rating_Catalogue_Id|Tariff_Name|Tariff_Name|Tariff_Desc|Sales_Start_Dat|Sales_End_Dat|Rating_Tarif_Tpe_id|Banding_Model_Id|Default_Cost_Band_Id|Modifier_Group_Id|Default_Event_Class_Id
*/
typedef struct rating_tariff_table {
    char    *rate_cat_id;           /* Rating Catelogue Id */
    char    *rate_tariff_type_id;       /* Rating Tariff Type Id */
    char    *rate_tariff_id;        /* Rating Tariff Id */
} RATING_TARIFF_TABLE;
// RATING_TARIFF_TABLE *glb_RatingTariffHead=NULL;     /* global pointer to Rating Tariff Table */

/* Introduce CDG Transaction. Added RATINGELEMENT.DAT Table - by Kawee on 02-Jun-2005 */
/*
** Structure of One Record in Memory Rating Element Table.
** Data from $DATADIR/RATINGELEMENT.DAT is loaded into this table.
** Size (Number of Records) in this Memory Table is decided at run-time.
** This table is used to find (Binary Search) existing of rate_tariff_id and cost_band_id.
** Input data record format is :
** Record_Type|Rating_Tariff_Id|Cost_Band_id|Event_Class_Id|Time_Rate_Id|Charge_Segment_Id|Start_Dat|Rating_Catalogue_Id|End_Dat|Fixed_Charge_Mny|Charging_Rate|Duration|Average_Fixed_Charge_Mny|Average_Charging_Rate
*/
typedef struct rating_element_table {
    char    *rate_tariff_id;        /* Rating Tariff Id */
    char    *cost_band_id;          /* Cost Band Id */
    char    *event_class_id;        /* Event Class Id */
    char    *charge_segment_id;     /* Charge Segment Id */ /* Introduce AIN - Added by Kawee on 04-Jan-2007 */
    char    *rating_catalogue_id;       /* Rating Catalogue Id */
    char    *fix_charge_mny;        /* Fixed Charge Money */
    char    *charge_rate;           /* Charging Rate */
    char    *unit_duration;         /* Unit Duration */ /* Introduce AIN - Added by Kawee on 04-Jan-2007 */
    char    *fix_charge_mny_avg;    /* Fixed Charge Money Average */
    char    *charge_rate_avg;       /* Charging Rate Average */
} RATING_ELEMENT_TABLE;
// RATING_ELEMENT_TABLE *glb_RatingElementHead=NULL;       /* global pointer to Rating Element Table */

/* Added COSTGROUPXREF.DAT Table - by Jetsada on 21-Jul-2005 */
/*
** Structure of One Record in Memory Cost Group Xref Table.
** Data from $DATADIR/COSTGROUPXREF.DAT is loaded into this table.
** Size (Number of Records) in this Memory Table is decided at run-time.
** This table is used to find (Binary Search) existing of banding_model_id, costgroup_1_id and costgroup_2_id.
** Input data record format is :
** Record_Type|Banding_Model_Id|Costgroup_1_Id|Costgroup_2_Id|Start_Dat|Rating_Catalogue_Id|End_Dat|Cost_Band_Id
*/
typedef struct costgroup_xref_table {
    char    *banding_model_id;      /* Field 2: Banding Model Id */
    char    *costgroup_1_id;        /* Field 3: Costgroup 1 Id */
    char    *costgroup_2_id;        /* Field 4: Costgroup 2 Id */
    char    *cost_band_id;          /* Field 8: Cost Band Id */
} COSTGROUP_XREF_TABLE;
// COSTGROUP_XREF_TABLE *glb_CostgroupXrefHead=NULL;       /* global pointer to Costgroup Xref Table */

/* Added SERVICE_KEY.DAT Table - by Jetsada on 21-Jul-2005 */
/*
** Structure of One Record in Memory Service Key Table.
** Data from $DATADIR/SERVICE_KEY.DAT is loaded into this table.
** Size (Number of Records) in this Memory Table is decided at run-time.
** This table is used to find (Binary Search) existing of service_key and fci.
** Input data record format is :
** CDR_Type|Service_Key|FCI|Service|Description
*/
typedef struct service_key_table {
    char    *cdr_type;      /* Field 1: CDR Type , Added by Jetsada on 25-Oct-2007 */
    char    *service_key;           /* Field 1: Service Key */
    char    *fci;               /* Field 2: FCI */
    char    *service;           /* Field 3: Service */
} SERVICE_KEY_TABLE;
// SERVICE_KEY_TABLE *glb_ServiceKeyHead=NULL;     /* global pointer to Service Key Table */

/* Added PPS_DESC.DAT Table - by Jetsada on 15-Aug-2005 */
/*
** Structure of One Record in Memory Service Key Table.
** Data from $DATADIR/PPS_DESC.DAT is loaded into this table.
** Size (Number of Records) in this Memory Table is decided at run-time.
** This table is used to find (Binary Search) existing of Service Type and Delivery Report.
** Input data record format is :
** PPS Description|Authentication Result|Service Type|SMS / Delivery Report
*/
typedef struct pps_desc_table {
    char    *pps_desc;          /* Field 1: PPS Description */
    char    *auth_result;           /* Field 2: Authentication Result */
    char    *service_type;          /* Field 3: Service Type */
    char    *report;            /* Field 3: SMS / Delivery Report */
} PPS_DESC_TABLE;
// PPS_DESC_TABLE *glb_PpsDescHead=NULL;       /* global pointer to Pps Desc Table */

/* Introduce AIN CDR Type - Added below by Kawee on 29-Jan-2007 */
/*
** Structure of One Record in Memory AIN Cost Code Table.
** Data from $DATADIR/AIN_COST_CODE.DAT is loaded into this table.
** Size (Number of Records) in this Memory Table is decided at run-time.
** This table is used to find (Binary Search) existing of Cost_Code_Mapping and Destination Code.
** Input data record format is :
**  Field No.   Field Name
**  =========   ==========================
**  1.      Record_Type
**  2.      Cost_Code_Mapping
**  3.      Cost_Code1
**  4.      Cost_Code_Name1
**  5.      Cost_Code2
**  6.      Cost_Code_Name2
**  7.      Cost_Code3
**  8.      Cost_Code_Name3
**  9.      Cost_Code4
**  10.     Cost_Code_Name4
**  11.     Cost_Code5
**  12.     Cost_Code_Name5
**  13.     Match_Length
**  14.     Cost_Band_Id
**  15.     Effective_DTM
**  16.     Expire_DTM
**  17.     Cli_Flag
**  18.     Service_Grade
**  19.     Display_Statement
**  20.     City
**  21.     Service
**  22.     Operator
**  23.     Price_Destination
**  24.     Price_Type
**  25.     Continent_Id
**  26.     Country_Desc_Thai
**  27.     Service_On_Off
**  28.     Tb_Schema
**  29.     Destination_Code
**  30.     Upd_DTM
**  31.     Upd_By
** Remark: All Record Separated by "|" (Pipe Mark)
*/
typedef struct ain_costcode_table {
    char    *costcode_mapping;      /* Field 2 : Cost Code Mapping */
    char    *dest_code;             /* Field 29: Destination Code */
    char    *cdr_type;              /* Field 33: CDR Type */
    char    *cost_band_id;          /* Field 14: Cost Band Id */
} AIN_COSTCODE_TABLE;
// AIN_COSTCODE_TABLE *glb_AinCostCodeHead=NULL;   /* global pointer to AIN Cost Code Table */

/* Introduce AIN CDR Type - Added below by Kawee on 29-Jan-2007 */
/*
** Structure of One Record in Memory AIN Operator Carrier Master Table.
** Data from $DATADIR/AIN_OPER_CARRIER_MASTER.DAT is loaded into this table.
** Size (Number of Records) in this Memory Table is decided at run-time.
** This table is used to find (Binary Search) existing of Oper_Carrier_Id.
** Input data record format is :
**  Field No.   Field Name
**  =========   ==========================
**  1.      Record_Type
**  2.      Oper_Carrier_Id
**  3.      Oper_Carrier_Type
**  4.      Short_Name
**  5.      Full_Name
**  6.      Single_Bill_Flag
**  7.      Dummy_Mobile_No
**  8.      Effective_DTM
**  9.      Expire_DTM
**  10.     Upd_DTM
**  11.     Upd_By
** Remark: All Record Separated by "|" (Pipe Mark)
*/
typedef struct ain_oper_carrier_master_table {
    char    *oper_carrier_id;       /* Field 2: Operator Carrier Id */
/*  char    *short_name; */         /* Field 4: Short Name */
  char  *single_bill_flag;  /* Field 6: Single Bill Flag */
} AIN_OPER_CARRIER_MASTER_TABLE;
// AIN_OPER_CARRIER_MASTER_TABLE *glb_AinOperCarrierMasterHead=NULL;   /* global pointer to AIN Operator Carrier Master Table */

/* Introduce AIN CDR Type - Added below by Kawee on 29-Jan-2007 */
/*
** Structure of One Record in Memory AIN Rating Tariff Element Table.
** Data from $DATADIR/AIN_RATING_TARIFF_ELEMENT.DAT is loaded into this table.
** Size (Number of Records) in this Memory Table is decided at run-time.
** This table is used to find (Binary Search) existing of Rating_Tariff_Id, Cost_Band_Id, and Modifier_Class_Id
** Input data record format is :
**  Field No.   Field Name
**  =========   ==========================
**  1.      Record_Type
**  2.      Rating_Tariff_Id
**  3.      Cost_Band_Id
**  4.      Modifier_Class_Id
**  5.      Time_Plan_Id
**  6.      Time_Rate_Id
**  7.      Step_Group
**  8.      Step_Seq
**  9.      Fixed_Rate
**  10.     Charge_Rate
**  11.     Min_Charge_Rate
**  12.     Max_Charge_Rate
**  13.     Effective_DTM
**  14.     Expire_DTM
**  15.     Upd_DTM
**  16.     Upd_By
**  17.     CHARGE_RATE_TYPE
**  18.     SUBSCRIBER_TYPE
**  19.     COSTING_RULE_ID
** Remark: All Record Separated by "|" (Pipe Mark)
*/
typedef struct ain_rating_tariff_element_table {
    char    *rating_tariff_id;      /* Field 2 : Rating Tariff Id */
    char    *cost_band_id;          /* Field 3 : Cost Band Id */
    char    *modifier_class_id;     /* Field 4 : Modifier Class Id */
    char    *time_plan_id;          /* Field 5 : Time Plan Id, Add by Thanakorn on 05-Mar-2010 */
    char    *time_rate_id;          /* Field 6 : Time Rate Id, Add by Thanakorn on 05-Mar-2010 */
    char    *step_group;            /* Field 7 : Step Group, Add by Thanakorn on 05-Mar-2010 */
    char    *step_seq;              /* Field 8 : Step Sequence, Add by Thanakorn on 05-Mar-2010 */
    char    *subs_type;             /* Field 18: Subscriber Type, Add by Thanakorn on 05-Mar-2010 */
    char    *charge_rate;           /* Field 10: Charge Rate */
} AIN_RATING_TARIFF_ELEMENT_TABLE;
// AIN_RATING_TARIFF_ELEMENT_TABLE *glb_AinRatingTariffElementHead=NULL;       /* global pointer to AIN Rating Tariff Element Table */

/* Introduce AIN CDR Type - Added below by Kawee on 29-Jan-2007 */
/*
** Structure of One Record in Memory AIN Route Master Table.
** Data from $DATADIR/AIN_ROUTE_MASTER.DAT is loaded into this table.
** Size (Number of Records) in this Memory Table is decided at run-time.
** This table is used to find (Binary Search) existing of Route_Id.
** Input data record format is :
**  Field No.   Field Name
**  =========   ==========================
**  1.      Record_Type
**  2.      Route_Id
**  3.      Route_Name
**  4.      Oper_Carrier_Id
**  5.      Cost_Code_Mapping
**  6.      Service_Type
**  7.      Effective_DTM
**  8.      Expier_DTM
**  9.      SPC
**  10.     From_Gateway
**  11.     Mtx_Id
**  12.     Region_Id
**  13.     Upd_DTM
**  14.     Upd_By
** Remark: All Record Separated by "|" (Pipe Mark)
*/
typedef struct ain_route_master_table {
    char    *route_id;              /* Field 2 : Route Id */
    char    *oper_carrier_id;       /* Field 4 : Oper Carrier Id */
    char    *costcode_mapping;      /* Field 5 : Cost Code mapping */
    char    *mtx_id;                /* Field 11: MTX Id */
} AIN_ROUTE_MASTER_TABLE;
// AIN_ROUTE_MASTER_TABLE *glb_AinRouteMasterHead=NULL;        /* global pointer to AIN Route Master Table */

/* Introduce AIN CDR Type - Added below by Kawee on 29-Jan-2007 */
/*
** Structure of One Record in Memory AIN Subscriber Type Master Table.
** Data from $DATADIR/AIN_SUBSCRIBERTYPE_MASTER.DAT is loaded into this table.
** Size (Number of Records) in this Memory Table is decided at run-time.
** This table is used to find (Binary Search) existing of Subscriber_Prefix.
** Input data record format is :
**  Field No.   Field Name
**  =========   ==========================
**  1.      Record_Type
**  2.      Subscriber_Prefix
**  3.      Subscriber_Type
**  4.      Match_Length
**  5.      Subscriber_Desc
**  6.      Subscriber_Carrier
** Remark: All Record Separated by "|" (Pipe Mark)
*/
typedef struct ain_sub_type_master_table {
    char    *sub_carrier;           /* Field 6: Subscriber Carrier */
    char    *sub_prefix;            /* Field 2: Subscriber Prefix */
    char    *sub_type;          /* Field 3: Subscriber Type */
} AIN_SUB_TYPE_TABLE;
// AIN_SUB_TYPE_TABLE *glb_AinSubTypeHead=NULL;        /* global pointer to AIN Subscriber Type Table */

/* Introduce AIN CDR Type - Added below by Jetsada on 17-Sep-2007 */
/*
** Structure of One Record in Memory AIN E1 Mapping Table.
** Data from $DATADIR/AIN_E1_MAPPING.DAT is loaded into this table.
** Size (Number of Records) in this Memory Table is decided at run-time.
** This table is used to find (Binary Search) existing of Source_Name.
** Input data record format is :
**  Field No.   Field Name
**  =========   ==========================
**  1.      Record Type
**  2.      Source_Name
**  3.      Target_Name
**  4.      Effective_Dtm
**  5.      Expire_Dtm
**  6.      E1_Type
**  7.      Upd_Dtm
**  8.      Upd_By
** Remark: All Record Separated by "|" (Pipe Mark)
*/
typedef struct ain_e1_mapping_table {
    char    *source_name;           /* Field 2: Source Name */
    char    *target_name;           /* Field 3: Target Name */
} AIN_E1_MAPPING_TABLE;
// AIN_E1_MAPPING_TABLE *glb_AinE1MappingHead=NULL;        /* global pointer to AIN E1 Mapping Table */

/* Introduce AIN CDR Type - Added below by Thanakorn on 04-Mar-2010 */
/*
** Structure of One Record in Memory AIN Basic Time Rate Table.
** Data from $DATADIR/AIN_BASIC_TIME_RATE.DAT is loaded into this table.
** Size (Number of Records) in this Memory Table is decided at run-time.
** This table is used to find (Binary Search) existing of Source_Name.
** Input data record format is :
**  Field No.   Field Name
**  =========   ==========================
**  1           Record_id       - User Only  Record  '02'
**  2           TIME_PLAN_ID    - group of time rate Ex.0001  ***
**                                  Prefix  Dial-number  '0050', '00500'  -> Fix '0001'
**                                  Prefix Dial-number '005'  -> Fix '0003'
**  3           TIME_PLAN_DESC
**  4           TIME_RATE_ID    - 0001:WeekDay, 0002:WeekEnd  ***
**  5           TIME_RATE_NAME  - WeekDay, WeekEnd
**  6           START_DATE
**  7           END_DATE
**  8           START_DAY       - Fix 'ALL' - Everyday
**                                  MON - Monday
**                                  TUE - Tuesday
**                                  WED - Wednesday
**                                  THU - Thursday
**                                  FRI - Friday
**                                  SAT - Saturday
**                                  SUN - Sunday
**  9           START_TIME      - Format HH24:MI:SS
**  10          END_TIME        - Format HH24:MI:SS
**  11          EFFECTIVE_DTM   - DATE
**  12          EXPIRE_DTM      - DATE
**  13          UPD_DTM         - DATE
**  14          UPD_BY
** Remark: All Record Separated by "|" (Pipe Mark)
*/
typedef struct ain_basic_time_rate_table {
    char    *start_day;             /* Field 8 : Start Day */
    char    *time_plan_id;          /* Field 2 : Time Plan ID */
    char    *start_date;            /* Field 6 : Start Date */
    char    *end_date;              /* Field 7 : End Date */
    char    *start_time;            /* Field 9 : Start Time */
    char    *end_time;              /* Field 10: End Time */
    char    *time_rate_id;          /* Filed 4 : Time Rate ID (output) */
} AIN_BASIC_TIME_RATE_TABLE;
// AIN_BASIC_TIME_RATE_TABLE *glb_AinBasicTimeRateHead=NULL;   /* global pointer to AIN Basic Time Rate Table */

/*
** Structure of One Record in WLAN PACKAGE Table.
** Data from $DATADIR/WLAN_PACKAGE.DAT is loaded into this table.
** Size (Number of Records) in this WLAN PACKAGE Table is decided at run-time.
** This table is used to find (Binary Search) existing of WLAN PACKAGE.
** Input data record format is :
**  Field No.   Field Name
**  =========   ==========================
**  1.      Access_Point_Name
** Remark: All Record Separated by "|" (Pipe Mark)
*/
typedef struct wlan_package_table {
    char    *apn;           /* Field 1: Access Point Name */
} WLAN_PACKAGE_TABLE;
// WLAN_PACKAGE_TABLE *glb_WlanPackageHead=NULL;           /* global pointer to WLAN_PACKAGE Table */

/*
** Structure of One Record in International Operator Direct Connection Table.
** Data from $DATADIR/IODC.DAT is loaded into this table.
** Size (Number of Records) in this International Operator Direct Connection Table is decided at run-time.
** This table is used to find (Binary Search) existing of IODC Access Code.
** Input data record format is :
**  Field No.   Field Name
**  =========   ==========================
**  1.      IODC Access Code
**  2.      Remark
** Remark: All Record Separated by "|" (Pipe Mark)
*/
typedef struct iodc_table {
    char    *iodcacc_code;          /* Field 1: IODC Access Code */
} IODC_TABLE;
// IODC_TABLE *glb_IodcHead=NULL;          /* global pointer to IODC Table */

/*
** Structure of One Record in IMSI Mapping Table.
** Data from $DATADIR/IMSI.DAT is loaded into this table.
** Size (Number of Records) in this IMSI Mapping Table is decided at run-time.
** This table is used to find (Binary Search) existing of IMSI.
** Input data record format is :
**  Field No.   Field Name
**  =========   ==========================
**  1.      IMSI
**  2.      Mobile Number
** Remark: All Record Separated by "|" (Pipe Mark)
*/
typedef struct imsi_table {
    char    *imsi;              /* Field 1: IMSI */
    char    *mobile_no;         /* Field 2: Mobile Number */
} IMSI_TABLE;
// IMSI_TABLE *glb_ImsiHead=NULL;          /* global pointer to IMSI Table */

/*
** Structure of One Record in IMSI VSO Mapping Table.
** Data from $DATADIR/IMSI_VSO.DAT is loaded into this table.
** Size (Number of Records) in this IMSI VSO Mapping Table is decided at run-time.
** This table is used to find (Binary Search) existing of IMSI.
** Input data record format is :
**  Field No.   Field Name
**  =========   ==========================
**  1.      IMSI
** Remark: All Record Separated by "|" (Pipe Mark)
*/
typedef struct imsi_vso_table {
    char    *imsi;              /* Field 1: IMSI Prefix */
} IMSI_VSO_TABLE;
// IMSI_VSO_TABLE *glb_ImsiVsoHead=NULL;       /* global pointer to IMSI VSO Table */

/*
** Structure of One Record in FIX RATE Mapping Table.
** Data from $DATADIR/FIX_RATE.DAT is loaded into this table.
** Size (Number of Records) in this FIX RATE Mapping Table is decided at run-time.
** This table is used to find (Binary Search) existing of Network.
** Input data record format is :
**  Field No.   Field Name
**  =========   ==========================
**  1.      Network (AIS,OTH)
**  2.      Charge Rate in Satang Unit
** Remark: All Record Separated by "|" (Pipe Mark)
*/
typedef struct fix_rate_table {
    char    *network;           /* Field 1: Network */
    char    *charge_rate;           /* Field 2: Charge Rate in Satang Unit */
} FIX_RATE_TABLE;
// FIX_RATE_TABLE *glb_FixRateHead=NULL;       /* global pointer to FIX RATE Table */

/*
** Added Condition check for reject SMC CDR that MD generate duplicate record with FDA CDR
** - by Wiraphot on 07-Jan-2009
*/
/*
** Structure of One Record in REJ ORIG ADDR Table.
** Data from $DATADIR/REJ_ORIG_ADDR.DAT is loaded into this table.
** Size (Number of Records) in this REJ ORIG ADDR Table is decided at run-time.
** This table is used to find (Binary Search) existing of REJ ORIG ADDR.
** Input data record format is :
**      Field No.       Field Name
**      =========       ==========================
**      1.              Originate addr
** Remark: All Record Separated by "|" (Pipe Mark)
*/
typedef struct rej_orig_addr_table {
    char    *orig_addr;                             /* Field 1: Originate Address */
} REJ_ORIG_ADDR_TABLE;
// REJ_ORIG_ADDR_TABLE *glb_RejOrigAddrHead=NULL;      /* global pointer to REJ_ORIG_ADDR Table */

// -----

/*
** NRTRDE SCP Pricing table (GEO_PLMN_CELLAREA.DAT)
** Input data record format is:
**      Field No.       Field Name
**      =========       ==========================
**      1.              PLMN Code
**      2.              Cell Area
**      3.              Carrer Code
**      4.              Country Code
**      5.              MOC CHR Local
**      6.              MOC CHR To Thailand
**      7.              MOC CHR INTER
**      8.              MTC CHR
**      9.              SMSO CHR
**      10.             GMT offset
**      11.             GPRS CHR
**      12.             GPRS min
**      13.             GPRS round
*/
#define CHR_MOC_LOCAL       1
#define CHR_MOC_TO_TH       2
#define CHR_MOC_INTER       3
#define CHR_MTC             4
#define CHR_SMSO            5
#define CHR_GPRS            6

typedef struct plmn_code_table {
    char plmn[5+1];
    char cell_area[5+1];
    char carrier_code[5+1];
    char country_code[4+1];
    double  n_chr_moc_local;
    double  n_chr_moc_to_th;
    double  n_chr_moc_inter;
    double  n_chr_mtc;
    double  n_chr_smso;
    double  n_utc_offset;
    double  n_chr_gprs;
    double  n_gprs_min;
    double  n_gprs_rnd;
} PLMN_CODE_TABLE;

// PLMN_CODE_TABLE *glb_PlmnCodeHead = NULL;
// PLMN_CODE_TABLE *glb_PlmnPriceHead = NULL;
// PLMN_CODE_TABLE *glb_PlmnCountryHead = NULL;

/*
** Structure of One Record in APN Mapping Table.
** Data from $DATADIR/APN.DAT is loaded into this table.
** Size (Number of Records) in this APN Mapping Table is decided at run-time.
** This table is used to find (Binary Search) existing of Network.
** Input data record format is :
**  Field No.   Field Name
**  =========   ==========================
**  1.          RecordType (18)
**  2.          APN
** Remark: All Record Separated by "|" (Pipe Mark)
*/
typedef struct apn_table {
    char *recordtype;           /* Field 1: recordtype */
    char *apn;                  /* Field 2: access point name */
} APN_TABLE;
// APN_TABLE *glb_ApnHead=NULL;    /* global pointer to APN Table */

/*
** Structure of Toll Free Mapping Table
*/
typedef struct toll_free_table {
    char   *szToll_Free;
    char   *szDescName;
} TOLLFREE_TABLE;
// TOLLFREE_TABLE *glb_TollFreeHead=NULL;  /* global pointer to Toll Free Table */

/*
** Structure of One Love Mapping Table
*/
typedef struct one_love_table {
    char *szAno;
    char *szBno;
} ONE_LOVE_TABLE;
// ONE_LOVE_TABLE *glb_OneLoveHead=NULL;   /* global pointer to One Love Table */
/*
**  Structure of One Record in IMEI for BOS subscriber Table.
**  Field No.   Field Name
**  =========   ==========================
**  1.          IMEI
**  Remark: All Record Separated by "|" (Pipe Mark)
*/
typedef struct bos_imei_table {
    char    *szImei;           /* Field 1: IMEI */
} BOS_IMEI_TABLE;
// BOS_IMEI_TABLE *glb_BosImeiHead=NULL;           /* global pointer to IMEI Table */


/*
**  Structure of Costcode Bno Table.
**  Field No.   Field Name
**  =========   ==========================
**  1.          Prefix Bno
**  2.          CDR Prefix Bno ( after convert bno )
**  3.          Cost code
**  Remark: All Record Separated by "|" (Pipe Mark)
**  Comment character is #
*/
typedef struct costcode_bno_table {
    char *szBnoPrefix;
    char *szCostcode;
} COSTCODE_BNO_TABLE;
// COSTCODE_BNO_TABLE *glb_CostcodeBNoHead=NULL;   /* global pointer to Costcode Bno Table */


// ----- Added USMAP mapping for VAS event by Thanakorn on Sep-2016 -----
// Structure of a record for usmap.
// Data from $DATADIR/USMAP.DAT is loaded into this table structure.
// Size (Number of Records) in this Memory Table is decided at run-time.
// Input data record format is:
// No.      Field Name
// =====    =============================
// 1        RECORD_TYPE
// 2        REFERENCE_ID
// 3      **SERVICE_ID
// 4      **CATEGORY_ID
// 5      **CHARGING_CP_ID
// 6        BEARER
// 7        VAS_GROUP
// 8        VAS_TYPE
// 9        PRICE
// 10       CHARGING_DESCRIPTION
// 11       CONFIRMED_DATE
// 12       GSM_ADVANCE
// 13       ONE_2_CALL
// 14       GSM_1800
// 15     **CCT
// 16       STATUS
// 17       LAST_UPDATE
// 18       AWN_POSTPAID
// 19       AWN_PREPAID
// 20       SERVICE_CATEGORY
// 21       SERVICE_MODEL
// 22       PKG_ID
// 23       BUSINESS_CODE
// 24       BUSINESS_GROUP_ID
// 25       DR_TYPE_ID
// 26       NAME
// 27       INCL_TAX
// 28       DESCRIPTION
// 29       SERVICE_NUMBER
// 30       REVENUE_SHARE_OPERATOR
// 31       REVENUE_SHARE_PARTNER
// 32      *COMPANY_NAME
// 33       PER_TRANSACTION
// 34       PER_PACKAGE
// 35       PER_MONTH
// 36       OTHER
// 37      *SERVICE_NAME
// 38       CURRENT_CALL_CHARGE_GROUP
// 39       TENTATIVELAUNCHDTM
// 40       DTUPDATE
// 41       CSEQ
// 42       CFILENAME
// Remark       : All Record Separated by "|" (Pipe Mark)
// Key Field**  : 3, 4, 5, 15
// Value Field* : 32, 37
typedef struct usmap_table {
    char    *service_id;        // From field 3rd as key
    char    *category_id;       // From field 4th as key
    char    *charging_cp_id;    // From field 5th as key
    char    *cct;               // From field 15th as key
    char    *company_name;      // From field 32nd as value
    char    *service_name;      // From field 37th as value
} USMAP_TABLE;
// USMAP_TABLE *glb_UsMapHead=NULL;    // global pointer to USMAP Table

typedef struct geo_table {
    char    *geo_cgi;           // From field 1st as key
    char    *geo_cell_id;       // From field 2nd as value
    char    *geo_cell_set;      // From field 3rd as value
    char    *geo_cell_area;     // From field 4th as value
} GEO_TABLE;

typedef struct cellarea_table {
    char *en_province;
    char *geo_cell_area;
} CELLAREA_TABLE;

typedef struct vasgrp_table {
    char    *cost_code_type;
    char    *grp_name;
    char    *grp_code;
} VASGROUP_TABLE;
/*-----------------------------------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------------------------------------*/

/*
** Prototypes of Functions native to cdr_mapping.c
*/
int     Init_MapdStat();
int     Check_MapdStat(int *);
int     Load_MapdTables();
void    Release_MapdTables();

int     Load_MnetTable (int);       /* Common Functions for AIS and DPC */
int     Cmp_Npa ();
/* Changed as below to support NPBN - by Kawee on 14-Jun-2004 */
int     Get_Network (char *, char *, char *, char *);
int     Get_Net (char *, char *);
int     Cmp_GivenNpa ();

int     Load_NportTable (int);
int     Cmp_Pnumber (const void *, const void *);
int     Get_NetPort (char *, char *);

int     Load_MsrnTable (int);
int     Get_MsrnCell (char *, char *, char *);

int     Load_ZoneTable (int);       /* Common Functions for AIS and DPC */
int     Cmp_NetCell (const void *, const void *);
int     Get_ZoneId (char *, char *, char *);
int     Parse_ZoneId (char *, char *, char *, char *);

int     Load_ZmapTable (int);           /* Common Functions for AIS and DPC */
int     Cmp_NetZoneAB (const void *, const void *);
int     Get_ZoneType (char *, char *, char *, char *);

int     Load_NatlRate (int);        /* Common Functions for AIS and DPC */
int     Get_NatlRate (char *, double *);
int     Parse_Area (char *, char *);
void    Get_DayNightRate (char *, time_t, double *);

int     Load_IntlRate (int);
int     Cmp_Idd (const void *, const void *);
int     Parse_Country (char *, char *, double *);

int     Load_CmapTable (int);
int     Get_NmtCellInfo (char *, char *, char *);

int     Load_SrateTable (int);
int     Get_ServiceRate (char *, char *, char, char *, double *, double *);

int     Load_FmapTable (int);
int     Get_LddCell (char *, char *, char *, char *);
int     Parse_LddCell (char *, char *, char *, char *);

int     Load_CustTariffTable (int);
int     Cmp_TariffPNo (const void *, const void *);
int     Get_Tariff (char *, char *);

/* Added VAS Table - on 13-Jan-2003 by Kawee */
int     Load_VasTable (int);
int     Cmp_VasKey (const void *, const void *);
int     Cmp_Vas (const void *, const void *);
int     Get_CostBandId (char, char *, char *);

int     Load_TariffRateTable (int);         /* Exclusive function for DPC */
int     Cmp_TariffKey (const void *, const void *);
int     Get_DpcTariffRate (char *, char *, char *, char *, double *);

/* Added for Temporary Phone Number Table - on 25-Apr-2002 by Kawee */
int     Load_TempPhoneTable (int);
int     Cmp_TempPhone (const void *, const void *);
int     Check_TempPhone (char *);

/* Added Number port by Nature Table - by Kawee on 14-Jun-2004 */
int     Load_NpbnTable (int);
int     Cmp_Npbn (const void *, const void *);
int     Get_NpbnPort (char *, char *, char *);

/* Introduce Adjacent Area. Added Bill Register Table - by  Kawee on 18-Nov-2004 */
int     Load_BillRegTable (int);
int     Cmp_BillReg (const void *, const void *);
int     Get_BillProvince (char *, char *);

/* Introduce Adjacent Area. Added Adjacent Group Table - by  Kawee on 18-Nov-2004 */
int     Load_AdjGrpTable (int);
int     Cmp_AdjGrp (const void *, const void *);
int     Get_AdjGrp (char *, char *);

/* Introduce Adjacent Area. Added Bill Area Table - by  Kawee on 18-Nov-2004 */
int     Load_BillAreaTable (int);
int     Cmp_BillArea (const void *, const void *);
int     Get_BillArea (char *, char *);
int     Validate_AdjArea (char *, char *, int *);

/* Introduce CDG CDR Type. Added Bearer Table - by Kawee on 02-Jun-2005 */
int     Load_BearerTable (int Idx);
int     Cmp_Bearer (const void *ptr1, const void *ptr2);
int     Get_Bearer (char *pchSgwid, char *pchBearer);

/* Introduce CDG CDR Type. Added Cost Code Table - by Kawee on 02-Jun-2005 */
int     Cmp_CostCodeTable (const void *, const void *);
int     Cmp_CostCode (const void *, const void *);
int     Get_RmCostBandIdExact (char *, char *, char *, char *);
int     Get_RmCostGroupIdExact (char *, char *, char *, char *);
int     Get_RmCostBandId (char *, char *, char *);
int     Get_RmCostGroupId2 (char *, char *, char *, char *);
/* Add Vas_Name field for USC and CDG Event Type - Changed as below by Kawee on 12-Mar-2007 */
int     Get_RmCostBandId2 (char *, char *, char *, char *);
int     Get_RmCostGroupId (char *, char *, char *);
int     Get_Network_New(char *, char *);
int     Get_RmNetwork(char *, const char *, char *, char *, char *);

/* Introduce CDG CDR Type. Added Cost Code Table - by Kawee on 02-Jun-2005 */
int     Load_CostBandTable (int);
int     Cmp_CostBandId (const void *, const void *);
int     Parse_CostBandId (char *, char *, char *);

/* Introduce CDG CDR Type. Added Cost Code Table - by Kawee on 02-Jun-2005 */
int     Load_RatingTariffTable (int);
int     Cmp_RatingTariffKey (const void *, const void *);
int     Parse_RatingTariff (char *, char *, char *);

/* Introduce CDG CDR Type. Added Cost Code Table - by Kawee on 02-Jun-2005 */
int     Cmp_RatingElementKey  (const void *, const void *);
int     Cmp_RatingElementKey2 (const void *, const void *);
int     Cmp_RatingElementKey3 (const void *, const void *);
int     Cmp_RatingElementKey4 (const void *, const void *); /* Introduce AIN - Added by Kawee on 04-Jan-2007 */
int     Parse_RatingElement (char *, char *, char *, char *, char *, char *, char *);
int     Parse_RatingElement2 (char *, char *, char *, char *);
int     Parse_RatingElement3 (char *, char *, char *, char *, char *);
int     Parse_RatingElement3Avg (char *, char *, char *, char *, char *);
int     Parse_RatingElement4 (char *, char *, char *, char *, char *, char *, char *);  /* Introduce AIN - Added by Kawee on 04-Jan-2007 */

/* Added Costgroup Xref Table - by Jetsada on 21-Jul-2005 */
int     Cmp_CostgroupXref (const void *, const void *);
int     Get_XfCostBandId (char *, char *, char *, char *);

/* Added Service Key Table - by Jetsada on 21-Jul-2005 */
int     Cmp_ServiceKey (const void *, const void *);
/* void Release_ServiceKeyTable (); */ /* Removed by Kawee on 22-Jun-2007 */
int     Get_Service (char *, char *, char *, char *);

/* Added PPS Desc Table - by Jetsada on 15-Aug-2005 */
int     Cmp_PpsDesc (const void *, const void *);
int     Parse_PpsDesc (char *, char *, char *, char *);

/* Added AIN Cost Code Table - by Kawee on 29-Jan-2007 */
int     Cmp_AinCostCodeKey  (const void *, const void *);
int     Cmp_AinCostCodeKey2 (const void *, const void *);                   /* Add by Thanakorn on 05-Mar-2010 */
int     Parse_AinCostCodeExact (char *, char *, char *);
int     Parse_AinCostCode (char *, char *, char *);
int     Parse_AinCostCodeExact2 (char *, char *, char *, char *);       /* Add by Thanakorn on 05-Mar-2010 */
int     Parse_AinCostCode2 (char *, char *, char *, char *);            /* Add by Thanakorn on 05-Mar-2010 */

/* Added AIN Operator Carrier Master Table - by Kawee on 29-Jan-2007 */
int     Cmp_AinOperCarrierMasterKey (const void *, const void *);
int     Get_AinOperSingleBillFlag (char *, char *);

/* Added AIN Rating Tariff Element Table - by Kawee on 29-Jan-2007 */
int     Cmp_AinRatingTariffElementTable (const void *, const void *);
int     Cmp_AinRatingTariffElementKey (const void *, const void *);
int     Cmp_AinRatingTariffElementKey2 (const void *ptr1, const void *ptr2);        /* Add by Thanakorn on 05-Mar-2010 */
int     Parse_AinRatingTariffElement (char *, char *, char *, char *);
int     Parse_AinRatingTariffElement2(char *rating_tariff_id, char *cost_band_id, char *modifier_class_id, char *time_plan_id, char *time_rate_id, char *step_group, char *step_seq, char *subs_type, char *charge_rate);  /* Add by Thanakorn on 05-Mar-2010 */


/* Added AIN Route Master Table - by Kawee on 29-Jan-2007 */
int     Cmp_AinRouteMaster (const void *, const void *);
int     Parse_AinRouteMaster (char *, char *, char *, char *);

/* Added AIN Subscriber Type Master Table - by Kawee on 29-Jan-2007 */
int     Cmp_AinSubTypeMasterKey (const void *, const void *);
int     Parse_AinSubTypeExact (char *, char *, char *);
int     Parse_AinSubType (char *, char *, char *);

/* Added AIN E1 Mapping Table - by Jetsada on 17-Sep-2007 */
int     Cmp_AinE1MappingKey (const void *, const void *);
int     Parse_AinE1Mapping (char *, char *);

/* Added AIN Basic Time Rate Table - by Thanakorn on 04-Mar-2010 */
int     Cmp_AinBasicTimeRate(const void *, const void *);
int     Parse_AinBasicTimeRate(char *start_day, char *time_plan_id, char *evt_dt, char *time_rate_id);

/* Added WLAN PACKAGE Table - by Jetsada on 10-Oct-2007 */
int     Cmp_WlanPackageKey (const void *, const void *);
int     Is_WlanPackage (char *);

/* Added Check for Reject Record that exist in IMSI VSO Table - by Kawee on 15-Aug-2008 */
int     Cmp_ImsiVsoKey (const void *, const void *);
int     Lookup_ImsiVsoExact (char *);
int     Lookup_ImsiVso (char *);

/* Added FIX RATE mapping table - by Kawee on 19-Sep-2008 */
int     Cmp_FixRateKey (const void *, const void *);
int     Get_FixRate (char *,char *);

/* Added REJ ORIG ADDR mapping table - by Wiraphot on 07-Jan-2009 */
int     Cmp_RejOrigAddr(const void *, const void *);
int     Is_RejOrigAddr(char *);

/* Added APN mapping table - by Watthikorn on 24-May-2010 */
int     Cmp_Apn(const void *, const void *);
int     Parse_Apn(char *, char *);

int     Load_VariableLengthTable(int);

/* Added by Thanakorn */
int     Load_Plmn(int);
int     preproc_avg_price_carrier(int nrec);
int     preproc_avg_price_plmn(int nrec);
int     Cmp_Plmn(const void*, const void*);
int     Cmp_Carrier(const void*, const void*);
int     Cmp_CountryCode(const void*, const void*);
int     Get_CellAreaFromCarrier(char *carrier_code, char *cell_area, char *plmn, char *country_code);
int     Get_CarrierPrice(char *carrier_code, int chr_type, double *price);
int     Get_CarrierPriceExact(char *carrier_code, int chr_type, double *price);
int     Get_CarrierPriceGprs(char *carrier_code, double *price, double *gprs_min, double *gprs_rnd);
int     Get_CarrierPriceGprsExact(char *carrier_code, double *price, double *gprs_min, double *gprs_rnd);
int     Get_PlmnPrice(char *plmn, int chr_type, double *price);
int     Get_PlmnPriceExact(char *plmn, int chr_type, double *price);
int     Get_PlmnPriceGprs(char *plmn, double *price, double *gprs_min, double *gprs_rnd);
int     Get_PlmnPriceGprsExact(char *plmn, double *price, double *gprs_min, double *gprs_rnd);
int     Get_CellArea(const char *plmn_code, char *cell_area, char *country_code);
int     Get_CountryCode(char *country_code, int chr_type, double *price, char *plmn, char *cell_area);

/* Added by Thanakorn */
int     Cmp_3G(const void*, const void*);
int     Get_SvcType(char *speed_type, char *rat, char *srv_pack_id, char *bearer, double *rating, char *svc_type);

/* Added by Thanakorn on Nov-2012 */
int     Cmp_TollFree(const void*, const void*);
int     Get_TollFreeExact(char *b_no, char *desc);
int     Is_TollFree(char *free_num, char *desc);

/* Added by Thanakorn on Sep-2013 */
int     Cmp_OneLoveNum(const void*, const void*);
int     Is_OneLoveNum(char *a_no, char *b_no);

/* Added IMEI Table - by Thanakorn on 13-Nov-2013  (FMS-IMEI-11112013) */
int     Cmp_Imei(const void *, const void *);
int     Is_BosImei(char *);

/* Added by Thanakorn on 23-Apr-2014 */
int     Cmp_CostcodeBno(const void*, const void*);
int     Get_CostcodeBnoExact(char *b_no, char *costcode);
int     Get_CostcodeBno(char *b_no, char *costcode);
int     Lookup_Costcode (char *banding_model_id, char *costcode, char *costgroup_id, char *cost_band_id, char *vas_name);

// Added by Thanakorn on Sep-2016
int     Cmp_UsMapTable(const void *ptr1, const void *ptr2);
int     Get_UsMapData(char *svc_id, char *cat_id, char *cp_id, char *cct, char *company_name, char *service_name);

// Added by Thanakorn on Mar-2020
int     Cmp_GeoTable(const void *ptr1, const void *ptr2);
int     Get_GeoDataExact(char *cgi, char *cell_id, char *cell_set, char *cell_area);
int     Get_GeoData(char *cgi, char *cell_id, char *cell_set, char *cell_area);

int     Cmp_CellAreaTable(const void *ptr1, const void *ptr2);
int     Get_CellAreaProv(char *en_province, char *cell_area);

int     Cmp_VasGroupTable(const void *ptr1, const void *ptr2);
int     Get_VasGroupCodeByGrpName(char *grp_name, char *grp_code);
int     Get_VasGroupCode(char *cost_code_type, char *grp_name, char *grp_code);

/*------------------------------------------------------------------------------------*/

int     Skip_Gsm_Header();
int     Read_Gsm_Cdr();
int     Gsm_To_Sms_Event();         /* SMS CDRs mapping */ /* Added by Jetsada on 16-Aug-2005 */
int     Gsm_To_Voice_Event();       /* Voice CDRs mapping */ /* Migrated CAMEL V.5 - Added by Kawee on 04-Jun-2007 */
int     Gsm_To_Event();             /* GSM CDRs mapping */ /* Migrated CAMEL V.5 - Added by Kawee on 04-Jun-2007 */
int     Read_OcsVpn_Cdr();          /* Added by Thanakorn on 18-Nov-2014, support Maritime MT */

/* Split SMS Type from GSM Type - Added SMS functions - by Kawee on 07-Apr-2005 */
int     Read_Sms_Cdr ();
int     Read_Smc_Cdr ();

/* Added GPRS functions - by kawin on 19-Sep-2002 */
int     Read_Gprs_Cdr ();
/* Migrated to function as below for HP FMS V8.1-1 - by Kawee on 08-Jul-2003 */
int     Gprs_To_Data_Event ();
int     Gprs_To_Data_Event_18 ();
int     DpcGprs_To_Data_Event ();           /* Added by Thanakorn on 19-Feb-2011 */
int     Gprst_To_Data_Event ();
int     Gprst_To_Data_Event_Bcudr ();
int     Gprst_To_IR_Event();

int     Read_Gprst_Cdr();
int     Read_Mms_Cdr();
int     Read_GsmTopUp_Cdr();

int     DpcGsm_To_Event();
int     GsmTopUp_To_Event();


int     Sms_To_Sms_Event ();
int     Smc_To_Sms_Event ();

int     Mms_To_Mms_Event ();
int     Mms_To_Mms_Event_Tcudr ();

int     Read_Ers_Cdr ();

int     Read_Ssp_Cdr ();
int     Ssp_To_Voice_Event ();
int     CommonVoice_To_Voice_Event (char *, VOICE_RECORD *, int, char *);   /* Migrated CAMEL V.5 - Added by Kawee on 04-Jun-2007 */

/* Added Delimiter functions - by Kawee on 02-Jun-2005 */
int     Read_Delimiter_Cdr(char, int, char **);
int     Read_Cdg_Cdr();
int     Cdg_To_Cdg_Event();
int     Cdg_To_Cdg_Event_Ctudr();
int     Cdg_To_Dcb_Event();

int     Read_Rbt_Cdr ();
int     Rbt_To_Rbt_Event ();

/* Added MSC PPS functions - by Kawee on 16-Dec-2002 */
int     Read_MscPps_Cdr ();

int     Read_Usc_Cdr ();
int     Usc_To_Usc_Event ();

int     Read_Cdg_Cdr ();
int     Cdg_To_Cdg_Event ();

int     Read_Rbt_Cdr ();
int     Rbt_To_Rbt_Event ();

int     Read_Ain_Cdr ();
int     Ain_To_Voice_Event ();

int     Read_Ccudr_Cdr ();
int     Ccudr_To_Session_Event ();

int     Read_DpcGsm_Cdr ();
int     DpcGsm_To_Voice_Event ();           /* Migrated CAMEL V.5 - Added below by Kawee on 04-Jun-2007 */
int     DpcGsm_To_Sms_Event ();

int     Read_Nrtrde_Cdr ();
int     Nrtrde_To_IR_Event ();

int     Read_IrScp_Cdr ();
int     IrScp_To_IR_Event ();               /* Added by Thanakorn on 24-Sep-2012, Support FMS V11 */

int     Read_Awn_Cdr ();
int     Awn_To_Event ();

int     Read_Amf_Cdr ();                    /* Added by Thanakorn on 04-Aug-2011 */
int     Amf_To_Event ();                    /* Added by Thanakorn on 04-Aug-2011 */

int     Read_Sdg_Cdr ();                    /* Added by Thanakorn on 01-Mar-2012 */
int     Sdg_To_Event ();                    /* Added by Thanakorn on 01-Mar-2012 */

int     Read_Ims_Cdr ();                    /* Added by Thanakorn on 28-Sep-2015 */
int     Ims_To_Voice_Event ();              /* Added by Thanakorn on 28-Sep-2015 */
int     Ims_To_Fix_Event (int cat, int feat, char *detect);     /* Added by Thanakorn on 12-Jan-2016 */

int     Read_Rdc_Cdr ();                    /* Added by Thanakorn on 22-Jan-2016 */
int     Rdc_To_Voice_Event ();               /* Added by Thanakorn on 22-Jan-2016 */

int     Read_RdcSms_Cdr ();                 /* Added by Thanakorn on 22-Jan-2016 */
int     Rdc_To_Sms_Event ();                /* Added by Thanakorn on 22-Jan-2016 */

int     Read_Rbm_Cdr();
int     Rbm_To_Voice_Event();


int     CommonVas_To_Vas_Event(int iEvtType, void *event_buf);     /* Added by Thanakorn on Sep-2012, FMS V11 */

unsigned int    Conv_Duration (char *);
int     Conv_DuraFormat (const char*, char*);           /* 20 NOV 2001 by Kawee */

time_t  Calc_SeizeTime (char *, char *);
char    *localtime_YYYYMMDDHHMMSS (const time_t *timer);
char    *gmtime_YYYYMMDDHHMMSS (const time_t *timer);
char    *getLocalGMT_byTimeDiff(time_t home_time, time_t local_time, float home_gmt);   // Added by Thanakorn on Sep-2016
double  Conv_UTCOffset (char *);
double  Calc_IntlCharge (unsigned int, double);
double  Calc_AinIntlCharge (unsigned int, double);
double  Calc_AinIntlCharge2(unsigned int, double);
double  Calc_NatlCharge (unsigned int, double);
double  Calc_DpcNatlCharge (unsigned int, double);
void    Trim_Str(char *);
void    Pad_Str (char *, int);
void    Scan_DialDigit (char *);
int     Scan_ForAlpha (const char *);
int     Is_Number (char *);
int     Is_LeapYear(int);                               /* Added by Kawee on 02-Feb-2007 */
int     IsValid_YYYYMMDDHHMMSS (char *);                /* Added by Kawee on 02-Feb-2007 */
int     IsValid_YYYYMMDD_HHMMSS (char *, char *);       /* Added by Kawee on 04-Jun-2007 */
void    StrToUpper(char *src, int s_size, char *dest);


int     Write_Event_Record ();
int     Wrt_Exn_Data();                         /* Write_Alm_Data is now Wrt_Exn_Data */

char    Is_IntlBno(char *);

int     Conv_Gsm_Ano ();
int     Conv_Gsm_Bno ();
int     Conv_Gprs_Ano ();                       /* Added for GPRS functions - by kawin on 19-Sep-2002 */
int     Conv_Gprst_Ano ();                      /* Added for GPRST functions - by Kawee on 20-May-2004 */
int     Conv_DpcGsm_Ano ();
int     Conv_DpcGsm_Bno ();
int     Conv_Sms_Bno ();
int     Conv_Smc_Bno ();
int     Conv_Ano (char *, char *);              /* Added Generic Mapping A_no function - by Kawee on 05-Nov-2004 */
int     Conv_Ain_Ano (char *, char *);          /* Introduce AIN CDR Type - Added by Kawee on 29-Jan-2007 */
int     Conv_CommonVoice_Bno (char *, char *);  /* Added Generic Mapping Voice B_no function - by Kawee on 06-Jun-2007 */
int     Conv_CommonSms_Bno (char *, char *);    /* Added Generic Mapping SMS B_no function - by Kawee on 25-Apr-2006 */
void    Conv_RealBno(const char *, char *);     /* Get only real bno (without access code or dialed digit) - by Thanakorn on Sep-2016 */

int     Is_ExpressLink (char *);
int     Is_VasNumber (char *);
void    Set_VasFeature (char *);
int     Is_DpcVasNumber (char *);
int     Is_DpcVoiceMail (char *);
int     Is_IodcNumber (char *iodcacc_code);
int     Is_IodcNumberExact (char *iodcacc_code);
long    bcudrCorrectVol (const char *, const char *);
char    *Get_StrTok(char *, const char);
long    Calc_Duration (char *, char *, char *, char *);
void    Trim_ZeroLeading(char *, const char *, int);
char    *RemoveChar(char *data, int size, char c);

/***** Temporary addtion *****/
int     Determine_Usc_Size ();
void    determine_size(char *, int, const char *, int *, int, int);
void    determine_num_field(char *, int, const char *, int *, int, int);
void    Determine_Cdr_Size(int);

int     Split_Pps_Cdr(char *wrtn_rec);      // Added by Thanakorn on 07-Mar-2013
void    Close_Split_Pps();                  // Added by Thanakorn on 07-Mar-2013

char    *decTo64(char *s_decimal);          // Added by Thanakorn on 28-Sep-2015, to convert 4G cell to fit 5 charactered cell
char    *reverseStr(char *str);             // Added by Thanakorn on 28-Sep-2015
void    strrcpy(char *dest, char *src, int num);    // Added by Thanakorn on Sep-2016

int     glb_UscNew;         /* Status Flag to Indicate USC CDR file is New Size or Old Size */
int     glb_SmsNew;
int     glb_SmcNew;
int     glb_GprsNew;
int     glb_BcNewField;
int     glb_TcNewField;
int     glb_CcNewField;
int     glb_CtNewField;

#define NEW_FORMAT          1
#define OLD_FORMAT          0
/*****************************/

/* Added for Multiple Tariff Rate by Kawee on 15-Jan-2003 */
#define TARIFF_FREE         1
#define TARIFF_BASIC        2
#define TARIFF_INTRANET     3
#define TARIFF_EXTRANET     4
#define TARIFF_NATIONWIDE   5

/* Added Vas Type by Kawee on 17-Jan-2003 */
/* In Vas Table: Vas Type 1 -> Voice, Vas Type 3 -> SMS */
#define VAS_VOICE           '1'
#define VAS_SMS             '3'

#ifdef  __cplusplus
    }
#endif

#endif /* _FRM_CDR_MAPP_H_ */
