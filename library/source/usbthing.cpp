
#include "usbthing.h"

#include <stdint.h>

class USBThing {
public:
	static int init()
	{
		USBTHING_init();
	}
	static int close()
	{
		USBTHING_close();
	}
	int connect(uint16_t vid_filter, uint16_t pid_filter) {
		return USBTHING_connect(this.usbthing, vid_filter, pid_filter);
	}
	int disconnect() {
		return USBTHING_disconnect(this.usbthing);
	}

private:
	struct usbthing_s usbthing;

};

