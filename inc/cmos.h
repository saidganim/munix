#ifndef MX_CMOS
#define MX_CMOS

#define CMOS_CURR_SECONDS 0x0
#define CMOS_ALRM_SECONDS 0x1
#define CMOS_CURR_MINUTES 0x2
#define CMOS_ALRM_MINUTE 0x3
#define CMOS_CURR_HOURS 0x4
#define CMOS_ALRM_HOURS 0x5
#define CMOS_CURR_WKD 0x6 // current day of week
#define CMOS_CURR_DOM 0x7 // current day of month
#define CMOS_CURR_MNTH 0x8 // current month
#define CMOS_CURR_YEAR 0x9

#define CMOS_CUR_STS_A 0xA // RTC status register A
#define CMOS_CUR_STS_B 0xB // RTC status register B
#define CMOS_CUR_STS_C 0xC // RTC status register C(Read-only)
#define CMOS_CUR_STS_D 0xD // RTC status register D
#define CMOS_DGNSTC 0xE // RTC diagnostics byte

#define CMOS_SHUTDOWN 0xF // shutdown status byte
#define CMOS_DD_TYPE 0x10 // diskette drive types
// 0x11 - RESERVED AT
#define CMOS_HDD_TYPE 0x12 // hard disk drive type  (between 1 and 14)
// 0x13 - RESERVED
#define CMOS_EQPMNT 0x14 // Equipment byte
#define CMOS_LOW_MEML 0x15 // Base memory (low byte)
#define CMOS_LOW_MEMH 0x16 // Base memory (high byte)
#define CMOS_EXT_MEML 0x14 // extended memory above 1M (low byte)
#define CMOS_EXT_MEMH 0x14 // extended memory (high byte)

#define CMOS_PORT1 0x70
#define CMOS_PORT2 0x71

char cmos_read(char);
void cmos_write(char, char);

#endif
