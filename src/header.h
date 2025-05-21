#pragma once
#define TITLE_OFFSET 0x0 // char[SFC_MAX_TITLE_LENGTH]
#define ROM_MODE_OFFSET 0x15 // uint8_t
#define CHIPSET_OFFSET 0x16 // uint8_t
#define ROM_SIZE_OFFSET 0x17 // uint8_t
#define RAM_SIZE_OFFSET 0x18 // uint8_t
#define COUNTRY_OFFSET 0x19 // uint8_t
#define DEVELOPER_OFFSET 0x1A // uint8_t
#define VERSION_OFFSET 0x1B // uint8_t
#define CHECKSUM_COMPLEMENT_OFFSET 0x1C // uint16_t
#define CHECKSUM_OFFSET 0x1E // uint16_t
#define HEADER_SIZE 32

#define TITLE(x) ((char*)(((uint8_t*)x)+TITLE_OFFSET))
#define ROM_MODE(x) (*((uint8_t*)(((uint8_t*)x)+ROM_MODE_OFFSET)))
#define CHIPSET(x) (*((uint8_t*)(((uint8_t*)x)+CHIPSET_OFFSET)))
#define ROM_SIZE(x) (*((uint8_t*)(((uint8_t*)x)+ROM_SIZE_OFFSET)))
#define RAM_SIZE(x) (*((uint8_t*)(((uint8_t*)x)+RAM_SIZE_OFFSET)))
#define COUNTRY(x) (*((uint8_t*)(((uint8_t*)x)+COUNTRY_OFFSET)))
#define DEVELOPER(x) (*((uint8_t*)(((uint8_t*)x)+DEVELOPER_OFFSET)))
#define VERSION(x) (*((uint8_t*)(((uint8_t*)x)+VERSION_OFFSET)))
#define CHECKSUM_COMPLEMENT(x) (*((uint16_t*)(((uint8_t*)x)+CHECKSUM_COMPLEMENT_OFFSET)))
#define CHECKSUM(x) (*((uint16_t*)(((uint8_t*)x)+CHECKSUM_OFFSET)))