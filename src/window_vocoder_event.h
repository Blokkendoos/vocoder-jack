/*
 * Vocoder
 * © 2007 Achim Settelmeier <vocoder@m1.sirlab.de>
 *
 * Vocoder Controller
 */

#ifndef __WINDOW_VOCODER_EVENT_H__
#define __WINDOW_VOCODER_EVENT_H__


class WindowVocoderEvent{
public:
	struct WindowVocoderEventBandChange{
		unsigned int bandnr;
	};
	
	enum WindowVocoderEventType {
		EventTypeNop=0,
		EventTypeExit,
		EventTypeBandChange,
		EventTypeVolumeChange,
		EventTypeTimerVumeterUpdate,
	};
	
	WindowVocoderEvent();
	WindowVocoderEvent::WindowVocoderEventType getEventType();
	void setExit();
	void setTimerVumeterUpdate();
	void setBandChangeEvent(unsigned int bandnr);
	unsigned int getBandChangeEvent();
	
private:
	WindowVocoderEventType type;
	union{
		struct WindowVocoderEventBandChange bandChange;
	}event;
};

#endif
