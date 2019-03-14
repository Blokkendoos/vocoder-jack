/*
 * Vocoder
 * © 2007 Achim Settelmeier <vocoder@m1.sirlab.de>
 *
 * vocoder core routine
 */

#ifndef __VOCODEr_H__
#define __VOCODEr_H__

#include <jack/jack.h>

#define VOCODER_MAXBANDS 16

typedef jack_default_audio_sample_t sample_t;
typedef jack_nframes_t nframes_t;


struct bandpass_t{
	sample_t low1, low2;
	sample_t mid1, mid2;
	sample_t high1, high2;
	sample_t y;
};

struct band_t{
	double volumeLeft, volumeRight;
	
	double freq;
	double c;
	double f;
	double att;
	
	double oldval;
	double decay;
	
	bandpass_t formant;
	bandpass_t carrier;
};

struct vumeter{
	double formant;
	double carrier;
	double band[VOCODER_MAXBANDS];
};

class Vocoder{
public:
	Vocoder();
	void process(sample_t *formant, 
		     sample_t *carrier,
		     sample_t *outLeft, 
		     sample_t *outRight,
		     const nframes_t nframes);
	
	void setSampleRate(const nframes_t sampleRate);
	void setBandVolume(const unsigned int bandNr, 
			   const double volumeLeft, 
			   const double volumeRight);
	struct vumeter *getVumeter();
	
private:
	void init();
	static void doBandpass(band_t *b, bandpass_t *bp, sample_t sample);
	
	band_t bands[VOCODER_MAXBANDS];
	nframes_t sampleRate;
	double volumeMain;
	struct vumeter vumeter;
};


#endif
