/*
 * Copyright © 2008-2011 Kristian Høgsberg
 * Copyright © 2011 Intel Corporation
 * Copyright © 2015 Giulio Camuffo
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef WESTON_COMPOSITOR_DRM_H
#define WESTON_COMPOSITOR_DRM_H

#include "compositor.h"
#include "plugin-registry.h"

#ifdef  __cplusplus
extern "C" {
#endif

#define WESTON_DRM_BACKEND_CONFIG_VERSION 2

struct libinput_device;

enum weston_drm_backend_output_mode {
	/** The output is disabled */
	WESTON_DRM_BACKEND_OUTPUT_OFF,
	/** The output will use the current active mode */
	WESTON_DRM_BACKEND_OUTPUT_CURRENT,
	/** The output will use the preferred mode. A modeline can be provided
	 * by setting weston_backend_output_config::modeline in the form of
	 * "WIDTHxHEIGHT" or in the form of an explicit modeline calculated
	 * using e.g. the cvt tool. If a valid modeline is supplied it will be
	 * used, if invalid or NULL the preferred available mode will be used. */
	WESTON_DRM_BACKEND_OUTPUT_PREFERRED,
};

#define WESTON_DRM_OUTPUT_API_NAME "weston_drm_output_api_v1"

struct weston_drm_output_api {
	/** The mode to be used by the output. Refer to the documentation
	 *  of WESTON_DRM_BACKEND_OUTPUT_PREFERRED for details.
	 *
	 * Returns 0 on success, -1 on failure.
	 */
	int (*set_mode)(struct weston_output *output,
			enum weston_drm_backend_output_mode mode,
			const char *modeline);

	/** The pixel format to be used by the output. Valid values are:
	 * - NULL - The format set at backend creation time will be used;
	 * - "xrgb8888";
	 * - "rgb565"
	 * - "xrgb2101010"
	 */
	void (*set_gbm_format)(struct weston_output *output,
			       const char *gbm_format);

	/** The seat to be used by the output. Set to NULL to use the
	 *  default seat.
	 */
	void (*set_seat)(struct weston_output *output,
			 const char *seat);
};

static inline const struct weston_drm_output_api *
weston_drm_output_get_api(struct weston_compositor *compositor)
{
	const void *api;
	api = weston_plugin_api_get(compositor, WESTON_DRM_OUTPUT_API_NAME,
				    sizeof(struct weston_drm_output_api));

	return (const struct weston_drm_output_api *)api;
}

#define WESTON_DRM_VIRTUAL_OUTPUT_API_NAME "weston_drm_virtual_output_api_v1"

struct weston_drm_virtual_output_api {
	/** Create virtual output.
	 *
	 * Returns output on success, NULL on failure.
	 */
	struct weston_output* (*virtual_create)(struct weston_compositor *c,
						char *name);

	/** Set pixel format same as drm_output set_gbm_format() */
	void (*set_gbm_format)(struct weston_output *output,
			       const char *gbm_format);

	/** Get dmabuf fd, size and stride of current_fb */
	void (*get_current_dmabuf)(struct weston_output *output, int *fd,
				   int *stride);
};

static inline const struct weston_drm_virtual_output_api *
weston_drm_virtual_output_get_api(struct weston_compositor *compositor)
{
	const void *api;
	api = weston_plugin_api_get(compositor,
				    WESTON_DRM_VIRTUAL_OUTPUT_API_NAME,
				    sizeof(struct weston_drm_virtual_output_api));
	return (const struct weston_drm_virtual_output_api *)api;
}

/** The backend configuration struct.
 *
 * weston_drm_backend_config contains the configuration used by a DRM
 * backend.
 */
struct weston_drm_backend_config {
	struct weston_backend_config base;

	/** The connector id of the output to be initialized.
	 *
	 * A value of 0 will enable all available outputs.
	 */
	int connector;

	/** The tty to be used. Set to 0 to use the current tty. */
	int tty;

	/** Whether to use the pixman renderer instead of the OpenGL ES renderer. */
	bool use_pixman;

	/** Whether to use the v4l2 renderer insted of the OpenGL ES renderer. */
	bool use_v4l2;

	/** Options of the v4l2 renderer. */
	struct {
		char *device;
		char *device_module;
		bool gl_fallback;
		bool defer_attach;
		/* for backend */
		int max_inputs;
		int max_compose;
		char *scaler_device;
		bool scaler_enable;
	} v4l2ops;

	/** The seat to be used for input and output.
	 *
	 * If NULL the default "seat0" will be used.  The backend will
	 * take ownership of the seat_id pointer and will free it on
	 * backend destruction.
	 */
	char *seat_id;

	/** The pixel format of the framebuffer to be used.
	 *
	 * Valid values are:
	 * - NULL - The default format ("xrgb8888") will be used;
	 * - "xrgb8888";
	 * - "rgb565"
	 * - "xrgb2101010"
	 * The backend will take ownership of the format pointer and will free
	 * it on backend destruction.
	 */
	char *gbm_format;

	/** Callback used to configure input devices.
	 *
	 * This function will be called by the backend when a new input device
	 * needs to be configured.
	 * If NULL the device will use the default configuration.
	 */
	void (*configure_device)(struct weston_compositor *compositor,
				 struct libinput_device *device);
};

#ifdef  __cplusplus
}
#endif

#endif /* WESTON_COMPOSITOR_DRM_H */
