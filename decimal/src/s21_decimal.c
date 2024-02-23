#include "s21_decimal.h"

#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//------------------------Конвертация----------------------//

// из инта в децимал
int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  s21_zero_decimal(dst);
  int x = 0;
  if (src) {
    if (src < 0) {
      s21_set_sign(dst);
      src *= -1;
    }
    dst->bits[0] = src;
  } else
    x = 1;
  return x;  // success
}

// из децимала в инт
int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int x = 0;
  int flag = 0;
  for (int i = 2; i > 0 && flag != 1; i--) {
    for (int j = 31; j >= 0 && flag != 1; j--) {
      if (s21_get_bits(src.bits[i], j) || s21_get_bits(src.bits[0], 31)) {
        x = 1;
        flag = 1;
      }
    }
  }

  if (x == 0) {
    for (int i = 0; i < 31; i++)
      s21_set_int_bit(dst, i, s21_get_bits(src.bits[0], i));

    if (s21_get_bits(src.bits[3], 31)) {
      src.bits[0] = ~src.bits[0];
      for (int i = 0; i < 31; i++)
        s21_set_int_bit(dst, i, s21_get_bits(src.bits[0], i));
      s21_set_int_bit(dst, 31, 1);
      s21_set_int_bit(dst, 0, 1);
    }
  }

  return x;
}

// из флоата в децимал
int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  s21_zero_decimal(dst);
  int return_value = 0;
  if (isinf(src) || isnan(src)) {
  } else {
    if (src != 0) {
      int sign = *(int *)&src >> 31;
      int exp = ((*(int *)&src & ~0x80000000) >> 23) - 127;
      double tmp = (double)fabs(src);
      int off = 0;
      for (; off < 28 && (int)tmp / (int)pow(2, 21) == 0; tmp *= 10, off++) {
      }
      tmp = round(tmp);
      if (off <= 28 && (exp > -94 && exp < 96)) {
        floatbits mant = {0};
        tmp = (float)tmp;
        for (; fmod(tmp, 10) == 0 && off > 0; off--, tmp /= 10) {
        }
        mant.fl = tmp;
        exp = ((*(int *)&mant.fl & ~0x80000000) >> 23) - 127;
        dst->bits[exp / 32] |= 1 << exp % 32;
        for (int i = exp - 1, j = 22; j >= 0; i--, j--)
          if ((mant.ui & (1 << j)) != 0) dst->bits[i / 32] |= 1 << i % 32;
        dst->bits[3] = (sign << 31) | (off << 16);
      }
    }
  }
  return return_value;
}

// из децимала во влоат
int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  double tmp = (double)*dst;
  for (int i = 0; i < 96; i++) {
    tmp += s21_get_bit(src, i) * pow(2, i);
  }
  tmp = tmp * pow(10, -s21_get_exp(src));
  if (s21_get_bit(src, 127)) tmp = tmp * (-1);
  *dst = tmp;
  return 0;
}

//------------------------Арифметика----------------------//

// сложение
int s21_add(s21_decimal a, s21_decimal b, s21_decimal *result) {
  int res = 0;
  // int flag = 0;
  int znak1 = s21_get_bits(a.bits[3], 31);
  int znak2 = s21_get_bits(b.bits[3], 31);

  if ((s21_get_exp(a) || s21_get_exp(b))) {
    res = s21_normolize(a, b, result, 1);

    if (res == 1 && znak1 && znak2) res = 2;

  } else {
    res = s21_mini_add(a, b, result);
  }

  if (znak1 && !znak2 && !great_bez_znak(a, b)) {
    s21_set_bit(&a, 127, 0);
    res = s21_sub(b, a, result);
  }

  if (znak1 && !znak2 && great_bez_znak(a, b)) {
    s21_set_bit(&a, 127, 0);
    if (s21_is_equal(a, b)) res = s21_sub(b, a, result);
    s21_set_sign(result);
  }

  if (!znak1 && znak2 && great_bez_znak(a, b)) {
    s21_set_bit(&b, 127, 0);
    res = s21_sub(a, b, result);
  }

  if (!znak1 && znak2 && !great_bez_znak(a, b)) {
    s21_set_bit(&b, 127, 0);
    res = s21_sub(b, a, result);
    s21_set_sign(result);
  }

  if (znak1 && !znak2 && great_bez_znak(a, b)) {
    res = s21_sub(a, b, result);
    s21_set_sign(result);
  }

  if (znak1 && !znak2 && !great_bez_znak(a, b)) {
    s21_set_bit(&a, 127, 0);
    res = s21_sub(b, a, result);
  }

  if (znak1 && znak2) {
    if (res == 1)
      res = 2;
    else
      s21_set_sign(result);
  }

  if (res) s21_zero_decimal(result);

  return res;
}

