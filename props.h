#define GET_CLOCK_SPEED 0x00030002
#define GET_MAX_CLOCK_SPEED 0x00030004
#define SET_POWER_STATE 0x00028001
#define GET_ARM_MEMORY 0x00010005

/* Packet HEADER */
struct props_header
{
	unsigned int buffer_size; // Size in bytes (including header and values)

	/*
	** Request codes:
	**        0x00000000: process request
	**        All other values reserved
	** Response codes:
	**        0x80000000: request successful
	**        0x80000001: error parsing request buffer
	**                     (partial response)
	*/
	unsigned int req_resp_code;
};

/* Packet CONTENTS */
struct prop_tags
{
	unsigned int id; // 0x00028001 = set power state
	unsigned int value_size;
	unsigned int req_resp_ind; // 0 = request
							   // unsigned int value[BUF_LEN]; // value length varies for each tag
};

/** A generic structure for requests / responses with 0-8 byte request size and 8 byte response size */
struct props8
{
	struct props_header props_header;
	struct prop_tags prop_tags;
	unsigned int selector;
	unsigned int value;
	unsigned int end_tag;
};

void *props_mail(volatile void *message);

/**
 * A generic method for requests / responses with 8 byte request size and 8 byte response size.
 * Returns the extra response word.
 */
unsigned int props8(unsigned int tag, unsigned int selector, unsigned int value);

/**
 * A generic method for requests / responses with 4 byte request size and 8 byte response size.
 * Returns the extra response word.
 */
unsigned int props_4_8(unsigned int tag, unsigned int selector);
