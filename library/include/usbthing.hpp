/**
 * USBThing C++ wrapper module
 * Wraps the C interface to provide an OO(ish) interface for c++ applications
 */

#include "usbthing.h"

#include <stdint.h>

class USBThing {
public:
	static int init()
	{
		return USBTHING_init();
	}
	static void close()
	{
		USBTHING_close();
	}
	USBThing(){
		connect(0x0001, 0x0001);
	}
	USBThing(int vid, int pid){
		connect(vid, pid);
	}
	~USBThing();

	int connect(uint16_t vid_filter, uint16_t pid_filter) {
		return USBTHING_connect(&this->usbthing, vid_filter, pid_filter);
	}
	int disconnect() {
		return USBTHING_disconnect(&this->usbthing);
	}

private:
	struct usbthing_s usbthing;

};