// вычитание
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 0;
  int znak1 = s21_get_bits(value_1.bits[3], 31),
      znak2 = s21_get_bits(value_2.bits[3], 31),
      great = s21_is_greater(value_1, value_2);
  s21_decimal buf1 = value_1;
  s21_decimal buf2 = value_2;
  int r = 0;

  if ((znak1 && !znak2 && !great) || (!znak1 && znak2 && great)) {
    s21_set_bit(&value_1, 127, 0);
    s21_set_bit(&value_2, 127, 0);
    res = s21_add(value_1, value_2, result);
    s21_set_sign(result);
    res = res == 1 ? 2 : 0;
  } else if (s21_get_exp(value_1) || s21_get_exp(value_2)) {
    if (znak1 && znak2 && great_bez_znak(value_2, value_1)) {
      s21_swap(&value_1, &value_2);
      r = 1;
    }

    res = s21_normolize(value_1, value_2, result, 0);

    if (!znak1 && !znak2 && great_bez_znak(value_2, value_1))
      res = s21_normolize(value_2, value_1, result, 0);

  } else {
    res = great_bez_znak(value_1, value_2)
              ? s21_mini_sub(value_1, value_2, result)
              : s21_mini_sub(value_2, value_1, result);
  }
  if ((!znak1 && !znak2 && !s21_is_greater(value_1, value_2)) ||
      (znak1 && znak2 && !great) || (znak1 && !znak2 && great) ||
      (znak1 && znak2 && !great) || r) {
    s21_set_bit(result, 127, 1);
  }
  if (!znak1 && !znak2 && !s21_is_greater(buf1, buf2))
    s21_set_bit(result, 127, 1);

  if (s21_is_equal(value_1, value_2)) s21_zero_decimal(result);

  return res;
}

// умножение
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 0, scale = 0;
  s21_big_decimal v1 = {0}, v2 = {0}, r = {0};
  s21_import_to_big_decimal(value_1, &v1);
  s21_import_to_big_decimal(value_2, &v2);
  if (s21_get_sign(value_1) != s21_get_sign(value_2)) s21_set_sign(result);
  scale = s21_get_exp(value_1) + s21_get_exp(value_2);
  res = s21_mul_big_decimal(v1, v2, &r);
  scale = s21_post_normalization(&r, scale);
  if (scale >= 0) {
    s21_set_exp(result, scale);
    s21_import_to_small_decimal(result, r);
  } else {
    res = 1;
  }
  if (res == 1 && s21_get_sign(*result)) res = 2;
  if (res) s21_zero_decimal(result);

  if ((!value_1.bits[0] && !value_1.bits[1] && !value_1.bits[2] &&
       !value_1.bits[3]) ||
      (!value_2.bits[0] && !value_2.bits[1] && !value_2.bits[2] &&
       !value_2.bits[3]))
    s21_zero_decimal(result);
  return res;
}

