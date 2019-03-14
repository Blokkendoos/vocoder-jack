/*
 * Vocoder
 * © 2007 Achim Settelmeier <vocoder@m1.sirlab.de>
 *
 */

// Includes /*fold00*/
#include <iostream>
#include "vocoder_controller.h"
#include "window_vocoder.h"
#include "window_vocoder_event.h"
#include "output.h"

// windowVocoderEventCallback /*fold00*/
void windowVocoderEventCallback(WindowVocoderEvent *event, void *obj){
	((VocoderController *)obj)->callback(event);
}

// VocoderController /*fold00*/
VocoderController::VocoderController(WindowVocoder *gui, Output *output):vocoder(NULL){
	this->gui=gui;
	this->output=output;
	vocoder=output->getVocoder();
	
	// we want to get notified whenever something interesting happens
	gui->setEventCallback(windowVocoderEventCallback, this);
}

// callback /*fold00*/
void VocoderController::callback(WindowVocoderEvent *event){
	switch(event->getEventType()){
	case WindowVocoderEvent::EventTypeBandChange:
		doUpdateBand(event->getBandChangeEvent());
		break;
	case WindowVocoderEvent::EventTypeTimerVumeterUpdate:
		doVumeterUpdate();
		break;
	default:
		break;
	}
}

// doUpdateBand /*fold00*/
void VocoderController::doUpdateBand(const unsigned int bandnr){
	// get volume and panning from band GUI element and
	// set the vocoder's band volume accordingly
	WindowVocoderBand *band=gui->getBands()->getBand(bandnr);
	
	double volume_left, volume_right;
	volume_left=volume_right=band->getVolume();
	double panning=band->getPanning();
	if(panning>0.0){
		volume_left=volume_left*(1.0-panning);
	}else if(panning<0.0){
		volume_right=volume_right*(1.0+panning);
	}
	vocoder->setBandVolume(bandnr, volume_left, volume_right);
}

// doVumeterUpdate /*fold00*/
void VocoderController::doVumeterUpdate(){
	struct vumeter *vumeter=vocoder->getVumeter();
	gui->updateVumeter(vumeter);
}
