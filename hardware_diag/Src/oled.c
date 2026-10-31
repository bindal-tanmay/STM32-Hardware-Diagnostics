#include "oled.h"
#include "i2c.h"

#define OLED_ADDRESS 0x3C

#define OLED_WIDTH   128
#define OLED_PAGES   8

/*
 * SH1106 has 132 columns internally.
 * The visible 128 columns start at column 2.
 */
#define OLED_COLUMN_OFFSET 2


/* ----------------------------------------------------
 * 5x7 font
 * ---------------------------------------------------- */

static const uint8_t font_digits[][5] =
{
    {0x3E, 0x51, 0x49, 0x45, 0x3E}, /* 0 */
    {0x00, 0x42, 0x7F, 0x40, 0x00}, /* 1 */
    {0x42, 0x61, 0x51, 0x49, 0x46}, /* 2 */
    {0x21, 0x41, 0x45, 0x4B, 0x31}, /* 3 */
    {0x18, 0x14, 0x12, 0x7F, 0x10}, /* 4 */
    {0x27, 0x45, 0x45, 0x45, 0x39}, /* 5 */
    {0x3C, 0x4A, 0x49, 0x49, 0x30}, /* 6 */
    {0x01, 0x71, 0x09, 0x05, 0x03}, /* 7 */
    {0x36, 0x49, 0x49, 0x49, 0x36}, /* 8 */
    {0x06, 0x49, 0x49, 0x29, 0x1E}  /* 9 */
};


/* Only characters needed for our diagnostic display */
static const uint8_t font_STM[] =
{
    /* S */
    0x46,0x49,0x49,0x49,0x31,

    /* T */
    0x01,0x01,0x7F,0x01,0x01,

    /* M */
    0x7F,0x02,0x0C,0x02,0x7F
};

static const uint8_t font_I[] =
{
    0x00,0x41,0x7F,0x41,0x00
};

static const uint8_t font_C[] =
{
    0x3E,0x41,0x41,0x41,0x22
};

static const uint8_t font_P[] =
{
    0x7F,0x09,0x09,0x09,0x06
};

static const uint8_t font_A[] =
{
    0x7E,0x09,0x09,0x09,0x7E
};

static const uint8_t font_O[] =
{
    0x3E,0x41,0x41,0x41,0x3E
};

static const uint8_t font_L[] =
{
    0x7F,0x40,0x40,0x40,0x40
};

static const uint8_t font_N[] =
{
    0x7F,0x02,0x04,0x08,0x7F
};

static const uint8_t font_E[] =
{
    0x7F,0x49,0x49,0x49,0x41
};

static const uint8_t font_Y[] =
{
    0x07,0x08,0x70,0x08,0x07
};
static const uint8_t font_H[] =
{
    0x7F, 0x08, 0x08, 0x08, 0x7F
};

static const uint8_t font_D[] =
{
    0x7F, 0x41, 0x41, 0x22, 0x1C
};

static const uint8_t font_W[] =
{
    0x7F, 0x20, 0x18, 0x20, 0x7F
};

static const uint8_t font_exclamation[] =
{
    0x00, 0x00, 0x5F, 0x00, 0x00
};

/* ----------------------------------------------------
 * Low-level OLED functions
 * ---------------------------------------------------- */

static void OLED_WriteCommand(uint8_t command)
{
    uint8_t buffer[2];

    /*
     * 0x00 = command control byte
     */
    buffer[0] = 0x00;
    buffer[1] = command;

    I2C1_Write(OLED_ADDRESS, buffer, 2);
}


static void OLED_WriteData(const uint8_t *data, uint8_t length)
{
    uint8_t buffer[17];

    /*
     * 0x40 = data control byte
     */
    buffer[0] = 0x40;

    for (uint8_t i = 0; i < length; i++)
    {
        buffer[i + 1] = data[i];
    }

    I2C1_Write(OLED_ADDRESS, buffer, length + 1);
}


/* ----------------------------------------------------
 * OLED initialization
 * ---------------------------------------------------- */

