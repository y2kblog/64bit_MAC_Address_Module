/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAC_24AA02EXX_HAL_H
#define __MAC_24AA02EXX_HAL_H

#ifdef __cplusplus
extern "C" {
#endif

/* Include system header files -----------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>

/* Include user header files -------------------------------------------------*/
#include "i2c.h"

/* Exported macro ------------------------------------------------------------*/
/* Exported function macro ---------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported enum tag ---------------------------------------------------------*/
/* Exported struct/union tag -------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported function prototypes ----------------------------------------------*/
extern bool get_MAC_address(I2C_HandleTypeDef *, bool *, uint8_t *);

#ifdef __cplusplus
}
#endif

#endif /* __MAC_24AA02EXX_HAL_H */

/***************************************************************END OF FILE****/
