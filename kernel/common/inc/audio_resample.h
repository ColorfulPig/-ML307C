#ifndef __AUDIORESAMPLE_H__
#define __AUDIORESAMPLE_H__

typedef struct resampler
{
    double	 factor;	/* Conversion factor = rate_out / rate_in.  */
    int	     large_filter;	/* Large filter?			    */
    int  	 high_quality;	/* Not fast?				    */
    unsigned	 xoff;		/* History and lookahead size, in samples   */
    unsigned	 frame_size;	/* Samples per frame.			    */
    short	*buffer;	/* Input buffer.			    */
}resampler;


extern int resampler_create(int high_quality,
					     int large_filter,
					     unsigned rate_in,
					     unsigned rate_out,
					     unsigned samples_per_frame,
					     resampler *p_resampler);

extern void resampler_run( resampler *resample,
				       short *input,
				       short *output );

extern void resampler_destroy(resampler *resample);

#endif

