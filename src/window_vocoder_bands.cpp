/*
 * Vocoder
 * © 2007 Achim Settelmeier <vocoder@m1.sirlab.de>
 *
 */

// Includes /*fold00*/
#include <iostream>
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include "vocoder.h"
#include "window_vocoder_bands.h"
#include "window_vocoder_band.h"

// WindowVocoderBands /*fold00*/
WindowVocoderBands::WindowVocoderBands(int x, int y, int w, int h, const char *label):Fl_Group(x, y, w, h, label){
}

// setCallback /*fold00*/
void WindowVocoderBands::setCallback(void (*cb)(WindowVocoderEvent *event, void *), void *obj){
	// add callback to all bands
	for(unsigned int i=0; i<VOCODER_MAXBANDS; i++){
		getBand(i)->setCallback(cb, obj);
	}
}

// getBand /*fold00*/
WindowVocoderBand *WindowVocoderBands::getBand(const int bandNr){
	return (WindowVocoderBand *)this->child(bandNr);
}
