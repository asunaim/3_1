#pragma once
static const struct des_test_case {
    int num, mode; // mode 1 = encrypt
    unsigned char key[8], txt[8], out[8];
}cases_[]=
{
     { 2, 1,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
                { 0x95, 0xF8, 0xA5, 0xE5, 0xDD, 0x31, 0xD9, 0x00 },
                { 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
     { 2, 0,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
                { 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
                { 0x95, 0xF8, 0xA5, 0xE5, 0xDD, 0x31, 0xD9, 0x00 } }
}



,cases[] = {
    { 1, 1,     { 0x10, 0x31, 0x6E, 0x02, 0x8C, 0x8F, 0x3B, 0x4A },
                { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
                { 0x82, 0xDC, 0xBA, 0xFB, 0xDE, 0xAB, 0x66, 0x02 } },
    { 2, 1,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
                { 0x95, 0xF8, 0xA5, 0xE5, 0xDD, 0x31, 0xD9, 0x00 },
                { 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 3, 1,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
                { 0xDD, 0x7F, 0x12, 0x1C, 0xA5, 0x01, 0x56, 0x19 },
                { 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 4, 1,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
                { 0x2E, 0x86, 0x53, 0x10, 0x4F, 0x38, 0x34, 0xEA },
                { 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 5, 1,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
                { 0x4B, 0xD3, 0x88, 0xFF, 0x6C, 0xD8, 0x1D, 0x4F },
                { 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 6, 1,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
                { 0x20, 0xB9, 0xE7, 0x67, 0xB2, 0xFB, 0x14, 0x56 },
                { 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 7, 1,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
                { 0x55, 0x57, 0x93, 0x80, 0xD7, 0x71, 0x38, 0xEF },
                { 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 8, 1,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
                { 0x6C, 0xC5, 0xDE, 0xFA, 0xAF, 0x04, 0x51, 0x2F },
                { 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 9, 1,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
                { 0x0D, 0x9F, 0x27, 0x9B, 0xA5, 0xD8, 0x72, 0x60 },
                { 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    {10, 1,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
                { 0xD9, 0x03, 0x1B, 0x02, 0x71, 0xBD, 0x5A, 0x0A },
                { 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },

    { 1, 0,     { 0x10, 0x31, 0x6E, 0x02, 0x8C, 0x8F, 0x3B, 0x4A },
                { 0x82, 0xDC, 0xBA, 0xFB, 0xDE, 0xAB, 0x66, 0x02 },
                { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 2, 0,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
                { 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
                { 0x95, 0xF8, 0xA5, 0xE5, 0xDD, 0x31, 0xD9, 0x00 } },
    { 3, 0,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
                { 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
                { 0xDD, 0x7F, 0x12, 0x1C, 0xA5, 0x01, 0x56, 0x19 } },
    { 4, 0,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
                { 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
                { 0x2E, 0x86, 0x53, 0x10, 0x4F, 0x38, 0x34, 0xEA } },
    { 5, 0,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
                { 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
                { 0x4B, 0xD3, 0x88, 0xFF, 0x6C, 0xD8, 0x1D, 0x4F } },
    { 6, 0,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
                { 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
                { 0x20, 0xB9, 0xE7, 0x67, 0xB2, 0xFB, 0x14, 0x56 } },
    { 7, 0,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
                { 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
                { 0x55, 0x57, 0x93, 0x80, 0xD7, 0x71, 0x38, 0xEF } },
    { 8, 0,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
                { 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
                { 0x6C, 0xC5, 0xDE, 0xFA, 0xAF, 0x04, 0x51, 0x2F } },
    { 9, 0,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
                { 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
                { 0x0D, 0x9F, 0x27, 0x9B, 0xA5, 0xD8, 0x72, 0x60 } },
    {10, 0,     { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
                { 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
                { 0xD9, 0x03, 0x1B, 0x02, 0x71, 0xBD, 0x5A, 0x0A } }

};
