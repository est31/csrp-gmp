#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>


#include "srp.h"


#define NITER          100
#define TEST_HASH      SRP_SHA256
#define TEST_NG        SRP_NG_1024

unsigned long long get_usec()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (((unsigned long long)t.tv_sec) * 1000000) + t.tv_usec;
}

// The test vectors from
// https://tools.ietf.org/html/rfc5054#appendix-B

static const char srp_5054_salt[] = {
    0xBE, 0xB2, 0x53, 0x79, 0xD1, 0xA8, 0x58, 0x1E,
    0xB5, 0xA7, 0x27, 0x67, 0x3A, 0x24, 0x41, 0xEE,
};

static const char srp_5054_v[] = {
    0x7E, 0x27, 0x3D, 0xE8, 0x69, 0x6F, 0xFC, 0x4F,
    0x4E, 0x33, 0x7D, 0x05, 0xB4, 0xB3, 0x75, 0xBE,
    0xB0, 0xDD, 0xE1, 0x56, 0x9E, 0x8F, 0xA0, 0x0A,
    0x98, 0x86, 0xD8, 0x12, 0x9B, 0xAD, 0xA1, 0xF1,
    0x82, 0x22, 0x23, 0xCA, 0x1A, 0x60, 0x5B, 0x53,
    0x0E, 0x37, 0x9B, 0xA4, 0x72, 0x9F, 0xDC, 0x59,
    0xF1, 0x05, 0xB4, 0x78, 0x7E, 0x51, 0x86, 0xF5,
    0xC6, 0x71, 0x08, 0x5A, 0x14, 0x47, 0xB5, 0x2A,
    0x48, 0xCF, 0x19, 0x70, 0xB4, 0xFB, 0x6F, 0x84,
    0x00, 0xBB, 0xF4, 0xCE, 0xBF, 0xBB, 0x16, 0x81,
    0x52, 0xE0, 0x8A, 0xB5, 0xEA, 0x53, 0xD1, 0x5C,
    0x1A, 0xFF, 0x87, 0xB2, 0xB9, 0xDA, 0x6E, 0x04,
    0xE0, 0x58, 0xAD, 0x51, 0xCC, 0x72, 0xBF, 0xC9,
    0x03, 0x3B, 0x56, 0x4E, 0x26, 0x48, 0x0D, 0x78,
    0xE9, 0x55, 0xA5, 0xE2, 0x9E, 0x7A, 0xB2, 0x45,
    0xDB, 0x2B, 0xE3, 0x15, 0xE2, 0x09, 0x9A, 0xFB,
};

static const char srp_5054_a[] = {
    0x60, 0x97, 0x55, 0x27, 0x03, 0x5C, 0xF2, 0xAD,
    0x19, 0x89, 0x80, 0x6F, 0x04, 0x07, 0x21, 0x0B,
    0xC8, 0x1E, 0xDC, 0x04, 0xE2, 0x76, 0x2A, 0x56,
    0xAF, 0xD5, 0x29, 0xDD, 0xDA, 0x2D, 0x43, 0x93,
};

static const char srp_5054_A[] = {
    0x61, 0xD5, 0xE4, 0x90, 0xF6, 0xF1, 0xB7, 0x95,
    0x47, 0xB0, 0x70, 0x4C, 0x43, 0x6F, 0x52, 0x3D,
    0xD0, 0xE5, 0x60, 0xF0, 0xC6, 0x41, 0x15, 0xBB,
    0x72, 0x55, 0x7E, 0xC4, 0x43, 0x52, 0xE8, 0x90,
    0x32, 0x11, 0xC0, 0x46, 0x92, 0x27, 0x2D, 0x8B,
    0x2D, 0x1A, 0x53, 0x58, 0xA2, 0xCF, 0x1B, 0x6E,
    0x0B, 0xFC, 0xF9, 0x9F, 0x92, 0x15, 0x30, 0xEC,
    0x8E, 0x39, 0x35, 0x61, 0x79, 0xEA, 0xE4, 0x5E,
    0x42, 0xBA, 0x92, 0xAE, 0xAC, 0xED, 0x82, 0x51,
    0x71, 0xE1, 0xE8, 0xB9, 0xAF, 0x6D, 0x9C, 0x03,
    0xE1, 0x32, 0x7F, 0x44, 0xBE, 0x08, 0x7E, 0xF0,
    0x65, 0x30, 0xE6, 0x9F, 0x66, 0x61, 0x52, 0x61,
    0xEE, 0xF5, 0x40, 0x73, 0xCA, 0x11, 0xCF, 0x58,
    0x58, 0xF0, 0xED, 0xFD, 0xFE, 0x15, 0xEF, 0xEA,
    0xB3, 0x49, 0xEF, 0x5D, 0x76, 0x98, 0x8A, 0x36,
    0x72, 0xFA, 0xC4, 0x7B, 0x07, 0x69, 0x44, 0x7B,
};

