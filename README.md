# libnetlink examples

This repo includes 3 example utilities using libnetlink to do the same operations you can do with the iproute2 'ip' command. You typically want to do this when embedding network operations in your own application 
(e.g your C application needs to create a macvlan) and you don't want to use ugly system() calls to the ip command. Not to mention calling system() on a multi-threaded application is looking for trouble :)

You will need the rpm package iproute-devel installed, or an equivalent package for your distribution. This is the package that ships libnetlink.a, the library required to compile this repo. To compile just type ```make```

The repo provides 3 example applications:

## addvlan

Usage: addvlan \<device> \<name>

This is the same as ```ip link add link <device> <name> type macvlan```

## rmif

usage: rmif \<device>

This is the same as ```ip link delete <device>```

## ifstate

usage: ifstate \<device> [up|down]

This is the same as ```ip link set <device> up|down```


## More info:

If you need information about what other parameters/commands are available, you can always browse the iproute2 repo: https://github.com/shemminger/iproute2

The code there is nicely organized

