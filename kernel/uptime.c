#include <types.h>
#include <defs.h>
#include <fs.h>
#include <file.h>


int write_uptime(struct inode *ip, char *buffer, int count) {
	cprintf("Permission denied of this character device.\n");
	return -1;
}

int read_uptime(struct inode *ip, char *buffer, int count) {
	uint _ticks;
	acquire(&tickslock);
	_ticks = ticks;
	release(&tickslock);
	cprintf("%x\n", _ticks);
	return 0;
}

void uptime_dev_init(void) {
	devsw[UPTIME_DEV].write = write_uptime;
	devsw[UPTIME_DEV].read = read_uptime;
}