.global _Reset
_Reset:
 LDR sp, =0x8000
 BL c_entry
 B .
