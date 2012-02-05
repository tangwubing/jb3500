#ifndef _MLAYER_H_
#define _MLAYER_H_

/****************************************************************************
 *
 *            Copyright (c) 2005 by HCC Embedded 
 *
 * This software is copyrighted by and is the sole property of 
 * HCC.  All rights, title, ownership, or other interests
 * in the software remain the property of HCC.  This
 * software may only be used in accordance with the corresponding
 * license agreement.  Any unauthorized use, duplication, transmission,  
 * distribution, or disclosure of this software is expressly forbidden.
 *
 * This Copyright notice may not be removed or modified without prior
 * written consent of HCC.
 *
 * HCC reserves the right to modify this software without notice.
 *
 * HCC Embedded
 * Budapest 1132
 * Victor Hugo Utca 11-15
 * Hungary
 *
 * Tel:  +36 (1) 450 1302
 * Fax:  +36 (1) 450 1303
 * http: www.hcc-embedded.com
 * email: info@hcc-embedded.com
 *
 ***************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#if 1
typedef unsigned char t_bit;
#else
typedef bit t_bit;
#endif

typedef unsigned short t_ba; //typedef for block address
typedef unsigned char  t_po; //typedef for page offset

typedef struct {
t_ba pba;	    //physical address of bad block, where information is stored
t_po ppo;		//current page which holds current status
unsigned char index;	//in FLTable where this was allocated from
} ST_BADBLOCK;   //��������ڴ��еľ���λ��


extern t_bit ml_init(void);
extern t_bit ml_format(void);

extern t_bit ml_open(unsigned long sector, unsigned long secnum, unsigned char mode);
extern t_bit ml_write(unsigned char *data);
extern t_bit ml_read(unsigned char *data);
extern t_bit ml_close(void);
extern unsigned long ml_getmaxsector(void);

#define MLAYER_SETS				4  //has meaning only if emlayer is used
#define MAX_BLOCK				(1024*3)//NAND_BLK_QTY//16384 //1024 //4096
#define MAX_PAGE_PER_BLOCK		NAND_BLK_PAGE//32
#define MAX_FREE_BLOCK			64	//(ori64)���п��� --����5�����������MAP�������
#define MAX_LOG_BLOCK			31	//(ori31)
#define MAX_RESERVED_BLOCK		32	//(ori32)Ԥ����������Ϊ����������


extern t_ba *gl_freetable;

#define	FT_ADDRMASK				((t_ba)0x3FFF)     //15λ
#define	FT_MAP					0x8000   //��ʾĳһ����ΪMAP���
#define	FT_LOG					0x4000   //��ʾĳһ����ΪLOG��
#define FT_BAD					(FT_MAP | FT_LOG)

typedef struct {
	unsigned long wear; /**< spare area 32-bit Wear Leveling Counter [byte 3] */

	union {
		unsigned char dummy[4]; //max 4 bytes

		struct {
			t_ba lba;
			t_po lpo;
		} log;

		struct {
		  unsigned long ref_count; /* 32bit MAP block reference counter */
		} map;			/* map block 1st entry spare area*/
	} u;

	unsigned char page_state;
	unsigned char frag;		/* fragment number if mappage*/

	unsigned char block_type;	/**< spare area Block Type (LOG, FREE, MAP, DATA) */
	unsigned char bad_tag;		/**< spare area offset of the bad block tag (0x0) */

	unsigned long ecc;
} ST_SPARE;

#define MAX_DATA_SIZE			NAND_PAGE_DATA
#define MAX_SPARE_SIZE			NAND_PAGE_SPARE
#if MAX_SPARE_SIZE < 16
#error Nand flash page spare area is too small!!!!!!
#endif

#define MAX_PAGE_SIZE			(MAX_DATA_SIZE + MAX_SPARE_SIZE)
#define MAX_BLOCK_SIZE			(MAX_PAGE_SIZE*MAX_PAGE_PER_BLOCK)
#define MAX_SIZE				(MAX_BLOCK * MAX_BLOCK_SIZE)
#define MAX_FRAGSIZE			((MAX_DATA_SIZE/2)/sizeof(t_ba))		//ÿ��ӳ���ɷ�ӳ���ϵ��Ŀ
#define MAX_FRAGNUM				(MAX_BLOCK / MAX_FRAGSIZE)				//�ܹ���Ҫ���ٸ�ӳ�������ӳ����Ϣ

