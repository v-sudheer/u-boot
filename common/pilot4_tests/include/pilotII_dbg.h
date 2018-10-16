/*******************************************************************************
 *
 * Copyright (C) 2004-2014 Emulex. All rights reserved.
 * EMULEX is a trademark of Emulex.
 * www.emulex.com
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of version 2 of the GNU General Public License as published by the
 * Free Software Foundation.
 * This program is distributed in the hope that it will be useful. ALL EXPRESS
 * OR IMPLIED CONDITIONS, REPRESENTATIONS AND WARRANTIES, INCLUDING ANY IMPLIED
 * WARRANTY OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR
 * NON-INFRINGEMENT, ARE DISCLAIMED, EXCEPT TO THE EXTENT THAT SUCH DISCLAIMERS
 * ARE HELD TO BE LEGALLY INVALID. See the GNU General Public License for more
 * details, a copy of which can be found in the file COPYING included
 * with this package.
 *
 ********************************************************************************/

#define	PILOTII_DEBUG	1

/*
  Debug message level
*/
#define MSG_LEVEL_ALL                0
#define MSG_LEVEL_ENTER_FUNCTION     1
#define	MSG_LEVEL_EXIT_FUNCTION	     2
#define MSG_LEVEL_INFO               5
#define MSG_LEVEL_TEST               8
#define MSG_LEVEL_HIGH               9
#define MSG_LEVEL_ALWAYS             10
#define MSG_LEVEL_WARNING            15
#define MSG_LEVEL_ERROR              20
#define MSG_LEVEL_FATAL              30
#define MSG_LEVEL_DEFAULT            MSG_LEVEL_ALWAYS   /* must less or equal MSG_LEVEL_ALWAYS */

#ifdef	PILOTII_DEBUG
extern	unsigned int gDbgLevel;
#define DBG_MSG( L, M )  if ( (L) >= gDbgLevel ) {                                                    \
                                 nc_printf M;                                                      \
                                 }
#else

#define	DBG_MSG( L, M)

#endif
