#pragma once
#include <stdint.h>
#include <stddef.h>
enum
{PROTOCOL_CRC_INIT = 0xFFFF,
PROTOCOL_CRC_VALIDATE = 0xF0B8 /* TODO: Do we need this? */
};
/*! @brief Добавить один байт в код CRC.
*
* @param v байт.
* @param[in,out] pcrc Указатель на переменную с CRC.
*/
static inline void protocol_crc_acc(uint8_t v, uint16_t *pcrc)
{
/* Accumulate one byte of data into the CRC */
uint8_t tmp = v ^ (uint8_t)(*pcrc & 0xff);
tmp ^= (tmp << 4);
*pcrc = (*pcrc >> 8) ^ (tmp << 8) ^ (tmp << 3) ^ (tmp >> 4);
}
/*! @brief Добавить буфер в код CRC.
*
* @param[in,out] pcrc Указатель на переменную с CRC.
* @param[in] buf Указатель на буфер.
* @param sz Размер буфера в байтах.
*/
static inline void protocol_crc_acc_buf(
uint16_t* pcrc, const void* buf, size_t sz)
{
const uint8_t* p = (const uint8_t*)buf;
while(sz--)
protocol_crc_acc(*p++, pcrc);
}
/*! @brief Инициализация CRC.
*
* @param[out] pcrc Указатель на переменную которая будет использована для
* вычисления CRC.
*/
static inline void protocol_crc_init(uint16_t* pcrc)
{
*pcrc = PROTOCOL_CRC_INIT;
}
/*! @brief Вычислить CRC для буфера.
*
* @param[in] buf Указатель на буфер.
* @param sz Размер буфера в байтах.
*
* @return Вычисленное значение CRC.
*/
static inline uint16_t protocol_crc_calc(const void* buf, size_t sz)
{
uint16_t crc;protocol_crc_init(&crc);
protocol_crc_acc_buf(&crc, buf, sz);
return crc;
}
