/*
 * Copyright © 2012 Intel Corporation
 *
 * Permission to use, copy, modify, distribute, and sell this software and
 * its documentation for any purpose is hereby granted without fee, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of the copyright holders not be used in
 * advertising or publicity pertaining to distribution of the software
 * without specific, written prior permission.  The copyright holders make
 * no representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 * THE COPYRIGHT HOLDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS
 * SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS, IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF
 * CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "../src/compositor.h"
#include "test-runner.h"

TEST(surface_transform)
{
	struct weston_surface *surface;
	int32_t x, y;

	surface = weston_surface_create(compositor);
	weston_surface_configure(surface, 100, 100, 200, 200);
	weston_surface_update_transform(surface);
	weston_surface_to_global(surface, 20, 20, &x, &y);

	fprintf(stderr, "20,20 maps to %d, %d\n", x, y);
	assert(x == 120 && y == 120);

	weston_surface_set_position(surface, 150, 300);
	weston_surface_update_transform(surface);
	weston_surface_to_global(surface, 50, 40, &x, &y);
	assert(x == 200 && y == 340);

	wl_display_terminate(compositor->wl_display);
}
