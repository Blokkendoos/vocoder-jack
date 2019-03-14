/*
 * Vocoder
 * © 2007 Achim Settelmeier <vocoder@m1.sirlab.de>
 *
 */

// Includes /*fold00*/
#include "window_vocoder_event.h"

// WindowVocoderEvent /*fold00*/
WindowVocoderEvent::WindowVocoderEvent():type(EventTypeNop){
}

// getEventType /*fold00*/
WindowVocoderEvent::WindowVocoderEventType WindowVocoderEvent::getEventType(){
	return type;
}

// setBandChangeEvent /*fold00*/
void WindowVocoderEvent::setBandChangeEvent(unsigned int bandnr){
	type=EventTypeBandChange;
	event.bandChange.bandnr=bandnr;
}

// getBandChangeEvent /*fold00*/
unsigned int WindowVocoderEvent::getBandChangeEvent(){
	if(type!=EventTypeBandChange)
		return 0;
	return event.bandChange.bandnr;
}

// setTimerVumeterUpdate /*fold00*/
void WindowVocoderEvent::setTimerVumeterUpdate(){
	type=EventTypeTimerVumeterUpdate;
}

// setExit /*fold00*/
void WindowVocoderEvent::setExit(){
	type=EventTypeExit;
}

