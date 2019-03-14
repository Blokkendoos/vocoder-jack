/*
 * Vocoder
 * © 2007 Achim Settelmeier <vocoder@m1.sirlab.de>
 *
 */

// Includes /*fold00*/
#include <string.h>
#include <iostream>
#include <math.h>
#include "vocoder.h"

// Vocoder /*fold00*/
Vocoder::Vocoder(){
	volumeMain=16.0;
	setSampleRate(48000);
	
	vumeter.formant=0.0;
	vumeter.carrier=0.0;
	for(unsigned int i=0; i<VOCODER_MAXBANDS; i++){
		vumeter.band[i]=0.0;
	}
}

// init /*fold00*/
void Vocoder::init(){
	for(int i=0; i<VOCODER_MAXBANDS; i++){
		memset(&bands[i], 0, sizeof(band_t));
		bands[i].volumeLeft=1.0;
		bands[i].volumeRight=1.0;
		bands[i].oldval=0.0;
		bands[i].decay=1/(100.0+i*10.0);
		
		if(i<4){
			bands[i].freq=150+420*i/4.0;
		}else{
			bands[i].freq=600*pow(1.23,i-4.0);
		}
		double steps=bands[i].freq/(double)sampleRate;
		double c=steps*2*M_PI;
		bands[i].c=c*c;
		bands[i].f=0.4/c;
		bands[i].att=1/(6.0+((exp(steps)-1)*10.0));
		
		bands[i].formant.low1=0.0;
		bands[i].formant.low2=0.0;
		bands[i].formant.mid1=0.0;
		bands[i].formant.mid2=0.0;
		bands[i].formant.high1=0.0;
		bands[i].formant.high2=0.0;
		bands[i].formant.y=0.0;
		
		bands[i].carrier.low1=0.0;
		bands[i].carrier.low2=0.0;
		bands[i].carrier.mid1=0.0;
		bands[i].carrier.mid2=0.0;
		bands[i].carrier.high1=0.0;
		bands[i].carrier.high2=0.0;
		bands[i].carrier.y=0.0;
	}
}


// setSampleRate /*fold00*/
void Vocoder::setSampleRate(const nframes_t _sampleRate){
	if(sampleRate==_sampleRate) return;
	
	sampleRate=_sampleRate;
	init();
}

// setBandVolume /*fold00*/
void Vocoder::setBandVolume(unsigned int bandNr,
			    double volumeLeft,
			    double volumeRight){
	bands[bandNr].volumeLeft=volumeLeft;
	bands[bandNr].volumeRight=volumeRight;
}

// process /*fold00*/
void Vocoder::process(sample_t *formant, 
		      sample_t *carrier, 
		      sample_t *outLeft, 
		      sample_t *outRight,
		      const nframes_t nframes){
	
	for(nframes_t pos=0; pos<nframes; pos++){
		sample_t l=0.0;
		sample_t r=0.0;
		sample_t x;
		
		for(int i=0; i<VOCODER_MAXBANDS; i++){
			Vocoder::doBandpass(&bands[i], &bands[i].formant, formant[pos]);
			Vocoder::doBandpass(&bands[i], &bands[i].carrier, carrier[pos]);
			
			bands[i].oldval+=(fabs(bands[i].formant.y)-bands[i].oldval)*bands[i].decay;
			x=bands[i].carrier.y*bands[i].oldval;
			l+=x*bands[i].volumeLeft;
			r+=x*bands[i].volumeRight;
		}
		
		outLeft[pos]=l*volumeMain;
		outRight[pos]=r*volumeMain;
		
		vumeter.formant=vumeter.formant*0.999+fabs(formant[pos])*0.003;
		vumeter.carrier=vumeter.carrier*0.999+fabs(carrier[pos])*0.003;
	}
	
	// for testing: 
	//memcpy(outLeft, formant, sizeof(jack_default_audio_sample_t) * nframes);
	//memcpy(outRight, carrier, sizeof(jack_default_audio_sample_t) * nframes);
}

// doBandpass /*fold00*/
void Vocoder::doBandpass(band_t *b, bandpass_t *bp, sample_t sample){
	bp->high1=sample - b->f * bp->mid1 - bp->low1;
	bp->mid1+=bp->high1 * b->c;
	bp->low1+=bp->mid1;
	
	bp->high2=bp->low1 - b->f * bp->mid2 - bp->low2;
	bp->mid2+=bp->high2 * b->c;
	bp->low2+=bp->mid2;
	bp->y=bp->high2 * b->att;
}


// getVumeter /*fold00*/
struct vumeter *Vocoder::getVumeter(){
	return &vumeter;
}
