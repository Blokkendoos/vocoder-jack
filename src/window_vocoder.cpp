/*
 * Vocoder
 * © 2007 Achim Settelmeier <vocoder@m1.sirlab.de>
 *
 */

// Includes /*fold00*/
#include <iostream>
#include "window_vocoder.h"
#include "window_vocoder_band.h"
#include "window_vocoder_bands.h"
#include "window_vocoder_event.h"
#include "vocoder.h"

// WindowVocoder /*fold00*/
WindowVocoder::WindowVocoder(int argc, char **argv):mainArgc(argc),mainArgv(argv),cbEvent(NULL){
	exitFlag=false;
	windowVocoderFltk=new WindowVocoderFltk(this);
	window=windowVocoderFltk->makeWindow();
	
	WindowVocoderBands *bands=windowVocoderFltk->bands;
	
	// create bands and add them to the container
	for(unsigned int i=0; i<VOCODER_MAXBANDS; i++){
		Fl_Double_Window *w=windowVocoderFltk->makeBand();
		WindowVocoderBand *band=(WindowVocoderBand *)w->child(0);
		bands->add(band);
		band->position(bands->x()+i*band->w(), bands->y());
		
		band->setId(i);
		char label[20];
		snprintf(label, sizeof(label), "#%i", i+1);
		band->setLabel(label);
		band->reset();
	}
	timerEvent(this);
}

// ~WindowVocoder /*fold00*/
WindowVocoder::~WindowVocoder(){
	if(window){
		window->hide();
		window=NULL;
	}
}

// run /*fold00*/
void WindowVocoder::run(){
	// time to show off :-)
	window->show(mainArgc, mainArgv);
	
	// let FLTK do the dirty work for us
	while(Fl::wait() && !exitFlag);
}

// setEventCallback /*fold00*/
void WindowVocoder::setEventCallback(void (*cb)(WindowVocoderEvent *, void *), void *obj){
	windowVocoderFltk->bands->setCallback(cb, obj);
	cbEvent=cb;
	cbEventObj=obj;
	doTimerEvent();
}

// doCallback /*fold00*/
void WindowVocoder::doCallback(WindowVocoderEvent *event){
	if(event->getEventType()==WindowVocoderEvent::EventTypeExit){
		exitFlag=true;
	}
	
	if(!cbEvent)
		return;
	
	cbEvent(event, cbEventObj);
}

// updateVumeter /*fold00*/
void WindowVocoder::updateVumeter(struct vumeter *vumeter){
	windowVocoderFltk->vumeterFormant->setVolume(vumeter->formant);
	windowVocoderFltk->vumeterCarrier->setVolume(vumeter->carrier);
}

// getBands /*fold00*/
WindowVocoderBands *WindowVocoder::getBands(){
	return windowVocoderFltk->bands;
}

// doTimerEvent /*fold00*/
void WindowVocoder::doTimerEvent(){
	if(!cbEvent)
		return;
	
	WindowVocoderEvent e;
	e.setTimerVumeterUpdate();
	doCallback(&e);
	Fl::add_timeout(1/25.0, timerEvent, this);
}

// timerEvent /*fold00*/
void WindowVocoder::timerEvent(void *obj){
	((WindowVocoder*)obj)->doTimerEvent();
}
