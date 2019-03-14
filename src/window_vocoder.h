/*
 * Vocoder
 * © 2007 Achim Settelmeier <vocoder@m1.sirlab.de>
 *
 * GUI headers
 */

#ifndef __WINDOW_VOCODER_H__
#define __WINDOW_VOCODER_H__

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include "thread.h"
#include "window_vocoder.fl.h"
#include "window_vocoder_event.h"
#include "window_vocoder_bands.h"

class WindowVocoderFltk;
class WindowVocoderBands;

class WindowVocoder : public Thread{
public:
	WindowVocoder(int argc, char **argv);
	~WindowVocoder();
	void setEventCallback(void (*cb)(WindowVocoderEvent *, void *), void *obj);
	WindowVocoderBands *getBands();
	void run();
	void updateVumeter(struct vumeter *vumeter);
	void doCallback(WindowVocoderEvent *event);
	void doTimerEvent();
	static void timerEvent(void *obj);
	
private:
	Fl_Double_Window* window;
	WindowVocoderFltk *windowVocoderFltk;
	bool exitFlag;
	int mainArgc;
	char **mainArgv;
	
	void (*cbEvent)(WindowVocoderEvent *event, void *);
	void *cbEventObj;
};

#endif
