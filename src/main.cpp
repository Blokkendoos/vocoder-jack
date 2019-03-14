/*
 * Vocoder
 * � 2007 Achim Settelmeier <vocoder@m1.sirlab.de>
 *
 */

// Includes /*fold00*/
#include <iostream>
#include <pthread.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/time.h>
#include <termios.h>
#include "thread.h"
#include "window_vocoder.h"
#include "vocoder_controller.h"
#include "output_jack.h"

// enum programState /*fold00*/
enum programState{
	programStateUndef=0,
	programStateInit,
	programStateRunning,
	programStateExiting
};
enum programState programState=programStateUndef;

// setProgramState /*fold00*/
void setProgramState(enum programState state){
	programState=state;
}


// runThread /*fold00*/
void *runThread(void *_obj){
	Thread *obj=(Thread *)_obj;
	obj->run();
	setProgramState(programStateExiting);
	return NULL;
}

// main /*fold00*/
int main(int argc, char **argv){
	
	setProgramState(programStateInit);
	
	OutputJack output;
	pthread_t thread_output;
	pthread_create(&thread_output, NULL, runThread, &output);
	
	WindowVocoder gui(argc, argv);
	VocoderController vc(&gui, &output);
	pthread_t thread_gui;
	pthread_create(&thread_gui, NULL, runThread, &gui);
	
	// main loop: wait for user input or until a thread calls the exitHandler
	for(setProgramState(programStateRunning); programState==programStateRunning;){
		fd_set sel_in;
		struct timeval timeout;
		timeout.tv_sec=0;
		timeout.tv_usec=100000;
		
		// wait for keypress
		FD_ZERO(&sel_in);
		int num=select(0, &sel_in, NULL, NULL, &timeout);
		if(num==-1) break;
		if(num==0) continue;
	}
	std::cout << "waiting for threads to shut down" << std::endl;
	
	output.exit();
	
	// wait for threads to shut down
	pthread_cancel(thread_gui);
	pthread_cancel(thread_output);
	pthread_join(thread_gui, NULL);
	pthread_join(thread_output, NULL);
	
	return 0;
}
