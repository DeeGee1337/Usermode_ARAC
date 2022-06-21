#pragma once
#include "globals.hpp"

//https://tesseract-ocr.github.io/

//Christoph Sommer, Anton Halder, Fabian Folger
namespace ocr
{
    bool execute_ocr()
    {
        char* outText;
        tesseract::TessBaseAPI myOCR;

        // Initialize tesseract-ocr with English, without specifying tessdata path
        if (myOCR.Init("C:\\Users\\Fabian\\Documents\\GitHub\\Usermode_ARAC\\ACAR\\ACAR\\tesseract\\tessdata", "eng"))
        {
            fprintf(stderr, "Could not initialize tesseract.\n");
        }

        // Open input image with leptonica library
        Pix* image = pixRead("C:\\Users\\Fabian\\Documents\\GitHub\\Usermode_ARAC\\ACAR\\ACAR\\tesseract\\Cheat_Examples\\menu.png");
        myOCR.SetImage(image);

        // Get OCR result
        outText = myOCR.GetUTF8Text();

        //std::cout << "----------------------------------" << std::endl;
        //printf("[OCR] Output:\n%s", outText);
        //std::cout << "----------------------------------" << std::endl;

        //Check OCRList
        std::string convert;
        convert.append(outText);
        std::cout << "[OCR] Printing content: " << convert << std::endl;

        // Destroy used object
        myOCR.End();
        myOCR.Clear();

        pixDestroy(&image);

        if (!check_OCRList(convert))
            return false;

        return true;
    }
}
