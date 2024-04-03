#include <stdio.h>
#include <stdlib.h>
#include "hpdf.h"

#define PAGE_WIDTH  595.0
#define PAGE_HEIGHT 842.0
#define MARGIN      40.0
#define FONT_SIZE   12.0
#define CHARS_PER_ROW  10
#define ROWS_PER_PAGE  20
#define CHARS_PER_PAGE (CHARS_PER_ROW * ROWS_PER_PAGE)

int main() {
    HPDF_Doc pdf;
    HPDF_Page page;
    HPDF_Font font;

    pdf = HPDF_New(NULL, NULL);
    if (!pdf) {
        printf("ERROR: cannot create PDF object.\n");
        return -1;
    }

    if (HPDF_UseUTFEncodings(pdf) != HPDF_OK) {
        printf("ERROR: cannot use UTF encodings.\n");
        HPDF_Free(pdf);
        return -1;
    }

    font = HPDF_GetFont(pdf, HPDF_LoadTTFontFromFile(pdf, "D:/software/Project/creat_pdf/ArialUnicodeMS.ttf", HPDF_TRUE), "UTF-8");
    if (!font) {
        printf("ERROR: cannot load font.\n");
        HPDF_Free(pdf);
        return -1;
    }

    int page_count = 0;
    HPDF_UNICODE char_code = 0x0020;

    while (char_code <= 0xFFFF) {
        page = HPDF_AddPage(pdf);
        HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);
        HPDF_Page_BeginText(page);
        HPDF_Page_SetFontAndSize(page, font, FONT_SIZE);

        float x = MARGIN;
        float y = PAGE_HEIGHT - MARGIN;

        for (int i = 0; i < ROWS_PER_PAGE && char_code <= 0xFFFF; ++i) {
            for (int j = 0; j < CHARS_PER_ROW && char_code <= 0xFFFF; ++j) {
                if (y < MARGIN) {
                    HPDF_Page_EndText(page);
                    page = HPDF_AddPage(pdf);
                    HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);
                    HPDF_Page_BeginText(page);
                    HPDF_Page_SetFontAndSize(page, font, FONT_SIZE);
                    x = MARGIN;
                    y = PAGE_HEIGHT - MARGIN;
                }
                HPDF_Page_TextOut(page, x, y, (const char*)&char_code);
                x += FONT_SIZE;
                char_code++;
            }
            y -= FONT_SIZE;
            x = MARGIN;
        }

        HPDF_Page_EndText(page);
        page_count++;
    }

    if (HPDF_SaveToFile(pdf, "ArialUnicodeMS_characters.pdf") != HPDF_OK) {
        printf("ERROR: cannot save PDF file.\n");
        HPDF_Free(pdf);
        return -1;
    }

    printf("PDF created successfully with %d pages.\n", page_count);
    HPDF_Free(pdf);
    return 0;
}
