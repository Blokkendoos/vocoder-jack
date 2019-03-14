/*
 * Vocoder
 * © 2007 Achim Settelmeier <vocoder@m1.sirlab.de>
 *
 */

// Includes /*fold00*/
#include <iostream>
#include "window_vocoder_band.h"

// WindowVocoderBand /*fold00*/
WindowVocoderBand::WindowVocoderBand(int x, int y, int w, int h, const char *label):Fl_Group(x, y, w, h, label),id(-1),cbChanged(NULL),cbChangedObj(NULL){
}

// reset /*fold00*/
void WindowVocoderBand::reset(){
	Fl_Slider *slider=(Fl_Slider *)this->child(0);
	Fl_Dial *dial=(Fl_Dial *)this->child(1);
	
	slider->bounds(0.0, 1.0);
	dial->bounds(-1.0, 1.0);
	setVolume(1.0);
	setPanning(0.0);
}

// setLabel /*fold00*/
void WindowVocoderBand::setLabel(const char *label){
	Fl_Slider *slider=(Fl_Slider *)this->child(0);
	slider->copy_label(label);
}

// setId /*fold00*/
void WindowVocoderBand::setId(const int id){
	this->id=id;
}

// setVolume /*fold00*/
void WindowVocoderBand::setVolume(const double volume){
	this->volume=volume;
	
	Fl_Slider *slider=(Fl_Slider *)this->child(0);
	slider->value(1.0-volume);
	changed();
}

// setPanning /*fold00*/
void WindowVocoderBand::setPanning(const double panning){
	this->panning=panning;
	
	Fl_Dial *dial=(Fl_Dial *)this->child(1);
	dial->value(panning);
	changed();
}

// getId /*fold00*/
int WindowVocoderBand::getId(){
	return id;
}

// getVolume /*fold00*/
double WindowVocoderBand::getVolume(){
	Fl_Slider *slider=(Fl_Slider *)this->child(0);
	return 1.0-slider->value();
}

// getPanning /*fold00*/
double WindowVocoderBand::getPanning(){
	Fl_Dial *dial=(Fl_Dial *)this->child(1);
	return dial->value();
}

// setCallback /*fold00*/
void WindowVocoderBand::setCallback(void (*cb)(WindowVocoderEvent *, void *), void *obj){
	cbChanged=cb;
	cbChangedObj=obj;
}

// changed /*fold00*/
void WindowVocoderBand::changed(){
	if(!cbChanged) return;
	
	// call callback function everytime a value has changed
	WindowVocoderEvent event;
	event.setBandChangeEvent(id);
	cbChanged(&event, cbChangedObj);
}
