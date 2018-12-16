/* Include system header files -----------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Include user header files -------------------------------------------------*/
#include "MAC_24AA02Exx_HAL.h"
#include "i2c.h"

/* Exported macros -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
#define MAC_24AA02Exx_DEV_ADDR			(0x50 << 1)
#define MAC_24AA02Exx_I2C_TIMEOUT_MS    0xFFFFFFFF
#define MAC_24AA02Exx_REG_ADDR_MAC  	0xF8

/* Exported variables --------------------------------------------------------*/
/* Imported variables --------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

// Microchip OUI (Organizationally Unique Identifier)
// http://standards-oui.ieee.org/oui/oui.txt
// NOTE : This will add as addresses are exhausted. (Last check date : 2018/11/23)
const uint8_t MICROCHIP_OUI[][3] =
{
	{0x54, 0x10, 0xEC},
	{0x80, 0x1F, 0x12},
	{0x04, 0x91, 0x62},
	{0x00, 0x1E, 0xC0},
	{0x00, 0x04, 0xA3},
	{0xD8, 0x80, 0x39}
};

/* Exported functions prototypes----------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
 * @fn
 * @brief get MAC address + check the address either EUI-48 or EUI-64
 * @param[in]	*hi2c		I2C handle Structure
 * @param[out]	*isEUI48	EUI-48 or EUI-64
 * @param[out]	*MAC_addr	MAC address. should set 8 Bytes variables. If EUI-48, first 6 Bytes are valid. If EUI-64, first 8 Bytes are valid.
 * @return		bool		true : Success, false : failure
*/
bool get_MAC_address(I2C_HandleTypeDef *hi2c, bool *isEUI48, uint8_t *MAC_addr)
{
	HAL_StatusTypeDef status;

	// Device check
	status = HAL_I2C_Master_Transmit(
		hi2c,
		MAC_24AA02Exx_DEV_ADDR,
		(uint8_t*)0x00,
		0,
		MAC_24AA02Exx_I2C_TIMEOUT_MS);

	if(status != HAL_OK) {
		printf("MAC address IC 24AA02Exx is not found\r\n");
		return false;
	}

	// get MAC address
	uint8_t MAC_addr_buf[8] = {'\0'};
	status = HAL_I2C_Mem_Read(
		hi2c,
		MAC_24AA02Exx_DEV_ADDR,
		MAC_24AA02Exx_REG_ADDR_MAC,
		I2C_MEMADD_SIZE_8BIT,
		MAC_addr_buf,
		8,
		MAC_24AA02Exx_I2C_TIMEOUT_MS);

	if (status != HAL_OK) {
		printf("HAL_I2C_Mem_Read error : %d\r\n", status);
		return false;
	}

	// Determine whether the MAC address is EUI-48 or EUI-64
	// NOTE : (from Datasheet) In conformance with IEEE guidelines, Microchip will not use the values 0xFFFE and 0xFFFF for the first two bytes of the EUI-64 Extension Identifier.
	if( (MAC_addr_buf[0] == 0xFF) && (MAC_addr_buf[1] == 0xFF) )
	{
		// EUI-48 (24AA02xE48)
		*isEUI48 = true;
		memcpy(MAC_addr, MAC_addr_buf + 2, 6);
	}
	else
	{
		// EUI-64 (24AA02xE64)
		*isEUI48 = false;
		memcpy(MAC_addr, MAC_addr_buf, 8);
	}


	// Check Microchip OUI
	bool isChecked = false;
	for(int i = 0; i < (sizeof(MICROCHIP_OUI) / 3); i++)
	{
		if(memcmp(MAC_addr, (uint8_t *)&MICROCHIP_OUI[i][0], 3) == 0) {
			isChecked = true;
			break;
		}
	}
	if(isChecked == false)
	{
		printf("OUI error or new Microchip OUI is added. OUI:%02X-%02X-%02X. refer : \"http://standards-oui.ieee.org/oui/oui.txt\"\r\n",
				MAC_addr[0], MAC_addr[1], MAC_addr[2]);
		//return false;
	}

	return true;
}

/* Private functions ---------------------------------------------------------*/

/***************************************************************END OF FILE****/
