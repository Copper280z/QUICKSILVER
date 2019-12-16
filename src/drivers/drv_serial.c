#include "drv_serial.h"

#include "profile.h"
#include "project.h"
#include "usb_configurator.h"

usart_ports_t serial_rx_port = USART_PORT_INVALID;
usart_ports_t serial_smart_audio_port = USART_PORT_INVALID;

//FUNCTION TO SET APB CLOCK TO USART BASED ON GIVEN UART
void serial_enable_rcc(usart_ports_t port) {
  switch (usart_port_defs[port].channel_index) {
  case 1:
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    break;
#ifdef F4
  case 2:
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    break;
  case 3:
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    break;
  case 4:
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
    break;
  case 5:
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
    break;
  case 6:
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);
    break;
#endif
  }
}
void serial_enable_isr(usart_ports_t port) {
  NVIC_InitTypeDef NVIC_InitStructure;

  switch (usart_port_defs[port].channel_index) {
  case 1:
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    break;
#ifdef F4
  case 2:
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    break;
  case 3:
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    break;
  case 4:
    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
    break;
  case 5:
    NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
    break;
  case 6:
    NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
    break;
#endif
  }

#ifdef F4
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
#else
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
#endif
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

#ifdef F4

#define USART4 UART4
#define USART5 UART5

#define GPIO_AF_USART4 GPIO_AF_UART4
#define GPIO_AF_USART5 GPIO_AF_UART5

#define USART_PORT(chan, port, rx, tx)     \
  {                                        \
      .channel_index = chan,               \
      .channel = USART##chan,              \
      .gpio_port = GPIO##port,             \
      .gpio_af = GPIO_AF_USART##chan,      \
      .rx_pin_index = rx,                  \
      .rx_pin = GPIO_Pin_##rx,             \
      .rx_pin_source = GPIO_PinSource##rx, \
      .tx_pin_index = tx,                  \
      .tx_pin = GPIO_Pin_##tx,             \
      .tx_pin_source = GPIO_PinSource##tx, \
  },
#endif

#ifdef F0
#define USART_PORT(chan, port, rx, tx)     \
  {                                        \
      .channel_index = chan,               \
      .channel = USART##chan,              \
      .gpio_port = GPIO##port,             \
      .gpio_af = GPIO_AF_1,                \
      .rx_pin_index = rx,                  \
      .rx_pin = GPIO_Pin_##rx,             \
      .rx_pin_source = GPIO_PinSource##rx, \
      .tx_pin_index = tx,                  \
      .tx_pin = GPIO_Pin_##tx,             \
      .tx_pin_source = GPIO_PinSource##tx, \
  },
#endif

usart_port_def_t usart_port_defs[USART_PORTS_MAX] = {
    {},
    USART_PORTS};

#undef USART_PORT

void handle_usart_isr(usart_ports_t channel) {
#ifdef SERIAL_RX
  extern void RX_USART_ISR(void);
  if (serial_rx_port == channel)
    RX_USART_ISR();
#endif
}

#define USART_PORT(channel, port, rx_pin, tx_pin) \
  void USART##channel##_IRQHandler(void) {        \
    handle_usart_isr(USART_IDENT(channel));       \
  }

USART_PORTS

#undef USART_PORT