// деление
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 0;
  if (s21_is_decimal_no_empty(value_2)) {
    int scale = 0, res_scale = 0;
    s21_big_decimal v1 = {0}, v2 = {0}, r = {0};
    s21_import_to_big_decimal(value_1, &v1);
    s21_import_to_big_decimal(value_2, &v2);
    if (s21_get_sign(value_1) != s21_get_sign(value_2)) s21_set_sign(result);
    scale = s21_div_big_decimal(v1, v2, &r);
    s21_set_exp(&value_1, s21_get_exp(value_1) + scale);
    res_scale = s21_get_exp(value_1) - s21_get_exp(value_2);
    if (res_scale > 0) {
      res_scale = s21_post_normalization(&r, res_scale);
    } else if (res_scale < 0) {
      s21_increase_scale_big_decimal(&r, abs(res_scale));
      res_scale = s21_post_normalization(&r, 0);
    }
    if (res_scale >= 0) {
      s21_import_to_small_decimal(result, r);
      s21_set_exp(result, res_scale);
    }
  } else {
    res = 3;
  }
  if (res == 1 && s21_get_sign(*result)) res = 2;
  if (res) s21_zero_decimal(result);
  return res;
}

//------------------------Сравнение----------------------//

// децимал равен
int s21_is_equal(s21_decimal a, s21_decimal b) {
  int res = 1;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 32; j++) {
      if (s21_get_bits(a.bits[i], j) != s21_get_bits(b.bits[i], j)) {
        res = 0;
      }
    }
  }
  return res;
}

// децимал не равен
int s21_is_not_equal(s21_decimal a, s21_decimal b) {
  int res = (s21_is_equal(a, b) == 0) ? 1 : 0;
  return res;
}

// Больше или равно
int s21_is_greater_or_equal(s21_decimal a, s21_decimal b) {
  int res = s21_is_greater(a, b);
  if (!s21_is_not_equal(a, b)) res = 1;
  return res;
}

// Больше
int s21_is_greater(s21_decimal a, s21_decimal b) {
  int res = 1;
  s21_decimal exp1 = {{0}};
  s21_decimal exp2 = {{0}};
  if (s21_get_exp(a) || s21_get_exp(b)) {
    s21_vbltashitb_exp(a, &exp1);
    s21_vbltashitb_exp(b, &exp2);
    s21_otbros_exp(&a, s21_get_exp(a));
    s21_otbros_exp(&b, s21_get_exp(b));
  }
  res = s21_mini_great(a, b);
  if (!s21_is_not_equal(a, b)) {
    res = s21_mini_great(exp1, exp2);
    if ((s21_get_bits(a.bits[3], 31) && s21_get_bits(b.bits[3], 31))) {
      res = res == 0 ? 1 : 0;
    }
  }
  if ((s21_get_bits(a.bits[3], 31) && !s21_get_bits(b.bits[3], 31))) res = 0;
  return res;
}

int s21_mini_great(s21_decimal a, s21_decimal b) {
  int res = 1;
  int flag = 0;
  if (s21_get_bits(a.bits[3], 31) && s21_get_bits(a.bits[3], 31))
    s21_swap(&a, &b);
  if (!s21_get_bits(a.bits[3], 31) && s21_get_bits(b.bits[3], 31)) flag = 1;
  for (int i = 3; i >= 0 && flag != 1; i--)
    for (int j = 31; j >= 0 && flag != 1; j--) {
      if ((s21_get_bits(a.bits[i], j) && !s21_get_bits(b.bits[i], j))) {
        flag = 1;
      }
      if ((!s21_get_bits(a.bits[i], j) && s21_get_bits(b.bits[i], j))) {
        res = 0;
        flag = 1;
      }
    }
  if (s21_is_equal(a, b)) res = 0;
  return res;
}

// меньше или равно
int s21_is_less_or_equal(s21_decimal a, s21_decimal b) {
  return s21_is_greater_or_equal(b, a);
}
// меньше
int s21_is_less(s21_decimal a, s21_decimal b) { return s21_is_greater(b, a); }

// Возвращаемое значение:

// 0 - FALSE
// 1 - TRUE

//------------------------Other----------------------------//

// отразить децимал
int s21_negate(s21_decimal value, s21_decimal *result) {
  result->bits[3] = value.bits[3] ^ (1u << 31);
  return 0;
}

// Возвращает целые цифры указанного Decimal числа; любые дробные цифры
// отбрасываются, включая конечные нули.
int s21_truncate(s21_decimal value, s21_decimal *result) {
  int res = 0;
  s21_otbros_exp(&value, s21_get_exp(value));
  *result = value;
  if (result->bits[0] == 0) s21_set_bit(result, 127, 0);
  return res;
}

