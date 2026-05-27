#ifndef _ASR_GPS_CASAID_H
#define _ASR_GPS_CASAID_H
#ifdef AGPS_ENABLE
#include "supl.h"
#elif defined AGPSTP_ENABLE
#include "asr_agps_api.h"
#endif

#define BIN_HEADER0						0xBA
#define BIN_HEADER1						0xCE

#define ID_AID_INIT						 0x01
#define ID_RXM_GPS_UTC					 0x02
#define ID_RXM_GPS_EPH					 0x03
#define ID_RXM_GPS_ION					 0x04
#define ID_RXM_BDS_UTC					 0x05
#define ID_RXM_BDS_EPH					 0x06
#define ID_RXM_GLO_EPH					 0x07
#define ID_RXM_GPS_ALM					 0x08
#define ID_RXM_BDS_ALM					 0x09
#define ID_RXM_GLO_ALM					 0x0A
#define ID_AID_INIP						 0x0B
#define ID_RXM_BDS_ION                   0x0C
#define ID_RXM_ALM_END					 0x0D
#define ID_AID_INITC					 0x0E
#define ID_AID_MCC                       0x0F

#define MAX_EPHEMERIS 		32

/* 电文有效标志    */
typedef enum _ME_MESSAGE_FLAG_ENUM
{
        NAVIGATION_MESSAGE_ABSENCE,						/* 缺省      */
        NAVIGATION_MESSAGE_UNHEALTHY,					/* 不健康    */
        NAVIGATION_MESSAGE_OUTOFDATE,					/* 过期      */
        NAVIGATION_MESSAGE_AVAILABLE					/* 有效      */

} ME_MESSAGE_FLAG_ENUM;

/* GPS定点开普勒轨道参数-星历   */
typedef struct _GPS_FIX_KEPLER_EPH_STR
{
        unsigned int		sqra;				
        unsigned int		es;					
        int				w;					
        int				m0;					
        int				i0;					
        int				omega0;				
        int				omegadot;			
        short int			deltn;				
        short int			idot;				
        short int			cuc;				
        short int			cus;				
        short int			crc;				
        short int			crs;				
        short int			cic;				
        short int			cis;				
        unsigned int		toe;				
        unsigned short int	wne;
		unsigned short int	padding;
} GPS_FIX_KEPLER_EPH_STR;

/* GPS定点时钟修正参数   */
typedef struct _GPS_FIX_SV_CLOCK_STR
{
        unsigned int		toc;				
        int				af0;				
        short int			af1;				
        signed char		af2;				
        signed char		tgd;				
} GPS_FIX_SV_CLOCK_STR;

/* GPS 定点星历   */
typedef struct _GPS_FIX_EPHEMERIS_STR
{
        GPS_FIX_KEPLER_EPH_STR		kepler;
		unsigned short int			iodc;	/* replace padding for kepler struct. */
        unsigned short int			iode;
        GPS_FIX_SV_CLOCK_STR		svClock;
        unsigned char				ura;
        unsigned char				health;
        unsigned char				svid;
        unsigned char				valid;

} GPS_FIX_EPHEMERIS_STR;

/* BDS定点开普勒轨道参数-星历   */
typedef struct _BDS_FIX_KEPLER_EPH_STR
{
        unsigned int		sqra;				
        unsigned int		es;					
        int				w;					
        int				m0;					
        int				i0;					
        int				omega0;				
        int				omegadot;			
        short int			deltn;				
        short int			idot;				
        int			cuc;				
        int			cus;				
        int			crc;				
        int			crs;				
        int			cic;				
        int			cis;				
        unsigned int	toe;				
        unsigned short	wne;	
        unsigned short	padding;				
} BDS_FIX_KEPLER_EPH_STR;

/* BDS定点时钟修正参数   */
typedef struct _BDS_FIX_SV_CLOCK_STR
{
        unsigned int		toc;				
        int				af0;				
        int			af1;				
        signed short		af2;				
        signed short		tgd;				
} BDS_FIX_SV_CLOCK_STR;

/* BDS 定点星历   */
typedef struct _BDS_FIX_EPHEMERIS_STR
{
        BDS_FIX_KEPLER_EPH_STR		kepler;
        BDS_FIX_SV_CLOCK_STR		svClock;
        unsigned short int			iodc;
        unsigned char				iode;
        unsigned char				ura;
        unsigned char				health;
        unsigned char				svid;
        unsigned char				valid;
        unsigned char				padding;
        unsigned char				padding1;
} BDS_FIX_EPHEMERIS_STR;

/* GLONASS 定点星历   */
typedef struct _GLO_FIX_EPHEMERIS_STR
{
        int						taon;
        int						x;
        int						y;
        int						z;
        int						dx;
        int						dy;
        int						dz;
        int 					taoc;
		int						taoGPS;
		short int				gamma;
		unsigned short int		tk;
		unsigned short int		nt;
		char 					ddx;
		char					ddy;
		char					ddz;
		char					dtaon;
		unsigned char			bn;
		unsigned char			tb;
		unsigned char			M;
		unsigned char			P;
		unsigned char			ft;
		unsigned char			en;
		unsigned char			p1;
		unsigned char			p2;
		unsigned char			p3;
		unsigned char			p4;
		unsigned char			ln;
		unsigned char			n4;
		unsigned char			svid;
		unsigned char			nl;
		unsigned char			valid;		
} GLO_FIX_EPHEMERIS_STR;

/* GPS-BDS ALM */
typedef struct _GPS_BDS_ALMANAC_STR
{
	unsigned int				e;
	int							omegadot;
	unsigned int				sqrta;
	int							omega0;
	int							omega;
	int 						m0;
	short int 					af0;
	short int					af1;
	short int					deltal;
	unsigned short int			health;
	unsigned char 				svid;
	unsigned char				wn;
	unsigned char				toa;
	unsigned char 				flag;
}GPS_BDS_ALMANAC_STR;

