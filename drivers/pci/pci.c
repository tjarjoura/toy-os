#include <stdint.h>

#include "common.h"
#include "lib.h"
#include "sysio.h"

#define CONFIG_ADDRESS 0xCF8
#define CONFIG_DATA    0xCFC

static const char *pci_classes[] = {"Pre Classification",
                                    "Mass Storage Controller",
                                    "Network Controller",
                                    "Display Controller",
                                    "Multimedia Controller",
                                    "Memory Controller",
                                    "Bridge Device",
                                    "Simple Communications Controller",
                                    "Base System Peripheral",
                                    "Input Device",
                                    "Docking Station",
                                    "Processor",
                                    "Serial Bus Controller",
                                    "Wireless Controller",
                                    "Intelligent I/O Controller",
                                    "Satellite Communications Controller",
                                    "Encryption/Decryption Controller",
                                    "Data Acquisition and Signal Processing Controller",
                                    "Unknown"
};

static uint32_t pci_config_read(uint8_t bus, uint8_t device, uint8_t function, uint8_t reg) {
    uint32_t config_address;

    config_address = 0x80000000 | ((uint32_t) bus << 16) | ((uint32_t) device << 11) \
      | ((uint32_t) function << 8) | (reg & 0xFC);

    outl(CONFIG_ADDRESS, config_address);

    return inl(CONFIG_DATA);
}



void pci_scan() {
    int bus, device, function;
    int n = 0;
    const char *class;
    uint32_t ret;
    uint16_t vendor_id, device_id;
    uint8_t class_id, subclass, prog_if;

    kprintf("Addr\tVendID\tDevID\tClass\tSubCl\tProgIF\n");

    for (bus = 0; bus < 256; bus++) {
        for (device = 0; device < 32; device++) {
            for (function = 0; function < 8; function++) {
                ret = pci_config_read(bus, device, function, 0);
                vendor_id = (ret & 0xFFFF);
                device_id = ((ret >> 16) & 0xFFFF);

                if (vendor_id == 0xFFFF)
                    continue;

                ret = pci_config_read(bus, device, function, 8);

                class_id = (ret >> 24);
                subclass = (ret >> 16) & 0xFF;
                prog_if = (ret >> 8) & 0xFF;

                if (class_id < 18)
                    class = pci_classes[class_id];
                else
                    class = pci_classes[18];

                kprintf("%d:%d:%d\t%x\t%x\t%x\t%x\t%x  %s\n",
                    bus, device, function, vendor_id, device_id, class_id, subclass, prog_if, class);
                n++;
            }
        }
    }

    kprintf("No. devices: %d\n", n);
}

