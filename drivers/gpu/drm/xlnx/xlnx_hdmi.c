// SPDX-License-Identifier: GPL-2.0
/*
 * Xilinx FPGA MIPI HDMI Tx Controller driver.
 *
 * Copyright (C) 2017 - 2018 Xilinx, Inc.
 *
 * Author : Saurabh Sengar <saurabhs@xilinx.com>
 *        : Siva Rajesh J <siva.rajesh.jarugula@xilinx.com>
 */

#define DEBUG 1

#include <drm/drm_atomic_helper.h>
#include <drm/drm_crtc.h>
#include <drm/drm_crtc_helper.h>
#include <drm/drm_encoder_slave.h>
#include <drm/drm_fourcc.h>
#include <drm/drm_gem_cma_helper.h>
#include <linux/component.h>
#include <linux/device.h>
#include <linux/of_device.h>
#include <linux/of_graph.h>
#include <linux/phy/phy.h>
#include <linux/debugfs.h>
#include <video/videomode.h>

#include "xlnx_bridge.h"

#ifdef DEBUG
#define prt_dbg(fmt,...)					\
	printk(KERN_DEBUG "%s (%s:%d): " fmt,			\
	       __FUNCTION__,__FILE__,__LINE__,__VA_ARGS__)
#define ret_dbg(ret,fmt,...)						\
	do {								\
		prt_dbg(fmt,__VA_ARGS__);				\
		return ret;						\
	} while (0)							
#else
#define prt_dbg(fmt,...) do {} while (0)
#define ret_dbg(ret,fmt,...) return ret
#endif

/**
 * struct xlnx_hdmi - Core configuration HDMI Tx subsystem device structure
 * @encoder: DRM encoder structure
 * @dev: device structure
 * @iomem: Base address of HDMI subsystem
 * @mode_flags: HDMI operation mode related flags
 * @format: pixel format for video mode of HDMI controller
 * @vm: videomode data structure
 * @height_out: configurable bridge output height parameter
 * @height_out_prop_val: configurable bridge output height parameter value
 * @width_out: configurable bridge output width parameter
 * @width_out_prop_val: configurable bridge output width parameter value
 * @in_fmt: configurable bridge input media format
 * @in_fmt_prop_val: configurable media bus format value
 * @out_fmt: configurable bridge output media format
 * @out_fmt_prop_val: configurable media bus format value
 */
struct xlnx_hdmi {
	struct drm_encoder encoder;
	struct i2c_client *encoder_slave;
	struct device *dev;
	void __iomem *iomem;
	u32 mode_flags;
	struct videomode vm;
	struct drm_property *height_out;
	u32 height_out_prop_val;
	struct drm_property *width_out;
	u32 width_out_prop_val;
	struct drm_property *in_fmt;
	u32 in_fmt_prop_val;
	struct drm_property *out_fmt;
	u32 out_fmt_prop_val;
};

#define encoder_to_hdmi(e) container_of(e, struct xlnx_hdmi, encoder)

/**
 * xlnx_hdmi_set_config_parameters - Configure HDMI Tx registers with parameters
 * given from user application.
 * @hdmi: HDMI structure having the updated user parameters
 *
 * This function takes the HDMI structure having drm_property parameters
 * configured from  user application and writes them into HDMI IP registers.
 */
static void xlnx_hdmi_set_config_parameters(struct xlnx_hdmi *hdmi)
{
	u32 reg;
	reg = 0xdeadbeef;
	prt_dbg("hdmi: %p\n",hdmi);
}

/**
 * xlnx_hdmi_set_display_mode - Configure HDMI timing registers
 * @hdmi: HDMI structure having the updated user parameters
 *
 * This function writes the timing parameters of HDMI IP which are
 * retrieved from panel timing values.
 */
static void xlnx_hdmi_set_display_mode(struct xlnx_hdmi *hdmi)
{
	u32 reg;
	reg = 0xdeadbeef;
	prt_dbg("hdmi: %p\n",hdmi);
}

