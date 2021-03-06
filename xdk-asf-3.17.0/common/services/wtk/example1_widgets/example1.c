/**
 * \file
 *
 * \brief Graphic Widget Toolkit library example application
 *
 * Copyright (c) 2012-2013 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

/**
 * \mainpage
 *
 * \section intro Introduction
 * This example will create a simple touch-enabled GUI on the board display,
 * using the Widget Toolkit library.
 *
 * \section files Main files:
 * - example1.c: main example implementation
 * - widget_gui.c : display GUI creation and event handling
 * - widget_gui.h : header for the GUI management routines
 * - touch_interface.c : touch screen interface routines
 * - touch_interface.h : header for the touch screen interface routines
 * - atmel_logo_small.h : Atmel logo for output to the display
 * - conf_board.h: board configuration
 * - conf_clock.h: system clock driver configuration
 * - conf_membag.h: memory bag allocation configuration
 * - conf_sysfont.h: system font configuration
 * - conf_twim.h: TWI driver configuration
 * - conf_win.h: window system configuration
 * - conf_wtk.h: widget library configuration
 *
 * For boards using the mXT143E Xplained only:
 * - conf_ili9341.h: display driver configuration
 * - conf_mxt.h: maXTouch driver configuration
 * - UC3 and SAM only: conf_spi_master.h: SPI master service configuration
 * - XMEGA only: conf_usart_spi.h: USART SPI service configuration
 *
 * For the UC3C-EK only:
 * - conf_ebi.h
 * - conf_et024006dhu.h
 * - conf_rtouch.h
 *
 * \section apiinfo Widget library API
 * The widget library API can be found \ref wtk_group "here", while the
 * graphics service API can be found \ref gfx_group "here".
 *
 * \section deviceinfo Device Info
 * This example has been tested with the following kits:
 * - UC3-A3 Xplained (with mXT143E Xplained top module)
 * - UC3-L0 Xplained (with mXT143E Xplained top module)
 * - XMEGA-A1 Xplained (with mXT143E Xplained top module)
 * - XMEGA-B1 Xplained (with mXT143E Xplained top module)
 * - XMEGA-A3BU Xplained (with mXT143E Xplained top module)
 * - SAM4S Xplained (with mXT143E Xplained top module)
 * - UC3C-EK
 *
 * \section exampledescription Description of the example
 * On startup the example application will create a simple GUI on the display of
 * the board, showing a slider, progress bar, check boxes, radio buttons and a
 * standard button. Touching the display will manipulate each of these
 * widgets.
 *
 * \section dependencies Dependencies
 * Relevant module dependencies for this application are:
 * - \ref wtk_group
 * - \ref gfx_group
 *
 * \section compinfo Compilation Info
 * This software was written for the GNU GCC and IAR for AVR.
 * Other compilers may or may not work.
 *
 * \section contactinfo Contact Information
 * For further information, visit
 * <A href="http://www.atmel.com/">Atmel</A>.\n
 */

#include <asf.h>
#include "touch_interface.h"
#include "widget_gui.h"
#include "atmel_logo_small.h"

/**
 * \brief Configure the root GUI window
 *
 * Configures the root window background to be a tiled version of the
 * Atmel company logo, and sets it as being visible.
 */
static void setup_gui_root_window(void)
{
	struct win_attributes root_attr;
	struct win_window *win_root;

	win_root = win_get_root();
	root_attr = *win_get_attributes(win_root);
	root_attr.background = &atmel_logo_small;
	win_set_attributes(win_root, &root_attr, WIN_ATTR_BACKGROUND);

	win_show(win_root);
}

/**
 * \brief Show an out of memory error on the display
 *
 * Shows a full screen error when called, signaling that the system
 * ran out of memory when initializing a WTK application.
 */
static void show_out_of_memory_error(void)
{
	const char memory_string[] = "OUT OF MEMORY";
	gfx_coord_t disp_width, disp_height;

	/* Get the display width and height */
	disp_width  = gfx_get_width();
	disp_height = gfx_get_height();

	/* Blank display */
	gfx_set_clipping(0, 0, disp_width, disp_height);
	gfx_draw_filled_rect(0, 0, disp_width, disp_height, GFX_COLOR_BLACK);

	/* Show centered out of memory error text */
	gfx_draw_string_aligned(memory_string,
		disp_width / 2, disp_height / 2,
		&sysfont,
		GFX_COLOR_TRANSPARENT, GFX_COLOR_RED,
		TEXT_POS_CENTER, TEXT_ALIGN_CENTER);
}

/**
 * \brief main example code
 *
 * The main function will create the widget GUI, and display the widgets to the
 * screen. It will then enter a message loop converting touch events from the
 * screen into touch events for the Widget Toolkit module.
 */
int main(void)
{
	board_init();
	sysclk_init();
	membag_init();

	gfx_init();

	touch_interface_init();
	win_init();

	setup_gui_root_window();
	if (app_widget_launch() == false) {
		show_out_of_memory_error();
		for (;;);
	}

	while (true) {
		struct win_pointer_event win_touch_event;

		/* Queue touch events from the touchscreen if any are available */
		while (touch_interface_read(&win_touch_event)) {
			win_queue_pointer_event(&win_touch_event);
		}

		/* Process queued events in the windowing system */
		win_process_events();
	}
}
