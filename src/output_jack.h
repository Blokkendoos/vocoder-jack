/*
 * Vocoder
 * © 2007 Achim Settelmeier <vocoder@m1.sirlab.de>
 *
 * output: jack driver
 */

#ifndef __OUTPUT_JACK_H__
#define __OUTPUT_JACK_H__

#include <jack/jack.h>
#include "output.h"

class OutputJack : public Output{
public:
	OutputJack();
	virtual ~OutputJack();
	virtual void run();
	virtual void start();
	virtual void stop();
	virtual void exit();
	
private:
	int processCallback(jack_nframes_t nframes);
	
	// helper functions for jack, which demands for a static callback
	static int processCallbackStatic(jack_nframes_t nframes, void *arg);
	static void shutdownCallbackStatic(void *arg);
	
	bool exitFlag;    // true, if this thread should exit
	bool isRunning;   // true, if connection to the jack server is up and running
	
	jack_client_t *jackClient;
	jack_port_t *portInputFormant;
	jack_port_t *portInputCarrier;
	jack_port_t *portOutputLeft;
	jack_port_t *portOutputRight;
};

#endif