// Округляет указанное Decimal число до ближайшего целого числа в сторону
// отрицательной бесконечности.
int s21_floor(s21_decimal value, s21_decimal *result) {
  int res = 0;
  s21_decimal mask = {{1, 0, 0, 0}};

  if (s21_get_exp(value) != 0) {
    s21_otbros_exp(&value, s21_get_exp(value));
    s21_set_exp(&value, 0);
    *result = value;

    if (s21_get_bits(value.bits[3], 31)) {
      s21_set_bit(&value, 127, 0);
      s21_add(value, mask, result);
      s21_set_bit(result, 127, 1);
    }

  } else
    *result = value;

  return res;
}

int s21_round(s21_decimal value, s21_decimal *result) {
  s21_zero_decimal(result);
  long double fl_to_round = 0.0;
  long double difference = 0;
  s21_from_decimal_to_double(value, &fl_to_round);
  fl_to_round = round(fl_to_round);
  if (fl_to_round < 0.0) {
    s21_set_bit(result, 127, 1);

    fl_to_round *= -1;
  }
  for (int i = 0; fl_to_round >= 1 && i < START_INFO; i++) {
    fl_to_round = floor(fl_to_round) / 2;
    difference = fl_to_round - floor(fl_to_round);
    if (difference > 0.0) {
      s21_set_bit(result, i, 1);
    } else {
      s21_set_bit(result, i, 0);
    }
  }
  s21_set_exp(result, 0);
  return 0;
}

//------------------------Доп Функции----------------------//

// зануление децимала
void s21_zero_decimal(s21_decimal *dst) {
  dst->bits[0] = dst->bits[1] = dst->bits[2] = dst->bits[3] = 0;
}

// сделать отрицательный децимал
void s21_set_sign(s21_decimal *dst) { s21_set_bit(dst, 127, 1); }

// получить бит
int s21_get_bits(unsigned int bits, unsigned int num) {
  unsigned int mask = 1 << num;
  return (bits & mask) ? 1 : 0;
}

// установить бит
void s21_set_bit(s21_decimal *dst, int index, int bit) {
  int mask = 1u << (index % 32);
  dst->bits[index / 32] =
      bit == 0 ? dst->bits[index / 32] & ~mask : dst->bits[index / 32] | mask;
}

// получить экспоненту
int s21_get_exp(s21_decimal dst) {
  int mask = 127 << 16;
  int exp = (mask & dst.bits[3]) >> 16;
  if (exp > 28) exp = 0;
  return exp;
}

// установить экспоненту
void s21_set_exp(s21_decimal *dst, int exp) {
  s21_get_exp(*dst);
  int flag = 0;
  if (s21_get_bits(dst->bits[3], 31)) flag = 1;
  dst->bits[3] &= 0x0000FFFF;  // Mask out the top 16 bits
  exp <<= 16;
  dst->bits[3] |= exp;  // Set the top 16 bits to the new exponent
  if (flag == 1) s21_set_sign(dst);
}

// свапалка децемалов
void s21_swap(s21_decimal *a, s21_decimal *b) {
  s21_decimal dec_buf = *a;
  *a = *b;
  *b = dec_buf;
}

void s21_set_int_bit(int *num, int index, int bit_value) {
  int mask = 1 << index;  // Создаем маску, где все биты равны 0, кроме бита с
                          // индексом index, который равен 1.
  if (bit_value == 1) {
    *num |= mask;  // Устанавливаем бит в 1 путем ИЛИ с маской.
  } else if (bit_value == 0) {
    *num &= ~mask;  // Устанавливаем бит в 0 путем И с инвертированной маской.
  }
}

void s21_div_by_10(s21_decimal *num) {
  unsigned long long rem = 0, tmp;
  for (int i = 2; i >= 0; i--) {
    tmp = 0;
    tmp = (unsigned long long)num->bits[i] + rem;
    num->bits[i] = (unsigned int)(tmp / 10);
    rem = (tmp % 10) << 32;
  }
}

void s21_otbros_exp(s21_decimal *a, int exp) {
  for (int i = exp; i > 0; i--) s21_div_by_10(a);
  s21_set_exp(a, 0);
}