/* GLO ALM */
typedef struct _GLO_ALMANAC_STR
{
	int							lambda;
	unsigned int				tlambda;
	int							delta;
	int							deltaT;
	unsigned short int			epsilon;
	short int 					omega;
	short int					tau;
	unsigned short int 			NA;
	unsigned char				na;
	unsigned char 				hna;
	char						deltaTdot;
	unsigned char				ca;
	unsigned char				ma;
	unsigned char				flag;
}GLO_ALMANAC_STR;

/* 定点UTC-GPS UTC  */
typedef struct _FIX_UTC_STR
{
        int				a0;						
        int				a1;						
        signed char		dtls;					
        signed char		dtlsf;					
        unsigned char		tot;					
        unsigned char		wnt;					
        unsigned char		wnlsf;					
        unsigned char		dn;						
        unsigned char		valid;
} FIX_UTC_STR;

/* BDS ADD UTC */
typedef struct _FIX_ADD_UTC_STR
{
        int				a0;						
        int				a1;						
        signed char		dtls;					
        signed char		dtlsf;					
        unsigned char		wnlsf;					
        unsigned char		dn;						
        unsigned char		valid;
        unsigned char		padding0;
		unsigned char		padding1;
		unsigned char		padding2;
} FIX_ADD_UTC_STR;

/* 8参数定点电离层-GPS和BD2卫星  */
typedef struct _FIX_IONO_STR
{
        signed char			alpha0;					
        signed char			alpha1;					
        signed char			alpha2;					
        signed char			alpha3;					
        signed char			beta0;					
        signed char			beta1;					
        signed char			beta2;					
        signed char			beta3;					
        unsigned char			valid;
} FIX_IONO_STR;

/* 辅助信息，位置、时间、频率*/
typedef struct  _AID_INIT_STR
{
        double						tow;
        double						df;
        double						tAcc;
        double						fAcc;             
		int tm_year; // since 1970		
		int tm_mon;   //month of year [1,12]
		int tm_mday;  //day of month [1,31]
		int tm_hour;  //hour [0,23]
		int tm_min;	//minutes [0,59]
		int tm_sec;	//seconds [0,59]
		int tm_msec; //utc miliSecond
		unsigned short int			wn;
		unsigned char				timeSource;
		unsigned char				flags;
} AID_INIT_STR;

typedef struct  _AID_MCC_STR
{
		unsigned short int      mcc;
		unsigned short int      flags;
} AID_MCC_STR;

/* 辅助信息，位置、时间、频率*/
typedef struct  _AID_INIP_STR
{
        double						xOrLat;
        double						yOrLon;
        double						zOrAlt;
        double						posAcc;
        unsigned char				flags;
} AID_INIP_STR;

typedef struct
{
        double		xOrLat, yOrLon, zOrAlt;
        float			df;
        float			posAcc;
        float			tAcc;
        float			fAcc;
        int			gnssMask;
        int			posFlags;	             /* 0->无效  1->ECEF坐标系 2->LIA坐标系   */  
        int			reqMode;	             /* 0->全部信息 1->可见信息		         */
        int			reqMask;
        int			year;
        int			month;
        int			day;
        int			hour;
        int			minute;
        int			second;
        float			ms;
} AID_REQ_STR;


unsigned int cas_make_msg(int id, unsigned char *msg, int n, unsigned char *buff);
void supl2cas_gps_eph(unsigned short wn, struct supl_ephemeris_s *eph_ctx, GPS_FIX_EPHEMERIS_STR *cas_eph);
void supl2cas_bds_eph(unsigned short wn, struct supl_ephemeris_bds_s *eph_ctx, BDS_FIX_EPHEMERIS_STR *cas_eph);
void supl2cas_glo_eph(struct supl_ephemeris_glo_s *eph_ctx, GLO_FIX_EPHEMERIS_STR *cas_eph);
void supl2cas_init(supl_assist_t *ctx, AID_INIT_STR *cas_int);
void supl2cas_inip(supl_assist_t *ctx, AID_INIP_STR *cas_init);
void supl2cas_utc(struct supl_utc_s *utc_ctx, FIX_UTC_STR *cas_utc);
void supl2cas_iono(struct supl_ionospheric_s *iono_ctx, FIX_IONO_STR *cas_iono);
void supl2cas_gps_alm(int wn, struct supl_almanac_s *alm_ctx, GPS_BDS_ALMANAC_STR *cas_alm);
void supl2cas_bds_alm(int wn, struct supl_almanac_bds_s *alm_ctx, GPS_BDS_ALMANAC_STR *cas_alm);
void supl2cas_glo_alm(int wn, struct supl_almanac_glo_s *alm_ctx, GLO_ALMANAC_STR *cas_alm);
void supl2cas_mcc(unsigned short int mcc, AID_MCC_STR *cas_mcc);

#ifdef AGPS_ENABLE
void supl2cas_add_utc( struct SUPLGANSSAddUTCModel *utc_add, FIX_ADD_UTC_STR *cas_add_utc);
#elif defined AGPSTP_ENABLE
void supl2cas_qzs_eph(unsigned short wn, struct supl_ephemeris_qzs_s *eph_ctx, GPS_FIX_EPHEMERIS_STR *cas_eph);
void supl2cas_qzs_alm(unsigned char wn, unsigned char qzs_toa, struct supl_almanac_qzs_s *alm_ctx, GPS_BDS_ALMANAC_STR *cas_alm);
void supl2cas_initc(struct reftime *time, AID_INIT_STR *cas_init);
#endif
#endif  /* _ASR_GPS_CASAID_H */
