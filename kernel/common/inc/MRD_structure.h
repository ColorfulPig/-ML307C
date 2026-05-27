#if !defined (MRD_STRUCTURE_H)
#define MRD_STRUCTURE_H

#define VALID_BUFFER_STAMP          0xCAFECAFE
#define MRD_MAX_FILENAME			116

typedef struct {
	UINT32	validBufferStamp;
	UINT32	bufCheckSum;
} MRD_id_t;

typedef struct {
	MRD_id_t MRD_id;
	UINT32	version;
	UINT32	image_size;
	UINT32	update_counter;

} MRD_header_t, *PMRD_header_t;

typedef struct MRD_entry_header_t {
	UINT32			entry_type;
	UINT32			entry_size;
	char  			fileName[MRD_MAX_FILENAME];

	/* NOTE: there field is the reserved in RD entry */
	UINT32			version;
	UINT32			timestamp;
	UINT32			header_checksum;
} MRD_entry_header_t, *PMRD_entry_header_t;

#endif// MRD_STRUCTURE_H