#define MAX_NUM_OF_DIF_MAP		4										//�ܹ���4���������ӳ���
#define MAX_FRAG_PER_BLK		(MAX_FRAGNUM/MAX_NUM_OF_DIF_MAP)		//ÿ��ӳ���ĸ���

#define BLK_TYPE_MAP_00			0x10									//ӳ���0    //���ӳ���ϵ
#define BLK_TYPE_MAP_01			(BLK_TYPE_MAP_00+0x01)					//ӳ���1
#define BLK_TYPE_MAP_02			(BLK_TYPE_MAP_00+0x02)					//ӳ���2
#define BLK_TYPE_MAP_03			(BLK_TYPE_MAP_00+0x03)					//ӳ���3

#define BLK_TYPE_DAT			0x00
#define BLK_TYPE_BAD			0x80

#define STA_ORIGFRAG			0x01   /* if page contains original fragment info only */
#define STA_MAPPAGE				0x02   /* if page contains FLT, a fragment, and other information */

#define MAX_MAP_BLK				0x03    //����ͬʱ���ڵ�map block��

#define GET_SPARE_AREA(_buf_) ((ST_SPARE*)(((unsigned char*)(_buf_)) + MAX_DATA_SIZE))

typedef struct {
	t_ba pba;
	t_po ppo;
	unsigned char index;   //����ӳ���ռһ��page��index��1��ǰ�벿�֣�0����벿��
} ST_MAPDIR;   //ÿ��ӳ����ŵľ���λ��



typedef struct {
	t_ba pba;		/* current map block address; BLK_NA if we don't have one (fatal error) */
	t_po ppo;		/* current page offset in map */
	t_ba last_pba;
	t_po last_ppo;				/*last good written map situated here */
	unsigned long ref_count;	/* last written counter in MAP block */
	unsigned char start_frag;   /* start fragment number in this MAP */
	unsigned char end_frag;     /* end fragment number in this MAP */
	unsigned char block_type;  /*type in the spare area of this block*/

	ST_MAPDIR *mapdir;		   /* start entry in mg_mapdir*/
	unsigned char index[MAX_MAP_BLK];
	unsigned char indexcou;	   /* current number of MAP blocks */
	//only at start up
	unsigned long mappagecou_hi;
	unsigned long mappagecou_lo; //searrching for the latest correct
} ST_MAPBLOCK;   //4�����ӳ������������Ϣ

typedef struct {
	t_ba last_pba;
	t_po last_ppo;
	ST_BADBLOCK badblock;
	unsigned long mappagecou_hi;
	unsigned long mappagecou_lo; //current counter
	unsigned char index[MAX_MAP_BLK]; //common!!!
} ST_MAPINFO; //���ڴ�������Ϣ��

#define MAX_CACHEFRAG 4          //���Ի��漸��ӳ���

typedef struct {
	unsigned char indexes[MAX_CACHEFRAG]; //����˵ڼ���ӳ���
	t_ba *ppbas[MAX_CACHEFRAG];           //ÿ��ӳ������ڴ��е�λ��
	unsigned char current;                //��ǰ�����ĸ�ӳ���
	unsigned char pos;                    //��ǰӳ����=indexes[pos]
	t_ba *ppba;                           //��ǰ����ӳ������ڴ��е�λ��
} ST_FRAG;    //������ڴ���ӳ�����Ϣ

enum {
	ML_CLOSE,
	ML_PENDING_READ,
	ML_PENDING_WRITE,
	ML_READ,
	ML_WRITE,
	ML_ABORT,
	ML_INIT
};

typedef struct {
	unsigned long wear;
	t_ba lba;		//which logical is this
	t_ba pba;		//what its phisical
	unsigned char ppo[MAX_PAGE_PER_BLOCK];
	unsigned char lastppo;
	unsigned char index;
	unsigned char switchable;
} ST_LOG;  //��¼��ǰ���ڸ��µ����ɸ�����Ϣ


#ifdef __cplusplus
}
#endif


/****************************************************************************
 *
 * end of mlayer.h
 *
 ***************************************************************************/

#endif	//_MLAYER_H_
