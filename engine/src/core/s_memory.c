#include "s_memory.h"

#include "core/logging.h"
#include "platform/platform.h"

// TODO: String library
#include <string.h>
#include <stdio.h>

struct memory_stats
{
    u64 total_allocated;
    u64 tagged_allocations[MEMORY_MAX_TAGS];
};

static const char *memory_tag_strings[MEMORY_MAX_TAGS] = {
    "UNKNOWN    ",
    "ARRAY      ",
    "DARRAY     ",
    "DICT       ",
    "RING_QUEUE ",
    "BST        ",
    "STRING     ",
    "APPLICATION",
    "JOB        ",
    "TEXTURE    ",
    "MATERIAL   ",
    "RENDERER   ",
    "GAME       ",
    "TRANSFORM  ",
    "ENTITY     ",
    "ENTITY_NODE",
    "SCENE      ",
};

static struct memory_stats stats;

void initialize_memory()
{
    platform_zero(&stats, sizeof(stats));
}

void shutdown_memory()
{
}

void *s_allocate(u64 size, memory_tag tag)
{
    if (tag == MEMORY_TAG_UNKNOWN)
        S_WARN("Allocation called using MEMORY_TAG_UNKNOWN.");

    stats.total_allocated += size;
    stats.tagged_allocations[tag] += size;

    // TODO: Alignment
    void *block = platform_allocate(size);
    platform_zero(block, size);
    return block;
}

void s_free(void *block, u64 size, memory_tag tag)
{
    if (tag == MEMORY_TAG_UNKNOWN)
        S_WARN("Free called using MEMORY_TAG_UNKNOWN.");

    stats.total_allocated -= size;
    stats.tagged_allocations[tag] -= size;

    // TODO: Alignment
    platform_free(block);
}

void *s_zero_memory(void *block, u64 size)
{
    return platform_zero(block, size);
}

void *s_copy_memory(void *dest, const void *source, u64 size)
{
    return platform_copy(dest, source, size);
}

void *s_set_memory(void *dest, i32 value, u64 size)
{
    return platform_setmem(dest, value, size);
}

char *get_memory_usage_str()
{
    const u64 gib = 1024 * 1024 * 1024;
    const u64 mib = 1024 * 1024;
    const u64 kib = 1024;

    char buffer[8000] = "System memory use (tagged):\n";
    u64 offset = strlen(buffer);
    for (u32 i = 0; i < MEMORY_MAX_TAGS; i++)
    {
        char unit[4] = " iB";
        float amount = 1.0f;
        if (stats.tagged_allocations[i] >= gib)
        {
            unit[0] = 'G';
            amount = stats.tagged_allocations[i] / (float)gib;
        }
        else if (stats.tagged_allocations[i] >= mib)
        {
            unit[0] = 'M';
            amount = stats.tagged_allocations[i] / (float)mib;
        }
        else if (stats.tagged_allocations[i] >= kib)
        {
            unit[0] = 'K';
            amount = stats.tagged_allocations[i] / (float)kib;
        }
        else
        {
            unit[0] = 'B';
            unit[1] = 0;
            amount = stats.tagged_allocations[i];
        }

        i32 length = snprintf(buffer + offset, 8000, "[%s]: %.2f%s\n", memory_tag_strings[i], amount, unit);
        offset += length;
    }
    char* out_string = _strdup(buffer);
    return out_string;
}