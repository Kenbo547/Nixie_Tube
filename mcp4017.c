
#include "avr/io.h"
#include "mcp4017.h"
#include "i2c.h"

void mcp4017_init(void) {
	//Nothing to do here
}


uint8_t mcp4017_set_resistance(uint8_t res) {
	if (res > 0x7F) {
		//error resistance too large
		return 2;
	}
	i2c_send_packet(res, MCP4017_WADDR);
		
	return 0;
}


uint8_t mcp4017_get_resistance(void) {
	return i2c_recv_byte();
}
