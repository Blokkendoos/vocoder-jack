/*
 * Vocoder
 * © 2007 Achim Settelmeier <vocoder@m1.sirlab.de>
 *
 */

// Includes /*fold00*/
#include <math.h>
#include <iostream>
#include <FL/fl_draw.H>
#include "window_vocoder_vumeter.h"

// WindowVocoderVumeter /*fold00*/
WindowVocoderVumeter::WindowVocoderVumeter(int x, int y, int w, int h, const char *label):Fl_Group(x, y, w, h, label){
	volume=0.0;
	horizontal=true;
	tilesize=20;
}

// draw /*fold00*/
void WindowVocoderVumeter::draw(){
	const struct {
		unsigned char r, g, b;
	} colors[6]={
		{   0, 255,   0}, 
		{   0, 255,   0}, 
		{   0, 255,   0}, 
		{   0, 255,   0}, 
		{ 255, 255,   0}, 
		{ 255,   0,   0}, 
	};
	
	fl_rectf(x(), y(), w(), h(), 0, 0, 0);
	
	if(horizontal){
		int numtiles=(int((w()-2)/tilesize));
		const int offset=1+w()-2-numtiles*tilesize;
		for(int i=0; i<int(numtiles*volume+0.8); i++){
			if(i==0){
				fl_rectf(x()+1, y()+1,
					 offset+tilesize-3, h()-2,
					 colors[0].r,
					 colors[0].g,
					 colors[0].b);
				continue;
			}
			unsigned int colpos=int(6*i/numtiles);
			fl_rectf(x()+offset+i*tilesize, y()+1, 
				 tilesize-2, h()-2, 
				 colors[colpos].r, 
				 colors[colpos].g, 
				 colors[colpos].b);
		}
	}else{
		int numtiles=(int((h()-2)/tilesize));
		const int offset=1+h()-2-numtiles*tilesize;
		for(int i=0; i<int(numtiles*volume+0.8); i++){
			if(i==0){
				fl_rectf(x()+1, y()+h()-offset-tilesize,
					 w()-2, offset+tilesize-1, 
					 colors[0].r,
					 colors[0].g,
					 colors[0].b);
				continue;
			}
			unsigned int colpos=int(6*i/numtiles);
			fl_rectf(x()+1, y()+h()-tilesize-offset-i*tilesize, 
				 w()-2, tilesize-2, 
				 colors[colpos].r, 
				 colors[colpos].g, 
				 colors[colpos].b);
		}
	}
}

// setVolume /*fold00*/
void WindowVocoderVumeter::setVolume(double volume){
	if(volume>1.0) volume=1.0;
	if(volume<0.0) volume=0.0;
	this->volume=volume;
	redraw();
}

// setDirection /*fold00*/
void WindowVocoderVumeter::setDirection(bool horizontal){
	this->horizontal=horizontal;
}

// setTilesize /*fold00*/
void WindowVocoderVumeter::setTilesize(unsigned int tilesize){
	this->tilesize=tilesize;
}
