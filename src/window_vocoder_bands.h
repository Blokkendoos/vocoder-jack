/*
 * Vocoder
 * © 2007 Achim Settelmeier <vocoder@m1.sirlab.de>
 *
 * GUI headers: Group of all bands
 */

#ifndef __WINDOW_VOCODER_BANDS_H__
#define __WINDOW_VOCODER_BANDS_H__

#include <FL/Fl.H>
#include <FL/Fl_Group.H>
#include "window_vocoder.fl.h"
#include "window_vocoder_band.h"

class WindowVocoderFltk;

class WindowVocoderBand;
class WindowVocoderBands : public Fl_Group{
public:
	WindowVocoderBands(int x, int y, int w, int h, const char *label=0);
	WindowVocoderBand *getBand(const int bandNr);
	void setCallback(void (*cb)(WindowVocoderEvent *, void *), void *obj);
private:
};

#endif
