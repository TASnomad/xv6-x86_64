#include <types.h>
#include <defs.h>
#include <memlayout.h>
#include <acpi.h>
#include <x86.h>

#define SLP_EN (1 << 13)
#define ACPI_OP_POSSIBLE 	1
#define ACPI_NOOP			0

#if X64
#define PHYSLIMIT 0x80000000
#else
#define PHYSLIMIT 0x0E000000
#endif

static uint SLP_TYPa;
static uint SLP_TYPb;
static int SCI_EN = ACPI_NOOP;

/*
 * enable_retry is a value used to look if
 * we already tried to enable acpi
 * it used due to a kernel panic when sleep spinlock is released
 * need to fix to make a better function
 */
static int enable_retry = -1;

/* Simple wrapper to auto fill data at a physical address to a regular C structure in virtual address space */
#define GET_ACPI_OBJ(struct_to_fill, type, ptr) struct_to_fill = (type *) p2v(ptr);

/*
 * So far all data needed to get ACPI details are in physical addresses
 * to store temporaly data, you can directly cast addresses with p2v
 * or simply fill structures with macros
 */

static unsigned entry_count(struct acpi_rsdt *rsdt) {
	unsigned count;

	count = (rsdt->header.length - sizeof(*rsdt)) / 4;

	return count;
}

static struct acpi_fadt *findFADT(void) {
	struct acpi_rdsp *rdsp;
	struct acpi_rsdt *rsdt;
	struct acpi_fadt *fadt;
	struct acpi_desc_header *hdr;
	unsigned n, count;

	rdsp = get_rdsp();

	/* Find the ACPI table pointer */
	if(!rdsp) goto lookup_error;

	GET_ACPI_OBJ(rsdt, struct acpi_rsdt, rdsp->rsdt_addr_phys);

	count = entry_count(rsdt);

	for(n = 0; n < count; n++) {
		GET_ACPI_OBJ(hdr, struct acpi_desc_header, rsdt->entry[n]);

		if(!strncmp((const char *) hdr->signature, "FACP", 4)) {
			fadt = (struct acpi_fadt *) p2v(rsdt->entry[n]);
			return fadt;
		}
		hdr = 0x0;
	}

	goto lookup_error;

lookup_error:
	cprintf("Cannot find intermetiades tables while searching FADT !\n");
	return 0x0;
}

static int find_dsdt(struct acpi_fadt *fadt) {
	if(fadt == 0x0) return -1;

	struct acpi_desc_header *hdr = 0x0;

	GET_ACPI_OBJ(hdr, struct acpi_desc_header, fadt->dsdt);

	if(!memcmp(hdr->signature, "DSDT", 4)) {
		uint32 *ptr = (uint32 *) p2v(fadt->dsdt);
		char *S5_addr = (char*) ptr + sizeof(struct acpi_desc_header);
		int dsdt_length = *(ptr + 1) - sizeof(struct acpi_desc_header);

		while(0 < dsdt_length--) {
			if(!memcmp(S5_addr, "_S5_", 4)) break;
			S5_addr++;
		}

		if(dsdt_length > 0) {
			if((*(S5_addr - 1) == 0x08 || (*(S5_addr - 2) == 0x08 && *(S5_addr - 1) == '\\')) && *(S5_addr + 4) == 0x12) {
				S5_addr += 5;
				S5_addr += ((*S5_addr & 0xC0) >> 6) + 2;

				if(*S5_addr == 0x0A) S5_addr++;

				SLP_TYPa = *(S5_addr) << 10;
				S5_addr++;

				if(*S5_addr == 0x0A) S5_addr++;

				SLP_TYPb = *(S5_addr) << 10;

				SCI_EN = ACPI_OP_POSSIBLE;

				return 0;
			} else return -1;
		} else return -1;

		return 0;
	}
	return -1;
}

static int enable_acpi_operations(struct acpi_fadt *fadt) {
	int sleep_time = 10;
	int _tick_diff;

	if(fadt == 0x0) return -1;

	/* check if acpi is enabled */
	if((inw((uint32) fadt->PM1a_control_block) & SCI_EN) == 0) {
		/* does ACPI can be enabled ? */
		if(fadt->smi_command_port != 0 && fadt->acpi_enable != 0) {
			outb((uint32) fadt->smi_command_port, fadt->acpi_enable);
			int i;

			/* sleeping 3 sec to enable acpi */
			/* TODO: create a "real" sleep function */
			for(i = 0; i < 300; i++) {
				if((inw((uint32) fadt->PM1a_control_block) & SCI_EN)) break;
				acquire(&tickslock);
				_tick_diff = ticks;
				while(ticks - _tick_diff < sleep_time) {

					if((inw((uint32) fadt->PM1a_control_block) & SCI_EN)) {
						release(&tickslock);
						break;
					}
					sleep(&ticks, &tickslock);
				}
				release(&tickslock);
			}

			if(fadt->PM1b_control_block != 0) {
				for(; i < 300; i++) {
					if((inw((uint32) fadt->PM1b_control_block) & SCI_EN) == 1) {

						acquire(&tickslock);
						_tick_diff = ticks;
						while(ticks - _tick_diff < sleep_time)
							sleep(&ticks, &tickslock);
						release(&tickslock);
						break;
					}
				}

				if(i < 300) return 0;
				else return -1;
			}
		} else return -1; /* ACPI can't be enabled ! */
	} else return 0; /* well, it was done before */
	return -1;
}

void poweroff(void) {
	struct acpi_fadt *fadt = 0x0;
	int _is_acpi_enabled;

	fadt = findFADT();

	if(fadt == 0x0) goto halt_only;

	find_dsdt(fadt);

	if(SCI_EN < 0) goto halt_only;

	_is_acpi_enabled = enable_acpi_operations(fadt);

	if(_is_acpi_enabled < 0 && enable_retry < 0) _is_acpi_enabled = enable_acpi_operations(fadt);

	if(_is_acpi_enabled < 0 && enable_retry != -1) goto halt_only;

	cprintf("System is going down.\n");

	outw((uint32) fadt->PM1a_control_block, SLP_TYPa | SLP_EN);

	if(fadt->PM1b_control_block != 0)
		outw((uint32) fadt->PM1b_control_block, SLP_TYPb | SLP_EN);

	cprintf("ACPI shutdown error !\n");


halt_only:
	cprintf("System has been halted, you can safely turn-off your computer !\n");
	hlt();
}

void reboot(void) {

	struct acpi_fadt *fadt = 0x0;
	struct acpi_generic_address addr;

	fadt = findFADT();

	addr = (fadt)->reset_reg;

	if(fadt == 0x0) goto awfull_method;
	// if(fadt->header.revision < 2) cprintf("Baaah !\n");

	outb(addr.address, fadt->reset_value);

	cprintf("ACPI reboot failed !\n");
	goto awfull_method;

	/*
	 * so far whitout ACPI and APM
	 * the only way to reboot is to
	 * load a null IDT,
	 * doing this will create a triple fault and reset the PC
	 */
awfull_method:
	cprintf("System is rebooting !\n");
	lidt(0, 0);
}