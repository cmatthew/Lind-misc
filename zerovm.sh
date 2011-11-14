#!/bin/bash
#Zero out the VMs free space so it compacts better.

dd if=/dev/zero of=junk
sync
rm junk