/**
 * xlnx_hdmi_set_display_enable - Enables the HDMI Tx IP core enable
 * register bit
 * @hdmi: HDMI structure having the updated user parameters
 *
 * This function takes the HDMI strucure and enables the core enable bit
 * of core configuration register.
 */
static void xlnx_hdmi_set_display_enable(struct xlnx_hdmi *hdmi)
{
	u32 reg;
	reg = 0xdeadbeef;
	prt_dbg("hdmi: %p\n",hdmi);
}

/**
 * xlnx_hdmi_set_display_disable - Disable the HDMI Tx IP core enable
 * register bit
 * @hdmi: HDMI structure having the updated user parameters
 *
 * This function takes the HDMI strucure and disables the core enable bit
 * of core configuration register.
 */
static void xlnx_hdmi_set_display_disable(struct xlnx_hdmi *hdmi)
{
	u32 reg;
	reg = 0xdeadbeef;
	prt_dbg("hdmi: %p\n",hdmi);
}

/**
 * xlnx_hdmi_atomic_mode_set -  derive the HDMI timing parameters
 *
 * @encoder: pointer to Xilinx DRM encoder
 * @crtc_state: Pointer to drm core crtc state
 * @connector_state: HDMI connector drm state
 *
 * This function derives the HDMI IP timing parameters from the timing
 * values given in the attached panel driver.
 */
static void
xlnx_hdmi_atomic_mode_set(struct drm_encoder *encoder,
			  struct drm_crtc_state *crtc_state,
			  struct drm_connector_state *connector_state)
{
	struct xlnx_hdmi *hdmi = encoder_to_hdmi(encoder);
	struct videomode *vm = &hdmi->vm;
	struct drm_display_mode *m = &crtc_state->adjusted_mode;
	prt_dbg("encoder: %p crtc_statue: %p connector_state: %p\n",encoder,crtc_state,connector_state);
	vm->hactive = m->hdisplay;
	vm->vactive = m->vdisplay;
	vm->vfront_porch = m->vsync_start - m->vdisplay;
	vm->vback_porch = m->vtotal - m->vsync_end;
	vm->vsync_len = m->vsync_end - m->vsync_start;
	vm->hfront_porch = m->hsync_start - m->hdisplay;
	vm->hback_porch = m->htotal - m->hsync_end;
	vm->hsync_len = m->hsync_end - m->hsync_start;
	xlnx_hdmi_set_display_mode(hdmi);
}

static void xlnx_hdmi_disable(struct drm_encoder *encoder)
{
	struct xlnx_hdmi *hdmi = encoder_to_hdmi(encoder);
	prt_dbg("encoder: %p\n",encoder);
	xlnx_hdmi_set_display_disable(hdmi);
}

static void xlnx_hdmi_enable(struct drm_encoder *encoder)
{
	struct xlnx_hdmi *hdmi = encoder_to_hdmi(encoder);
	prt_dbg("encoder: %p\n",encoder);
	xlnx_hdmi_set_display_enable(hdmi);
}

static const struct drm_encoder_helper_funcs xlnx_hdmi_encoder_helper_funcs = {
	.atomic_mode_set = xlnx_hdmi_atomic_mode_set,
	.enable = xlnx_hdmi_enable,
	.disable = xlnx_hdmi_disable,
};

static const struct drm_encoder_funcs xlnx_hdmi_encoder_funcs = {
	.destroy = drm_encoder_cleanup,
};

static int xlnx_hdmi_parse_dt(struct xlnx_hdmi *hdmi)
{
	struct device *dev = hdmi->dev;
	struct device_node *np = dev->of_node;
	struct device_node *slave_node;

	slave_node = of_graph_get_remote_node(np,1,0);
	if (!slave_node)
		ret_dbg(-EINVAL,"slave_node: ERROR=%d\n",-EINVAL);

	hdmi->encoder_slave = of_find_i2c_device_by_node(slave_node);
	of_node_put(slave_node);

	if (!hdmi->encoder_slave || !hdmi->encoder_slave->dev.driver)
		ret_dbg(-EPROBE_DEFER,"hdmi->encoder_slave: ERROR=%d\n",-EPROBE_DEFER);

	return 0;
}

