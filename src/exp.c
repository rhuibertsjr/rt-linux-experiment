/// main.c: Linux Module for Real-time Linux experiment.
///
/// Author: René Huiberts
/// Date: 25 - 01 - 2024
///
//

#include <linux/interrupt.h>
#include <linux/ktime.h>
#include <linux/delay.h>

#include "exp.h"

// rhjr:

global ktime_t start_time, end_time;

global uint32_t gpio_irq_pin;
global uint8_t gpio_irq_state;


internal irq_handler_t 
exp_interrupt_service_routine(uint32_t irq, void* dev_id, struct pt_regs *regs)
{
  LOG("Interrupt");
  end_time = ktime_get();

  if (gpio_irq_state)
  {
    gpiod_set_value(GPIO_24, 0);
    gpio_irq_state = 0;
  }
  else
  {
    gpiod_set_value(GPIO_24, 1);
    gpio_irq_state = 1;
  }

  return (irq_handler_t) IRQ_HANDLED;  
}

// rhjr: gpio

internal uint32_t
exp_gpio_initialize(void)
{
  LOG("Initializing GPIO pin.\n");

  // rhjr: request pin 23, and set it as input.
  gpio_request(GPIO_23, "GPIO_23");
  gpio_direction_input(GPIO_23);  
  gpio_set_debounce(GPIO_23, 20);

  // rhjr: request pin 24, and set it as output.
  gpio_request(GPIO_24, "GPIO_24");
  gpio_direction_output(GPIO_24, 1);  

  // rhjr: interrupt service routine
  uint8_t result; // rhjr: ISO C90 actually forbids mixing declerations and code lol.
  gpio_irq_state = 0;

  gpio_irq_pin = gpio_to_irq(GPIO_23);
  result = request_irq(
    gpio_irq_pin, (irq_handler_t) exp_interrupt_service_routine, 
    IRQF_TRIGGER_FALLING | IRQF_TRIGGER_RISING, 
    STRINGIFY(exp_interrupt_service_routine), NULL);

  return 0;
}

// rhjr: linux module 

internal int __init
exp_initialize(void)
{
  LOG("Initializing the experiment.\n");

  exp_gpio_initialize();

  u64 delta_time_ns;
  s64 delta_time_ms;

  start_time = ktime_get();

  while(1)
  {
    if (gpio_irq_state == 1)
    {
      // rhjr: convert time difference to milliseconds.
      delta_time_ns = ktime_to_ns(ktime_sub(end_time, start_time));
      delta_time_ms = div_s64(delta_time_ns, 1000000);
      break;
    }
  }

  printk(KERN_WARNING "Elapsed time: %lld ms\n", delta_time_ms);

  return 0;
}

internal void __exit
exp_terminate(void)
{
  LOG("Terminating the experiment.");

  gpio_set_value(GPIO_24, 0);

  // rhjr: consistency :/
  free_irq(gpio_irq_pin, NULL);

  gpio_free(GPIO_23);
  gpio_free(GPIO_24);
}

module_init(exp_initialize);
module_exit(exp_terminate);

// rhjr: linux module configurations

MODULE_AUTHOR(AUTHOR);
MODULE_DESCRIPTION(DESCRIPTION);
MODULE_LICENSE(LICENSE);
MODULE_VERSION(VERSION);

