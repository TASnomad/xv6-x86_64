// References: ACPI 5.0 Errata A
// http://acpi.info/spec.htm

// 5.2.5.3
#define SIG_RDSP "RSD PTR "
struct acpi_rdsp {
  uchar signature[8];
  uchar checksum;
  uchar oem_id[6];
  uchar revision;
  uint32 rsdt_addr_phys;
  uint32 length;
  uint64 xsdt_addr_phys;
  uchar xchecksum;
  uchar reserved[3];
} __attribute__((__packed__));

// 5.2.6
struct acpi_desc_header {
  uchar signature[4];
  uint32 length;
  uchar revision;
  uchar checksum;
  uchar oem_id[6];
  uchar oem_tableid[8];
  uint32 oem_revision;
  uchar creator_id[4];
  uint32 creator_revision;
} __attribute__((__packed__));

// 5.2.7
struct acpi_rsdt {
  struct acpi_desc_header header;
  uint32 entry[0];
} __attribute__((__packed__));

struct acpi_xsdt {
  struct acpi_desc_header header;
  uint64 entry[0];
} __attribute__((packed));

struct acpi_generic_address {
  uchar address_space;
  uchar bit_width;
  uchar bit_offset;
  uchar access_size;
  uint64 address;
} __attribute__((packed));

struct acpi_fadt {
  struct acpi_desc_header header;
  uint32 firmware_ctrl;
  uint32 dsdt;

  /* Field used in ACPI 1.0, compatibilty usage only */
  uchar reserved;

  uchar preferred_power_management_profile;
  ushort sci_interrput;
  uint smi_command_port;
  uchar acpi_enable;
  uchar acpi_disable;
  uchar S4BIOS_REQ;
  uchar pstate_control;
  uint PM1a_event_block;
  uint PM1b_event_block;
  uint PM1a_control_block;
  uint PM1b_control_block;
  uint PM2_control_block;
  uint PM_timer_block;
  uint GPE0_block;
  uint GPE1_block;
  uchar PM1_event_length;
  uchar PM1_control_length;
  uchar PM2_control_length;
  uchar PM_timer_length;
  uchar GPE0_length;
  uchar GPE1_length;
  uchar GPE1_base;
  uchar c_state_control;
  ushort worst_C2_latency;
  ushort worst_C3_latency;
  ushort flush_size;
  ushort flush_stride;
  uchar duty_offset;
  uchar duty_width;
  uchar day_alarm;
  uchar month_alarm;
  uchar century;

  /* reserved in ACPI 1.0, used in ACPI 2.0+ */
  ushort boot_architecture_flags;

  uchar reserved_2;
  uint32 flags;

  struct acpi_generic_address reset_reg;

  uchar reset_value;
  uchar reserved_3[3];

  /* pointers available since ACPI 2.0+ */
  uint64 X_firmwareControl;
  uint64 X_dsdt;

  struct acpi_generic_address X_PM1a_event_block;
  struct acpi_generic_address X_PM1b_event_block;
  struct acpi_generic_address X_PM1a_control_block;
  struct acpi_generic_address X_PM1b_control_block;
  struct acpi_generic_address X_PM2_control_block;
  struct acpi_generic_address X_PT_timer_block;
  struct acpi_generic_address X_GPE0_block;
  struct acpi_generic_address X_GPE1_block;
} __attribute__((packed));

struct apci_dsdt {
  
} __attribute__((packed));

#define TYPE_LAPIC 0
#define TYPE_IOAPIC 1
#define TYPE_INT_SRC_OVERRIDE 2
#define TYPE_NMI_INT_SRC 3
#define TYPE_LAPIC_NMI 4

// 5.2.12 Multiple APIC Description Table
#define SIG_MADT "APIC"
struct acpi_madt {
  struct acpi_desc_header header;
  uint32 lapic_addr_phys;
  uint32 flags;
  uchar table[0];
} __attribute__((__packed__));

// 5.2.12.2
#define APIC_LAPIC_ENABLED 1
struct madt_lapic {
  uchar type;
  uchar length;
  uchar acpi_id;
  uchar apic_id;
  uint32 flags;
} __attribute__((__packed__));

// 5.2.12.3
struct madt_ioapic {
  uchar type;
  uchar length;
  uchar id;
  uchar reserved;
  uint32 addr;
  uint32 interrupt_base;
} __attribute__((__packed__));