void OLED_Init(void)
{
    /*
     * Display OFF
     */
    OLED_WriteCommand(0xAE);

    /*
     * Display clock
     */
    OLED_WriteCommand(0xD5);
    OLED_WriteCommand(0x80);

    /*
     * Multiplex ratio = 64
     */
    OLED_WriteCommand(0xA8);
    OLED_WriteCommand(0x3F);

    /*
     * Display offset
     */
    OLED_WriteCommand(0xD3);
    OLED_WriteCommand(0x00);

    /*
     * Display start line
     */
    OLED_WriteCommand(0x40);

    /*
     * DC-DC converter ON
     */
    OLED_WriteCommand(0xAD);
    OLED_WriteCommand(0x8B);

    /*
     * Segment remap
     */
    OLED_WriteCommand(0xA1);

    /*
     * COM scan direction
     */
    OLED_WriteCommand(0xC8);

    /*
     * COM pins configuration
     */
    OLED_WriteCommand(0xDA);
    OLED_WriteCommand(0x12);

    /*
     * Contrast
     */
    OLED_WriteCommand(0x81);
    OLED_WriteCommand(0x80);

    /*
     * Pre-charge period
     */
    OLED_WriteCommand(0xD9);
    OLED_WriteCommand(0x22);

    /*
     * VCOMH
     */
    OLED_WriteCommand(0xDB);
    OLED_WriteCommand(0x40);

    /*
     * Entire display follows RAM
     */
    OLED_WriteCommand(0xA4);

    /*
     * Normal display
     */
    OLED_WriteCommand(0xA6);

    /*
     * Clear display RAM
     */
    OLED_Clear();

    /*
     * Display ON
     */
    OLED_WriteCommand(0xAF);
}


/* ----------------------------------------------------
 * Set cursor
 * ---------------------------------------------------- */

void OLED_SetCursor(uint8_t x, uint8_t page)
{
    uint8_t column;

    column = x + OLED_COLUMN_OFFSET;

    /*
     * Page address
     */
    OLED_WriteCommand(0xB0 | page);

    /*
     * Lower column address
     */
    OLED_WriteCommand(0x00 | (column & 0x0F));

    /*
     * Higher column address
     */
    OLED_WriteCommand(0x10 | ((column >> 4) & 0x0F));
}


/* ----------------------------------------------------
 * Clear display
 * ---------------------------------------------------- */

void OLED_Clear(void)
{
    uint8_t blank[16];

    for (uint8_t i = 0; i < 16; i++)
    {
        blank[i] = 0x00;
    }

    for (uint8_t page = 0; page < OLED_PAGES; page++)
    {
        OLED_SetCursor(0, page);

        for (uint8_t block = 0; block < 8; block++)
        {
            OLED_WriteData(blank, 16);
        }
    }
}


/* ----------------------------------------------------
 * Write character
 * ---------------------------------------------------- */

void OLED_WriteChar(char c)
{
    uint8_t data[6];

    /*
     * Space
     */
    if (c == ' ')
    {
        data[0] = 0;
        data[1] = 0;
        data[2] = 0;
        data[3] = 0;
        data[4] = 0;
    }

    /*
     * Digits
     */
    else if (c >= '0' && c <= '9')
    {
        for (uint8_t i = 0; i < 5; i++)
            data[i] = font_digits[c - '0'][i];
    }

    /*
     * Letters
     */
    else
    {
        const uint8_t *font = 0;

        switch (c)
        {
            case 'S': font = font_STM + 0; break;
            case 'T': font = font_STM + 5; break;
            case 'M': font = font_STM + 10; break;

            case 'I': font = font_I; break;
            case 'C': font = font_C; break;
            case 'P': font = font_P; break;
            case 'A': font = font_A; break;
            case 'O': font = font_O; break;
            case 'L': font = font_L; break;
            case 'N': font = font_N; break;
            case 'E': font = font_E; break;
            case 'Y': font = font_Y; break;
            case 'H': font = font_H; break;
            case 'D': font = font_D; break;
            case 'W': font = font_W; break;
            case '!': font = font_exclamation; break;

            default:
                font = font_I;
                break;
        }

        for (uint8_t i = 0; i < 5; i++)
            data[i] = font[i];
    }

    /*
     * Blank column between characters
     */
    data[5] = 0x00;

    OLED_WriteData(data, 6);
}


/* ----------------------------------------------------
 * Write string
 * ---------------------------------------------------- */

void OLED_WriteString(const char *str)
{
    while (*str)
    {
        OLED_WriteChar(*str);
        str++;
    }
}