void s21_vbltashitb_exp(s21_decimal a, s21_decimal *res) {
  if (s21_get_exp(a)) {
    s21_decimal buf = {{0}};
    s21_decimal ten = {{10, 0, 0, 0}};
    s21_decimal buf2 = {{0}};

    buf = a;

    s21_otbros_exp(&buf, s21_get_exp(a));

    for (int i = s21_get_exp(a); i > 0; i--) {
      s21_mul(buf, ten, &buf2);

      buf = buf2;
    }
    s21_set_bit(&buf2, 127, 0);
    s21_set_bit(&a, 127, 0);
    s21_set_exp(&a, 0);

    s21_mini_sub(a, buf2, res);
  }
}

int s21_normolize(s21_decimal a, s21_decimal b, s21_decimal *result, int x) {
  int exp1 = s21_get_exp(a);
  int exp2 = s21_get_exp(b);
  s21_decimal mul = {{10, 0, 0, 0}};
  s21_decimal buf = {0};
  s21_decimal bufer1 = a;
  s21_decimal bufer2 = b;
  s21_decimal odin = {{1, 0, 0, 0}};
  int res = 0;
  int realexp1 = 0;
  int realexp2 = 0;

  while (bufer1.bits[0] != 0) {
    realexp1++;
    s21_div_by_10(&bufer1);
  }

  bufer1 = a;

  while (bufer2.bits[0] != 0) {
    realexp2++;
    s21_div_by_10(&bufer2);
  }

  bufer2 = b;

  s21_decimal posl_znak_exp1 = {0};
  s21_decimal posl_znak_exp2 = {0};

  if (realexp1 > 27 || realexp2 > 27) {
    if (realexp1 + exp2 > 28)
      while (realexp1 + exp2 > 28 && exp2 != 0) {
        if (exp2 == 1) s21_vbltashitb_exp(bufer2, &posl_znak_exp1);
        s21_div_by_10(&bufer2);
        exp2--;
      }
  }
  int max_exp = exp1 > exp2 ? exp1 : exp2;
  if (exp2 > exp1) {
    for (int i = exp1; i != exp2; i++) {
      res = s21_mul(bufer1, mul, &buf);
      bufer1 = buf;
    }
  }

  if (exp2 < exp1) {
    for (int i = exp2; i != exp1; i++) {
      res = s21_mul(bufer2, mul, &buf);
      bufer2 = buf;
    }
  }

  if (x == 1) {
    res = s21_mini_add(bufer1, bufer2, result);
    bufer2 = *result;
    if (posl_znak_exp1.bits[0] + posl_znak_exp2.bits[0] > 5) {
      res = s21_mini_add(bufer2, odin, result);
    }
    s21_set_exp(result, max_exp);
  } else {
    res = s21_mini_sub(bufer1, bufer2, result);
    bufer2 = *result;
    if (posl_znak_exp1.bits[0] + posl_znak_exp2.bits[0] > 5) {
      res = s21_mini_sub(bufer2, odin, result);
      if (res == 1) res = 2;
    }
    s21_set_exp(result, max_exp);
  }

  if (res == 1 || res == 2) s21_zero_decimal(result);

  return res;
}

int great_bez_znak(s21_decimal a, s21_decimal b) {
  int znak1 = s21_get_bits(a.bits[3], 31);
  int znak2 = s21_get_bits(b.bits[3], 31);
  s21_set_bit(&a, 127, 0);
  s21_set_bit(&b, 127, 0);
  int res = s21_is_greater(a, b);
  s21_set_bit(&a, 127, znak1);
  s21_set_bit(&b, 127, znak2);
  return res;
}

