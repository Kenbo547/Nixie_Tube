

#ifndef MCP4017_H
#define MCP4017_H

//definitions
#define MCP4017_WADDR 0b01011110
#define MCP4017_RADDR 0b01011111

//functions
void mcp4017_init(void);
uint8_t mcp4017_set_resistance(uint8_t res);
uint8_t mcp4017_get_resistance(void);

#endif //MCP4017

