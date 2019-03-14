/**
 * play_loop
 * © 2007 Achim Settelmeier <vocoder@m1.sirlab.de>
 *
 */ 

// Includes /*fold00*/
#include <string.h>
#include <getopt.h>
#include <sndfile.h>
#include <jack/jack.h>
#include <string>
#include <iostream>
#include <list>

using std::string;
using std::cout;
using std::endl;
using std::list;

// Globals /*fold00*/
jack_client_t *jackClient=NULL;
list<string> files;
string port="play_loop";
string connectPort="";
bool loop=false;
bool exitFlag=false;
jack_port_t *jackPortOutLeft=NULL;
jack_port_t *jackPortOutRight=NULL;
jack_nframes_t sampleRate;
SNDFILE *sndFileFD=NULL;
SF_INFO sndFileInfo;
bool playSampleFlag=false;

#define SAMPLEBUFFERSIZE 4096
double sampleBuffer[SAMPLEBUFFERSIZE];


// play_sample /*fold00*/
void playSample(const string &file){
	sndFileInfo.format=0;
	sndFileFD=sf_open(file.c_str(), SFM_READ, &sndFileInfo);
	if(sndFileFD==NULL){
		std::cerr << file << ": can't open, skipping." << std::endl;
		return;
	}
	
	playSampleFlag=true;
	while(playSampleFlag) sleep(1);
	
	
	sf_close(sndFileFD);
}

// jackExit /*fold00*/
void jackExit(void *arg){
	exitFlag=true;
}


// jackProcess /*fold00*/
int jackProcess(jack_nframes_t nframes, void *arg){
	jack_default_audio_sample_t *outLeft=(jack_default_audio_sample_t *) jack_port_get_buffer(jackPortOutLeft, nframes);
	jack_default_audio_sample_t *outRight=(jack_default_audio_sample_t *) jack_port_get_buffer(jackPortOutRight, nframes);
	if(!playSampleFlag){
		memset(outLeft, 0, sizeof(jack_default_audio_sample_t) * nframes);
		memset(outRight, 0, sizeof(jack_default_audio_sample_t) * nframes);
		return 0;
	}
	
	// read file in chunks, just as much as it fits into our buffer
	jack_nframes_t pos=0;
	while(pos<nframes){
		double stretch=sndFileInfo.samplerate/(double)sampleRate;
		
		jack_nframes_t maxcount=int(SAMPLEBUFFERSIZE/sndFileInfo.channels);
		if(int((nframes-pos)*stretch)<maxcount) maxcount=int((nframes-pos)*stretch);
		sf_count_t count=sf_readf_double(sndFileFD, sampleBuffer, maxcount);
		if(count<=0){
			// fill the rest of the buffer with silence and set the flag we're done
			playSampleFlag=false;
			for(; pos<nframes; pos++){
				outLeft[pos]=0;
				outRight[pos]=0;
			}
			return 0;
		}
		
		// copy sample to output buffer. Does convert sample rates but no interpolation is done
		jack_nframes_t channel=(sndFileInfo.channels>1) ? 1 : 0;
		for(jack_nframes_t i=0; pos<nframes; i++, pos++){
			jack_nframes_t srcPos=int(i*stretch);
			if(srcPos>=count) break;
			srcPos*=sndFileInfo.channels;
			outLeft[pos]=sampleBuffer[srcPos];
			outRight[pos]=sampleBuffer[srcPos+channel];
		}
	}
	
	return 0;
}

// main /*fold00*/
int main(int argc, char **argv){
	// read command line options
	while(1){
		int option_index=0;
		static struct option long_options[]={
			{"help", 0, 0, 'h'},
			{"port", 1, 0, 'p'},
			{"connect", 1, 0, 'c'},
			{"loop", 0, 0, 'l'}
		};
		int c=getopt_long(argc, argv, "hp:l",
				  long_options, &option_index);
		if(c==-1) break;
		if(c==0) 
			c=long_options[option_index].val;
		
		switch(c){
		case 'h':
			printf("usage: play_loop [<options>] <file> [<file> ...]\n");
			printf("  available options are:\n");
			printf("    -h, --help            show help\n");
			printf("    -l, --loop             loop sample forever\n");
			printf("    -p, --port <name>     set name of JACK port\n");
			printf("    -c, --connect <name>  set name of JACK port to connect to\n");
			return 0;
		case 'l':
			loop=true;
			break;
		case 'p':
			port=optarg;
			break;
		case 'c':
			connectPort=optarg;
			break;
		}
	}
	
	// error handling
	if(optind>=argc){
		std::cerr << "no file to play" << std::endl;
		return 1;
	}
	
	// save all non-option arguments as filenames
	for(;optind<argc; optind++){
		files.push_back(argv[optind]);
	}
	
	// set up jack client
	jackClient=jack_client_new(port.c_str());
	if(jackClient==0){
		std::cerr << "Can't connect to jack, jack_client_new() failed" << std::endl;
		return 1;
	}
	
	jack_on_shutdown(jackClient, jackExit, NULL);
	jack_set_process_callback(jackClient, jackProcess, NULL);
	
	jackPortOutLeft=jack_port_register(jackClient, "left", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
	jackPortOutRight=jack_port_register(jackClient, "right", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
	
	sampleRate=jack_get_sample_rate(jackClient);
	if(jack_activate(jackClient)){
		std::cerr << "cannot activate client" << std::endl;
		jack_client_close(jackClient);
		return 1;
	}
	
	
	// main loop: play sample(s)
	while(!exitFlag){
		for(list<string>::iterator i=files.begin(); i!=files.end(); i++){
			playSample(*i);
		}
		if(!loop) break;
	}
	
	// cleanup
	jack_client_close(jackClient);
	
	return 0;
}