static const char srp_5054_b[] = {
    0xE4, 0x87, 0xCB, 0x59, 0xD3, 0x1A, 0xC5, 0x50,
    0x47, 0x1E, 0x81, 0xF0, 0x0F, 0x69, 0x28, 0xE0,
    0x1D, 0xDA, 0x08, 0xE9, 0x74, 0xA0, 0x04, 0xF4,
    0x9E, 0x61, 0xF5, 0xD1, 0x05, 0x28, 0x4D, 0x20,
};

static const char srp_5054_B[] = {
    0xBD, 0x0C, 0x61, 0x51, 0x2C, 0x69, 0x2C, 0x0C,
    0xB6, 0xD0, 0x41, 0xFA, 0x01, 0xBB, 0x15, 0x2D,
    0x49, 0x16, 0xA1, 0xE7, 0x7A, 0xF4, 0x6A, 0xE1,
    0x05, 0x39, 0x30, 0x11, 0xBA, 0xF3, 0x89, 0x64,
    0xDC, 0x46, 0xA0, 0x67, 0x0D, 0xD1, 0x25, 0xB9,
    0x5A, 0x98, 0x16, 0x52, 0x23, 0x6F, 0x99, 0xD9,
    0xB6, 0x81, 0xCB, 0xF8, 0x78, 0x37, 0xEC, 0x99,
    0x6C, 0x6D, 0xA0, 0x44, 0x53, 0x72, 0x86, 0x10,
    0xD0, 0xC6, 0xDD, 0xB5, 0x8B, 0x31, 0x88, 0x85,
    0xD7, 0xD8, 0x2C, 0x7F, 0x8D, 0xEB, 0x75, 0xCE,
    0x7B, 0xD4, 0xFB, 0xAA, 0x37, 0x08, 0x9E, 0x6F,
    0x9C, 0x60, 0x59, 0xF3, 0x88, 0x83, 0x8E, 0x7A,
    0x00, 0x03, 0x0B, 0x33, 0x1E, 0xB7, 0x68, 0x40,
    0x91, 0x04, 0x40, 0xB1, 0xB2, 0x7A, 0xAE, 0xAE,
    0xEB, 0x40, 0x12, 0xB7, 0xD7, 0x66, 0x52, 0x38,
    0xA8, 0xE3, 0xFB, 0x00, 0x4B, 0x11, 0x7B, 0x58,
};

// This isn't used (yet)
static const char srp_5054_u[] = {
    0xCE, 0x38, 0xB9, 0x59, 0x34, 0x87, 0xDA, 0x98,
    0x55, 0x4E, 0xD4, 0x7D, 0x70, 0xA7, 0xAE, 0x5F,
    0x46, 0x2E, 0xF0, 0x19,
};

// This is SHA-1(<premaster secret>)
static const char srp_5054_S[] = {
    0x01, 0x7e, 0xef, 0xa1, 0xce, 0xfc, 0x5c, 0x2e,
    0x62, 0x6e, 0x21, 0x59, 0x89, 0x87, 0xf3, 0x1e,
    0x0f, 0x1b, 0x11, 0xbb,
};

int test_rfc_5054_compat()
{
    struct SRPVerifier * ver;
    struct SRPUser     * usr;

    const unsigned char * bytes_s = (const unsigned char *) srp_5054_salt;
    const unsigned char * bytes_v = 0;
    const unsigned char * bytes_A = 0;
    const unsigned char * bytes_B = 0;

    const unsigned char * bytes_M    = 0;
    const unsigned char * bytes_HAMK = 0;
    const unsigned char * bytes_S    = 0;

    int len_s   = 16;
    int len_v   = 0;
    int len_A   = 0;
    int len_B   = 0;
    int len_M   = 0;
    int len_S   = 0;
    int i;

    const char * username = "alice";
    const char * password = "password123";

    SRP_HashAlgorithm alg     = SRP_SHA1;
    SRP_NGType        ng_type = SRP_NG_1024; //TEST_NG;

    printf("Testing RFC 5054 test vectors...");

    srp_create_salted_verification_key( alg, ng_type, username,
                (const unsigned char *)password,
                strlen(password), &bytes_s, &len_s, &bytes_v, &len_v, NULL, NULL );

    if (len_v != 128 || memcmp(&srp_5054_v, bytes_v, len_v) != 0)
    {
        printf(" computed v doesn't match!\n");
        return 1;
    }

    usr =  srp_user_new( alg, ng_type, username, username,
                         (const unsigned char *)password,
                         strlen(password), NULL, NULL );

    srp_user_start_authentication( usr, NULL, srp_5054_a, 32, &bytes_A, &len_A );

    if (memcmp(&srp_5054_A, bytes_A, len_A) != 0)
    {
        printf(" computed A doesn't match!\n");
        return 1;
    }

    /* User -> Host: (username, bytes_A) */
    ver =  srp_verifier_new( alg, ng_type, username, (const unsigned char *) srp_5054_salt,
                             len_s, bytes_v, len_v, bytes_A, len_A, srp_5054_b, 32, &bytes_B,
                             &len_B, NULL, NULL );

    if ( !bytes_B )
    {
        printf(" SRP-6a safety check violated for B!\n");
        return 1;
    }

    if (memcmp(&srp_5054_B, bytes_B, len_B) != 0)
    {
        printf(" computed B doesn't match!\n");
        return 1;
    }


    /* Host -> User: (bytes_s, bytes_B) */
    srp_user_process_challenge( usr, (const unsigned char *) srp_5054_salt, len_s, bytes_B, len_B, &bytes_M, &len_M );

    if ( !bytes_M )
    {
        printf(" SRP-6a safety check violated for M!\n");
        goto cleanup;
    }

    /* User -> Host: (bytes_M) */
    srp_verifier_verify_session( ver, bytes_M, &bytes_HAMK );

    if ( !bytes_HAMK )
    {
        printf(" user authentication failed!\n");
        goto cleanup;
    }

    /* Host -> User: (HAMK) */
    srp_user_verify_session( usr, bytes_HAMK );

    if ( !srp_user_is_authenticated(usr) )
    {
        printf(" server authentication failed!\n");
    }

    bytes_S = srp_verifier_get_session_key(ver, &len_S);

    if (memcmp(&srp_5054_S, bytes_S, len_S) != 0)
    {
        printf(" computed session key doesn't match!\n");
        return 1;
    }

    printf(" success.\n");

    cleanup:
    srp_verifier_delete( ver );
    srp_user_delete( usr );

    free( (char *)bytes_v );

    return 0;
}

