#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

// Rutgers University 2021 Spring CS 211 Computer Architecture
// Assignment based on CS:APP Cache Lab

// 64-bit memory address
typedef unsigned long long int mem_addr_t;

// cache properties
unsigned char s = 2; // set index bits
unsigned short S = 1<<2; // set count

unsigned char E = 4; // way associative cache; lines per set

unsigned char b = 4; // block offset bits
// unsigned short B = 1<<4; // block size in bytes

// fully associative cache line
typedef struct cache_line 
{
    bool valid;
    mem_addr_t tag;
    size_t recent_use; // For keeping track of recent use of cache line
} cache_line_t;

typedef cache_line_t* cache_set_t;
typedef cache_set_t* cache_t;

// For keeping track of recent use of cache line
size_t program_counter = 0;

// does nor work qith test mTRIX  8x8+
// accessData - Access data at memory address addr.
void accessData (mem_addr_t addr, cache_t cache,
    unsigned int* hit_count, // If it is already in cache, increase hit_count
    unsigned int* miss_count, // If it is not in cache, bring it in cache, increase miss_count
    unsigned int* eviction_count)// Also increase eviction_count if a line is evicted
 {
    // Cache indices for this address
    // s = 2, b = 4
    mem_addr_t tag = addr >> (s+b);
    mem_addr_t set = addr >> b;
    if (set > 3) { set %= 4; }

    // Find the correct cache set, find the correct E in the set
    for (int i = 0; i < E; i++)
    {
        // Cache hit
        if (cache[set][i].tag == tag && cache[set][i].valid)
        {
            (*hit_count)++;
            cache[set][i].recent_use = program_counter;
            return; 
        }
    }

    // Otherwise, record a cache miss
    (*miss_count)++; 

    // Within the correct cache set, find the least recently used line; that is 
    // where missed cache block will be placed
    size_t leastUsed = cache[set][0].recent_use; //set the least used to the first index
    size_t indexLeastUsed = 0; // the index of the line with the least value used
    for (int i = 0; i < E; i++) // loops through all lines of the set
    {
        if (cache[set][i].recent_use < leastUsed) // if the number is the least so far, keep the index and set it
        { 
            indexLeastUsed = i; 
            leastUsed = cache[set][i].recent_use; 
        }
    }

    // If cache set line already in use, then record an eviction
    if (cache[set][indexLeastUsed].valid) // conflict miss
    {
        (*eviction_count)++;
        cache[set][indexLeastUsed].tag = tag;
        cache[set][indexLeastUsed].recent_use = program_counter;
        return; 
    }

    // dry miss
    cache[set][indexLeastUsed].valid = true; 
    cache[set][indexLeastUsed].tag = tag; 
    cache[set][indexLeastUsed].recent_use = program_counter; 
}
 
int main(int argc, char* argv[]) 
{
    // path to memory trace
    if (argc!= 2) 
    {
        printf("Usage: ./fullyAssociative <mem_trace_file>\n");
        exit(EXIT_FAILURE);
    }
    char* mem_trace_file = argv[1];
    FILE *fp = fopen(mem_trace_file, "r");
    if (!fp) 
    {
        fprintf(stderr, "Error opening file '%s'\n", mem_trace_file);
        exit(EXIT_FAILURE);
    }

    // Allocate memory, write 0's for valid and tag and LRU
    cache_t cache = calloc(S, sizeof(cache_set_t));
    for (size_t set_index = 0; set_index < S; set_index++) // 2d array with size arr[S][E], s=4, e=4
    {
        cache[set_index] = calloc(E, sizeof(cache_line_t));
    }

    for (int i = 0; i < S; i++)
    {
        for (int j = 0; j < E; j++)
        {
            cache[i][j].valid = false;
            cache[i][j].recent_use = 0;
        }
    }

    // cache simulation statistics
    unsigned int miss_count = 0;
    unsigned int hit_count = 0;
    unsigned int eviction_count = 0;

    // Loop through until we are done with the file.
    size_t line_buf_size = 256;
    char line_buf[line_buf_size];
    while (fgets(line_buf, line_buf_size, fp) != NULL) 
    {
        program_counter++;
        // replay the given trace file against the cache
        if (line_buf[1]=='L' || line_buf[1]=='S' || line_buf[1]=='M') 
        {
            char access_type = '\0';
            mem_addr_t addr = 0;
            unsigned int len = 0;
            sscanf (line_buf, " %c %llx,%u", &access_type, &addr, &len);

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

    // free the array
    for (size_t set_index=0; set_index < S; set_index++) 
    {
        free(cache[set_index]);
        cache[set_index] = NULL;
    }
    free(cache);
    cache = NULL;
    fclose(fp);

    /* Output the hit and miss statistics for the autograder */
    printf("hits:%d misses:%d evictions:%d\n", hit_count, miss_count, eviction_count);

    exit(EXIT_SUCCESS);
}
