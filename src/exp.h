#ifndef MAIN_H
#define MAIN_H

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/gpio.h>

// rhjr: linux module configurations
#define AUTHOR      "rhuibertsjr"
#define DESCRIPTION "Real-time Linux external trigger experiment."
#define LICENSE     "GPL"
#define VERSION     "0.1"

// rhjr: keywords
#define internal static
#define global   static

// rhjr: gpio
#define GPIO_23 (23)
#define GPIO_24 (24)

// rhjr: helpers
#define STATEMENT(x) do { x } while(0) /* MACRO-expansion fix */

#define LOG(msg) STATEMENT( \
  pr_debug("[%s] %s::%d -> %s\n" __FILE__, __FUNCTION__, __LINE__, msg);)

#define _STRINGIFY(str) #str
#define STRINGIFY(str) _STRINGIFY(str)

#endif
