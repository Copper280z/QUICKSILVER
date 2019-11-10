#pragma once

#include "target.h"

//  CONDITIONAL SELECTION OF DEFINES BASED ON USER SELECTED UART FOR SERIAL RX, TARGET MCU, AND TARGET DEFINED USART ALTERNATE MAPPING PINS
#ifdef F0
#define USART1_PA3PA2 USART_PORT(1, A, 3, 2)
#define USART1_SDA USART_PORT(1, A, 14, 15)
#endif

#ifdef F4
#define USART1_PA10PA9 USART_PORT(1, A, 10, 9)
#define USART2_PA3PA2 USART_PORT(2, A, 3, 2)
#define USART3_PB11PB10 USART_PORT(3, B, 11, 10)
#define USART3_PC11PC10 USART_PORT(3, C, 11, 10)
#define USART4_PA1PA0 USART_PORT(4, A, 1, 0)
#define USART6_PC7PC6 USART_PORT(6, C, 7, 6)
#endif

#define USART_IDENT(channel) USART_PORT##channel
#define USART_PORT(channel, port, rx_pin, tx_pin) USART_IDENT(channel),

typedef enum {
  USART_PORT_INVALID,
  USART_PORTS
  USART_PORTS_MAX,
} usart_ports_t;

#undef USART_PORT