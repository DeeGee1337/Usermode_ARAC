#pragma once
#include "globals.hpp"


//https://tesseract-ocr.github.io/

namespace ocr
{
    bool execute_ocr()
    {
        char* outText;

        //tesseract::TessBaseAPI* api = new tesseract::TessBaseAPI(); // DUNNO WHY BUT THROWS EXEPTION
        //Removed the pointer here

        tesseract::TessBaseAPI myOCR;

        // Initialize tesseract-ocr with English, without specifying tessdata path
        if (myOCR.Init("F:\\ProjectsVS\\Vcpkg\\tessdata", "eng"))
        {
            fprintf(stderr, "Could not initialize tesseract.\n");
            //exit(1);
        }

        // Open input image with leptonica library
        Pix* image = pixRead("F:\\ProjectsVS\\Usermode_ARAC\\ACAR\\ACAR\\tesseract\\Cheat_Examples\\menu.png");
        myOCR.SetImage(image);
        // Get OCR result
        outText = myOCR.GetUTF8Text();

        std::cout << "----------------------------------" << std::endl;
        printf("[OCR] Output:\n%s", outText);
        std::cout << "----------------------------------" << std::endl;

        //Check OCRList
        std::vector<char> content;
        while (*(outText) != '\0')
        {
            content.push_back(*(outText++));
        }

        /*std::cout << "[OCR] Printing content: \n";
        for (auto itt : content)
            std::cout << itt;*/

        // Destroy used object and release memory
        //api->End();
        myOCR.End();

        //delete api; //throws exception
        //delete[] outText; //throws exception
        myOCR.Clear();

        pixDestroy(&image);

        if(!check_OCRList(content))
            return false;

        return true;
    }
}
