/*
 * AT24C02.h
 *
 *  Created on: Apr 26, 2023
 *      Author: bbhn2
 */

#ifndef APPLICATION_AT24C02_H_
#define APPLICATION_AT24C02_H_

u8 AT24CXX_ReadOneByte(u16 ReadAddr);
void AT24CXX_WriteOneByte(u16 WriteAddr, u8 DataToWrite);
void AT24CXX_Read(u16 ReadAddr, u8 *pBuffer, u16 NumToRead);
void AT24CXX_Write(u16 WriteAddr, u8 *pBuffer, u16 NumToWrite);

#endif /* APPLICATION_AT24C02_H_ */
