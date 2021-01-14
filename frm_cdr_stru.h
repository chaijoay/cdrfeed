//
//
// FACILITY    : CDR Feed Process
//
// FILE NAME   : frm_cdr_stru.h
//
// ABSTRACT    : Structure of all the CDR Formats. (Base HP FMS Ver 9.2-1)
//       Structure provided here are internal to CDR Feed Process only and they are not RAW format.
//       All the character array field size includes null byte size.
//       All the Date fields store date in YYYYMMDD format.
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

#ifndef _FRM_CDR_STRU_H_
#define _FRM_CDR_STRU_H_

#ifdef  __cplusplus
    extern "C" {
#endif

#define SKIP_DET_STR_SIZE       1024
#define SIZE_CELL_AREA          6
#define SIZE_CELL_SET           7
#define SIZE_CELL_ID            5

// standard length of character for fixed width field CDR (commonly used)
#define STDWID_ACT_CODE         1
#define STDWID_ANO              18
#define STDWID_BNO              32
#define STDWID_CAUS_TERM        10
#define STDWID_CELL_ID_A        15
#define STDWID_CELL_ID_B        15
#define STDWID_CHG_IND          1
#define STDWID_CHG_ORI          4
#define STDWID_CHNL_REQ         1
#define STDWID_CHNL_USE         1
#define STDWID_DATA_REF         6
#define STDWID_DATA_VOL         6
#define STDWID_DATE             8
#define STDWID_DURATION         6
#define STDWID_FCI              8
#define STDWID_IMEI             17
#define STDWID_IMEI_ABC         1
#define STDWID_IMSI             17
#define STDWID_IN_TGRP          1
#define STDWID_MSC_ID           15
#define STDWID_MS_CLS_MARK      1
#define STDWID_MS_ROAM_NO       17
#define STDWID_OPER_A           15
#define STDWID_OPER_B           15
#define STDWID_ORI_ANO          12
#define STDWID_OUT_TGRP         10
#define STDWID_REC_TYPE         2
#define STDWID_SP_NAME_MO       15
#define STDWID_SRV_CODE         2
#define STDWID_SRV_TYPE         1
#define STDWID_SS_CODE          2
#define STDWID_SS_EVT           4
#define STDWID_SUPP_CODE        2
#define STDWID_SVC_ID           4
#define STDWID_SVC_KEY          6
#define STDWID_SVC_PCK_ID       3
#define STDWID_SVC_TYPE         2
#define STDWID_TARIF_CLS        4
#define STDWID_TIME             6
#define STDWID_TRANS_IND        1

// standard length of character for fixed width field CDR (specific cdr type used)
#define WID_APN                 37
#define WID_AUTH_RSLT           9
#define WID_BEARER              1
#define WID_BEARER_ID           5
#define WID_BRAND_ID            15
#define WID_CALLED_NO           63
#define WID_CALL_FLG            1
#define WID_CALL_PARTY          21
#define WID_CALL_REF            12
#define WID_CAT_ID              4
#define WID_CAUSE               3
#define WID_CAUS_CLOS           2
#define WID_CDRN_ID             20
#define WID_CGRN_ID             20
#define WID_CELL_4G             19
#define WID_CELL_ID             15
#define WID_CF_ADDR             15
#define WID_CHG_AMNT            10
#define WID_CHG_FLG             1
#define WID_CHG_ID              10
#define WID_CHN_EVT_ID          10
#define WID_CID                 18
#define WID_CLLING_NO           15
#define WID_COMPLT              2
#define WID_CONT_CODE           20
#define WID_CONT_ID             3
#define WID_COS                 6
#define WID_COST                9
#define WID_COS_B               3
#define WID_CP_ID               3
#define WID_CUST_ID             15
#define WID_DATE_TIME           14
#define WID_DR_CAUSE            3
#define WID_DR_FEE              5
#define WID_DR_STOP             14
#define WID_ERF                 50
#define WID_EXCH_ID             15
#define WID_FILLER              14
#define WID_INF                 100
#define WID_INIT_RAT            3
#define WID_INIT_SPEED          10
#define WID_LCUN                15
#define WID_LEG                 1
#define WID_MSC_ADDR            20
#define WID_MSG_SIZE            8
#define WID_MSISDN              20
#define WID_MUS                 10
#define WID_NET_PRICE           10
#define WID_NID                 14
#define WID_NRF                 30
#define WID_OB                  1
#define WID_OBJ                 21
#define WID_OPER_ID             20
#define WID_ORI_ADDR            15
#define WID_OTON                20
#define WID_PDP_ADDR            15
#define WID_PDP_TYPE            4
#define WID_PKG_ID              10
#define WID_PKG_REF             15
#define WID_PLM_CODE            5
#define WID_PPF                 1
#define WID_PPS_DESC            2
#define WID_PROD_NO             10
#define WID_PROD_VAL            8
#define WID_PYMENT_TYPE         2
#define WID_QOS_DELAY           2
#define WID_QOS_MEAN            2
#define WID_QOS_PEAK            2
#define WID_QOS_PREC            2
#define WID_QOS_RELI            2
#define WID_RAT                 5
#define WID_REC_SEQ_NO          10
#define WID_ROAM_FLG            1
#define WID_RQR                 158
#define WID_RR_CAUSE            3
#define WID_RR_FEE              5
#define WID_RR_STOP             14
#define WID_SCOS                6
#define WID_SGSN_ADDR           15
#define WID_SGW_ID              3
#define WID_SID                 6
#define WID_SIZE                10
#define WID_SPEED               10
#define WID_SPEED_TYPE          4
#define WID_SP_NAME_MT          15
#define WID_SUBJ                21
#define WID_SVD_IMEI            17
#define WID_TBCF                1
#define WID_TELE_ID             5
#define WID_TI                  1
#define WID_TSID                18
#define WID_TXN_ID              2
#define WID_UNIT_CHG            2
#define WID_USR_LOC_INF         20
#define WID_VERSION             4
#define WID_VOLUME              10
#define WID_VOL_CHG             15
#define WID_VOL_COST            10
#define WID_VOL_UPDW            20
//extern char gFxLenField[SIZE_ITEM_M][SIZE_ITEM_M];

/*----------------------------------------------------------------------------- */
/* Data Structure to hold one Parsed GSM CDR                                    */
int fdef_gms[] = {
    STDWID_REC_TYPE
    , STDWID_IMSI
    , STDWID_BNO
    , STDWID_ANO
    , STDWID_SVC_TYPE
    , STDWID_SVC_ID
    , STDWID_MSC_ID
    , STDWID_CELL_ID_A
    , STDWID_CELL_ID_B
    , STDWID_MS_CLS_MARK
    , STDWID_DATE
    , STDWID_TIME
    , STDWID_DURATION
    , STDWID_DATA_VOL
    , STDWID_DATA_REF
    , STDWID_TARIF_CLS
    , STDWID_MS_ROAM_NO
    , STDWID_CHG_IND
    , STDWID_CHG_ORI
    , STDWID_ORI_ANO
    , STDWID_SUPP_CODE
    , STDWID_IMEI_ABC
    , STDWID_IMEI
    , STDWID_OUT_TGRP
    , STDWID_IN_TGRP
    , STDWID_CAUS_TERM
    , STDWID_SRV_TYPE
    , STDWID_SRV_CODE
    , STDWID_CHNL_REQ
    , STDWID_CHNL_USE
    , STDWID_TRANS_IND
    , STDWID_ACT_CODE
    , STDWID_SS_CODE
    , STDWID_ACT_CODE
    , STDWID_SS_CODE
    , STDWID_ACT_CODE
    , STDWID_SS_CODE
    , STDWID_ACT_CODE
    , STDWID_SS_CODE
    , STDWID_ACT_CODE
    , STDWID_SS_CODE
    , STDWID_SVC_KEY
    , STDWID_FCI
    , WID_CELL_4G
};

int fdef_sms[] = {
    STDWID_REC_TYPE
    , STDWID_IMSI
    , (STDWID_BNO - 8)
    , STDWID_ANO
    , STDWID_SVC_TYPE
    , STDWID_SVC_ID
    , STDWID_MSC_ID
    , STDWID_CELL_ID_A
    , STDWID_CELL_ID_B
    , STDWID_MS_CLS_MARK
    , STDWID_DATE
    , STDWID_TIME
    , STDWID_DURATION
    , STDWID_DATA_VOL
    , STDWID_DATA_REF
    , STDWID_TARIF_CLS
    , STDWID_MS_ROAM_NO
    , STDWID_CHG_IND
    , STDWID_CHG_ORI
    , STDWID_ORI_ANO
    , STDWID_SUPP_CODE
    , STDWID_IMEI_ABC
    , STDWID_IMEI
    , STDWID_OUT_TGRP
    , STDWID_IN_TGRP
    , STDWID_CAUS_TERM
    , STDWID_SRV_TYPE
    , STDWID_SRV_CODE
    , STDWID_CHNL_REQ
    , STDWID_CHNL_USE
    , STDWID_TRANS_IND
    , STDWID_ACT_CODE
    , STDWID_SS_CODE
    , STDWID_ACT_CODE
    , STDWID_SS_CODE
    , STDWID_ACT_CODE
    , STDWID_SS_CODE
    , STDWID_ACT_CODE
    , STDWID_SS_CODE
    , STDWID_ACT_CODE
    , STDWID_SS_CODE
    , WID_CALL_REF
    , WID_PPS_DESC
    , WID_AUTH_RSLT
    , STDWID_OPER_A
    , STDWID_OPER_B
    , WID_MSC_ADDR
    , STDWID_SVC_PCK_ID
};

int fdef_smc[] = {
    STDWID_REC_TYPE
    , STDWID_IMSI
    , (STDWID_BNO - 8)
    , STDWID_ANO
    , STDWID_SVC_TYPE
    , STDWID_SVC_ID
    , STDWID_MSC_ID
    , STDWID_CELL_ID_A
    , STDWID_CELL_ID_B
    , STDWID_MS_CLS_MARK
    , STDWID_DATE
    , STDWID_TIME
    , STDWID_DURATION
    , STDWID_DATA_VOL
    , STDWID_DATA_REF
    , STDWID_TARIF_CLS
    , STDWID_MS_ROAM_NO
    , STDWID_CHG_IND
    , STDWID_CHG_ORI
    , STDWID_ORI_ANO
    , STDWID_SUPP_CODE
    , STDWID_IMEI_ABC
    , STDWID_IMEI
    , STDWID_OUT_TGRP
    , STDWID_IN_TGRP
    , STDWID_CAUS_TERM
    , STDWID_SRV_TYPE
    , STDWID_SRV_CODE
    , STDWID_CHNL_REQ
    , STDWID_CHNL_USE
    , STDWID_TRANS_IND
    , WID_ORI_ADDR
    , WID_CALL_REF
    , WID_PPS_DESC
    , WID_AUTH_RSLT
    , STDWID_OPER_A
    , STDWID_OPER_B
    , WID_MSC_ADDR
};

int fdef_ssp[] = {
    STDWID_REC_TYPE
    , STDWID_IMSI
    , STDWID_BNO
    , STDWID_ANO
    , STDWID_SVC_TYPE
    , STDWID_SVC_ID
    , STDWID_MSC_ID
    , STDWID_CELL_ID_A
    , STDWID_CELL_ID_B
    , STDWID_MS_CLS_MARK
    , STDWID_DATE
    , STDWID_TIME
    , STDWID_DURATION
    , STDWID_DATA_VOL
    , STDWID_DATA_REF
    , STDWID_TARIF_CLS
    , STDWID_MS_ROAM_NO
    , STDWID_CHG_IND
    , STDWID_CHG_ORI
    , STDWID_ORI_ANO
    , STDWID_SUPP_CODE
    , STDWID_IMEI_ABC
    , STDWID_IMEI
    , STDWID_OUT_TGRP
    , STDWID_IN_TGRP
    , STDWID_CAUS_TERM
    , STDWID_SRV_TYPE
    , STDWID_SRV_CODE
    , STDWID_CHNL_REQ
    , STDWID_CHNL_USE
    , STDWID_TRANS_IND
    , STDWID_ACT_CODE
    , STDWID_SS_CODE
    , STDWID_ACT_CODE
    , STDWID_SS_CODE
    , STDWID_ACT_CODE
    , STDWID_SS_CODE
    , STDWID_ACT_CODE
    , STDWID_SS_CODE
    , STDWID_ACT_CODE
    , STDWID_SS_CODE
    , STDWID_SVC_KEY
    , STDWID_FCI
    , WID_CELL_4G
};

int fdef_mscpps[] = {
    STDWID_REC_TYPE
    , STDWID_IMSI
    , STDWID_BNO
    , STDWID_ANO
    , STDWID_SVC_TYPE
    , STDWID_SVC_ID
    , STDWID_MSC_ID
    , STDWID_CELL_ID_A
    , STDWID_CELL_ID_B
    , STDWID_MS_CLS_MARK
    , STDWID_DATE
    , STDWID_TIME
    , STDWID_DURATION
    , STDWID_DATA_VOL
    , STDWID_DATA_REF
    , STDWID_TARIF_CLS
    , STDWID_MS_ROAM_NO
    , STDWID_CHG_IND
    , STDWID_CHG_ORI
    , STDWID_ORI_ANO
    , STDWID_SUPP_CODE
    , STDWID_IMEI_ABC
    , STDWID_IMEI
    , STDWID_OUT_TGRP
    , STDWID_IN_TGRP
    , STDWID_CAUS_TERM
    , STDWID_SRV_TYPE
    , STDWID_SRV_CODE
    , STDWID_CHNL_REQ
    , STDWID_CHNL_USE
    , STDWID_TRANS_IND
    , STDWID_ACT_CODE
    , STDWID_SS_CODE
    , STDWID_ACT_CODE
    , STDWID_SS_CODE
    , STDWID_ACT_CODE
    , STDWID_SS_CODE
    , STDWID_ACT_CODE
    , STDWID_SS_CODE
    , STDWID_ACT_CODE
    , STDWID_SS_CODE
    , STDWID_SVC_KEY
    , STDWID_FCI
};

int fdef_usc[] = {
    STDWID_REC_TYPE
    , STDWID_ANO
    , STDWID_DATE
    , STDWID_TIME
    , STDWID_DATE
    , STDWID_TIME
    , (WID_CAT_ID-1)
    , WID_CONT_ID
    , WID_BEARER
    , (STDWID_SVC_ID + 2)
    , STDWID_SVC_TYPE
    , WID_TXN_ID
    , WID_COMPLT
    , WID_CHG_FLG
    , WID_PYMENT_TYPE
    , WID_PROD_VAL
    , WID_VERSION
    , WID_UNIT_CHG
    , WID_CONT_CODE
    , WID_INF
    , WID_CHN_EVT_ID
    , WID_MSG_SIZE
    , WID_COS_B
};

int fdef_mms[] = {
    (STDWID_REC_TYPE+8)
    , WID_TSID
    , WID_NID
    , WID_NRF
    , WID_SUBJ
    , (WID_CUST_ID-5)
    , WID_OBJ
    , WID_LEG
    , WID_SCOS
    , WID_OTON
    , WID_DR_FEE
    , WID_SID
    , (WID_CAT_ID+1)
    , WID_CP_ID
    , WID_CHG_ID
    , WID_PPF
    , WID_DATE_TIME
    , WID_DATE_TIME
    , WID_CAUSE
    , WID_DR_FEE
    , WID_RR_FEE
    , WID_SIZE
    , WID_MUS
    , WID_DR_CAUSE
    , WID_DR_STOP
    , WID_RR_CAUSE
    , WID_RR_STOP
    , WID_TELE_ID
    , (STDWID_IMSI+3)
    , WID_CGRN_ID
    , WID_CDRN_ID
    , WID_OPER_ID
    , WID_PKG_REF
    , WID_OB
    , WID_TI
    , WID_NET_PRICE
    , STDWID_SVC_PCK_ID
    , (WID_PROD_NO-6)
    , (WID_PKG_ID-5)
    , WID_ERF
    , WID_CF_ADDR
    , STDWID_SP_NAME_MO
    , WID_BRAND_ID
    , WID_SP_NAME_MT
};


typedef enum {
    GSM_REC_TYPE = 0
    , GSM_IMSI
    , GSM_BNO
    , GSM_ANO
    , GSM_SVC_TYPE
    , GSM_SVC_ID
    , GSM_MSC_ID
    , GSM_CELL_ID_A
    , GSM_CELL_ID_B
    , GSM_MS_CLS_MARK
    , GSM_DATE
    , GSM_TIME
    , GSM_DURATION
    , GSM_DATA_VOL
    , GSM_DATA_REF
    , GSM_TARIF_CLS
    , GSM_MS_ROAM_NO
    , GSM_CHG_IND
    , GSM_CHG_ORI
    , GSM_ORI_ANO
    , GSM_SUPP_CODE
    , GSM_IMEI_ABC
    , GSM_IMEI
    , GSM_OUT_TGRP
    , GSM_IN_TGRP
    , GSM_CAUS_TERM
    , GSM_SRV_TYPE
    , GSM_SRV_CODE
    , GSM_CHNL_REQ
    , GSM_CHNL_USE
    , GSM_TRANS_IND
    , GSM_ACT_CODE1
    , GSM_SS_CODE1
    , GSM_ACT_CODE2
    , GSM_SS_CODE2
    , GSM_ACT_CODE3
    , GSM_SS_CODE3
    , GSM_ACT_CODE4
    , GSM_SS_CODE4
    , GSM_ACT_CODE5
    , GSM_SS_CODE5
    , GSM_SVC_KEY
    , GSM_FCI
    , GSM_CELL_4G
    NOF_GSM_FIELD
} IDX_GSM_FIELD;

typedef enum {
    SMS_REC_TYPE = 0
    , SMS_IMSI
    , SMS_BNO
    , SMS_ANO
    , SMS_SVC_TYPE
    , SMS_SVC_ID
    , SMS_MSC_ID
    , SMS_CELL_ID_A
    , SMS_CELL_ID_B
    , SMS_MS_CLS_MARK
    , SMS_DATE
    , SMS_TIME
    , SMS_DURATION
    , SMS_DATA_VOL
    , SMS_DATA_REF
    , SMS_TARIF_CLS
    , SMS_MS_ROAM_NO
    , SMS_CHG_IND
    , SMS_CHG_ORI
    , SMS_ORI_ANO
    , SMS_SUPP_CODE
    , SMS_IMEI_ABC
    , SMS_IMEI
    , SMS_OUT_TGRP
    , SMS_IN_TGRP
    , SMS_CAUS_TERM
    , SMS_SRV_TYPE
    , SMS_SRV_CODE
    , SMS_CHNL_REQ
    , SMS_CHNL_USE
    , SMS_TRANS_IND
    , SMS_ACT_CODE1
    , SMS_SS_CODE1
    , SMS_ACT_CODE2
    , SMS_SS_CODE2
    , SMS_ACT_CODE3
    , SMS_SS_CODE3
    , SMS_ACT_CODE4
    , SMS_SS_CODE4
    , SMS_ACT_CODE5
    , SMS_SS_CODE5
    , SMS_CALL_REF
    , SMS_PPS_DESC
    , SMS_AUTH_RSLT
    , SMS_OPER_A
    , SMS_OPER_B
    , SMS_MSC_ADDR
    , SMS_SVC_PCK_ID
    , NOF_SMS_FIELD
} IDX_SMS_FIELD;

typedef enum {
    SMC_REC_TYPE = 0
    , SMC_IMSI
    , SMC_BNO
    , SMC_ANO
    , SMC_SVC_TYPE
    , SMC_SVC_ID
    , SMC_MSC_ID
    , SMC_CELL_ID_A
    , SMC_CELL_ID_B
    , SMC_MS_CLS_MARK
    , SMC_DATE
    , SMC_TIME
    , SMC_DURATION
    , SMC_DATA_VOL
    , SMC_DATA_REF
    , SMC_TARIF_CLS
    , SMC_MS_ROAM_NO
    , SMC_CHG_IND
    , SMC_CHG_ORI
    , SMC_ORI_ANO
    , SMC_SUPP_CODE
    , SMC_IMEI_ABC
    , SMC_IMEI
    , SMC_OUT_TGRP
    , SMC_IN_TGRP
    , SMC_CAUS_TERM
    , SMC_SRV_TYPE
    , SMC_SRV_CODE
    , SMC_CHNL_REQ
    , SMC_CHNL_USE
    , SMC_TRANS_IND
    , SMC_ORI_ADDR
    , SMC_CALL_REF
    , SMC_PPS_DESC
    , SMC_AUTH_RSLT
    , SMC_OPER_A
    , SMC_OPER_B
    , SMC_MSC_ADDR
    , NOF_SMC_FIELD
} IDX_SMC_FIELD;

typedef enum {
    SSP_REC_TYPE = 0
    , SSP_IMSI
    , SSP_BNO
    , SSP_ANO
    , SSP_SVC_TYPE
    , SSP_SVC_ID
    , SSP_MSC_ID
    , SSP_CELL_ID_A
    , SSP_CELL_ID_B
    , SSP_MS_CLS_MARK
    , SSP_DATE
    , SSP_TIME
    , SSP_DURATION
    , SSP_DATA_VOL
    , SSP_DATA_REF
    , SSP_TARIF_CLS
    , SSP_MS_ROAM_NO
    , SSP_CHG_IND
    , SSP_CHG_ORI
    , SSP_ORI_ANO
    , SSP_SUPP_CODE
    , SSP_IMEI_ABC
    , SSP_IMEI
    , SSP_OUT_TGRP
    , SSP_IN_TGRP
    , SSP_CAUS_TERM
    , SSP_SRV_TYPE
    , SSP_SRV_CODE
    , SSP_CHNL_REQ
    , SSP_CHNL_USE
    , SSP_TRANS_IND
    , SSP_ACT_CODE1
    , SSP_SS_CODE1
    , SSP_ACT_CODE2
    , SSP_SS_CODE2
    , SSP_ACT_CODE3
    , SSP_SS_CODE3
    , SSP_ACT_CODE4
    , SSP_SS_CODE4
    , SSP_ACT_CODE5
    , SSP_SS_CODE5
    , SSP_SVC_KEY
    , SSP_FCI
    , SSP_CELL_4G
    , NOF_SSP_FIELD
} IDX_SSP_FIELD;

typedef enum {
    MSCPPS_REC_TYPE = 0         
    , MSCPPS_IMSI
    , MSCPPS_BNO
    , MSCPPS_ANO
    , MSCPPS_SVC_TYPE
    , MSCPPS_SVC_ID
    , MSCPPS_MSC_ID
    , MSCPPS_CELL_ID_A
    , MSCPPS_CELL_ID_B
    , MSCPPS_MS_CLS_MARK
    , MSCPPS_DATE
    , MSCPPS_TIME
    , MSCPPS_DURATION
    , MSCPPS_DATA_VOL
    , MSCPPS_DATA_REF
    , MSCPPS_TARIF_CLS
    , MSCPPS_MS_ROAM_NO
    , MSCPPS_CHG_IND
    , MSCPPS_CHG_ORI
    , MSCPPS_ORI_ANO
    , MSCPPS_SUPP_CODE
    , MSCPPS_IMEI_ABC
    , MSCPPS_IMEI
    , MSCPPS_OUT_TGRP
    , MSCPPS_IN_TGRP
    , MSCPPS_CAUS_TERM
    , MSCPPS_SRV_TYPE
    , MSCPPS_SRV_CODE
    , MSCPPS_CHNL_REQ
    , MSCPPS_CHNL_USE
    , MSCPPS_TRANS_IND
    , MSCPPS_ACT_CODE
    , MSCPPS_SS_CODE
    , MSCPPS_ACT_CODE
    , MSCPPS_SS_CODE
    , MSCPPS_ACT_CODE
    , MSCPPS_SS_CODE
    , MSCPPS_ACT_CODE
    , MSCPPS_SS_CODE
    , MSCPPS_ACT_CODE
    , MSCPPS_SS_CODE
    , MSCPPS_SVC_KEY
    , MSCPPS_FCI
    , NOF_MSCPPS_FIELD
} IDX_MSCPPS_FIELD;

typedef enum {
    USC_REC_TYPE = 0
    , USC_ANO
    , USC_DATE
    , USC_TIME
    , USC_DATE
    , USC_TIME
    , USC_CAT_ID
    , USC_CONT_ID
    , USC_BEARER
    , USC_SVC_ID
    , USC_SVC_TYPE
    , USC_TXN_ID
    , USC_COMPLT
    , USC_CHG_FLG
    , USC_PYMENT_TYPE
    , USC_PROD_VAL
    , USC_VERSION
    , USC_UNIT_CHG
    , USC_CONT_CODE
    , USC_INF
    , USC_CHN_EVT_ID
    , USC_MSG_SIZE
    , USC_COS_B
    , NOF_USC_FIELD
} IDX_USC_ FIELD;

typedef enum  {
    MMS_REC_TYPE = 0
    , MMS_TSID
    , MMS_NID
    , MMS_NRF
    , MMS_SUBJ
    , MMS_CUST_ID
    , MMS_OBJ
    , MMS_LEG
    , MMS_SCOS
    , MMS_OTON
    , MMS_DR_FEE
    , MMS_SID
    , MMS_CAT_ID
    , MMS_CP_ID
    , MMS_CHG_ID
    , MMS_PPF
    , MMS_DATE_TIME
    , MMS_DATE_TIME
    , MMS_CAUSE
    , MMS_DR_FEE
    , MMS_RR_FEE
    , MMS_SIZE
    , MMS_MUS
    , MMS_DR_CAUSE
    , MMS_DR_STOP
    , MMS_RR_CAUSE
    , MMS_RR_STOP
    , MMS_TELE_ID
    , MMS_IMSI
    , MMS_CGRN_ID
    , MMS_CDRN_ID
    , MMS_OPER_ID
    , MMS_PKG_REF
    , MMS_OB
    , MMS_TI
    , MMS_NET_PRICE
    , MMS_SVC_PCK_ID
    , MMS_PROD_NO
    , MMS_PKG_ID
    , MMS_ERF
    , MMS_CF_ADDR
    , MMS_SP_NAME_MO
    , MMS_BRAND_ID
    , MMS_SP_NAME_MT
    NOF_MMS_FIELD
} IDX_MMS_FIELD;



typedef struct gsm_record {
    char record_type[3];
    char imsi_no[SIZE_IMSI+1];
    char b_no[25+8];          /* dialled_digit is renamed to b_no, increase 8 char by N.Thanakorn on 2015-09-15 */
    char a_no[19];
    char service_type[3];
    char service_id[5];
    char msc_id[16];
    char cell_id_a[16];     /* Changed Size to 15 Chars for New MD Format - by Kawee on 22-Oct-2003 */
    char cell_id_b[16];     /* Changed Size to 15 Chars for New MD Format - by Kawee on 22-Oct-2003 */
    char ms_class_mark;
    char start_date[9];     /* YYYYMMDD format */
    char start_time[7];     /* HHMMSS format */
    char duration[7];       /* HHMMSS format */
    char data_vol[7];
    char data_vol_ref[7];
    char tariff_class[5];
    char ms_roaming_num[18];
    char charging_indicator;
    char charging_origin[5];
    char originate_a_no[13];
    char supplementary_code[3];
    char imei_a_b_c;
    char imei[SIZE_IMEI+1];
    char out_tgrp[11];
    char in_tgrp[11];
    char cause_termination[11];
    char dual_serv_type;
    char dual_serv_code[3];
    char channel_req;
    char channel_used;
    char transperancy_ind;
    struct {
        char action_code;
        char ss_code[3];
    } ss_event[5];          /* Table occurs 5 times */
    char service_key[7];    /* Added by Kawee on 05-Apr-2005 */
    char fci[9];            /* Added by Kawee on 05-Apr-2005 */
    char cell4G[19+1];
} GSM_RECORD;


/* Added SMS CDR - by Kawee on 05-Apr-2005 */
/*----------------------------------------------------------------------------- */
/* Data Structure to hold one Parsed GSM SMS CDR                                */
typedef struct sms_record {
    char record_type[3];
    char imsi_no[SIZE_IMSI+1];
    char b_no[25];          /* dialled_digit is renamed to b_no */
    char a_no[19];
    char service_type[3];
    char service_id[5];
    char msc_id[16];
    char cell_id_a[16]; /* Changed Size to 15 Chars for New MD Format - by Kawee on 22-Oct-2003 */
    char cell_id_b[16]; /* Changed Size to 15 Chars for New MD Format - by Kawee on 22-Oct-2003 */
    char ms_class_mark;
    char start_date[9]; /* YYYYMMDD format */
    char start_time[7]; /* HHMMSS format */
    char duration[7];   /* HHMMSS format */
    char data_vol[7];
    char data_vol_ref[7];
    char tariff_class[5];
    char ms_roaming_num[18];
    char charging_indicator;
    char charging_origin[5];
    char originate_a_no[13];
    char supplementary_code[3];
    char imei_a_b_c;
    char imei[SIZE_IMEI+1];
    char out_tgrp[11];
    char in_tgrp[11];
    char cause_termination[11];
    char dual_serv_type;
    char dual_serv_code[3];
    char channel_req;
    char channel_used;
    char transperancy_ind;
    struct {
        char action_code;
        char ss_code[3];
    } ss_event[5];              /* Table occurs 5 times */
    char call_ref[13];          /* Added by Kawee on 05-Apr-2005 */
    char pps_desc[3];           /* Added by Kawee on 05-Apr-2005 */
    char auth_result[10];       /* Added by Kawee on 05-Apr-2005 */
    char operator_a[16];        /* Added by Thanakorn on 28-Dec-2009 */
    char operator_b[16];        /* Added by Thanakorn on 28-Dec-2009 */
    char md_msc_address[21];    /* Added by Thanakorn on Sep-2011 to Support 3G AIS/TOT */
    char servpackid[4];         /* Added by Thanakorn on Aug-2013 to Support BOS */
} SMS_RECORD;

/* Added SMC CDR - by Jetsada on 15-Aug-2005 */
/*----------------------------------------------------------------------------- */
/* Data Structure to hold one Parsed GSM SMS CDR                                */
typedef struct smc_record {
    char record_type[3];
    char imsi_no[SIZE_IMSI+1];
    char b_no[25];              /* dialled_digit is renamed to b_no */
    char a_no[19];
    char service_type[3];
    char service_id[5];
    char msc_id[16];
    char cell_id_a[16]; /* Changed Size to 15 Chars for New MD Format - by Kawee on 22-Oct-2003 */
    char cell_id_b[16]; /* Changed Size to 15 Chars for New MD Format - by Kawee on 22-Oct-2003 */
    char ms_class_mark;
    char start_date[9]; /* YYYYMMDD format */
    char start_time[7]; /* HHMMSS format */
    char duration[7];   /* HHMMSS format */
    char data_vol[7];
    char data_vol_ref[7];
    char tariff_class[5];
    char ms_roaming_num[18];
    char charging_indicator;
    char charging_origin[5];
    char originate_a_no[13];
    char supplementary_code[3];
    char imei_a_b_c;
    char imei[SIZE_IMEI+1];
    char out_tgrp[11];
    char in_tgrp[11];
    char cause_termination[11];
    char dual_serv_type;
    char dual_serv_code[3];
    char channel_req;
    char channel_used;
    char transperancy_ind;
    char orig_addr[16]; /* Changed ss_event to orig_addr for reject duplicate SMC CDR with FDA CDR - by Wiraphot on 07-Jan-2009 */
    char call_ref[13];  /* Added by Kawee on 05-Apr-2005 */
    char pps_desc[3];       /* Added by Kawee on 05-Apr-2005 */
    char auth_result[10];   /* Added by Kawee on 05-Apr-2005 */
    char operator_a[16];    /* Added by Thanakorn on 28-Dec-2009 */
    char operator_b[16];    /* Added by Thanakorn on 28-Dec-2009 */
    char md_msc_address[21];    /* Added by Thanakorn on Oct-2011 to Support 3G AIS/TOT */
} SMC_RECORD;


/*----------------------------------------------------------------------------- */
/* Data Structure to hold one Parsed SSP CDR                                    */
typedef struct ssp_record {
    char record_type[3];
    char imsi_no[SIZE_IMSI+1];
    char b_no[25+8];        /* dialled_digit is renamed to b_no, increase 8 char by N.Thanakorn on 2015-09-15 */
    char a_no[19];
    char service_type[3];
    char service_id[5];
    char msc_id[16];
    char cell_id_a[16]; /* Changed Size to 15 Chars for New MD Format - by Kawee on 22-Oct-2003 */
    char cell_id_b[16]; /* Changed Size to 15 Chars for New MD Format - by Kawee on 22-Oct-2003 */
    char ms_class_mark;
    char start_date[9]; /* YYYYMMDD format */
    char start_time[7]; /* HHMMSS format */
    char duration[7];   /* HHMMSS format */
    char data_vol[7];
    char data_vol_ref[7];
    char tariff_class[5];
    char ms_roaming_num[18];
    char charging_indicator;
    char charging_origin[5];
    char originate_a_no[13];
    char supplementary_code[3];
    char imei_a_b_c;
    char imei[SIZE_IMEI+1];
    char out_tgrp[11];
    char in_tgrp[11];
    char cause_termination[11];
    char dual_serv_type;
    char dual_serv_code[3];
    char channel_req;
    char channel_used;
    char transperancy_ind;
    struct {
        char action_code;
        char ss_code[3];
    } ss_event[5];          /* Table occurs 5 times */
    char service_key[7];    /* Added by Kawee on 05-Apr-2005 */
    char fci[9];            /* Added by Kawee on 05-Apr-2005 */
    char cell4G[19+1];
} SSP_RECORD;
typedef SSP_RECORD VOICE_RECORD;

/* Added MSC PPS CDR by Kawee on 13-Dec-2002 */
/*----------------------------------------------------------------------------- */
/* Data Structure to hold one Parsed MSC PPS CDR                                */
typedef struct mscpps_record {
    char record_type[3];
    char imsi_no[SIZE_IMSI+1];
    char b_no[25+8];        /* dialled_digit is renamed to b_no, increase 8 char by N.Thanakorn on 2015-09-15 */
    char a_no[19];
    char service_type[3];
    char service_id[5];
    char msc_id[16];
    char cell_id_a[16]; /* Changed Size to 15 Chars for New MD Format - by Kawee on 22-Oct-2003 */
    char cell_id_b[16]; /* Changed Size to 15 Chars for New MD Format - by Kawee on 22-Oct-2003 */
    char ms_class_mark;
    char start_date[9]; /* YYYYMMDD format */
    char start_time[7]; /* HHMMSS format */
    char duration[7];   /* HHMMSS format */
    char data_vol[7];
    char data_vol_ref[7];
    char tariff_class[5];
    char ms_roaming_num[18];
    char charging_indicator;
    char charging_origin[5];
    char originate_a_no[13];
    char supplementary_code[3];
    char imei_a_b_c;
    char imei[SIZE_IMEI+1];
    char out_tgrp[11];
    char in_tgrp[11];
    char cause_termination[11];
    char dual_serv_type;
    char dual_serv_code[3];
    char channel_req;
    char channel_used;
    char transperancy_ind;
    struct {
        char action_code;
        char ss_code[3];
    } ss_event[5];      /* Table occurs 5 times */
    /* char     filler[15]; */  /* Changed as below by Kawee on 05-Apr-2005 */
    char service_key[7];    /* Added by Kawee on 05-Apr-2005 */
    char fci[9];        /* Added by Kawee on 05-Apr-2005 */
} MSCPPS_RECORD;

/*----------------------------------------------------------------------------- */
/* Data Structure to hold one Parsed USC CDR                                    */
typedef struct usc_record {
    char record_type[3];
    char a_no[19];
    char start_date[9]; /* YYYYMMDD format */
    char start_time[7]; /* HHMMSS format */
    char end_date[9];   /* YYYYMMDD format */
    char end_time[7];   /* HHMMSS format */
    char category_id[4];
    char content_id[4];
    char bearer;
    char service_id[7];
    char service_type[3];
    char txn_id[3];
    char completion[3];
    char charge_flag;
    char payment_type[3];       /* as a COS_A filed */
    char product_value[9];
    char version[5];
    char unit_charge[3];
    /* char mtx_id[11];     Now Not Use in New USC format on 22-Oct-2003 by Kawee */
    char content_code[21];      /* Added on 15-Nov-2001 */
    char Info[101];             /* Added again - by Kawee on 22-Oct-2003 */
    char change_event_id[11];   /* Added on 04-Apr-2002 by Kawee */
    char message_size[9];       /* Added on 24-Dec-2002 by Kawee */
    char cos_b[4];              /* Added by Thanakorn on 28-Dec-2009 */
} USC_RECORD;

/*----------------------------------------------------------------------------- */
/* GPRS structure added by kawin on 19-Sep-2002 */
typedef struct gprs_record {
    char record_type[3];
    char rec_seq_number[11];
    char served_imsi[SIZE_IMSI+1];
    char served_imei[SIZE_IMEI+1];
    char calling_number[16];    /* MSISDN */
    char called_number[64];     /* Access Point Name NI */
    char exchange_id[16];       /* GSSN Address */
    char tariff_class[5];
    char date[9];
    char time[7];
    char duration[7];
    char cause_for_rec_closing[3];
    char cell_identity[16];     /* Changed Size to 15 Chars for New MD Format - by Kawee on 22-Oct-2003 */
    char msc_id[21];
    char volume_up_link[21];
    char volume_down_link[21];
    char charge_amount[11];
    char charge_id[11];
    /* New MD Format - Added The following fields - by Kawee on 22-Oct-2003 */
    char sgsn_address[16];
    char pdp_type[5];
    char pdp_address[16];
    char access_point_name[38];
    char qos_reliability[3];
    char qos_delay[3];
    char qos_precedence[3];
    char qos_peak[3];
    char qos_mean[3];
    char roam_flg[2];
} GPRS_RECORD;

/*----------------------------------------------------------------------------- */
/* GPRST (GSM/DPC/PPS GPRS Time Based) structure added by Kawee on 20-May-2004 */
typedef struct gprst_record {
    char record_type[11];
    char cid[19];               /* Chunk ID */ /* Resize from 9+1 to 18+1 - by Kawee on 31-Aug-2007 */
    char pkgref[10];
    char msisdn[21];            /* A_number */
    char customer_id[16];
    char cos[7];                /* Class of service */
    char bearer_id[6];
    char vol[11];               /* Total of usage (byte) */
    char vol_cost[11];          /* Total costs of usage (??? unit) only PPS subscriber */
    char sid[7];                /* Service Id */
    char cat_id[5];             /* Category Id */
    char cp_id[4];              /* Content provider id */
    char start_datetime[15];    /* Start Time in YYYYMMDDHHMMSS format */
    char stop_datetime[15];     /* Stop Time in YYYYMMDDHHMMSS format */
    char cause[4];              /* End Cause */
    char ppf;                   /* Prepaid Indicator */
    char sgw_id[4];             /* Service Gateway ID */
    /* Changed as below by Kawee on 14-Jan-2005 */
    char rqr[31+128];           /* Requester Reference (use only pps) */ /* Add 128 character to prevent Size too small by Kawee on 20-May-2005 */
    char imsi[21];
    char cgrn_id[21];
    char oper_id[21];
    char tbcf;                  /* 0 = GPRS Volumn Base, 1 = GPRS Time Base */
    char ti;                    /* Test Indicator, 1 = Test, 0 = Production */  /* Added by Kawee on 01-Feb-2006 */
    char calledparty_id[22];    /* Called Party ID */   /* Added by Kawee on 31-Aug-2007 */
    char netprice[11];          /* Cost After Discount */   /* Added by Kawee on 31-Aug-2007 */
    char servpackid[4];         /*Added by Wiraphot on 14-Jan-2009*/
    char productno[11];         /*Added by Wiraphot on 14-Jan-2009*/
    char pkgid[11];             /*Added by Wiraphot on 14-Jan-2009*/
    char nid[15];
    char erf[51];
    char cfaddress[16];
    char spnamemo[16];
    char brandidmo[16];
    char spnamemt[16];
    char rat[6];
    char userlocinfo[21];
    char initrat[4];            /* Added by Thanakorn on 06-Jan-2010 */
    char lCun[16];              /* Added by Thanakorn on 08-Sep-2011 to Support 3G */
    char volChg[16];
    char initSpeed[11];
    char speed[11];
    char speedType[5];

} GPRST_RECORD;

/*----------------------------------------------------------------------------- */
/* MMS (GSM/PPS MMS) structure added by Kawee on 05-Nov-2004 */
typedef struct mms_record {
    char record_type[11];
    char tsid[19];              /* Tele service session ID */   /* Resize from 9+1 to 18+1 - by Kawee on 31-Aug-2007 */
    char nid[4];                /* Network Element ID */
    char nrf[31];               /* Network Element Reference */
    char subject[22];           /* MSISDN */
    char customer_id[11];       /* Customer ID */
    char object[22];
    char leg;                   /* Party ID: 0 = Mobile Originate, 1 = Mobile Transfer */
    char scos[7];               /* Class of Service: 31=DPC, 51=GSM Advance, Others=PPS */
    char oton[21];              /* Type of Number of Object */
    char fee[6];                /* Service Fee in Satang Unit (Charge PPS) */
    char sid[7];                /* Service ID */
    char cat_id[6];             /* Category ID */
    char cp_id[4];              /* Content Provider ID */
    char chg_id[11];            /* Charging ID */
    char ppf;                   /* Prepaid Indicator: 0=Postpaid, 1=Prepaid PPS */
    char start_datetime[15];    /* Start Time in YYYYMMDDHHMMSS format */
    char stop_datetime[15];     /* Stop Time in YYYYMMDDHHMMSS format */
    char cause[4];              /* End Cause: 000=Success */
    char dr_fee[6];             /* Delivery Report Fee */
    char rr_fee[6];             /* Read Reply Report Fee */
    char size[11];              /* Total Message Size */
    char mus[11];               /* Value of Message Step Size */
    char dr_cause[4];           /* Delivery Report End Cause */
    char dr_stop[15];           /* Delivery Report Stop Time in YYYYMMDDHHMMSS format */
    char rr_cause[4];           /* Read Reply Report End Cause */
    char rr_stop[15];           /* Read Reply Report Stop Time in YYYYMMDDHHMMSS format */
    char tele_id[6];            /* Tele Service ID */
    char imsi[21];              /* IMSI */
    char cgrn_id[21];           /* Calling Party Roaming Network Id */
    char cdrn_id[21];           /* Called Party Roaming Network Id */
    char oper_id[21];           /* Operator ID */
    char pkgref[16];            /* Package Reference */ /* Added by Kawee on 01-Feb-2006 */
    char ob;                    /* Old Bearer: Always '6' */
    char ti;                    /* Test Indicator, 1 = Test, 0 = Production */  /* Added by Kawee on 01-Feb-2006 */
    char netprice[11];          /* Net Price */ /* Added by Kawee on 31-Aug-2006 */
    char servpackid[4];
    char productno[5];
    char pkgid[6];
    char erf[51];
    char cfaddress[16];
    char spnamemo[16];
    char brandidmo[16];
    char spnamemt[51];
} MMS_RECORD;

/*----------------------------------------------------------------------------- */
/* CDG structure added by Kawee on 02-Jun-2005 */
typedef enum {
    CDG_REC_TYPE = 0,   /* Record Type */
    CDG_TID,            /* Transaction Id */
    CDG_MKID,           /* Master Key Id */
    CDG_CHAL_FLAG,      /* Challenge Transaction Indicatior */
    CDG_SGWID,          /* Service Gateway Id */
    CDG_MKTYPE,         /* Master Key Type */
    CDG_MSISDN,         /* MSISDN Number */
    CDG_APPID,          /* Application Id */
    CDG_MENU,           /* Application Menu */
    CDG_START,          /* Start Time in YYYYMMDDHHMSS format */
    CDG_STOP,           /* Stop Time in YYYYMMDDHHMSS format */
    CDG_CAUSE,          /* End Cause */
    CDG_SIZE,           /* Total size of transaction */
    CDG_SILENT_FLAG,    /* Silent Flag */
    CDG_SERF,           /* Sender Reference */
    CDG_RREF,           /* Receiver Reference */
    CDG_LEG,            /* Leg */
    CDG_CCT,            /* Content Contained Type */
    CDG_PFEE,           /* Partial Fee */
    CDG_PDISC,          /* Partial Discount */
    CDG_IMSI,           /* IMSI */
    CDG_CGRNID,         /* Calling Party Roaming Country Code */
    CDG_OPERID,         /* Operatior Id */
    CDG_COS,            /* Class of Service */
    CDG_PPF,            /* Prepaid Flag */
    CDG_SID,            /* Service Id */
    CDG_CATID,          /* Category Id */
    CDG_CPID,           /* Content Id */
    CDG_CHGID,          /* Chaging Id */
    CDG_CUSTID,         /* Customer Id (Char 15) */ /* Added by Kawee on 15-Aug-2007 */
    CDG_TI,             /* Testing Indicator, 0=Production, 1=Test (Char 1) */  /* Added by Kawee on 15-Aug-2007 */
    CDG_PKGREF,         /* Package Reference (Char 20) */                       /* Added by Kawee on 15-Aug-2007 */
    CDG_PRODNO,         /* Product Number (Char 4) */                           /* Added by Kawee on 29-Aug-2007 */
    CDG_PKGID,          /* Package Id (Char 5) */                               /* Added by Kawee on 29-Aug-2007 */
    CDG_SRVPKID,        /* ServPackId : Identify the Charging System (Char 3) */
    CDG_NID,            /* Network Element ID (Char 6) */
    CDG_ERF,            /* End - Reference:The identification of Starting point to the end node (Char 50)*/
    CDG_CFADDR,         /* Charging-Function-Address: The Gt address of charging function (Char 15)*/
    CDG_SPNAMEMO,       /* Service-Provider-Name-MO : The owner of sub identity(ais , dpc ,awn) (Char 15)*/
    CDG_BRANDIDMO,      /* Brand-ID-MO:  Service Promotion(0 = GSM, 1 = 1-2-call, 2 = Sawasdee, 3 = 3G) (Char 2)*/
    CDG_SPNAMEMT,       /* Service-Provider-Name-MT : The owner of sub identity (ais, dpc, awn, dtac, tmv, hutch) (Char 15)*/  /* Added by Thanakorn on 08-Jan-2010 */
    NOF_FLD_CDG
} IDX_CDG_FIELD;

/*----------------------------------------------------------------------------- */
typedef enum {
    RBT_MSISDN = 0,     /* MSISDN Number */
    RBT_TRT,            /* Tradetime */
    RBT_EVT,            /* Event Type */
    RBT_COS,            /* RBT Cos ID */
    RBT_RGNO,           /* Ring No */
    RBT_NRMFEE,         /* Normal Fee */
    RBT_BLC,            /* Balance */
    RBT_PMFEE,          /* Pmfee */
    RBT_PMMNY,          /* Pmmoney */
    RBT_INFLG,          /* INFlag */
    RBT_CLBNM,          /* CLUBNAME */
    RBT_SRC,            /* SOURCE */
    NOF_FLD_RBT
} IDX_RBT_FIELD;

/*----------------------------------------------------------------------------- */
/* Introduce AIN CDR Type - Added below by Kawee on 29-Jan-2007 */
typedef enum {
    AIN_TAG = 0,        /* Tag/Record Type */
    AIN_ESRC,           /* Event Source (A-Number) */
    AIN_ETID,           /* Event Type Id */
    AIN_DIALNO,         /* Dialed Number (B-Number) */
    AIN_MCIDA,          /* Mapped Cell ID A */
    AIN_ORGBNO,         /* Original B-Number */
    AIN_EDTM,           /* Event Date-Time (YYYY/MM/DD-hh-mm-ss.tt format) */
    AIN_DURA,           /* Duration (Seconds format) */
    AIN_MT,             /* Modifier Type */
    AIN_CHGPTY,         /* Charge Party */
    AIN_INROUTE,        /* Incoming Route */
    AIN_OUTROUTE,       /* Outgoing Route */
    AIN_MTXID,          /* Exchange ID */
    AIN_FLD14,          /* Reserved Field (Always Null) */
    AIN_FLD15,          /* Reserved Field (Always Null) */
    AIN_FLD16,          /* Reserved Field (Always Null) */
    AIN_FLD17,          /* Reserved Field (Always Null) */
    AIN_FLD18,          /* Reserved Field (Always Null) */
    AIN_FLD19,          /* Reserved Field (Always Null) */
    AIN_FLD20,          /* Reserved Field (Always Null) */
    AIN_FLD21,          /* Reserved Field (Always Null) */
    AIN_FLD22,          /* Reserved Field (Always Null) */
    AIN_FLD23,          /* Reserved Field (Always Null) */
    AIN_FLD24,          /* Reserved Field (Always Null) */
    AIN_FLD25,          /* Reserved Field (Always Null) */
    AIN_FLD26,          /* Reserved Field (Always Null) */
    AIN_FLD27,          /* Reserved Field (Always Null) */
    AIN_FLD28,          /* Reserved Field (Always Null) */
    AIN_FLD29,          /* Reserved Field (Always Null) */
    AIN_FLD30,          /* Reserved Field (Always Null) */
    AIN_FLD31,          /* Reserved Field (Always Null) */
    AIN_FLD32,          /* Reserved Field (Always Null) */
    AIN_FLD33,          /* Reserved Field (Always Null) */
    AIN_FLD34,          /* Reserved Field (Always Null) */
    AIN_FLD35,          /* Reserved Field (Always Null) */
    AIN_FLD36,          /* Reserved Field (Always Null) */
    AIN_FLD37,          /* Reserved Field (Always Null) */
    NOF_FLD_AIN
} IDX_AIN_FIELD;

/*----------------------------------------------------------------------------- */
/* Introduce CCUDR CDR Type - Added below by Jetsada on 24-Aug-2007 */
typedef enum {
    CCUDR_REC_TYPE = 0,     /* Recodr Type */
    CCUDR_SSID,             /* Service Session ID*/
    CCUDR_MSISDN,           /* Calling Party MSISDN */
    CCUDR_APPID,            /* Application ID */
    CCUDR_FEE,              /* Fee */
    CCUDR_DL_FEE,           /* Delivered Cost */
    CCUDR_UNDL_FEE,         /* UnDelivered Cost */
    CCUDR_CONTENT,          /* Content Information */
    CCUDR_BEARER_TYPE,
    CCUDR_BERARER_FEE,
    CCUDR_DELIVERED,
    CCUDR_START,            /* Start Time */
    CCUDR_STOP,             /* Stop Time */
    CCUDR_CAUSE,            /* End Cause */
    CCUDR_SID,              /* Service ID */
    CCUDR_CATID,            /* Category ID */
    CCUDR_CPID,             /* Content ID */
    CCUDR_CHAIN_STAT,       /* Chain Status */
    CCUDR_MSSID,            /* Master Service Session ID */
    CCUDR_CHGID,            /* Charging ID */
    CCUDR_MENU,             /* Application Menu */
    CCUDR_ASPID,            /* Application Service Provider ID */
    CCUDR_PPF,              /* Prepaid Indicator (0 = postpaid, 1 = PPS) */
    CCUDR_CUSTOMERID,       /* Customer ID of Calling Party */
    CCUDR_COS,              /* Class of Service of Calling Party */
    CCUDR_DISCOUNT,         /* Discount Value */
    CCUDR_IMSI,             /* IMSI */
    CCUDR_CGRNID,           /* Calling Party Roaming Network ID */
    CCUDR_OPERID,           /* Operator ID */
    CCUDR_OB,               /* Old Bearer */
    CCUDR_TI,               /* Test Indicator (0 = production, 1 = test) */
    CCUDR_PKGREF,           /* Package Reference Number */
    CCUDR_PRODNO,           /* Product Number */
    CCUDR_PKGID,            /* Package Id */
    CCUDR_SRVPCKID,
    CCUDR_NID,
    CCUDR_ERF,
    CCUDR_CFADDR,
    CCUDR_SPNAMEMO,
    CCUDR_BRANDIDMO,
    CCUDR_SPNAMEMT,         /* */ /* Added by Thanakorn on 11-Jan-2010 */
    NOF_FLD_CCUDR
} IDX_CCUDR_FIELD;

/*----------------------------------------------------------------------------- */
/* Introduce NRTRDE CDR Type - Added below by Kawee on 30-Jan-2008 */
typedef enum {
    NRTRDE_REC_TYPE = 0,        /* Record Type */
    NRTRDE_IMSI,                /* IMSI */
    NRTRDE_MOBILE_NO,           /* Mobile Number */
    NRTRDE_START_TIME,          /* Start Time (YYYYMMDDHHMMSS format) */
    NRTRDE_UTC_OFFSET,          /* UTC Offset (+99.99 format) */
    NRTRDE_DURATION,            /* Duration (Seconds format) */
    NRTRDE_CALLTYPE,            /* Call Type (20=MOC,21=SMS Orig,22=MFC,25=MOCC,26=MFCC,30=MTC,SMS Term,18=GPRS */
    NRTRDE_SVC_CODE,            /* Service Code */
    NRTRDE_BNO,                 /* Bno MOC=Called Number, MTC=Calling Number */
    NRTRDE_APN,                 /* Access Point Name */
    NRTRDE_VOL_IN,              /* Data Volume Incoming */
    NRTRDE_VOL_OUT,             /* Data Volume Outgoing */
    NRTRDE_SGSN,                /* SGSN Address */
    NRTRDE_GGSN,                /* GGSN Address */
    NRTRDE_CHG_ID,              /* Charging ID */
    NRTRDE_CHG_AMOUNT,          /* Charging Amount (Satang Format) */
    NRTRDE_PLMN_CODE,           /* PLMN Code */
    NOF_FLD_NRTRDE
} IDX_NRTRDE_FIELD;

// NRTRDE CDR is now used of output from merging NRTRDE, SCP and TAP - Added by Thanakorn on Sep-2016
typedef enum {
    CMNIR_REC_TYPE = 0,         // 0
    CMNIR_IMSI,                 // 1
    CMNIR_START_DATE,           // 2
    CMNIR_START_TIME,           // 3
    CMNIR_DURATION,             // 4
    CMNIR_CALL_NUM,             // 5
    CMNIR_CHARGE,               // 6
    CMNIR_PLMN,                 // 7
    CMNIR_PROC_DATE,            // 8
    CMNIR_VOLUME,               // 9
    CMNIR_CHRG_TYPE,            // 10
    CMNIR_COMPANY,              // 11
    CMNIR_ONE_CHARGE,           // 12
    CMNIR_TH_TIME,              // 13
    CMNIR_CALLNO_TYPE,          // 14
    CMNIR_RSK_NO_FLG,           // 15
    CMNIR_RSKNO_ID,             // 16
    CMNIR_BILL_SYS,             // 17
    CMNIR_START_DT,             // 18
    CMNIR_STOP_DT,              // 19
    CMNIR_CHRG_ID,              // 20
    CMNIR_UTC_TIME,             // 21
    CMNIR_TOT_EVT_DUR,          // 22
    CMNIR_ORI_REC_TYPE,         // 23
    CMNIR_MOBILE_NUM,           // 24
    CMNIR_IMEI,                 // 25
    CMNIR_ORI_SRC,              // 26
    CMNIR_ORI_FILENAME,         // 27
    CMNIR_COUNTRY_CODE,         // 28
    CMNIR_PLMN_NAME,            // 29
    CMNIR_ROAM_COUNTRY,         // 30
    CMNIR_ROAM_REGION,          // 31
    CMNIR_ORI_DUR,              // 32
    CMNIR_ORI_VOL,              // 33
    CMNIR_ORI_CHG,              // 34
    NOF_FLD_CMNIR               // 35
} IDX_CMNIR_FIELD;

/*----------------------------------------------------------------------------- */
/* Introduce NRTRDE SCP CDR Type - Added by Thanakorn on 24-Sep-2012 */
typedef enum {
    NRTSCP_SVC_KEY = 0,         /* service type */
    NRTSCP_SUBS_TYPE,           /* service class id */
    NRTSCP_CALL_TYPE,           /* either originating or terminating call type */
    NRTSCP_CHRG_TYPE,           /* Charge type */
    NRTSCP_ROAM_FLAG,           /* Whether to roam or not */
    NRTSCP_CALLING_PARTY_NO,    /* Calling party's phone number */
    NRTSCP_CALLED_PARTY_NO,     /* Called party's phone number */
    NRTSCP_ROAM_AREA_NO,        /* Area number for the roaming place of the calling */
    NRTSCP_CALL_BEGIN_TIME,     /* Call Begin Time */
    NRTSCP_CALL_DUR,            /* Call duration */
    NRTSCP_CALL_COST,           /* The deducted fee from Balance account for this call */
    NRTSCP_CHRG_PARTY_IND,      /* Charge calling or called */
    NRTSCP_CALLING_AREA_NO,     /* Calling visiting area number */
    NRTSCP_CALLED_AREA_NO,      /* Called visiting area number */
    NRTSCP_TERM_REASON,         /* Reason to release call */
    NRTSCP_NEW_BALANCE,         /* Subscriber's balance after call */
    NRTSCP_SVC_TYPE,            /* Service type for the preference discount enjoyed by this call */
    NRTSCP_ORG_CALLED_PARTY,    /* International number format when forwarding */
    NRTSCP_DISCOUNT,            /* Service Discount rate enjoyed by subscriber */
    NRTSCP_PROMO_AMNT_USED,     /* Promotion amount spent in this call */
    NRTSCP_PROMO_AMNT,          /* Promotion account after this call */
    NRTSCP_RESERVED,            /* No Use */
    NRTSCP_BRAND_ID,            /* Brand id */
    NRTSCP_CALL_CAC,            /* Charge Area Code for subscriber to initiate call */
    NRTSCP_PAYMENT_TYPE,        /* Subscriber's payment type */
    NRTSCP_FREE_MINUTES,        /* Record the free call time spent in the call */
    NRTSCP_GRP_NO,              /* Subscriber's group id */
    NRTSCP_SUBS_STATE,          /* Subscriber's current state */
    NRTSCP_FREE_MIN_LEFT,       /* Remaining free time after call */
    NRTSCP_CALLED_ROAM_NO,
    NRTSCP_SSP_ADDR,
    NRTSCP_SCREEN_FLG,
    NOF_FLD_NRTSCP
} IDX_NRTSCP_FIELD;

/*----------------------------------------------------------------------------- */
/* Introduce AMF CDR Type - Added below by Thanakorn on 04-Aug-2011 */
typedef enum {
    AMF_APP_ID,
    AMF_REF_NO,
    AMF_TIMESTAMP,
    AMF_REC_NAME,
    AMF_MSISDN,
    AMF_CUST_ID,
    AMF_START_TIME,
    AMF_COST_INFO,
    AMF_COUNTER_NAME,
    AMF_PRODUCT_NO,
    AMF_EXPIRY_TIME,
    AMF_CYCLE_NO,
    AMF_LIFE_CYCLE,
    AMF_CHRG_PHASE,
    AMF_COS,
    AMF_ORG_ACC_TYPE,
    AMF_VAL_TYPE,
    AMF_PACK_ID,
    AMF_SVC_ID,
    AMF_CAT_ID,
    AMF_CON_ID,
    AMF_CCT,
    AMF_BEARER,
    AMF_SSS_TYPE,
    AMF_PARTNER_CODE,
    AMF_CONFIRM_CH,
    AMF_SESSION_ID,
    NOF_FLD_AMF
} IDX_AMF_FIELD;

/*----------------------------------------------------------------------------- */
/* Introduce SDG CDR Type - Added below by Thanakorn on 01-Mar-2012 */
typedef enum {
    SDG_REC_TYPE,
    SDG_TIMESTAMP,
    SDG_SESSID,
    SDG_ANO,
    SDG_COSA,
    SDG_PPF,
    SDG_CUSTID,
    SDG_BNO,
    SDG_COSB,
    SDG_SERVID,
    SDG_CATID,
    SDG_CONT_ID,
    SDG_BEARER,
    SDG_FEE,
    SDG_CCT,
    SDG_PACKID,
    SDG_SDGID,
    SDG_CHGID,
    SDG_TESTIND,
    SDG_SERVPROVNAME,
    SDG_BRANDID,
    SDG_SRVPKID,
    NOF_FLD_SDG
} IDX_SDG_FIELD;
/*----------------------------------------------------------------------------- */
/* Introduce SDG CDR Type - Added below by Thanakorn on 01-Mar-2012 */
typedef enum {
    GPR_REC_TYPE,
    GPR_REC_SEQ_NUM,
    GPR_SRV_IMSI,
    GPR_SRV_IMEI,
    GPR_SRV_MSISDN,
    GPR_APN_NI,
    GPR_GGSN_ADDR,
    GPR_TARIFF_CLASS,
    GPR_REC_OPEN_DATE,
    GPR_REC_OPEN_TIME,
    GPR_DUR,
    GPR_CAUSE_4_CLOSE,
    GPR_CELL_ID,
    GPR_NODE_ID,
    GPR_VOL_UP_LINK,
    GPR_VOL_DW_LINK,
    GPR_CHRG_AMT,
    GPR_CHRG_ID,
    GPR_SGSN_ADDR,
    GPR_PDP_TYPE,
    GPR_SRV_PDP_ADDR,
    GPR_APN_OI,
    GPR_QOS_RELIA,
    GPR_QOS_DELAY,
    GPR_QOS_PRECE,
    GPR_QOS_PEAK_THRU,
    GPR_QOS_MEAN_THRU,
    GPR_ROAM_FLG,
    GPR_CHG_CHAR,
    GPR_BEAR_ID,
    GPR_SYS_TYPE,
    NOF_FLD_GPRS
} IDX_GPRS_FIELD;
/*----------------------------------------------------------------------------- */
/* Introduce SDG CDR Type - Added below by Thanakorn on 01-Mar-2012 */
typedef enum {
    OCS_CALLER_NO,
    OCS_CALLED_NO,
    OCS_IMSI,
    OCS_START_DT,
    OCS_DURATION,
    OCS_CALLED_LAC,
    OCS_END_CAUSE,
    OCS_CDR_REF_NO,
    OCS_CALL_REF,
    OCS_SERV_KEY,
    OCS_MSC_ADDR,
    OCS_BEARER_CAP,
    NOF_FLD_OCS
} IDX_OCS_FIELD;

/* Introduce GSM Topup Type - Added below by Thanakorn on 5-Feb-2015 */
typedef enum {
    GTU_BODY_TAG,
    GTU_ACCT_NO,
    GTU_EVT_SEQ,
    GTU_EVT_SRC,            // A-Number
    GTU_CLU_SUB_ID,
    GTU_EVT_TYPE_ID,
    GTU_CRT_DTM,
    GTU_EVT_DTM,            // Start time
    GTU_EVT_COST_MNY,       // charge
    GTU_MNG_FILE_ID,
    GTU_ROW_NO,
    GTU_COST_CENTRE_ID,
    GTU_TARIFF_ID,
    GTU_EVT_ATTR1,
    GTU_EVT_ATTR2,
    GTU_EVT_ATTR3,
    GTU_EVT_ATTR4,
    GTU_EVT_ATTR5,
    GTU_EVT_ATTR6,
    GTU_EVT_ATTR7,
    GTU_EVT_ATTR8,
    GTU_EVT_ATTR9,
    GTU_EVT_ATTR10,
    GTU_EVT_ATTR11,
    GTU_EVT_ATTR12,         // Service name
    GTU_EVT_ATTR13,
    GTU_EVT_ATTR14,
    GTU_EVT_ATTR15,
    GTU_EVT_ATTR16,
    GTU_EVT_ATTR17,
    GTU_EVT_ATTR18,         // B-Number
    GTU_EVT_ATTR19,
    GTU_EVT_ATTR20,         // VAS name
    GTU_EVT_ATTR21,
    GTU_EVT_ATTR22,         // Oper name
    NOF_FLD_TOP
} IDX_GTU_FIELD;


/* IMS Type - Added below by Thanakorn on 28-Sep-2015 */
typedef enum {
    IMS_REC_TYPE,
    IMS_START_DT,
    IMS_ANO,
    IMS_BNO,
    IMS_TOTAL_DUR,
    IMS_CALL_DUR,
    IMS_VDO_DUR,
    IMS_CF,
    IMS_ORI_ANO,
    IMS_USR_TYPE,
    IMS_SVC_KEY,
    IMS_RN_CODE,
    IMS_FCI,
    IMS_REAL_ANO,
    IMS_REAL_BNO,
    IMS_CALL_WAIT,
    IMS_CALL_HOLD,
    IMS_CALL_CONF,
    IMS_CALLER_MCC,
    IMS_CALLER_MNC,
    IMS_CALLER_TAC,
    IMS_CALLER_ENODEB_ID,
    IMS_CALLER_CI,
    IMS_CALLED_MCC,
    IMS_CALLED_MNC,
    IMS_CALLED_TAC,
    IMS_CALLED_ENODEB_ID,
    IMS_CALLED_CI,
    IMS_CAUSE_FOR_TERM,
    IMS_SMSC_ADDR,
    IMS_END_DT,
    IMS_FAX_DATA_CALL,
    IMS_ICID,
    IMS_WIFI_COUNTRY_INFO,
    IMS_CALLER_IMSI,
    IMS_CALLED_IMSI,
    IMS_CALLER_LAC_3G,
    IMS_CALLER_IC_3G,
    IMS_CALLED_LAC_3G,
    IMS_CALLED_IC_3G,
    IMS_RBT_FLG,
    IMS_SDP_MEDIA_ID,
    IMS_ORI_SCCP_ADDR,
    IMS_MSG_REF,
    IMS_VMSC_ADDR,
    IMS_SMS_RESULT,
    IMS_MAP_FAIL_CAUSE,
    IMS_MAX_CONCAT_SMS,
    IMS_CONCAT_SMS_REF_NO,
    IMS_SEQ_NO_CUR_SMS,
    IMS_CALLER_IMEI,
    IMS_CALLED_IMEI,
    IMS_CAUSE_FOR_FWD,
    NOF_FLD_IMS
} IDX_IMS_FIELD;

/* RDC Type - Added below by Thanakorn on 22-Jan-2016 */
typedef enum {
    RDC_CALL_ST_TIME,
    RDC_MOBILE_NO,
    RDC_IMSI,
    RDC_CELL_ID,
    RDC_NW_TYPE,
    RDC_CALLED_B_NO,
    RDC_CALL_END_TIME,
    RDC_CALL_DUR,
    RDC_UNITS,
    RDC_MSC_ID,
    RDC_CALL_WAIT,
    RDC_CALL_HOLD,
    RDC_CALL_CONF,
    RDC_FAX_DATA_CALL,
    RDC_VISITED_NW,
    RDC_NAT_ROAMING,
    RDC_HANDOVER,
    RDC_CALL_FWD,
    RDC_DROP_CALL,
    RDC_ORI_A_NO,
    RDC_CHRG_IND,
    RDC_IMEI,
    RDC_REAL_B,
    RDC_TERM_FLG,
    RDC_MOD_FLAG,
    RDC_USR_TYPE,
    RDC_AIS_NW_HAND,
    RDC_AIS_NW_DUR,
    RDC_DPC_NW_HAND,
    RDC_DPC_NW_DUR,
    RDC_AWN_NW_HAND,
    RDC_AWN_NW_DUR,
    RDC_TOT_NW_HAND,
    RDC_TOT_NW_DUR,
    RDC_DTAC_NW_HAND,
    RDC_DTAC_NW_DUR,
    RDC_REV_NW_HAND,
    RDC_REV_NW_DUR,
    RDC_RN_CODE,
    RDC_REF_KEY,
    RDC_CALL_CI,
    RDC_ROAMING_NO,
    NOF_FLD_RDC
} IDX_RDC_FIELD;

/* RDC SMS Type - Added below by Thanakorn on 22-Jan-2016 */
typedef enum {
    RDSMS_CALL_ST_TIME,
    RDSMS_MOBILE_NO,
    RDSMS_IMSI,
    RDSMS_NW_TYPE,
    RDSMS_CALLED_B_NO,
    RDSMS_MSC_ID,
    RDSMS_VISITED_NW,
    RDSMS_ROAMING_FLG,
    RDSMS_REAL_A,
    RDSMS_IMEI,
    RDSMS_REAL_B,
    RDSMS_SMSC_ADDR,
    RDSMS_CALLER_LAC,
    RDSMS_CALLER_CI,
    RDSMS_CALLER_TAC,
    RDSMS_CALLER_ECI,
    NOF_FLD_RDCSMS
} IDX_RDCSMS_FIELD;

typedef enum {
    RBM_TAG = 0,                // -- 01 --
    RBM_ACCT_NO,                // -- 02 --
    RBM_EVT_SEQ,                // -- 03 --
    RBM_EVT_SRC,                // -- 04 --
    RBM_CLST_ID,                // -- 05 --
    RBM_EVT_ID,                 // -- 06 --
    RBM_CREATE_DTM,             // -- 07 --
    RBM_EVT_DTM,                // -- 08 --
    RBM_EVT_COST,               // -- 09 --
    RBM_MNG_FILE_ID,            // -- 10 --
    RBM_ROW_NO,                 // -- 11 --
    RBM_COST_CNTRE,             // -- 12 --
    RBM_TARIF_ID,               // -- 13 --
    RBM_DIAL_NO,                // -- 14 --
    RBM_CELL_IDA,               // -- 15 --
    RBM_CELL_IDB,               // -- 16 --
    RBM_DUR,                    // -- 17 --
    RBM_MOD_TYPE,               // -- 18 --
    RBM_MTX_ID,                 // -- 19 --
    RBM_RL_DUR,                 // -- 20 --
    RBM_TIMERATE_NAME,          // -- 21 --
    RBM_PROV_DEST,              // -- 22 --
    RBM_PROV_NAME,              // -- 23 --
    RBM_ZONE_NAME_DST,          // -- 24 --
    RBM_ZONE_NAME_SRC,          // -- 25 --
    RBM_NET_TYPE,               // -- 26 --
    RBM_TARIF_NAME,             // -- 27 --
    RBM_COSTBAND_NAME,          // -- 28 --
    RBM_COSTBAND_DESC,          // -- 29 --
    RBM_NET_TYPE_FLG,           // -- 30 --
    RBM_COSTCODE_B,             // -- 31 --
    RBM_ROAM_FLG,               // -- 32 --
    RBM_ORI_ANO,                // -- 33 --
    RBM_RATE_TAR_DSC,           // -- 34 --
    RBM_INV_COMP,               // -- 35 --
    RBM_RSRV,                   // -- 36 --
    RBM_ADD_INF,                // -- 37 --
    RBM_LOY_PNT,                // -- 38 --
    RBM_COMP_COST,              // -- 39 --
    RBM_INT_COST,               // -- 40 --
    RBM_EXT_COST,               // -- 41 --
    RBM_FRG_NO,                 // -- 42 --
    RBM_COST_MNY,               // -- 43 --
    RBM_CRRNCY_CODE,            // -- 44 --
    RBM_RATE_DISC_TOTAL,        // -- 45 --
    RBM_PREDISC_COST,           // -- 46 --
    RBM_HIPRIOR_DISC,           // -- 47 --
    RBM_TAX_OVRR,               // -- 48 --
    RBM_UST_CAT_ID,             // -- 49 --
    RBM_UST_COD_ID,             // -- 50 --
    RBM_ORI_ACCT_NO,            // -- 51 --
    RBM_RULE_NO,                // -- 52 --
    RBM_MOB_IR,                 // -- 53 --
    RBM_EVT_REF,                // -- 54 --
    RBM_MOD_BOO,                // -- 55 --
    RBM_ATTR_25,                // -- 56 --
    RBM_ATTR_26,                // -- 57 --
    RBM_ATTR_27,                // -- 58 --
    RBM_DHW_ADD_INF,            // -- 59 --
    RBM_ATTR_29,                // -- 60 --
    RBM_ATTR_30,                // -- 61 --
    RBM_ATTR_31,                // -- 62 --
    RBM_ATTR_32,                // -- 63 --
    RBM_ATTR_33,                // -- 64 --
    RBM_EXT_NO,                 // -- 65 --
    RBM_ATTR_35,                // -- 66 --
    RBM_ATTR_36,                // -- 67 --
    RBM_HIPRIOR_PROD,           // -- 68 --
    RBM_RECV_CLASS_ID,          // -- 69 --
    RBM_REVN_COD_ID,            // -- 70 --
    RBM_DISC_DATA,              // -- 71 --
    RBM_PRI_EVT_REF,            // -- 72 --
    RBM_INHOST_COST_BOO,        // -- 73 --
    RBM_HOST_EVT_TYPE,          // -- 74 --
    RBM_CHG_NO,                 // -- 75 --
    RBM_BILL_COST_MNY,          // -- 76 --
    NOF_FLD_RBM
} IDX_RBM_FIELD;

/*----------------------------------------------------------------------------- */
/* Data Structure to hold one Parsed DPCGSM_TYPE, DPCTAC_TYPE or DPCAIS_TYPE */
typedef struct dpcgsm_record {
    char plmn_code[6];
    char record_type[3];
    char imsi_no[SIZE_IMSI+1];
    char b_no[25+8];        /* dialled_digit is renamed to b_no, increase 8 char by N.Thanakorn on 2015-09-15 */
    char a_no[19];
    char service_type[3];
    char service_id[5];
    char msc_id[16];
    char cell_id_a[16];     /* Changed Size to 15 Chars for New MD Format - by Kawee on 22-Oct-2003 */
    char cell_id_b[16];     /* Changed Size to 15 Chars for New MD Format - by Kawee on 22-Oct-2003 */
    char ms_class_mark;
    char start_date[9];     /* YYYYMMDD format */
    char start_time[7];     /* HHMMSS format */
    char duration[7];       /* HHMMSS format */
    char data_vol[7];
    char data_vol_ref[7];
    char tariff_class[5];
    char ms_roaming_num[18];
    char charging_indicator;
    char charging_origin[5];
    char originate_a_no[13];
    char supplementary_code[3];
    char imei_a_b_c;
    char imei[SIZE_IMEI+1];
    char out_tgrp[11];
    char in_tgrp[11];
    char cause_termination[11];
    char dual_serv_type;
    char dual_serv_code[3];
    char channel_req;
    char channel_used;
    char transperancy_ind;
    struct {
        char action_code;
        char ss_code[3];
    } ss_event[5];          /* Table occurs 5 times */
    char call_flag;
    char cost[10];
    char filler[15];
} DPCGSM_RECORD;

/*--------------------------------------------------------------------------------- */
/*
** For better readability, the following field names are changed (chamy on 25-Jan-2001):
**  calling_no          => a_no
**  called_no           => b_no
**  network_type_origin     => net_orig
**  network_type_destination    => net_dest
*/

/* Revised Data Structure to hold Common CDR after 1st Level mapping of GSM/SSP CDR */
typedef struct com_record {
    char a_no[11];
    char start_call_date[9];    /* YYYYMMDD format */
    char start_call_time[7];    /* HHMMSS format */
    char b_no[25+8];            /* increase 8 char by N.Thanakorn on 2015-09-15 */
    char duration[7];           /* HHMMSS format */
    char net_orig[4];
    char net_dest[4];
    char record_type[3];
    char cell_ida[7];
    char cell_idb[7];
    char mtx_id[16];            /* Changed Size from 10 to 15 Chars for New MD Format - by Kawee on 22-Oct-2003 */
    char call_type[3];          /* Supplementary service code */
    char call_transfer[3];
    char imsi_no[SIZE_IMSI+1];
    char msrn[18];
    char serv_type[3];
    char serv_id[3];
    char feature_code[5];
    char feature_type;
    char zone_cellida[4];
    char zone_cellidb[4];
    char zone_type[3];
    char imei[SIZE_IMEI+1];
    char tariff_code[5];        /* added for DPC rating */
    char service[4];            /* Added by Jetsada on 26-Jul-2005 */
    char event_class_id[3];     /* Added by Jetsada on 28-Jul-2005 */
    double charging_rate;       /* Added by Jetsada on 03-Aug-2005 */
} COM_RECORD;

/*----------------------------------------------------------------------------- */

/* Data Structure of HP FMS Version 8.1-1 CDR Record                                     */

#define BooleanType     char
#define UInt4Type       unsigned int
#define UInt2Type       unsigned short int
#define Int2Type        short int
#define EnumIDType      int



/*
** Structure of External Alarm Buffer at runtime.
** External Alarm Record is created in ASCII format (no nulls) and records are terminated with CR.
*/
typedef struct alm_record {
    char partition_id[2+1];             /* Partion ID (02=Postpaid, 04=Prepaid, 08=AIN) */
    char a_no[SIZE_MOBILE_NUM+1];      /* Phone number */
    char cdr_date[SIZE_YYYYMMDD+1];     /* CDR Date YYYYMMDD */
    char cdr_time[SIZE_HHMMSS+1];       /* CDR Time HHMMSS */
    char imsi_no[SIZE_IMSI+1];          /* MIN or IMSI  */
    char imei_no[SIZE_IMEI+1];          /* IMEI  */
    char b_no[26+8];                    /* B number for Call Forward External Alarm detection, increase 8 char by N.Thanakorn on 2015-09-15 */
    char acc_code[3+1];                 /* International Access Code */ /* Added by Kawee on 07-Sep-2004 */
    char country_code[4+1];             /* Country Code */
    char area_code[5+1];                /* Area Code  */
    char is_cf;                         /* Call Forward Flag ('1'= Forwarded Call, '0'= Otherwise) */
    char is_premium;                    /* Premium Flag ('1'= Premium Call, '0'= Otherwise) */
    char is_vas;                        /* VAS Flag ('1'= Value Added Service Call, '0'= Otherwise) */
    char is_incoming;                   /* Incoming Flag ('1'= Incoming Call, '0'= Otherwise) */
    char is_ir;                         /* IR Flag ('1'= IR Call, '0'= Otherwise) */
    char charge[10+1];                  /* Charge */
    char duration[6+1];                 /* Duration */
} ALM_RECORD;

/*
** Structure of VOICE EVENT CDR Record
** VOICE EVENT requires all the fields in ASCII format (no nulls) and each record terminated with CR.
** While writing VOICE EVENT record from the following buffer, nulls would be stripped-off from each field.
*/
typedef struct voice_event {
    char    event_type[2+1];            // Voice Event Type
    char    partition_id[2+1];          // Partion ID (02=Postpaid, 04=AIN), Added partition_id - by Kawee on 06-Feb-2007
    char    mobile_num[16+1];           // Service Number
    char    service_type[2];               // Service Type (HOME,HOST or ROAM), Added by Kawee on 27-Feb-2007
    char    bno[18+1];                  // Bno with access code - added by Thanakorn on Aug-2016 (FRMv12)
    char    from_number[18+1];          // From Mobile Number
    char    to_number[32+1];           // To Mobile Number, increase 14 char by N.Thanakorn on 2015-09-15
    char    time_key[14+1];             // Seize Time & Date
    char    duration[6+1];              // Duration in Seconds
    char    charge[10+1];               // Charge in Satang
    char    country_code[4+1];          // Country Code Called
    char    area_code[5+1];             // Area Code Called (within Thailand)
    char    cell_area[SIZE_CELL_AREA+1];            // Cell Area ID (A-no Location)
    char    cell_set[SIZE_CELL_SET+1];             // Cell Set ID (A-no Location)
    char    cell[SIZE_CELL_ID+1];                 // Cell ID (A-no Location)
    char    remarks[SIZE_VAS_NAME+1];    // Formerly is VAS Name
    char    oper_name[SIZE_OPER+1];     // Operator Network Name, Added by Kawee on 18-Sep-2008
    char    categories[10+1];            // CDR Categories
    char    features[10+1];              // Features used during the call
    char    detect_params[SKIP_DET_STR_SIZE];         // Detection Skip Parameters
    char    imei[SIZE_IMEI+1];                 // IMEI, Added by Thanakorn on 20-Jul-2011 to support IMEI Blacklist
    char    original_bno[80+1];         // Original B_no - added by Thanakorn on 09-Sep-2012 FMS V11
    char    event_src[100+1];           // Event source - added by Thanakorn on 09-Sep-2012 FMS V11
    char    fee_charge[10+1];           // extra charge for maritime call - added by N.Thanakorn on 08-Oct-2014
    char    direction[2+1];
    char    utc_offset[5+1];
} VOICE_EVENT;

/*
** Structure of SMS EVENT CDR Record
** SMS EVENT requires all the fields in ASCII format (no nulls) and each record terminated with CR.
** While writing SMS EVENT record from the following buffer, nulls would be stripped-off from each field.
*/
typedef struct sms_event {
    char    event_type[2+1];            // Sms Event Type
    char    partition_id[2+1];          // Partition Id - added by Thanakorn on 09-Sep-2012 FMS V11
    char    mobile_num[16+1];           // Service Number
    char    from_number[18+1];          // From Mobile Number
    char    to_number[18+1];            // To Mobile Number
    char    time_key[14+1];             // Seize Time & Date
    char    charge[10+1];               // Charge in Satang
    char    direction[2+1];             // Direction
    char    country_code[4+1];          // Country Code Called
    char    categories[10+1];            // CDR Categories
    char    detect_params[SKIP_DET_STR_SIZE];         // Detection Skip Parameters
    char    original_bno[80];           // Original B_no - added by Thanakorn on 09-Sep-2012 FMS V11
    char    event_src[100+1];           // Event source - added by Thanakorn on 09-Sep-2012 FMS V11
    char    bno[18+1];                  // Bno with access code - added by Thanakorn on Aug-2016
    char    area_code[5+1];             // Area Code Called (within Thailand) - added by Thanakorn on Aug-2016
    char    cell_area[SIZE_CELL_AREA+1];    // Cell Area ID (A-no Location) - added by Thanakorn on Aug-2016
    char    cell_set[SIZE_CELL_SET+1];      // Cell Set ID (A-no Location) - added by Thanakorn on Aug-2016
    char    cell[SIZE_CELL_ID+1];           // Cell ID (A-no Location) - added by Thanakorn on Aug-2016
    char    remarks[SIZE_VAS_NAME+1];   // Formerly is VAS Name - added by Thanakorn on Aug-2016
    char    service_id[10+1];           // added by Thanakorn on Aug-2016 (FRMv12)
    char    fee_charge[10+1];           // added by Thanakorn on Aug-2016 (FRMv12)
    char    imei[SIZE_IMEI+1];
    char    utc_offset[5+1];
    char    duration[6+1];
    char    service_type[2];
    char    features[10+1];
} SMS_EVENT;

/*
** Structure of Data EVENT CDR Record
** Data EVENT requires all the fields in ASCII format (no nulls) and each record terminated with CR.
** While writing DATA EVENT record from the following buffer, nulls would be stripped-off from each field.
*/
typedef struct data_event {
    char    event_type[2+1];            // Data Event Type
    char    partition_id[2+1];          // Partition Id - added by Thanakorn on 09-Sep-2012 FMS V11
    char    mobile_num[16+1];           // Service Number
    char    from_number[18+1];          // From Mobile Number
    char    time_key[14+1];             // Seize Time & Date
    char    duration[6+1];              // Duration in Seconds
    char    vol_total[10+1];            // Total Volume in Kbytes
    char    charge[10+1];               // Charge in Satang
    char    cell_area[SIZE_CELL_AREA+1];    // Cell Area ID (A-no Location)
    char    cell_set[SIZE_CELL_SET+1];      // Cell Set ID (A-no Location)
    char    cell[SIZE_CELL_ID+1];           // Cell ID (A-no Location)
    char    categories[10+1];           // CDR Categories
    char    detect_params[SKIP_DET_STR_SIZE];       // Detection Skip Parameters
    char    event_src[100+1];           // Event source - added by Thanakorn on 09-Sep-2012 FMS V11
    char    remarks[SIZE_VAS_NAME+1];   // added by Thanakorn on Aug-2016 (FRMv12)
    char    bno[18+1];
    char    original_bno[80+1];
    char    to_number[32+1];
    char    direction[2+1];
    char    imei[SIZE_IMEI+1];
    char    utc_offset[5+1];
    char    service_type[2];
    char    features[10+1];
} DATA_EVENT;

/*
** Structure of USC EVENT CDR Record
** USC EVENT requires all the fields in ASCII format (no nulls) and each record terminated with CR.
** While writing USC EVENT record from the following buffer, nulls would be stripped-off from each field.
*/
typedef struct usc_event {
    char    event_type[2+1];                    /* Usc Event Type */
    char    mobile_num[16+1];                   /* Service Number */
    char    from_number[18+1];                  /* From Mobile Number */
    char    time_key[14+1];                     /* Seize Time & Date */
    char    charge[10+1];                       /* Charge in Satang */
    char    direction[2+1];                     /* Direction */
    char    service_cat[3+1];                   /* Service Category ID */
    char    content_id[3+1];                    /* Content Provider ID */
    char    bearer_code[1+1];                   /* Bearer Code */
    char    service_id[6+1];                    /* Sub-Service ID */
    char    payment_type[2+1];                  /* Payment Type */
    char    content_code[20+1];                 /* Content Code (Special Field) */
    char    content_value[10+1];                /* Product/Content Value (in satang) */
    char    vas_name[SIZE_VAS_NAME+1];          /* VAS Name */  /* Added by Kawee on 12-Mar-2007 */
    char    categories[10+1];                   /* CDR Categories */
    char    detect_params[SKIP_DET_STR_SIZE];   /* Detection Skip Parameters */
    char    charging_id[11+1];                  // FRM 14
    char    vas_grp[2+1];
} USC_EVENT;

/*
** Structure of MMS EVENT CDR Record
** MMS EVENT requires all the fields in ASCII format (no nulls) and each record terminated with CR.
** While writing MMS EVENT record from the following buffer, nulls would be stripped-off from each field.
*/
typedef struct mms_event {
    char    event_type[2+1];        /* Mms Event Type */
    char    mobile_num[16+1];      /* Service Number */
    char    from_number[18+1];      /* From Mobile Number */
    char    target_object[21+1];    /* Target Object */
    char    time_key[14+1];         /* Seize Time & Date */
    char    data_volume[10+1];      /* Total Message Size */
    char    charge[10+1];           /* Charge in Satang */
    char    service_id[6+1];        /* Sub-Service ID */
    char    service_cat[3+1];       /* Service Category ID */
    char    content_id[3+1];        /* Content Provider ID */
    char    bearer_code[1+1];       /* Bearer Code */
    char    categories[10+1];        /* CDR Categories */
    char    detect_params[SKIP_DET_STR_SIZE];     /* Detection Skip Parameters */
    char    vas_grp[2+1];
} MMS_EVENT;

/*
** Structure of CDG EVENT CDR Record
** CDG EVENT requires all the fields in ASCII format (no nulls) and each record terminated with CR.
** While writing CDG EVENT record from the following buffer, nulls would be stripped-off from each field.
*/
typedef struct cdg_event {
    char event_type[2+1];                   // Cdg Event Type
    char mobile_num[16+1];                  // Service Number
    char from_number[18+1];                 // From Mobile Number
    char app_id[11+1];                      // Application Id
    char time_key[14+1];                    // Seize Time & Date
    char data_volume[10+1];                 // Total Message Size
    char charge[10+1];                      // Charge in Satang
    char content_type[2+1];                 // Content Contained Type
    char service_id[6+1];                   // Sub-Service ID
    char service_cat[3+1];                  // Service Category ID
    char content_id[5+1];                   // Content Provider ID
    char sgw_id[3+1];                       // Service Gateway ID */
    char bearer_code[1+1];                  // Bearer Code
    char vas_name[SIZE_VAS_NAME+1];         // VAS Name - Added by Kawee on 12-Mar-2007
    char categories[10+1];                  // CDR Categories
    char detect_params[SKIP_DET_STR_SIZE];  // Detection Skip Parameters
    char pre_disc[10+1];                    // Pre Discount
    char direction[2+1];                    // Direction (incoming/outgoing)
    char imsi[SIZE_IMSI+1];                 // IMSI
    char vas_desc[100+1];                   // VAS Description (from mapping)
    char short_code[20+1];                  // Short Code (compound fields)
    char charging_id[15+1];                 // Charging Id
    char remarks[SIZE_VAS_NAME+1];
    char vas_grp[2+1];
} CDG_EVENT;

/*
** Structure of RBT EVENT CDR Record
** RBT EVENT requires all the fields in ASCII format (no nulls) and each record terminated with CR.
** While writing RBT EVENT record from the following buffer, nulls would be stripped-off from each field.
*/
typedef struct rbt_event {
    char    event_type[2+1];    /* Rbt Event Type */
    char    mobile_num[16+1];   /* Service Number */
    char    from_number[18+1];  /* From Mobile Number */
    char    ring_no[20+1];      /* Ring Number requested */
    char    time_key[14+1];     /* Seize Time & Date */
    char    charge[10+1];       /* Charge in Satang */
    char    ring_type[3+1];     /* Ring Type */
    char    op_source[1+1];     /* Operation Source */
    char    categories[10+1];    /* CDR Categories */
    char    detect_params[SKIP_DET_STR_SIZE]; /* Detection Skip Parameters */
    char    cos_id[10+1];
    char    vas_grp[2+1];
} RBT_EVENT;

/*
** Structure of SESSION EVENT CDR Record
** SESSION EVENT requires all the fields in ASCII format (no nulls) and each record terminated with CR.
** While writing SESSION EVENT record from the following buffer, nulls would be stripped-off from each field.
*/
typedef struct session_event {
    char    event_type[2+1];            /* Session Event Type */
    char    mobile_num[16+1];          /* Service Number */
    char    from_number[18+1];          /* From Mobile Number */
    char    service_session_id[9+1];    /* Service Session ID */
    char    time_key[14+1];             /* Seize Time & Date */
    char    charge[10+1];               /* Charge in Satang */
    char    service_id[6+1];            /* Service ID */
    char    service_cat[3+1];           /* Service Category */
    char    content_id[3+1];            /* Content ID */
    char    cos[6+1];                   /* Class of Service */
    char    imsi[SIZE_IMSI+1];                 /* IMSI */
    char    ob[1+1];                    /* Old Bearer */
    char    vas_name[SIZE_VAS_NAME+1];  /* VAS Service Name */
    char    categories[10+1];            /* CDR Categories */
    char    detect_params[SKIP_DET_STR_SIZE];         /* Detection Skip Parameters */
    char    vas_grp[2+1];
} SESSION_EVENT;

/*
** Structure of IR EVENT CDR Record
** IR EVENT requires all the fields in ASCII format (no nulls) and each record terminated with CR.
** While writing IR EVENT record from the following buffer, nulls would be stripped-off from each field.
*/
typedef struct ir_event {
    char    event_type[2+1];            // IR Event Type
    char    partition_id[2+1];          // Partition Id
    char    mobile_num[16+1];           // Service Number
    char    from_number[18+1];          // From Mobile Number
    char    to_number[18+1];            // To Mobile Number
    char    time_key[14+1];             // Seize Time & Date
    char    utc_offset[5+1];            // GMT Offset
    char    duration[6+1];              // Duration in Seconds
    char    charge[10+1];               // Charge in Satang
    char    direction[2+1];             // Direction
    char    country_code[4+1];          // Country Code B-Number (Called for MOC, Calling for MTC)
    char    imsi[SIZE_IMSI+1];          // IMSI
    char    vol_total[10+1];            // Total Volume in Kbytes
    char    plmn_code[10+1];            // PLMN Code
    char    categories[10+1];           // CDR Categories
    char    features[10+1];             // Features used during the call
    char    detect_params[SKIP_DET_STR_SIZE];   // Detection Skip Parameters
    char    startdate_gmt7[SIZE_YYYYMMDD+1];
    char    starttime_gmt7[SIZE_HHMMSS+1];
    char    original_bno[80];           // Original B_no - added by Thanakorn on 09-Sep-2012 FMS V11
    char    event_src[100+1];           // Event source - added by Thanakorn on 09-Sep-2012 FMS V11
    char    call_start_time[14+1];
    char    charge_type[2+1];
    char    plmn_name[10+1];            // added by Thanakorn on Aug-2016 (FRMv12)
    char    roam_country[20+1];         // added by Thanakorn on Aug-2016 (FRMv12)
    char    roam_region[20+1];          // added by Thanakorn on Aug-2016 (FRMv12)
    char    imei[SIZE_IMEI+1];          // added by Thanakorn on Aug-2016 (FRMv12)
    char    pre_disc[10+1];             // added by Thanakorn on Aug-2016 (FRMv12)
    char    pre_dura[10+1];             // added by Thanakorn on Aug-2016 (FRMv12)
    char    remarks[SIZE_VAS_NAME+1];   // added by Thanakorn on Aug-2016 (FRMv12)
    char    risk_bno[20+1];             // added by Thanakorn on Aug-2016 (FRMv12)
    char    bno[18+1];
    char    service_type[2];
    char    pre_vol[10+1];
} IR_EVENT;

/*
** Structure of AMF EVENT CDR Record
** AMF EVENT requires all the fields in ASCII format (no nulls) and each record terminated with CR.
** While writing AMF EVENT record from the following buffer, nulls would be stripped-off from each field.
*/
typedef struct amf_event {
    char    event_type[2+1];            // Amf Event Type
    char    mobile_num[16+1];           // Service Number
    char    from_number[18+1];          // From Mobile Number
    char    time_key[14+1];             // Seize Time & Date
    char    charge[10+1];               // Charge in Satang
    char    pack_id[5+1];               // Package Id, Service Code
    char    service_id[6+1];            // Sub-Service Id
    char    cat_id[3+1];                // Service Category Id
    char    con_id[5+1];                // Content Id
    char    cct[2+1];                   // Content Type
    char    bearer_code[1+1];           // Bearer Code
    char    vas_name[SIZE_VAS_NAME+1];  // VAS Name
    char    categories[10+1];            // CDR Categories */
    char    detect_params[SKIP_DET_STR_SIZE];         // Detection Skip Parameters */
    char    direction[2+1];             // Direction (incoming/outgoing)
    char    vas_desc[100+1];            // VAS Description (from mapping)
    char    short_code[20+1];           // Short Code (compound fields)
    char    charging_id[15+1];          // Charging Id
    char    app_id[11+1];
    char    cpid[30+1];
    char    vas_grp[2+1];
} AMF_EVENT;

//
// Structure of VAS EVENT CDR Record
// VAS EVENT requires all the fields in ASCII format (no nulls) and each record terminated with CR.
// While writing VAS EVENT record from the following buffer, nulls would be stripped-off from each field.
//
typedef struct vas_event {
    char    event_type[2+1];            // Vas Event Type
    char    partition_id[2+1];          // Partition Id
    char    mobile_num[16+1];           // Service Number
    char    from_number[18+1];          // From Mobile Number
    char    time_key[14+1];             // Seize Time & Date
    char    charge[10+1];               // Charge in Satang
    char    service_id[6+1];            // Sub-Service ID
    char    cpid[30+1];                 // Content Provider ID
    char    vas_name[SIZE_VAS_NAME+1];  // VAS Name
    char    direction[2+1];             // (USC) Direction
    char    app_id[11+1];               // (CDG) Application Id
    char    imsi[SIZE_IMSI+1];          // (SES) IMSI
    char    categories[10+1];           // CDR Categories=
    char    detect_params[SKIP_DET_STR_SIZE];         // Detection Skip Parameters
    char    event_src[100+1];           // Event source - added by Thanakorn on 09-Sep-2012 FMS V11
    char    vas_desc[100+1];            // added by Thanakorn on 22-Aug-2016 (FRMv12)
    char    short_code[20+1];           // added by Thanakorn on 22-Aug-2016 (FRMv12)
    char    charging_id[15+1];          // added by Thanakorn on 22-Aug-2016 (FRMv12)
    char    bno[18+1];
    char    original_bno[80+1];
    char    to_number[32+1];
    char    imei[SIZE_IMEI+1];
    char    utc_offset[5+1];
    char    duration[6+1];
    char    service_type[2];
    char    service_cat[3+1];
    char    remarks[SIZE_VAS_NAME+1];
    char    features[10+1];
    char    vas_grp[2+1];
} VAS_EVENT;

#ifdef  __cplusplus
    }
#endif

#endif // _FRM_CDR_STRU_H_

