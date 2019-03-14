/*
 * Vocoder
 * © 2007 Achim Settelmeier <vocoder@m1.sirlab.de>
 *
 */

// Includes /*fold00*/
#include <iostream>
#include "output_jack.h"

// OutputJack /*fold00*/
OutputJack::OutputJack(){
	exitFlag=false;
	isRunning=false;
	
	jackClient=NULL;
	portInputFormant=NULL;
	portInputCarrier=NULL;
	portOutputLeft=NULL;
	portOutputRight=NULL;
}

// ~OutputJack /*fold00*/
OutputJack::~OutputJack(){
	stop();
}

// start /*fold00*/
void OutputJack::start(){
	if(isRunning)
		return;
	
	jackClient=jack_client_open("vocoder", JackNullOption, NULL);
	if(jackClient==0){
		std::cerr << "Can't connect to jack, jack_client_new() failed" << std::endl;
		return;
	}
	
	// relay sample rate to Vocoder class
	vocoder.setSampleRate(jack_get_sample_rate(jackClient));
	
	jack_on_shutdown(jackClient, OutputJack::shutdownCallbackStatic, this);
	jack_set_process_callback(jackClient, OutputJack::processCallbackStatic, this);
	
	portInputCarrier=jack_port_register(jackClient, "carrier", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
	portInputFormant=jack_port_register(jackClient, "formant", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
	portOutputLeft=jack_port_register(jackClient, "left", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
	portOutputRight=jack_port_register(jackClient, "right", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
	
	if(jack_activate(jackClient)){
		std::cerr << "cannot activate client" << std::endl;
		return;
	}
	
	isRunning=true;
}

// stop /*fold00*/
void OutputJack::stop(){
	if(!isRunning)
		return;
	
	isRunning=false;
	
	jack_client_close(jackClient);
	jackClient=NULL;
}

// exit /*fold00*/
void OutputJack::exit(){
	exitFlag=true;
	stop();
}

// run /*fold00*/
void OutputJack::run(){
	start();
	while(!exitFlag) 
		sleep(1);
	stop();
}
// processCallback /*fold00*/
int OutputJack::processCallback(jack_nframes_t nframes){
	if(!isRunning) return 0;
	
	jack_default_audio_sample_t *formant=(jack_default_audio_sample_t *) jack_port_get_buffer(portInputFormant, nframes);
	jack_default_audio_sample_t *carrier=(jack_default_audio_sample_t *) jack_port_get_buffer(portInputCarrier, nframes);
	jack_default_audio_sample_t *outLeft=(jack_default_audio_sample_t *) jack_port_get_buffer(portOutputLeft, nframes);
	jack_default_audio_sample_t *outRight=(jack_default_audio_sample_t *) jack_port_get_buffer(portOutputRight, nframes);
	
	vocoder.process(formant, carrier, outLeft, outRight, nframes);
	
	return 0;
}

/* **************************************************************** */ /*FOLD00*/
// shutdownCallbackStatic /*fold00*/
void OutputJack::shutdownCallbackStatic(void *arg){
	OutputJack *j=(OutputJack *)arg;
	j->exit();
}

// processCallbackStatic /*fold00*/
int OutputJack::processCallbackStatic(jack_nframes_t nframes, void *arg){
	OutputJack *j=(OutputJack *)arg;
	
	return j->processCallback(nframes);
}