const char * test_n_hex = "EEAF0AB9ADB38DD69C33F80AFA8FC5E86072618775FF3C0B9EA2314C9C256576D674DF7496"
   "EA81D3383B4813D692C6E0E0D5D8E250B98BE48E495C1D6089DAD15DC7D7B46154D6B6CE8E"
   "F4AD69B15D4982559B297BCF1885C529F566660E57EC68EDBC3C05726CC02FD4CBF4976EAA"
   "9AFD5138FE8376435B9FC61D2FC0EB06E3";
const char * test_g_hex = "2";

int main( int argc, char * argv[] )
{
    test_rfc_5054_compat();
    printf("Performing the speedtest.\n");

    struct SRPVerifier * ver;
    struct SRPUser     * usr;

    const unsigned char * bytes_s = 0;
    const unsigned char * bytes_v = 0;
    const unsigned char * bytes_A = 0;
    const unsigned char * bytes_B = 0;

    const unsigned char * bytes_M    = 0;
    const unsigned char * bytes_HAMK = 0;

    int len_s   = 0;
    int len_v   = 0;
    int len_A   = 0;
    int len_B   = 0;
    int len_M   = 0;
    int i;

    unsigned long long start;
    unsigned long long duration;

    const char * username = "TestUser";
    const char * ver_unam = "testuser";
    const char * password = "password";

    const char * n_hex         = 0;
    const char * g_hex         = 0;

    SRP_HashAlgorithm alg     = TEST_HASH;
    SRP_NGType        ng_type = SRP_NG_8192; //TEST_NG;

    if (ng_type == SRP_NG_CUSTOM)
    {
        n_hex = test_n_hex;
        g_hex = test_g_hex;
    }


    srp_create_salted_verification_key( alg, ng_type, ver_unam,
                (const unsigned char *)password,
                strlen(password),
                &bytes_s, &len_s, &bytes_v, &len_v, n_hex, g_hex );



    start = get_usec();

    for( i = 0; i < NITER; i++ )
    {
        usr =  srp_user_new( alg, ng_type, username, ver_unam,
                             (const unsigned char *)password,
                             strlen(password), n_hex, g_hex );

        srp_user_start_authentication( usr, NULL, NULL, 0, &bytes_A, &len_A );

        /* User -> Host: (username, bytes_A) */
        ver =  srp_verifier_new( alg, ng_type, username, bytes_s, len_s, bytes_v, len_v,
                                 bytes_A, len_A, NULL, 0, & bytes_B, &len_B, n_hex, g_hex );

        if ( !bytes_B )
        {
            printf("Verifier SRP-6a safety check violated!\n");
            goto cleanup;
        }

        /* Host -> User: (bytes_s, bytes_B) */
        srp_user_process_challenge( usr, bytes_s, len_s, bytes_B, len_B, &bytes_M, &len_M );

        if ( !bytes_M )
        {
            printf("User SRP-6a safety check violation!\n");
            goto cleanup;
        }

        /* User -> Host: (bytes_M) */
        srp_verifier_verify_session( ver, bytes_M, &bytes_HAMK );

        if ( !bytes_HAMK )
        {
            printf("User authentication failed!\n");
            goto cleanup;
        }

        /* Host -> User: (HAMK) */
        srp_user_verify_session( usr, bytes_HAMK );

        if ( !srp_user_is_authenticated(usr) )
        {
            printf("Server authentication failed!\n");
        }

cleanup:
        srp_verifier_delete( ver );
        srp_user_delete( usr );
    }

    duration = get_usec() - start;

    printf("Usec per call: %d\n", (int)(duration / NITER));


    free( (char *)bytes_s );
    free( (char *)bytes_v );

    return 0;
}