int s21_mini_sub(s21_decimal value_1, s21_decimal value_2,
                 s21_decimal *result) {
  s21_zero_decimal(result);

  unsigned buffer = 0;
  int index = 0;
  int res = 0;
  int flag = 0;

  if (s21_is_equal(value_1, value_2) == 1) {
    flag = 1;
  }

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 32 && flag == 0; j++, index++) {
      if (!s21_get_bits(value_1.bits[i], j) &&
          s21_get_bits(value_2.bits[i], j) && buffer == 0) {
        s21_set_bit(result, index, 1);
        buffer++;
      }

      if (s21_get_bits(value_1.bits[i], j) &&
          !s21_get_bits(value_2.bits[i], j) && buffer == 0)
        s21_set_bit(result, index, 1);

      if (s21_get_bits(value_1.bits[i], j) &&
          s21_get_bits(value_2.bits[i], j) && buffer != 0)
        s21_set_bit(result, index, 1);

      if (s21_get_bits(value_1.bits[i], j) &&
          !s21_get_bits(value_2.bits[i], j) && buffer != 0)
        buffer = 0;

      if (!s21_get_bits(value_1.bits[i], j) &&
          !s21_get_bits(value_2.bits[i], j) && buffer != 0)
        s21_set_bit(result, index, 1);

      if (i == 2 && j == 31 && buffer != 0) {
        flag = 1;
        res = 2;
      }
    }
  }

  return res;
}

int s21_mini_add(s21_decimal a, s21_decimal b, s21_decimal *result) {
  s21_zero_decimal(result);
  unsigned buffer = 0;
  int index = 0;
  int res = 0;

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 32 && res == 0; j++, index++) {
      if (s21_get_bits(a.bits[i], j) && s21_get_bits(b.bits[i], j) &&
          buffer != 0)
        s21_set_bit(result, index, 1);

      if (s21_get_bits(a.bits[i], j) && s21_get_bits(b.bits[i], j) &&
          buffer == 0)
        buffer++;

      if (!s21_get_bits(a.bits[i], j) && !s21_get_bits(b.bits[i], j) &&
          buffer != 0) {
        s21_set_bit(result, index, 1);
        buffer--;
      }

      if ((!s21_get_bits(a.bits[i], j) && s21_get_bits(b.bits[i], j) &&
           buffer == 0) ||
          (s21_get_bits(a.bits[i], j) && !s21_get_bits(b.bits[i], j) &&
           buffer == 0))
        s21_set_bit(result, index, 1);

      if (i == 2 && j == 31 && buffer != 0) {
        res = 1;
      }
    }
  }

  return res;
}

//------------------------------func for div----------------------------//

int s21_is_decimal_no_empty(s21_decimal dst) {
  return dst.bits[0] + dst.bits[1] + dst.bits[2];
}

int s21_is_big_decimal_not_empty(s21_big_decimal dst) {
  return dst.bits[0] + dst.bits[1] + dst.bits[2] + dst.bits[3] + dst.bits[4] +
         dst.bits[5] + dst.bits[6] + dst.bits[7];
}

void s21_import_to_big_decimal(s21_decimal value_1, s21_big_decimal *value_2) {
  value_2->bits[0] = value_1.bits[0];
  value_2->bits[1] = value_1.bits[1];
  value_2->bits[2] = value_1.bits[2];
}

int s21_div_big_decimal(s21_big_decimal value_1, s21_big_decimal value_2,
                        s21_big_decimal *result) {
  int b_1 = 0, b_2 = 0, bit_2 = 0, scale = 0, diff = 0, save_scale = 0;
  s21_big_decimal tmp = {0};
  s21_find_highest_bit_big_decimal(value_1, value_2, &b_1, &b_2);
  bit_2 = b_2;
  for (int i = 0; i < 96 && s21_is_big_decimal_not_empty(value_1);) {
    if (i > 0) {
      s21_shift_big_dec_l(&value_2, 1);
      s21_increase_scale_big_decimal(result, 1);
      s21_increase_scale_big_decimal(&value_1, 1);
      save_scale++;
    }
    scale = s21_equation_bits_big_decimal(&value_1, &value_2);
    save_scale += scale;
    b_1 = b_2 = 0;
    s21_find_highest_bit_big_decimal(value_1, value_2, &b_1, &b_2);
    diff = b_2 - bit_2;
    if (diff < 0) diff = 0;
    for (; diff >= 0 && s21_is_big_decimal_not_empty(value_1);) {
      if (s21_is_greater_big_decimal(value_2, value_1)) {
        s21_set_bit_big(&tmp, 0, 0);
      } else {
        s21_sub_big_decimal(value_1, value_2, &value_1);
        s21_set_bit_big(&tmp, 0, 1);
      }
      i++;
      diff--;
      if (diff >= 0) s21_shift_big_dec_r(&value_2, 1);
      s21_shift_big_dec_l(&tmp, 1);
    }
    if (diff >= 0) s21_shift_big_dec_l(&tmp, diff + 1);
    s21_shift_big_dec_r(&tmp, 1);
    s21_add_big_decimal(*result, tmp, result);
    s21_zero_big_decimal(&tmp);
  }
  return save_scale;
}

