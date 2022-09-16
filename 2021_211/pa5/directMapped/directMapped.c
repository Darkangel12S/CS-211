#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

// Rutgers University 2021 Spring CS 211 Computer Architecture
// Assignment based on CS:APP Cache Lab

// 64-bit memory address
typedef unsigned long long int mem_addr_t;

// cache properties
unsigned char s = 4; // set index bits
unsigned short S = 1<<4; // set count = 16

unsigned char b = 4; // block offset bits
// unsigned short B = 1<<4; // block size in bytes

// direct mapped cache set/line
typedef struct cache_set_line 
{
    bool valid;
    mem_addr_t tag;
} cache_set_line_t;
typedef cache_set_line_t* cache_t;

// accessData - Access data at memory address addr.
void accessData(mem_addr_t addr, cache_t cache,
    unsigned int* hit_count, // If it is already in cache, increase hit_count
    unsigned int* miss_count, // If it is not in cache, bring it in cache, increase miss_count
    unsigned int* eviction_count) // Also increase eviction_count if a line is evicted
{
    // Cache indices for this address
    // not sure if this is right
    mem_addr_t tag = addr >> (s+b);
    // shift it by 4 bits s=4=b
    mem_addr_t set = (addr >> s); 
    if (set > 15) { set %= 16; }

    // Cache hit
    if (cache[set].valid && cache[set].tag == tag) // if valid and the tag matches, update hit count and return
    {
        (*hit_count)++;
        return; 
    }

    // Otherwise, record cache miss
    (*miss_count)++;

    // If cache set line already in use as indicated by the valid variable, 
    // then evict the existing cache set line
    if (cache[set].valid) // if everything is valid, evict evict the set
    {
        (*eviction_count)++; 
        // change the tag into the new tag
        cache[set].tag = tag; 
        return;
    }
    
    // dry miss
    cache[set].valid = true; 
    cache[set].tag = tag;
}

int main(int argc, char* argv[]) 
{
    // path to memory trace
    if (argc!= 2) 
    {
        printf( "Usage: ./directMapped <mem_trace_file>" );
    }
    char* mem_trace_file = argv[1];
    FILE *fp = fopen(mem_trace_file, "r");
    if (!fp) 
    {
        fprintf(stderr, "Error opening file '%s'\n", mem_trace_file);
        exit( EXIT_FAILURE );
    }

    // Allocate memory, write 0's for valid and tag and LRU
    // creates cache[] with 16 struct(cache_t) in it
    cache_t cache = (cache_set_line_t*)calloc(S, sizeof(cache_set_line_t));

    // cache simulation statistics
    unsigned int miss_count = 0;
    unsigned int hit_count = 0;
    unsigned int eviction_count = 0;

    // Loop through until we are done with the file.
    size_t line_buf_size = 256;
    char line_buf[line_buf_size];

    while (fgets(line_buf, line_buf_size, fp) != NULL) 
    {
        // replay the given trace file against the cache
        if (line_buf[1]=='L' || line_buf[1]=='S' || line_buf[1]=='M') 
        {
            char access_type = '\0';
            mem_addr_t addr = 0;
            unsigned int len = 0;
            sscanf ( line_buf, " %c %llx,%u", &access_type, &addr, &len );

            if (access_type == 'L' || access_type == 'S' || access_type == 'M') 
            {
                accessData(addr, cache, &hit_count, &miss_count, &eviction_count);
            }
            // If the instruction is M indicating L followed by S then access again
            if (access_type == 'M')
            {
                accessData(addr, cache, &hit_count, &miss_count, &eviction_count);
            }
        }
    }
    /* Output the hit and miss statistics for the autograder */
    printf("hits:%d misses:%d evictions:%d\n", hit_count, miss_count, eviction_count);

    // free mememoy
    free(cache);
    return 0; 
}
