/*****************************************************************************
 * pce                                                                       *
 *****************************************************************************/

/*****************************************************************************
 * File name:     src/arch/sim405/sercons.c                                  *
 * Created:       2004-06-01 by Hampa Hug <hampa@hampa.ch>                   *
 * Last modified: 2004-12-15 by Hampa Hug <hampa@hampa.ch>                   *
 * Copyright:     (C) 2004 Hampa Hug <hampa@hampa.ch>                        *
 *****************************************************************************/

/*****************************************************************************
 * This program is free software. You can redistribute it and / or modify it *
 * under the terms of the GNU General Public License version 2 as  published *
 * by the Free Software Foundation.                                          *
 *                                                                           *
 * This program is distributed in the hope  that  it  will  be  useful,  but *
 * WITHOUT  ANY   WARRANTY,   without   even   the   implied   warranty   of *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU  General *
 * Public License for more details.                                          *
 *****************************************************************************/

/* $Id$ */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/poll.h>

#include "main.h"


static
int scon_readable (int fd, int t)
{
  int           r;
  struct pollfd pfd[1];

  pfd[0].fd = fd;
  pfd[0].events = POLLIN;

  r = poll (pfd, 1, t);
  if (r < 0) {
    return (0);
  }

  if ((pfd[0].revents & POLLIN) == 0) {
    return (0);
  }

  return (1);
}

void scon_check (sim405_t *sim)
{
  unsigned      i, n;
  unsigned char buf[8];
  ssize_t       r;

  if (par_sig_int) {
    par_sig_int = 0;
    s405_set_keycode (sim, 0x03);
  }

  if (!scon_readable (0, 0)) {
    return;
  }

  r = read (0, buf, 8);
  if (r <= 0) {
    return;
  }

  n = (unsigned) r;

  if ((n == 1) && (buf[0] == 0)) {
    s405_set_msg (sim, "emu.stop", "");
    return;
  }
#if 0
  if ((n == 1) && (buf[0] == 0x1b)) {
    s405_set_msg (sim, "emu.stop", "");
    return;
  }
#endif
  else if ((n == 1) && (buf[0] == 0xe0)) {
    s405_set_msg (sim, "emu.abort", "");
    return;
  }

  for (i = 0; i < n; i++) {
    s405_set_keycode (sim, buf[i]);
  }
}
