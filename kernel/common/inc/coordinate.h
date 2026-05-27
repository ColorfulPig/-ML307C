#ifndef COORDINATE_H
#define COORDINATE_H


#define MAX_MAC_LEN (100)
#define MAX_ADDR_LEN (4096)

typedef struct _LOCDATA {
	double lat;
	double lon;
    double alt;
    int sta;
    double radius;
    int type;
    char addr[MAX_ADDR_LEN];
    char src[MAX_ADDR_LEN];
    int timestamp;
    int timestamp_all;
}LOCDATA;

 void s_Gcj02ToWgs84(LOCDATA *loc);
 void s_Wgs84ToGcj02(LOCDATA *loc);
 void s_Gcj02ToBd09(LOCDATA *loc);
 void s_Wgs84ToBd09(LOCDATA *loc);
 void Gcj02ToBd09(double *lat, double *lon);
 void Wgs84ToBd09(double *lat, double *lon);
 void Bd09ToWgs84(double *lat, double *lon);
 void Wgs84ToGcj02(double *lat, double *lon);

#endif


