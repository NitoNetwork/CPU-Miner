#ifndef __ALGO_GATE_API_H__
#define __ALGO_GATE_API_H__ 1

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "miner.h"
#include "simd-utils.h"



typedef  uint32_t set_t;

#define EMPTY_SET        0
#define SSE2_OPT         1
#define AES_OPT          2  
#define SSE42_OPT        4
#define AVX_OPT          8   // Sandybridge
#define AVX2_OPT      0x10   // Haswell, Zen1
#define SHA_OPT       0x20   // Zen1, Icelake (sha256)
#define AVX512_OPT    0x40   // Skylake-X (AVX512[F,VL,DQ,BW])
#define VAES_OPT      0x80   // Icelake (VAES & AVX512)


// return set containing all elements from sets a & b
inline set_t set_union ( set_t a, set_t b ) { return a | b; }

// return set contained common elements from sets a & b
inline set_t set_intsec ( set_t a, set_t b) { return a & b; }

// all elements in set a are included in set b
inline bool set_incl ( set_t a, set_t b ) { return (a & b) == a; }

// no elements in set a are included in set b
inline bool set_excl ( set_t a, set_t b ) { return (a & b) == 0; }

typedef struct
{
// Mandatory functions, one of these is mandatory. If a generic scanhash
// is used a custom target hash function must be registered, with a custom
// scanhash the target hash function can be called directly and doesn't need
// to be registered with the gate. 
int ( *scanhash ) ( struct work*, uint32_t, uint64_t*, struct thr_info* );

int ( *hash )     ( void*, const void*, int );

//optional, safe to use default in most cases

// Called once by each miner thread to allocate thread local buffers and
// other initialization specific to miner threads.
bool ( *miner_thread_init )     ( int );

// Get thread local copy of blockheader with unique nonce.
void ( *get_new_work )          ( struct work*, struct work*, int, uint32_t* );

// Decode getwork blockheader
bool ( *work_decode )           ( struct work* );

// Extra getwork data
void ( *decode_extra_data )     ( struct work*, uint64_t* );

bool ( *submit_getwork_result ) ( CURL*, struct work* );

void ( *gen_merkle_root )       ( char*, struct stratum_ctx* );

// Increment extranonce
void ( *build_extraheader )     ( struct work*, struct stratum_ctx* );

void ( *build_block_header )    ( struct work*, uint32_t, uint32_t*,
	                                uint32_t*,   uint32_t, uint32_t,
                                   unsigned char* );

// Build mining.submit message
void ( *build_stratum_request ) ( char*, struct work*, struct stratum_ctx* );

char* ( *malloc_txs_request )   ( struct work* );

// Big endian or little endian
void ( *set_work_data_endian )  ( struct work* );

// Diverge mining threads
bool ( *do_this_thread )        ( int );

// After do_this_thread
void ( *resync_threads )        ( int, struct work* );

json_t* ( *longpoll_rpc_call )  ( CURL*, int*, char* );

set_t optimizations;
int  ( *get_work_data_size )     ();
int  ntime_index;
int  nbits_index;
int  nonce_index;            // use with caution, see warning below
int  work_cmp_size;
} algo_gate_t;

extern algo_gate_t algo_gate;

// Declare generic null targets, default for many gate functions
// Functions that use one of these generic targets do not have
// a default defined below. Some algos may override a defined default
// with a generic.
void do_nothing();
bool return_true();
bool return_false();
void *return_null();
void algo_not_tested();
void algo_not_implemented();
void four_way_not_tested();

// Warning: algo_gate.nonce_index should only be used in targetted code
// due to different behaviours by different targets. The JR2 index uses an
// 8 bit offset while all others user 32 bit offset. c/c++ pointer arithmetic
// conventions results in different behaviour for pointers with different
// target sizes requiring customized casting to make it work consistently.
// Rant mode: yet another thing I hate about c/c++. Array indexes should
// be scaled, pointer offsets should always be bytes. No confusion and no
// hidden math.

#define STD_NTIME_INDEX 17
#define STD_NBITS_INDEX 18
#define STD_NONCE_INDEX 19   // 32 bit offset
#define STD_WORK_DATA_SIZE 128
#define STD_WORK_CMP_SIZE 76

//#define JR2_NONCE_INDEX 39  // 8 bit offset

