/*
 * Copyright (C) 1999-2001, 2016 Free Software Foundation, Inc.
 * This file is part of the GNU LIBICONV Library.
 *
 * The GNU LIBICONV Library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * The GNU LIBICONV Library is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with the GNU LIBICONV Library; see the file COPYING.LIB.
 * If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * TIS620.2533-1
 */

static int
lk8t_mbtowc (conv_t conv, ucs4_t *pwc, const unsigned char *s, size_t n)
{
  unsigned char c = *s;
  if (c < 0x80) {
    *pwc = (ucs4_t) c;
    return 1;
  }
  else if (c >= 0x80 && c <= 0xf4 && !(c >= 0xc7 && c <= 0xf0)) {
    *pwc = (ucs4_t) (c + 0x0d00);
    return 1;
  }
  return RET_ILSEQ;
}

static int
lk8t_wctomb (conv_t conv, unsigned char *r, ucs4_t wc, size_t n)
{
  if (wc < 0x0080) {
    *r = wc;
    return 1;
  }
  else if (wc == 0x200d) {
      return 0;
  }
  else if (wc >= 0x0d80 && wc <= 0x0Dff && !(wc >= 0x0d3b && wc <= 0x0d3e)) {
    if (wc == 0x0dd9) {
        //*(r-1) = 0xd9ff & (*(r - 1) & 0xff00 >> 8);
        unsigned char tmp = *(r - 1);
        unsigned char shift = (wc-0x0ce0);
        *(r-1) = shift;
        *r = tmp;
        //return 0;
        //*(r - 1) = *r && (wc-0x0d00 << 8);
    } else if (wc == 0x0dda) {
        unsigned char tmp = *(r - 1);
        unsigned char shift = (wc-0x0ce0);
        *(r-1) = 0xf9;
        r[0] = tmp;
        r[1] = 0xea;
        //*r = 0xea;
        return 2;
    } else if (wc == 0x0ddb) {
        unsigned char tmp = *(r - 1);
        unsigned char shift = (0x0dd9-0x0ce0);
        *(r-1) = shift;
        r[0] = shift;
        r[1] = tmp;
        //*r = 0xea;
        return 2;
    } else if (wc == 0x0ddc) {
        unsigned char tmp = *(r - 1);
        unsigned char shift = (0x0dcf-0x0ce0);
        *(r-1) = 0xf9;
        r[0] = tmp;
        r[1] = shift;
        //*r = 0xea;
        return 2;
    } else if (wc == 0x0ddd) {
        unsigned char tmp = *(r - 1);
        unsigned char shift = (wc-0x0ce0);
        *(r-1) = 0xf9;
        r[0] = tmp;
        r[1] = 0x0dcf-0x0ce0;
        r[2] = 0x0dca-0x0ce0;
        //*r = 0xea;
        return 3;
    } else if (wc == 0x0dde) {
        unsigned char tmp = *(r - 1);
        unsigned char shift = (wc-0x0ce0);
        *(r-1) = 0xf9;
        r[0] = tmp;
        r[1] = 0x0ddf-0x0ce0;

        //*r = 0xea;
        return 2;
    } else if (wc == 0x0df2) {
        r[0] = 0x0dd8-0x0ce0;
        r[1] = 0x0dd8-0x0ce0;
        return 2;
    }
    else *r = wc-0x0ce0;
    return 1;
  }
  return RET_ILUNI;
}

static int
lk8t_reset (conv_t conv, unsigned char *r, size_t n)
{
  state_t state = conv->ostate;
  if (state) {
    if (n < 2)
      return RET_TOOSMALL;
    r[0] = '~';
    r[1] = '}';
    /* conv->ostate = 0; will be done by the caller */
    return 2;
  } else
    return 0;
}
