/*
 * Vocoder
 * © 2007 Achim Settelmeier <vocoder@m1.sirlab.de>
 *
 * base class for objects running as threads
 */

#ifndef __THREAD_H__
#define __THREAD_H__

class Thread{
public:
	virtual ~Thread(){};
	virtual void run()=0;
};

#endif
