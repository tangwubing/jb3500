#ifndef _LLAYER_H_
#define _LLAYER_H_

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

#include "mlayer.h"

extern unsigned char ll_init(void);
extern unsigned char ll_erase(t_ba pba);
extern unsigned char ll_write(t_ba pba,t_po ppo,unsigned char *buffer);
extern unsigned char ll_writedouble(t_ba pba,t_po ppo, unsigned  char *buffer0,unsigned  char *buffer1);
extern unsigned char ll_read(t_ba pba,t_po ppo, unsigned char *buffer);

enum {
	LL_OK,
	LL_ERASED,
	LL_ERROR
};

#ifdef __cplusplus
}
#endif


/****************************************************************************
 *
 * end of llayer.h
 *
 ***************************************************************************/

#endif	//_LLAYER_H_
