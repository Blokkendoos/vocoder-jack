/*
 * Vocoder
 * © 2007 Achim Settelmeier <vocoder@m1.sirlab.de>
 *
 * GUI headers: VUmeter
 */

#ifndef __WINDOW_VOCODER_VUMETER_H__
#define __WINDOW_VOCODER_VUMETER_H__

#include <FL/Fl.H>
#include <FL/Fl_Group.H>
#include "window_vocoder.fl.h"


class WindowVocoderVumeter : public Fl_Group{
public:
	WindowVocoderVumeter(int x, int y, int w, int h, const char *label=0);
	void draw();
	void setVolume(const double volume);
	void setDirection(const bool horizontal);
	void setTilesize(const unsigned int tilesize);
	
private:
	double volume;
	unsigned int tilesize;
	bool horizontal;
};

#endif
