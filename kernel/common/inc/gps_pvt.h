#ifndef _ASR_GPS_PVT_H
#define _ASR_GPS_PVT_H

int asr_gps_start_pvtupdate(unsigned int sectionid, void *buff, unsigned int len);
int asr_gps_update_pvtimage(void *buff);
int asr_gps_clear_pvtimage( void );
int asr_gps_save_pvt(void *buff);


#endif  /* _ASR_GPS_PVT_H */