static int xlnx_hdmi_bind(struct device *dev, struct device *master, void *data)
{
	struct xlnx_hdmi *hdmi = dev_get_drvdata(dev);
	struct drm_encoder *encoder = &hdmi->encoder;
	struct drm_device *drm_dev = data;
	struct drm_bridge *bridge;
	int ret;

	/*
	 * TODO: The possible CRTCs are 1 now as per current implementation of
	 * HDMI tx drivers. DRM framework can support more than one CRTCs and
	 * HDMI driver can be enhanced for that.
	 */
	encoder->possible_crtcs = 1;
	drm_encoder_init(drm_dev, encoder, &xlnx_hdmi_encoder_funcs, DRM_MODE_ENCODER_TMDS, NULL);
	drm_encoder_helper_add(encoder, &xlnx_hdmi_encoder_helper_funcs);

	bridge = of_drm_find_bridge(hdmi->encoder_slave->dev.of_node);
	if (!bridge) {
		dev_err(hdmi->dev, "Failed to find bridge\n");
		ret = -ENODEV;
		goto err_encoder;
	}
	
	bridge->encoder = encoder;
	encoder->bridge = bridge;
	ret = drm_bridge_attach(encoder, bridge, NULL);
	if (ret) {
		dev_err(hdmi->dev, "Failed to attach bridge\n");
		goto err_encoder;
	}

	ret_dbg(ret,"drm_bridge_attach: RETURN=%d\n",ret);

err_encoder:
	drm_encoder_cleanup(encoder);
	ret_dbg(ret,"drm_encoder: ERROR=%d\n",ret);	
}

static void xlnx_hdmi_unbind(struct device *dev, struct device *master,
			    void *data)
{
	struct xlnx_hdmi *hdmi = dev_get_drvdata(dev);

	xlnx_hdmi_disable(&hdmi->encoder);
}

static const struct component_ops xlnx_hdmi_component_ops = {
	.bind	= xlnx_hdmi_bind,
	.unbind	= xlnx_hdmi_unbind,
};

static int xlnx_hdmi_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct xlnx_hdmi *hdmi;
	int ret;

	hdmi = devm_kzalloc(dev, sizeof(*hdmi), GFP_KERNEL);
	if (!hdmi)
		ret_dbg(-ENOMEM,"devm_kzalloc: ERROR=%d\n",-ENOMEM);

	hdmi->dev = dev;

	ret = xlnx_hdmi_parse_dt(hdmi);
	if (ret)
		ret_dbg(ret,"xlnx_hdmi_parse_dt: ERROR=%d\n",ret);

	platform_set_drvdata(pdev, hdmi);

	ret = component_add(dev, &xlnx_hdmi_component_ops);

	ret_dbg(ret,"component_add: RETURN=%d\n",ret);
}

static int xlnx_hdmi_remove(struct platform_device *pdev)
{
	component_del(&pdev->dev, &xlnx_hdmi_component_ops);

	return 0;
}

static const struct of_device_id xlnx_hdmi_of_match[] = {
	{ .compatible = "xlnx,hdmi"},
	{ }
};
MODULE_DEVICE_TABLE(of, xlnx_hdmi_of_match);

static struct platform_driver hdmi_driver = {
	.probe = xlnx_hdmi_probe,
	.remove = xlnx_hdmi_remove,
	.driver = {
		.name = "xlnx-hdmi",
		.of_match_table = xlnx_hdmi_of_match,
	},
};

module_platform_driver(hdmi_driver);

MODULE_AUTHOR("Ole Rohne");
MODULE_DESCRIPTION("Xilinx FPGA HDMI Tx Driver");
MODULE_LICENSE("GPL v2");
