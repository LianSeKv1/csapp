#include <stdio.h>
typedef unsigned float_bits;

// 2.92
float_bits float_negate(float_bits f)
{
    unsigned frac = f & 0x7FFFFF;
    unsigned exp = (f >> 23) & 0xFF;
    unsigned sign = f >> 31;
    if (exp == 0xff && !frac)
    {
        return f;
    }
    return ~sign << 31 | exp << 23 | frac;
}

// 2.93

float_bits float_absval(float_bits f)
{
    unsigned frac = f & 0x7FFFFF;
    unsigned exp = (f >> 23) & 0xFF;
    if (exp == 0xff && !frac)
    {
        return f;
    }
    return 0 << 31 | exp << 23 | frac;
}

// 2.94
float_bits float_twice(float_bits f)
{
    unsigned frac = f & 0x7FFFFF;
    unsigned exp = (f >> 23) & 0xFF;
    unsigned sign = f >> 31;

    //  NaN | 无穷
    if (exp == 0xff)
    {
        return f;
    }
    else if (exp == 0x0)
    {
        if ((frac >> 22) == 0x1)
        {
            exp = 1;
        }
        frac = (frac << 1) & 0x7FFFFF;
    }
    else
    {
        if (exp == 0xFE)
        {
            exp = 0xFF;
            frac = 0;
        }
        else
        {
            exp += 1;
        }
    }
    return sign << 31 | exp << 23 | frac;
}

// 2.95
float_bits float_half(float_bits f)
{
    unsigned frac = f & 0x7fffff;
    unsigned sign = f >> 31;
    unsigned exp = (f >> 23) & 0xff;

    unsigned add = (frac & 0x3) == 0x3;
    if (exp == 0xff)
    {
        return f;
    }
    else if (exp == 0)
    {
        frac = frac >> 1;
        frac += add;
    }
    else
    {
        if (exp == 1)
        {
            exp = 0;
            frac = (frac >> 1) | (1 << 22);
            frac += add;
        }
        else
        {
            exp -= 1;
        }
    }
    return sign << 31 | exp << 23 | frac;
}

int float_f2i(float_bits f)
{
    unsigned exp = (f >> 23) & 0xFF;
    unsigned frac = f & 0x7FFFFF;
    unsigned sign = f >> 31 == 0 ? 1 : -1;
    //  ZERO
    if (exp == 0xFF)
    {
        return 0x80000000;
    }
    else if (exp == 0)
    {
        return 0;
    }
    else
    {
        //  2
        int w = exp - 127;
        if (w < 0)
        {
            return 0;
        }
        else if (w > 31)
        {
            return 0x80000000;
        }
        else if (w >= 23)
        {
            return sign * ((frac | 1 << 23) << (w - 23));
        }
        else
        {
            return sign * ((frac | 1 << 23) >> (23 - w));
        }
    }
}