int s21_post_normalization(s21_big_decimal *result, int scale) {
  int dop = 0;
  if (dop == 0) {
    while ((result->bits[3] || result->bits[4] || result->bits[5] ||
            result->bits[6] || result->bits[7]) &&
           scale > 0) {
      if (scale == 1 && result->bits[3]) dop = 1;
      s21_decr_exp_big_decimal(result, 1);
      scale--;
    }
  }
  if ((result->bits[3] || result->bits[4] || result->bits[5] ||
       result->bits[6] || result->bits[7]))
    scale = -1;
  return scale;
}

void s21_find_highest_bit_big_decimal(s21_big_decimal v1, s21_big_decimal v2,
                                      int *bit_1, int *bit_2) {
  for (int i = 255; i >= 0 && (!(*bit_1) || !(*bit_2)); i--) {
    if (*bit_1 == 0 && s21_get_bit_big(v1, i)) *bit_1 = i;
    if (*bit_2 == 0 && s21_get_bit_big(v2, i)) *bit_2 = i;
  }
}

int s21_equation_bits_big_decimal(s21_big_decimal *value_1,
                                  s21_big_decimal *value_2) {
  int scale = 0;
  while (s21_is_greater_or_equal_big_decimal(*value_1, *value_2)) {
    s21_shift_big_dec_l(value_2, 1);
  }
  s21_shift_big_dec_r(value_2, 1);
  return scale;
}

void s21_import_to_small_decimal(s21_decimal *value_1,
                                 s21_big_decimal value_2) {
  value_1->bits[0] = value_2.bits[0];
  value_1->bits[1] = value_2.bits[1];
  value_1->bits[2] = value_2.bits[2];
}

void s21_set_bit_big(s21_big_decimal *dst, int index, int bit) {
  int mask = 1u << (index % 32);
  if (bit == 0)
    dst->bits[index / 32] = dst->bits[index / 32] & ~mask;
  else
    dst->bits[index / 32] = dst->bits[index / 32] | mask;
}

void s21_increase_scale_big_decimal(s21_big_decimal *dst, int n) {
  s21_big_decimal ten = {{10, 0, 0, 0, 0, 0, 0, 0}}, tmp = {0};
  for (int i = 0; i < n; i++) {
    s21_mul_big_decimal(*dst, ten, &tmp);
    *dst = tmp;
    s21_zero_big_decimal(&tmp);
  }
}

int s21_is_greater_big_decimal(s21_big_decimal value_1,
                               s21_big_decimal value_2) {
  int result = 0, out = 0;
  for (int i = 7; i >= 0 && !result && !out; i--) {
    if (value_1.bits[i] || value_2.bits[i]) {
      if (value_1.bits[i] > value_2.bits[i]) {
        result = 1;
      }
      if (value_1.bits[i] != value_2.bits[i]) out = 1;
    }
  }
  return result;
}

int s21_is_greater_or_equal_big_decimal(s21_big_decimal value_1,
                                        s21_big_decimal value_2) {
  int result = 0, out = 0;
  for (int i = 7; i >= 0 && !out && !result; i--) {
    if (value_1.bits[i] != 0 || value_2.bits[i] != 0) {
      if (value_1.bits[i] >= value_2.bits[i]) {
        result = 1;
      }
      out = 1;
    }
  }
  return result;
}