// These indexes are only used with JSON RPC2 and are not gated.
//#define JR2_WORK_CMP_INDEX_2 43
//#define JR2_WORK_CMP_SIZE_2 33

// deprecated, use generic instead
int null_scanhash();

// Default generic, may be used in many cases.
// N-way is more complicated, requires many different implementations
// depending on architecture, input format, and output format.
// Naming convention is scanhash_[N]way_[input format]in_[output format]out
// N = number of lanes
// input/output format:
//    32: 32 bit interleaved parallel lanes
//    64: 64 bit interleaved parallel lanes
//    640: input only, not interleaved, contiguous serial 640 bit lanes.
//    256: output only, not interleaved, contiguous serial 256 bit lanes.

int scanhash_generic( struct work *work, uint32_t max_nonce,
                      uint64_t *hashes_done, struct thr_info *mythr );

#if defined(__AVX2__)

//int scanhash_4way_64in_64out( struct work *work, uint32_t max_nonce,
//                      uint64_t *hashes_done, struct thr_info *mythr );

//int scanhash_4way_64in_256out( struct work *work, uint32_t max_nonce,
//                      uint64_t *hashes_done, struct thr_info *mythr );

int scanhash_4way_64in_32out( struct work *work, uint32_t max_nonce,
                      uint64_t *hashes_done, struct thr_info *mythr );

//int scanhash_8way_32in_32out( struct work *work, uint32_t max_nonce,
//                      uint64_t *hashes_done, struct thr_info *mythr );

#endif

#if defined(__AVX512F__) && defined(__AVX512VL__) && defined(__AVX512DQ__) && defined(__AVX512BW__)

//int scanhash_8way_64in_64out( struct work *work, uint32_t max_nonce,
//                      uint64_t *hashes_done, struct thr_info *mythr );

//int scanhash_8way_64in_256out( struct work *work, uint32_t max_nonce,
//                      uint64_t *hashes_done, struct thr_info *mythr );

int scanhash_8way_64in_32out( struct work *work, uint32_t max_nonce,
                      uint64_t *hashes_done, struct thr_info *mythr );

//int scanhash_16way_32in_32out( struct work *work, uint32_t max_nonce,
//                      uint64_t *hashes_done, struct thr_info *mythr );

#endif

// displays warning
int null_hash();

// optional safe targets, default listed first unless noted.

void std_get_new_work( struct work *work, struct work *g_work, int thr_id,
                       uint32_t* end_nonce_ptr );

void sha256d_gen_merkle_root( char *merkle_root, struct stratum_ctx *sctx );
void SHA256_gen_merkle_root ( char *merkle_root, struct stratum_ctx *sctx );

bool std_le_work_decode( struct work *work );
bool std_be_work_decode( struct work *work );

bool std_le_submit_getwork_result( CURL *curl, struct work *work );
bool std_be_submit_getwork_result( CURL *curl, struct work *work );

void std_le_build_stratum_request( char *req, struct work *work );
void std_be_build_stratum_request( char *req, struct work *work );

char* std_malloc_txs_request( struct work *work );

// Default is do_nothing, little endian is assumed
void set_work_data_big_endian( struct work *work );

void std_build_block_header( struct work* g_work, uint32_t version,
	                          uint32_t *prevhash,  uint32_t *merkle_root,
   	                       uint32_t ntime,      uint32_t nbits,
                             unsigned char *final_sapling_hash );

void std_build_extraheader( struct work *work, struct stratum_ctx *sctx );

json_t* std_longpoll_rpc_call( CURL *curl, int *err, char *lp_url );

int std_get_work_data_size();

// Gate admin functions

// Called from main to initialize all gate functions and algo-specific data
// by calling the algo's register function.
bool register_algo_gate( int algo, algo_gate_t *gate );

// Called by algos to verride any default gate functions that are applicable
// and do any other algo-specific initialization.
// The register functions for all the algos can be declared here to reduce
// compiler warnings but that's just more work for devs adding new algos.
bool register_algo( algo_gate_t *gate );

// use this to call the hash function of an algo directly, ie util.c test.
void exec_hash_function( int algo, void *output, const void *pdata );

// Validate a string as a known algo and alias, updates arg to proper
// algo name if valid alias, NULL if invalid alias or algo.
void get_algo_alias( char **algo_or_alias );

#endif
