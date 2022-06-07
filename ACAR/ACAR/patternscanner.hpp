#pragma once

#include "globals.hpp"

bool CopyProcessSection(HANDLE process_handle, uintptr_t base, char* section_name, std::vector<uint8_t>& section_buf, size_t section_max_size)
{
    DWORD_PTR moduleBase = (DWORD_PTR)base;
    if (moduleBase)
    {
        IMAGE_DOS_HEADER dosHeader;
        BOOL result;
        result = ReadProcessMemory(process_handle, (void*)moduleBase, (void*)&dosHeader, sizeof(dosHeader), nullptr);
        if (!result)
            return false;

        if (dosHeader.e_magic != IMAGE_DOS_SIGNATURE)
            return false;

        IMAGE_NT_HEADERS ntHeader;
        result = ReadProcessMemory(process_handle, (void*)(moduleBase + dosHeader.e_lfanew), (void*)&ntHeader, sizeof(ntHeader), nullptr);
        if (!result)
            return false;

        if (ntHeader.Signature != IMAGE_NT_SIGNATURE)
            return false;

        //((PIMAGE_SECTION_HEADER)((ULONG_PTR)(&ntHeader)+FIELD_OFFSET(IMAGE_NT_HEADERS, OptionalHeader) + ((&ntHeader))->FileHeader.SizeOfOptionalHeader))

        IMAGE_SECTION_HEADER section_header;
        bool found_section = false;
        for (int i = 0; i < ntHeader.FileHeader.NumberOfSections; ++i)
        {
            auto psection_header = IMAGE_FIRST_SECTION(&ntHeader) + i;

            uintptr_t process_section_header = ((ULONG_PTR)(moduleBase + dosHeader.e_lfanew) + FIELD_OFFSET(IMAGE_NT_HEADERS, OptionalHeader) + (ntHeader.FileHeader.SizeOfOptionalHeader)) + i * sizeof(IMAGE_SECTION_HEADER);
            result = ReadProcessMemory(process_handle, (void*)process_section_header, &section_header, sizeof(section_header), nullptr);
            if (!result)
                return false;

            int cmp_result;
            cmp_result = strcmp((char*)section_header.Name, section_name);
            if (!cmp_result)
            {
                found_section = true;

                auto section_base = (base + section_header.VirtualAddress);
                auto section_size = section_header.SizeOfRawData;

                if (section_size >= section_max_size)
                {
                    //LOG("section_size too big: %x, adjusting...", section_size);
                    section_size = section_max_size;
                }

                section_buf.reserve(section_buf.size() + section_size);

                SIZE_T read_bytes = 0;
                result = ReadProcessMemory(process_handle, (void*)section_base, section_buf.data() + section_buf.size(), section_size, &read_bytes);
                if (!result || read_bytes != section_size)
                {
                    printf("%s: failed to read section (read_bytes: %X)", read_bytes);
                    return false;
                }

                section_buf.resize(section_buf.capacity());
            }
        }

        if (!found_section)
            return false;
        return true;
    }
    return false;
}