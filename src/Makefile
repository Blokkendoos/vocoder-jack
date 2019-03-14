# Settings #fold00
CPPFLAGS= -O2 -Wall -W
CPPFLAGS+= $(shell fltk-config --cflags)
LFLAGS= -ljack -lm
LFLAGS+= $(shell fltk-config --ldflags) 
FLUIDFLAGS=

VERSION=$(shell cat VERSION)
CPPFLAGS+= '-DVERSION="$(VERSION)"'

CC=g++
LD=g++
FLUID=fluid

OBJECTS=main.o window_vocoder.o window_vocoder.fl.o window_vocoder_band.o window_vocoder_bands.o output_jack.o vocoder.o vocoder_controller.o window_vocoder_event.o window_vocoder_vumeter.o


.SUFFIXES: .fl.cpp .fl.h .cpp .h .o .fl
.PHONY: all clean libvocoder

# Rules #fold00
%.o: %.cpp
	$(CC) $(CPPFLAGS) -o $@ -c $< 

%.fl.h %.fl.cpp: %.fl
	$(FLUID) $(FLUIDFLAGS) -c $<

vocoder: $(OBJECTS)
	$(LD) $(LFLAGS) -o $@ $(OBJECTS)


# Tool rules #fold00
run:
	$(MAKE) vocoder
	$(MAKE) -C ../util/ play_loop
	( \
		../util/play_loop -p formant -l ../data/formant.wav &\
		pids="$$!" ;\
		../util/play_loop -p carrier -l ../data/carrier.wav &\
		pids="$$pids $$!" ;\
		./vocoder &\
		vpid="$$!" ;\
		while ! (jack_lsp | grep -q vocoder) ; do sleep 1 ; done ;\
		jack_connect carrier:left vocoder:carrier ;\
		jack_connect formant:left vocoder:formant ;\
		jack_connect vocoder:left alsa_pcm:playback_1 ;\
		jack_connect vocoder:right alsa_pcm:playback_2 ;\
		while kill -0 $$vpid ; do sleep 1 ; done ;\
		for i in $$pids ; do kill $$i ; sleep 1; done ;\
	)

clean:
	rm -f *~ *.bak
	rm -f *.o *.fl.h *.fl.cpp
	rm -f vocoder

# Dependencies #fold00
main.o: thread.h window_vocoder.h output_jack.h output.h vocoder.h vocoder_controller.h
window_vocoder.h: window_vocoder.fl.h window_vocoder_bands.h window_vocoder_event.h
window_vocoder.o: thread.h window_vocoder.h window_vocoder_band.h window_vocoder_bands.h window_vocoder.fl.h window_vocoder_event.h
window_vocoder_band.o: window_vocoder_band.h window_vocoder.fl.h 
window_vocoder_bands.o: window_vocoder_bands.h window_vocoder_band.h window_vocoder.fl.h window_vocoder.h 
window_vocoder.fl.o: window_vocoder.h window_vocoder_band.h window_vocoder_bands.h window_vocoder.fl.h
output_jack.o: output_jack.h output.h thread.h vocoder.h
vocoder_controller.o: vocoder_controller.h vocoder.h output.h window_vocoder.h window_vocoder_event.h
window_vocoder_event.o: window_vocoder_event.h
window_vocoder_vumeter.o: window_vocoder_vumeter.h window_vocoder.fl.h vocoder.h
vocoder.o: vocoder.h
