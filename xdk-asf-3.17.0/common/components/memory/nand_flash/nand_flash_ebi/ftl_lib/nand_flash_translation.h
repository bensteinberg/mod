/**
 * \file
 *
 * \brief NAND Flash Translation.
 *
 * This file contains definitions and functions for NAND Flash translation
 * operation.
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

#ifndef NAND_FLASH_TRANSLATION_H_INCLUDED
#define NAND_FLASH_TRANSLATION_H_INCLUDED

#include "nand_flash_management.h"

struct nand_flash_map {
	struct nand_flash_management managed;
	int16_t logical_mapping_array[NAND_COMMON_MAX_BLOCK_NUMBER];
	int16_t logical_mapping_block;
	uint8_t mapping_change;
	uint8_t reserved;
};

struct nand_flash_translation {
	struct nand_flash_map mapped;
	int16_t current_logical_block;
	int16_t previous_physical_block;
	uint8_t current_block_page_status[NAND_COMMON_MAX_PAGES_PER_BLOCK / 8];
};

/**
 * \brief  Initialize a nand_flash_translation instance.
 *
 * \param translated  Pointer to a nand_flash_translation instance.
 * \param model  Pointer to the underlying nand chip model. Can be 0.
 * \param command_address  Address at which commands are sent.
 * \param address_address  Address at which addresses are sent.
 * \param data_address  Address at which data is sent.
 * \param base_block Basic physical block address of mapped area.
 * \param size_in_block Number of blocks that is mapped.
 * \param p_page_write_buffer  Pointer to buffer for page write, must be
 *  allocated with the same size of the NAND Flash page size used.
 * \param p_page_read_buffer  Pointer to buffer for page read, must be
 *  allocated with the same size of the NAND Flash page size used.
 *
 * \return 0 if successful; otherwise return the error code.
 */
uint32_t nand_flash_translation_initialize(
		struct nand_flash_translation *translated,
		const struct nand_flash_model *model, uint32_t command_address,
		uint32_t address_address, uint32_t data_address,
		uint16_t base_block, uint16_t size_in_block,
		uint8_t *p_page_write_buffer, uint8_t *p_page_read_buffer);
/**
 * \brief  Read the data and/or the spare area of a page on a translated
 * NAND Flash. If the block is not currently mapped but could be (i.e. there
 * are available physical blocks), then the data/spare is filled with 0xFF.
 *
 * \param translated  Pointer to a nand_flash_translation instance.
 * \param block  Logical block number.
 * \param page  Number of page to read inside logical block.
 * \param data  Data area buffer, can be 0.
 * \param spare  Spare area buffer, can be 0.
 *
 * \return 0 if successful; otherwise returns NAND_COMMON_ERROR_NOMOREBLOCKS
 */
uint32_t nand_flash_translation_read_page(
		const struct nand_flash_translation *translated, uint16_t block,
		uint16_t page, void *data, void *spare);
/**
 * \brief  Writes the data and/or spare area of a page on a translated
 * NAND Flash. Allocate block has needed to keep the wear even between
 * all blocks.
 *
 * \param translated  Pointer to a nand_flash_translation instance.
 * \param block  Logical block number.
 * \param page  Number of page to read inside logical block.
 * \param data  Data area buffer, can be 0.
 * \param spare  Spare area buffer, can be 0.
 *
 * \return 0 if successful; otherwise return the error code
 */
uint32_t nand_flash_translation_write_page(
		struct nand_flash_translation *translated, uint16_t block,
		uint16_t page, void *data, void *spare);
/**
 * \brief  Terminate the current write operation by copying all the missing
 * pages from the previous physical block.
 *
 * \param translated  Pointer to a nand_flash_translation instance.
 *
 * \return 0 if successful; otherwise return the error code
 */
uint32_t nand_flash_translation_flush(
		struct nand_flash_translation *translated);
/**
 * \brief Erase all blocks in the translated area of NAND Flash.
 *
 * \param translated Pointer to a nand_flash_translation instance.
 * \param level Erase level.
 *
 * \return 0 if successful; otherwise return the error code
 */
uint32_t nand_flash_translation_erase_all(
		struct nand_flash_translation *translated, uint32_t level);
/**
 * \brief Allocate a free block to save the current logical mapping in it.
 *
 * \param translated Pointer to a nand_flash_translation instance.
 *
 * \return 0 if successful; otherwise return the error code.
 */
uint32_t nand_flash_translation_save_logical_mapping(
		struct nand_flash_translation *translated);
/**
 * \brief Return the number of available blocks in a translated NAND Flash.
 *
 * \param translated  Pointer to a nand_flash_translation instance.
 *
 * \return the number of available blocks in a translated NAND Flash.
 */
uint16_t nand_flash_translation_get_device_size_in_block(
		const struct nand_flash_translation *translated);
/**
 * \brief Return the number of available pages in a translated NAND Flash.
 *
 * \param translated  Pointer to a nand_flash_translation instance.
 *
 * \return the number of available pages in a translated NAND Flash.
 */
uint32_t nand_flash_translation_get_device_size_in_page(
		const struct nand_flash_translation *translated);
/**
 * \brief Return the number of available data bytes in a translated NAND Flash.
 *
 * \param translated  Pointer to a nand_flash_translation instance.
 *
 * \return the number of available data bytes in a translated NAND Flash.
 */
uint64_t nand_flash_translation_get_device_size_in_byte(
		const struct nand_flash_translation *translated);

/**
 * \brief Carry out all pending operations.
 */
void nand_flash_flush(void);

/**
 * \brief Read data at the specified address of NAND Flash.
 *
 * \param address  Address at which the data shall be read.
 * \param data  Data buffer.
 * \param length  Number of bytes to read.
 *
 * \returns 0 if successful; otherwise return the error code.
 */
uint32_t nand_flash_read(uint32_t address, void *data, uint32_t length);

/**
 * \brief Write a data buffer at the specified address to NAND Flash.
 *
 * \param address  Address where the data shall be written.
 * \param data  Data buffer.
 * \param length  Number of bytes to write.
 *
 * \returns 0 if successful; otherwise return the error code.
 */
uint32_t nand_flash_write(uint32_t address, const void *data,
		uint32_t length);

/**
 * \brief Get FTL version.
 *
 * \param minor_version  Minor version number.
 * \param major_version  Major version number.
 *
 * \returns 0 if successful; otherwise return the error code.
 */
void nand_flash_get_version(uint32_t* minor_version,
		uint32_t* major_version);

#endif /*#ifndef NAND_FLASH_TRANSLATION_H_INCLUDED */