int s21_shift_big_dec_l(s21_big_decimal *dst, int num) {  // сдвиг в лево
  int res = 0;
  int buffer[8] = {0};
  for (int k = 0; k < num; k++) {
    for (int i = 0; i < 7; i++) {
      buffer[i] = s21_get_bit_big(*dst, (i + 1) * 32 - 1);
    }
    for (int i = 7; i > 0 && !res; i--) {
      if (s21_get_bit_big(*dst, 255)) res = 1;
      dst->bits[i] <<= 1;
      s21_set_bit_big(dst, i * 32, buffer[i - 1]);
    }
    dst->bits[0] <<= 1;
  }
  return res;
}

void s21_zero_big_decimal(s21_big_decimal *dst) {  // зануление
  dst->bits[0] = dst->bits[1] = dst->bits[2] = dst->bits[3] = dst->bits[4] =
      dst->bits[5] = dst->bits[6] = dst->bits[7] = 0;
}

int s21_mul_big_decimal(s21_big_decimal value_1, s21_big_decimal value_2,
                        s21_big_decimal *result) {  // умножение
  int res = 0, count = 0;
  for (int i = 0; i < 256 && !res; i++) {
    if (s21_get_bit_big(value_2, i)) {
      res = s21_shift_big_dec_l(&value_1, i - count);
      s21_add_big_decimal(value_1, *result, result);
      count = i;
    }
  }
  return res;
}

void s21_add_big_decimal(s21_big_decimal value_1, s21_big_decimal value_2,
                         s21_big_decimal *result) {  // сложение биг дец
  int res = 0, flag = 0;
  for (int i = 0; i < 256; i++) {
    res = s21_get_bit_big(value_1, i) + s21_get_bit_big(value_2, i) + flag;
    flag = res / 2;
    s21_set_bit_big(result, i, res % 2);
  }
}

void s21_sub_big_decimal(s21_big_decimal value_1, s21_big_decimal value_2,
                         s21_big_decimal *result) {  // вычитание биг дец
  int tmp = 0, res = 0;
  for (int i = 0; i < 256; i++) {
    res = s21_get_bit_big(value_1, i) - s21_get_bit_big(value_2, i) - tmp;
    tmp = res < 0;
    res = abs(res);
    s21_set_bit_big(result, i, res % 2);
  }
}

void s21_decr_exp_big_decimal(
    s21_big_decimal *dst,
    int n) {  // уменьшает значение переданного ей большого десятичного числа
  s21_big_decimal ten = {{10, 0, 0, 0, 0, 0, 0, 0}}, tmp = {0};
  for (int i = 0; i < n; i++) {
    s21_div_big_decimal(*dst, ten, &tmp);
    *dst = tmp;
    s21_zero_big_decimal(&tmp);
  }
}

void s21_shift_big_dec_r(s21_big_decimal *dst, int num) {  // сдвиг вправо
  int buffer[7] = {0};
  for (int j = 0; j < num; j++) {
    for (int i = 0; i < 7; i++) {
      buffer[i] = s21_get_bit_big(*dst, (i + 1) * 32);
    }
    for (int i = 0; i < 7; i++) {
      dst->bits[i] >>= 1;
      s21_set_bit_big(dst, (i + 1) * 32 - 1, buffer[i]);
    }
    dst->bits[7] >>= 1;
  }
}

int s21_get_bit_big(s21_big_decimal dst, int index) {
  int mask = 1u << (index % 32);
  return (dst.bits[index / 32] & mask) != 0;
}

//--------------------for float to decimal--------------------//

int s21_get_bit(s21_decimal dst, int index) {
  int mask = 1u << (index % 32);
  return (dst.bits[index / 32] & mask) != 0;
}

int s21_get_sign(s21_decimal dst) { return (dst.bits[3] & 1u << 31) != 0; }

int s21_from_decimal_to_double(s21_decimal src, long double *dst) {
  long double tmp = (double)*dst;
  for (int i = 0; i < 96; i++) tmp += s21_get_bit(src, i) * pow(2, i);

  tmp = tmp * pow(10, -s21_get_exp(src));
  if (s21_get_bit(src, 127)) tmp = tmp * (-1);
  *dst = tmp;
  return 0;
}
