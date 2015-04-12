#include <stdio.h>
#include <stdlib.h>
#include <sha2.h>

unsigned char app_b1[SHA256_DIGEST_LENGTH] = {
    0xba, 0x78, 0x16, 0xbf, 0x8f, 0x01, 0xcf, 0xea,
    0x41, 0x41, 0x40, 0xde, 0x5d, 0xae, 0x22, 0x23,
    0xb0, 0x03, 0x61, 0xa3, 0x96, 0x17, 0x7a, 0x9c,
    0xb4, 0x10, 0xff, 0x61, 0xf2, 0x00, 0x15, 0xad
};

unsigned char app_b2[SHA256_DIGEST_LENGTH] = {
    0xcd, 0xc7, 0x6e, 0x5c, 0x99, 0x14, 0xfb, 0x92,
    0x81, 0xa1, 0xc7, 0xe2, 0x84, 0xd7, 0x3e, 0x67,
    0xf1, 0x80, 0x9a, 0x48, 0xa4, 0x97, 0x20, 0x0e,
    0x04, 0x6d, 0x39, 0xcc, 0xc7, 0x11, 0x2c, 0xd0
};

int main(int argc, char **argv)
{
	unsigned char md[SHA256_DIGEST_LENGTH];

	fprintf(stdout, "Testing SHA-256 ");
	SHA256_CTX *state = malloc(sizeof(SHA256_CTX));
	SHA256_Init(state);

	SHA256_Update(state, "abc", 3);
	SHA256_Final(md, state);
	if (memcmp(md, app_b1, sizeof(app_b1))) {
		fflush(stdout);
		fprintf(stderr, "\nTEST 1 of 2 failed.\n");
		return 1;
    } else
		fprintf(stdout, ".");
	fflush(stdout);

	SHA256_Init(state);
	int i;
	for (i = 0; i < 1000000; i += 160)
		SHA256_Update(state, "aaaaaaaa" "aaaaaaaa" "aaaaaaaa" "aaaaaaaa"
		                     "aaaaaaaa" "aaaaaaaa" "aaaaaaaa" "aaaaaaaa"
		                     "aaaaaaaa" "aaaaaaaa" "aaaaaaaa" "aaaaaaaa"
		                     "aaaaaaaa" "aaaaaaaa" "aaaaaaaa" "aaaaaaaa"
		                     "aaaaaaaa" "aaaaaaaa" "aaaaaaaa" "aaaaaaaa",
		                     (1000000 - i) < 160 ? 1000000 - i : 160);
	SHA256_Final(md, state);

	if (memcmp(md, app_b2, sizeof(app_b2))) {
		fflush(stdout);
		fprintf(stderr, "\nTEST 2 of 2 failed.\n");
		return 1;
    } else
		fprintf(stdout, ".");

	free(state);
	fprintf(stdout, " passed.\n");
	fflush(stdout);

	return 0;
} 
