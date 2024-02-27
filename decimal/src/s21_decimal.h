#ifndef _SRC_S21_DECIMAL_H_
#define _SRC_S21_DECIMAL_H_

#include <stdio.h>

typedef struct {
  unsigned int bits[4];
} s21_decimal;

typedef struct {
  unsigned bits[8];
} s21_big_decimal;

typedef struct {
  unsigned int bits[8];
} s21_decimal_long;

union float_to_uint {
  float fl;
  unsigned int changed_fl;
};

typedef union {
  int ui;
  float fl;
} floatbits;

#define START_INFO 96

//--------------------вспомогательные функции--------------------------------//

void s21_zero_decimal(s21_decimal *dst);  // зануление децимала
void s21_set_sign(s21_decimal *dst);  // установить отрицательный знак децимала

void s21_set_bit(s21_decimal *dst, int index, int bit);  // установка бита

void s21_set_exp(s21_decimal *dst, int scale);  // установить еспоненту
void s21_swap(s21_decimal *a, s21_decimal *b);  // смена переменных
void s21_set_int_bit(int *num, int index,
                     int bit_value);   // установка битов инта
void s21_div_by_10(s21_decimal *num);  // деление на 10
void s21_otbros_exp(s21_decimal *a, int exp);  // убирает экспоненту

void s21_vbltashitb_exp(s21_decimal a,
                        s21_decimal *res);  // вытаскивает экспоненту
                                            // например 12.3456 возвращает 3456
int s21_get_bits(unsigned int bits,
                 unsigned int num);  /// получить код нужного бита
int s21_get_exp(s21_decimal dst);
int s21_mini_great(s21_decimal a, s21_decimal b);  // целое больше
int s21_mini_add(s21_decimal a, s21_decimal b,
                 s21_decimal *result);  // целое сложение
int s21_normolize(s21_decimal a, s21_decimal b, s21_decimal *result,
                  int x);  // выравнивание экспоненты
int s21_mini_sub(s21_decimal value_1, s21_decimal value_2,
                 s21_decimal *result);  // целое вычитание
int s21_bank_okr(s21_decimal a, s21_decimal b, s21_decimal *result);
int great_bez_znak(
    s21_decimal a,
    s21_decimal b);  // возвращает больше или меньше не смотря на знак

//---------------------div_func----------------------------------------//
int s21_is_decimal_no_empty(s21_decimal dst);
void s21_import_to_big_decimal(s21_decimal value_1, s21_big_decimal *value_2);
int s21_div_big_decimal(s21_big_decimal value_1, s21_big_decimal value_2,
                        s21_big_decimal *result);
int s21_post_normalization(s21_big_decimal *result, int scale);
void s21_find_highest_bit_big_decimal(s21_big_decimal v1, s21_big_decimal v2,
                                      int *bit_1, int *bit_2);
void s21_import_to_small_decimal(s21_decimal *value_1, s21_big_decimal value_2);
void s21_increase_scale_big_decimal(s21_big_decimal *dst, int n);

int s21_is_greater_big_decimal(s21_big_decimal value_1,
                               s21_big_decimal value_2);
int s21_is_greater_or_equal_big_decimal(s21_big_decimal value_1,
                                        s21_big_decimal value_2);
int s21_shift_big_dec_l(s21_big_decimal *dst, int num);
int s21_equation_bits_big_decimal(s21_big_decimal *value_1,
                                  s21_big_decimal *value_2);
void s21_set_bit_big(s21_big_decimal *dst, int index, int bit);
void s21_zero_big_decimal(s21_big_decimal *dst);
int s21_mul_big_decimal(s21_big_decimal value_1, s21_big_decimal value_2,
                        s21_big_decimal *result);
void s21_add_big_decimal(s21_big_decimal value_1, s21_big_decimal value_2,
                         s21_big_decimal *result);

void s21_sub_big_decimal(s21_big_decimal value_1, s21_big_decimal value_2,
                         s21_big_decimal *result);

void s21_decr_exp_big_decimal(s21_big_decimal *dst, int n);
void s21_shift_big_dec_r(s21_big_decimal *dst, int num);
int s21_get_bit_big(s21_big_decimal dst, int index);

//----------------------float decimal----------------------------------//

int s21_get_bit(s21_decimal dst, int index);
int s21_get_sign(s21_decimal dst);
int s21_from_decimal_to_double(s21_decimal src, long double *dst);
int s21_get_scale(s21_decimal dst);
void s21_set_scale(s21_decimal *dst, int scale);

//--------------------Основные функции--------------------------------//

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);
int s21_is_less(s21_decimal a, s21_decimal b);
int s21_is_less_or_equal(s21_decimal a, s21_decimal b);
int s21_is_greater(s21_decimal a, s21_decimal b);
int s21_is_greater_or_equal(s21_decimal a, s21_decimal b);
int s21_is_equal(s21_decimal a, s21_decimal b);
int s21_is_not_equal(s21_decimal a, s21_decimal b);

#endif  // _SRC_S21_DECIMAL_H_