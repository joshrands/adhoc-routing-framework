#include <stddef.h>
#include <errno.h>
#include <endian.h>

#define get_unaligned_16(p)					\
({								\
	struct packed_dummy_struct {				\
		uint16_t __val;				\
	} __attribute__((packed)) *__ptr = (packed_dummy_struct*) (p);	\
								\
	__ptr->__val;						\
})

#define get_unaligned_32(p)					\
({								\
	struct packed_dummy_struct {				\
		uint32_t __val;				\
	} __attribute__((packed)) *__ptr = (packed_dummy_struct*) (p);	\
								\
	__ptr->__val;						\
})


#define get_unaligned_le16(p)	le16toh(get_unaligned_16((uint16_t *)(p)))
#define get_unaligned_le32(p)	le32toh(get_unaligned_32((uint32_t *)(p)))
