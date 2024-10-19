#include <ssl-utils.h>
#include <stdlib.h>
#include <string-utils.h>
#include <string.h>

#define MAX_PASSWORD_LEN 64
#define SALT_LEN 8

#define ITERATIONS 10000

CipherEntry cipher_functions[] = {
    {"aes128-ecb", EVP_aes_128_ecb},
    {"aes128-cbc", EVP_aes_128_cbc},
    {"aes128-cfb", EVP_aes_128_cfb},
    {"aes128-ofb", EVP_aes_128_ofb},
    {"aes192-ecb", EVP_aes_192_ecb},
    {"aes192-cbc", EVP_aes_192_cbc},
    {"aes192-cfb", EVP_aes_192_cfb},
    {"aes192-ofb", EVP_aes_192_ofb},
    {"aes256-ecb", EVP_aes_256_ecb},
    {"aes256-cbc", EVP_aes_256_cbc},
    {"aes256-cfb", EVP_aes_256_cfb},
    {"aes256-ofb", EVP_aes_256_ofb},
    {"3des-ecb", EVP_des_ede3_ecb},
    {"3des-cbc", EVP_des_ede3_cbc},
    {"3des-cfb", EVP_des_ede3_cfb},
    {"3des-ofb", EVP_des_ede3_ofb},
    {NULL, NULL}};

CipherParams *init_cipher_params(const char *password, const char *algorithm, const char *mode) {
    // Allocate space for the parameters
    CipherParams *params = (CipherParams *)malloc(sizeof(CipherParams));
    if (params == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    params->password_len = strlen(password);
    params->password = strndup(password, params->password_len);

    params->cipher = get_cipher(algorithm, mode);

    // Derive key and iv from password
    const unsigned char salt[SALT_LEN] = {0};
    const EVP_MD *dgst = EVP_sha256();

    const int keylen = EVP_CIPHER_key_length(params->cipher);
    params->key = (uint8_t *)malloc(keylen);
    if (params->key == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free_cipher_params(params);
        exit(1);
    }

    const int ivlen = EVP_CIPHER_iv_length(params->cipher);
    params->iv = (uint8_t *)malloc(ivlen);
    if (params->iv == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free_cipher_params(params);
        exit(1);
    }

    unsigned char key_iv_pair[keylen + ivlen];
    if (1 != PKCS5_PBKDF2_HMAC(params->password, params->password_len, salt, SALT_LEN, ITERATIONS, dgst, keylen + ivlen, key_iv_pair)) {
        fprintf(stderr, "Error deriving key and iv\n");
        free_cipher_params(params);
        exit(1);
    }

    memcpy(params->key, key_iv_pair, keylen);
    memcpy(params->iv, key_iv_pair + keylen, ivlen);

    return params;
}

void free_cipher_params(CipherParams *params) {
    if (params->key != NULL) free(params->key);
    if (params->iv != NULL) free(params->iv);

    free(params->password);
    free(params);
}

const EVP_CIPHER *get_cipher(const char *algorithm, const char *mode) {
    char cipher_name[16] = {0};
    snprintf(cipher_name, sizeof(cipher_name), "%s-%s", algorithm, mode);

    for (int i = 0; cipher_functions[i].name != NULL; i++) {
        if (strcmp(cipher_name, cipher_functions[i].name) == 0) {
            return cipher_functions[i].cipher_func();
        }
    }
    return NULL;
}

int encrypt(const CipherParams *params, const uint8_t *plaintext, const int plaintext_len, uint8_t **ciphertext) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (ctx == NULL) {
        fprintf(stderr, "Error creating cipher context\n");
        exit(1);
    }

    const EVP_CIPHER *cipher = params->cipher;
    if (1 != EVP_EncryptInit_ex(ctx, cipher, NULL, params->key, params->iv)) {
        fprintf(stderr, "Error initializing encryption\n");
        EVP_CIPHER_CTX_free(ctx);
        exit(1);
    }

    int len;
    int ciphertext_len = 0;
    int max_ciphertext_len = plaintext_len + EVP_CIPHER_block_size(cipher);
    *ciphertext = malloc(max_ciphertext_len);
    if (*ciphertext == NULL) {
        fprintf(stderr, "Error allocating memory for ciphertext\n");
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }

    if (1 != EVP_EncryptUpdate(ctx, *ciphertext, &len, plaintext, plaintext_len)) {
        fprintf(stderr, "Error encrypting data\n");
        EVP_CIPHER_CTX_free(ctx);
        free(*ciphertext);
        return -1;
    }
    ciphertext_len = len;

    if (1 != EVP_EncryptFinal_ex(ctx, *ciphertext + len, &len)) {
        fprintf(stderr, "Error finalizing encryption\n");
        EVP_CIPHER_CTX_free(ctx);
        free(*ciphertext);
        exit(1);
    }
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

int decrypt(const CipherParams *params, const uint8_t *ciphertext, const int ciphertext_len, uint8_t **plaintext) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (ctx == NULL) {
        fprintf(stderr, "Error creating cipher context\n");
        return -1;
    }

    const EVP_CIPHER *cipher = params->cipher;
    if (1 != EVP_DecryptInit_ex(ctx, cipher, NULL, params->key, params->iv)) {
        fprintf(stderr, "Error initializing decryption\n");
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }

    int len;
    int plaintext_len = 0;
    *plaintext = malloc(ciphertext_len);
    if (*plaintext == NULL) {
        fprintf(stderr, "Error allocating memory for plaintext\n");
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }

    if (1 != EVP_DecryptUpdate(ctx, *plaintext, &len, ciphertext, ciphertext_len)) {
        fprintf(stderr, "Error decrypting data\n");
        EVP_CIPHER_CTX_free(ctx);
        free(*plaintext);
        return -1;
    }
    plaintext_len = len;

    if (1 != EVP_DecryptFinal_ex(ctx, *plaintext + len, &len)) {
        fprintf(stderr, "Error finalizing decryption\n");
        EVP_CIPHER_CTX_free(ctx);
        free(*plaintext);
        return -1;
    }
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}
