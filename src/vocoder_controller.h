/*
 * Vocoder
 * © 2007 Achim Settelmeier <vocoder@m1.sirlab.de>
 *
 * Vocoder Controller
 */

#ifndef __VOCODER_CONTROLLER_H__
#define __VOCODER_CONTROLLER_H__

#include "vocoder.h"
#include "output.h"
#include "window_vocoder.h"
#include "window_vocoder_event.h"


class VocoderController{
public:
	VocoderController(WindowVocoder *gui, Output *output);
	void callback(WindowVocoderEvent *event);
private:
	void doVumeterUpdate();
	void doUpdateBand(const unsigned int bandnr);
	
	WindowVocoder *gui;
	Output *output;
	Vocoder *vocoder;
};

#endif
