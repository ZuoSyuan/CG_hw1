/*
 * OpenGL Font Support Functions from Chapter 7.
 *
 * Written by Michael Sweet.
 */

/*
 * Include necessary headers.
 */

#include "font.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>


/*
 * Limits...
 */

#define MAX_STRING	1024


/*
 * 'FontCreate()' - Load Windows font bitmaps into OpenGL display lists.
 */

GLFONT *                         /* O - Font data */
FontCreate(HDC        hdc,       /* I - Device Context */
           const char *typeface, /* I - Font specification */
           int        height,    /* I - Font height/size in pixels */
	   int        weight,    /* I - Weight of font (bold, etc) */
	   DWORD      italic)    /* I - Text is italic */
    {
    GLFONT *font;                /* Font data pointer */
    HFONT  fontid;               /* Windows font ID */
    int    charset;              /* Character set code */

    /* Allocate memory */
    if ((font = (GLFONT*)calloc(1, sizeof(GLFONT))) == (GLFONT *)0)
        return ((GLFONT *)0);
 
    /* Allocate display lists */
    if ((font->base = glGenLists(256)) == 0)
        {
        free(font);
        return (0);
        }

    /* Select a character set */
    if (stricmp(typeface, "symbol") == 0)
        charset = SYMBOL_CHARSET;
    else
        charset = ANSI_CHARSET;

    /* Load the font */
    fontid = CreateFont(height, 0, 0, 0, weight, italic, FALSE, FALSE,
                        charset, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
                        DRAFT_QUALITY, DEFAULT_PITCH, typeface);

    SelectObject(hdc, fontid);

    /* Create bitmaps for each character */
    wglUseFontBitmaps(hdc, 0, 256, font->base);

    /* Get the width and height information for each character */
    GetCharWidth(hdc, 0, 255, font->widths);
    font->height = height;

    return (font);
    }

/*
 * 'FontDestroy()' - Delete the specified font.
 */

void
FontDelete(GLFONT *font) /* I - Font to delete */
    {
    if (font == (GLFONT *)0)
        return;

    glDeleteLists(font->base, 256);
    free(font);
    }


/*
 * 'FontPuts()' - Display a string using the specified font.
 */

void
FontPuts(GLFONT     *font, /* I - Font to use */
         const char *s)    /* I - String to display */
    {
    if (font == (GLFONT *)0 || s == NULL)
        return;

    glPushAttrib(GL_LIST_BIT);
    glListBase(font->base);
    glCallLists(strlen(s), GL_UNSIGNED_BYTE, s);
    glPopAttrib();
    }


/*
 * 'FontPrintf()' - Display a formatted string using the specified font.
 */

void
FontPrintf(GLFONT     *font,   /* I - Font to use */
           int        align,   /* I - Alignment to use */
           const char *format, /* I - printf() style format string */
           ...)                /* I - Other arguments as necessary */
    {
    va_list       ap;          /* Argument pointer */
    unsigned char s[1024],     /* Output string */
                  *ptr;        /* Pointer into string */
    int           width;       /* Width of string in pixels */

    if (font == (GLFONT *)0 || format == (char *)0)
        return;

    /* Format the string */
    va_start(ap, format);
    vsprintf((char *)s, format, ap);
    va_end(ap);

    /* Figure out the width of the string in pixels... */
    for (ptr = s, width = 0; *ptr; ptr ++)
        width += font->widths[*ptr];

    /* Adjust the bitmap position as needed to justify the text */
    if (align < 0)
        glBitmap(0, 0, 0, 0, -width, 0, NULL);
    else if (align == 0)
        glBitmap(0, 0, 0, 0, -width / 2, 0, NULL);

    /* Draw the string */
    FontPuts(font, (char *)s);
    }
