/*
 * Vocoder
 * © 2007 Achim Settelmeier <vocoder@m1.sirlab.de>
 *
 * GUI headers: Control elements for one vocoder band
 */

#ifndef __WINDOW_VOCODER_BAND_H__
#define __WINDOW_VOCODER_BAND_H__

#include <FL/Fl.H>
#include <FL/Fl_Group.H>
#include "window_vocoder_event.h"
#include "window_vocoder.fl.h"

class WindowVocoderFltk;

class WindowVocoderBand : public Fl_Group{
public:
	WindowVocoderBand(int x, int y, int w, int h, const char *label=0);
	void reset(void);
	void setLabel(const char *label);
	void setId(int id);
	void setVolume(const double volume);
	void setPanning(const double panning);
	int getId();
	double getVolume();
	double getPanning();
	void changed();
	void setCallback(void (*cb)(WindowVocoderEvent *event, void *), void *obj);
	
private:
	double volume;
	double panning;
	int id;
	void (*cbChanged)(WindowVocoderEvent *event, void *);
	void *cbChangedObj;
};

#endif
