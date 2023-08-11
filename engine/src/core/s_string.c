#include "s_string.h"

#include "s_memory.h"

u64 string_length(const char* str)
{
    /*
    u64 length = 0;
    while (str[length] != 0)
    {
        length++;
    }
    return length;
    */
    return strlen(str);
}

char* string_duplicate(const char* str)
{
    u64 length = string_length(str);
    u64 size_bytes = (sizeof(char) * length) + 1;
    char* out_string = s_allocate(size_bytes, MEMORY_TAG_STRING);
    s_copy_memory(out_string, str, size_bytes);
    return out_string;
}

b8 strings_equal(const char* str0, const char* str1)
{
    /*
    u64 str0_length = string_length(str0);
    u64 str1_length = string_length(str1);

    if(str0_length != str1_length)
        return false;

    for(u16 i = 0; i < str0_length; i++)
    {
        if(str0[i] != str1[i])
            return false;
        if(str0[i] == 0)
            break;
    }
    
    return true;
    */
   return strcmp(str0, str1) == 0;
}