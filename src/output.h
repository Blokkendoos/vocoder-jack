/*
 * Vocoder
 * © 2007 Achim Settelmeier <vocoder@m1.sirlab.de>
 *
 * base class for output
 */

#ifndef __OUTPUT_H__
#define __OUTPUT_H__

#include "thread.h"
#include "vocoder.h"

class Output{
public:
	virtual ~Output(){};
	virtual void run()=0;
	virtual void start(){};
	virtual void stop(){};
	virtual void exit(){};
	virtual Vocoder* getVocoder(){ return &vocoder; };

protected:
	Vocoder vocoder;
};

#endif
