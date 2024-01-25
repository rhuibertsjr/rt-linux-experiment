/// main.c: Linux Module for Real-time Linux experiment.
///
/// Author: René Huiberts
/// Date: 25 - 01 - 2024
///
/// rhjr: 
///
///

#include <linux/interrupt.h>
#include "exp.h"

// rhjr:

global uint32_t gpio_irq_pin;

internal irq_handler_t 
exp_interrupt_service_routine(uint32_t irq, void* dev_id, struct pt_regs *regs)
{
  LOG("Interrupt");
  gpio_set_value(GPIO_24, 1);
  return (irq_handler_t) IRQ_HANDLED;  
}

// rhjr: gpio

internal uint32_t
exp_gpio_initialize(void)
{
  LOG("Initializing GPIO pin.");

  // rhjr: request pin 23, and set it as input.
  gpio_request(GPIO_23, "GPIO_23");
  gpio_direction_input(GPIO_23);  
  gpio_set_debounce(GPIO_23, 20);

  // rhjr: request pin 24, and set it as input.
  gpio_request(GPIO_24, "GPIO_24");
  gpio_direction_output(GPIO_24, 0);  

  // rhjr: /sys/class/gpio, `false` prevents the direction from being changed.
  gpio_export(GPIO_23, false);
  gpio_export(GPIO_24, false);

  // rhjr: interrupt service routine
  uint8_t result; // rhjr: ISO C90 actually forbids mixing declerations and code lol.

  gpio_to_irq(GPIO_23);
  result = request_irq(gpio_irq_pin, (irq_handler_t) exp_interrupt_service_routine, 
    IRQF_TRIGGER_RISING, STRINGIFY(exp_interrupt_service_routine), NULL);

  return result;
}

// rhjr: linux module 

internal int __init
exp_initialize(void)
{
  LOG("Initializing the experiment.");

  exp_gpio_initialize();

  return 0;
}

internal void __exit
exp_terminate(void)
{
  LOG("Terminating the experiment.");

  gpio_set_value(GPIO_24, 0);

  // rhjr: consistency :/
  free_irq(gpio_irq_pin, NULL);

  gpio_unexport(GPIO_23);
  gpio_free(GPIO_23);

  gpio_unexport(GPIO_24);
  gpio_free(GPIO_24);

  return;
}

module_init(exp_initialize);
module_exit(exp_terminate);

// rhjr: linux module configurations

MODULE_AUTHOR(AUTHOR);
MODULE_DESCRIPTION(DESCRIPTION);
MODULE_LICENSE(LICENSE);
MODULE_VERSION(VERSION);

