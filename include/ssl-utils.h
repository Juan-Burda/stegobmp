#ifndef _SSL_UTILS_H_
#define _SSL_UTILS_H_

#include <openssl/evp.h>

/**
 * @brief Structure to hold the cipher and its corresponding function
 * 
 * @param name The name of the cipher
 * @param cipher_func The function to get the cipher
 */
typedef struct {
    const char *name;
    const EVP_CIPHER *(*cipher_func)(void);
} CipherEntry;

/**
 * @brief Structure to hold the parameters for encryption/decryption
 * 
 * @param password The password to use for encryption/decryption
 * @param password_len The length of the password
 * @param key The key to use for encryption/decryption
 * @param iv The initialization vector to use for encryption/decryption
 * @param cipher The cipher to use for encryption/decryption
 */
typedef struct {
    char *password;
    int password_len;
    uint8_t *key;
    uint8_t *iv;
    const EVP_CIPHER *cipher;
} CipherParams;

CipherParams *init_cipher_params(const char *password, const char *algorithm, const char *mode);
void free_cipher_params(CipherParams *params);
const EVP_CIPHER *get_cipher(const char *algorithm, const char *mode);

/**
 * @brief Encrypts the plaintext using the given parameters
 * 
 * @param params The parameters to use for encryption
 * @param plaintext The plaintext to encrypt
 * @param plaintext_len The length of the plaintext
 * @param ciphertext The ciphertext to store the encrypted data
 * 
 * @return int The length of the ciphertext
 * 
 * @note The caller is responsible for freeing the memory allocated for the ciphertext
 * @note Memory is allocated for the ciphertext
 */
int encrypt(const CipherParams *params, const uint8_t *plaintext, const int plaintext_len, uint8_t **ciphertext);

/**
 * @brief Decrypts the ciphertext using the given parameters
 * 
 * @param params The parameters to use for decryption
 * @param ciphertext The ciphertext to decrypt
 * @param ciphertext_len The length of the ciphertext
 * @param plaintext The plaintext to store the decrypted data
 * 
 * @return int The length of the plaintext
 * 
 * @note The caller is responsible for freeing the memory allocated for the plaintext
 * @note Memory is allocated for the plaintext
 */
int decrypt(const CipherParams *params, const uint8_t *ciphertext, const int ciphertext_len, uint8_t **plaintext);

#endif  // _SSL_UTILS_H_