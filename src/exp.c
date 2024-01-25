/// main.c: Linux Module for Real-time Linux experiment.
///
/// Author: René Huiberts
/// Date: 25 - 01 - 2024
///
/// rhjr: 
///
///
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include "exp.h"

internal int __init
exp_initialize(void)
{
  printk(KERN_INFO, "Starting experiment...\n");
  return 0;
}

internal void __exit
exp_terminate(void)
{
  printk(KERN_INFO, "Terminating experiment...\n");
  return;
}

module_init(exp_initialize);
module_exit(exp_initialize);
