// For more info:
// man 7 rtnetlink
// man 3 libnetlink
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <net/if.h>
#include <libnetlink.h>

struct iplink_req {
	struct nlmsghdr         n;  
	struct ifinfomsg        i;  
	char                    buf[1024];
};

int main(int argc, char *argv[])
{
	const char *type = "macvlan";
	uint32_t mode = MACVLAN_MODE_PRIVATE;
	const char *name = NULL;
	const char *dev = NULL;
	struct iplink_req req;
	struct rtnl_handle rth = { .fd = -1 };
	unsigned int ifindex = 0;
	int len = 0;

	if (argc < 3) {
		fprintf(stderr, "usage: addvlan <dev> <name>\n");
		exit(1);
	}

	dev = argv[1];
	name = argv[2];

	len = strlen(name) + 1;

	ifindex = if_nametoindex(dev);
	if (!ifindex) {
		fprintf(stderr, "Invalid device %s!\n", dev);
		exit(1);
	}

	if (rtnl_open(&rth, 0)) {
		fprintf(stderr, "Cannot open rtnetlink!\n");
		exit(1);
	}

	memset(&req, 0, sizeof(req));
	req.n.nlmsg_len = NLMSG_LENGTH(sizeof(struct ifinfomsg));
	req.n.nlmsg_flags = NLM_F_REQUEST | NLM_F_CREATE | NLM_F_EXCL;
	req.n.nlmsg_type = RTM_NEWLINK;
	req.i.ifi_family = AF_UNSPEC;

	struct rtattr *linkinfo = NLMSG_TAIL(&req.n);
	addattr_l(&req.n, sizeof(req), IFLA_LINKINFO, NULL, 0);
	addattr_l(&req.n, sizeof(req), IFLA_INFO_KIND, type, strlen(type));
	
	struct rtattr *data = NLMSG_TAIL(&req.n);
	addattr_l(&req.n, sizeof(req), IFLA_INFO_DATA, NULL, 0);

	addattr32(&req.n, 1024, IFLA_MACVLAN_MODE, mode);

	data->rta_len = (void *)NLMSG_TAIL(&req.n) - (void *)data;
	linkinfo->rta_len = (void *)NLMSG_TAIL(&req.n) - (void *)linkinfo;

	addattr_l(&req.n, sizeof(req), IFLA_LINK, &ifindex, 4);

	if (len > IFNAMSIZ) {
		fprintf(stderr, "Interface name too long: %s\n", name);
		exit(1);
	}

	addattr_l(&req.n, sizeof(req), IFLA_IFNAME, name, len);

	if (rtnl_talk(&rth, &req.n, 0, 0, NULL, NULL, NULL) < 0) {
		fprintf(stderr, "failed to talk to netlink!\n");
		exit(1);
	}

	rtnl_close(&rth);

	printf("Added macvlan interface %s bound to %s\n", name, dev);
}

