#pragma once
#define SFC_HDR_TITLE_OFFSET 0x0 // char[SFC_MAX_TITLE_LENGTH]
#define SFC_HDR_ROM_MODE_OFFSET 0x15 // uint8_t
#define SFC_HDR_CHIPSET_OFFSET 0x16 // uint8_t
#define SFC_HDR_ROM_SIZE_OFFSET 0x17 // uint8_t
#define SFC_HDR_RAM_SIZE_OFFSET 0x18 // uint8_t
#define SFC_HDR_COUNTRY_OFFSET 0x19 // uint8_t
#define SFC_HDR_DEVELOPER_OFFSET 0x1A // uint8_t
#define SFC_HDR_VERSION_OFFSET 0x1B // uint8_t
#define SFC_HDR_CHECKSUM_COMPLEMENT_OFFSET 0x1C // uint16_t
#define SFC_HDR_CHECKSUM_OFFSET 0x1E // uint16_t
#define SFC_HDR_SIZE 32

#define SFC_HDR_TITLE(x) ((char*)(((uint8_t*)x)+SFC_HDR_TITLE_OFFSET))
#define SFC_HDR_ROM_MODE(x) (*((uint8_t*)(((uint8_t*)x)+SFC_HDR_ROM_MODE_OFFSET)))
#define SFC_HDR_CHIPSET(x) (*((uint8_t*)(((uint8_t*)x)+SFC_HDR_CHIPSET_OFFSET)))
#define SFC_HDR_ROM_SIZE(x) (*((uint8_t*)(((uint8_t*)x)+SFC_HDR_ROM_SIZE_OFFSET)))
#define SFC_HDR_RAM_SIZE(x) (*((uint8_t*)(((uint8_t*)x)+SFC_HDR_RAM_SIZE_OFFSET)))
#define SFC_HDR_COUNTRY(x) (*((uint8_t*)(((uint8_t*)x)+SFC_HDR_COUNTRY_OFFSET)))
#define SFC_HDR_DEVELOPER(x) (*((uint8_t*)(((uint8_t*)x)+SFC_HDR_DEVELOPER_OFFSET)))
#define SFC_HDR_VERSION(x) (*((uint8_t*)(((uint8_t*)x)+SFC_HDR_VERSION_OFFSET)))
#define SFC_HDR_CHECKSUM_COMPLEMENT(x) (*((uint16_t*)(((uint8_t*)x)+SFC_HDR_CHECKSUM_COMPLEMENT_OFFSET)))
#define SFC_HDR_CHECKSUM(x) (*((uint16_t*)(((uint8_t*)x)+SFC_HDR_CHECKSUM_OFFSET)))