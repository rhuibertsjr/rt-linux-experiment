#!/bin/bash

sudo cyclictest --mlockall -smp --priority=80 --interval=200 --distance=0 --loops=100000
