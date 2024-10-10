#include <argument-parser.h>
#include <bmp-utils.h>
#include <init-parser.h>
#include <lsb1.h>
#include <lsbi.h>
#include <ssl-utils.h>
#include <stdint.h>
#include <stdio.h>
#include <subcommands.h>

// TODO: delete
void example_encrypt_decrypt();

int main(int argc, char *argv[]) {
    ArgParser *parser = init_steganography_parser();

    parse_arguments(parser, argc, argv);

    run_subcommand(parser);

    /*
    // Extract data
    uint8_t* bmpSecretData = read_bmp_data(bmpFilePathOutput, &fileHeader, &infoHeader);
    if (!bmpSecretData) {
        exit(1);
        char *extractedMessage = calloc(payloadLength, sizeof(char));
    lsb1_extract(bmpSecretData, infoHeader.biWidth, infoHeader.biHeight, infoHeader.biBitCount, extractedMessage, payloadLength);
    char content[payloadLength];
    char extension[payloadLength];
    size_t extractedMessageLength = dfmt_payload(extractedMessage, content, extension);
    char *outputFilePathWithExtension = (char*)malloc(strlen((char*)outputFileName) + strlen((char*)extension) + 1);
    sprintf(outputFilePathWithExtension, "%s%s", (char*)outputFileName, (char*)extension);
    
    create_file(outputFilePathWithExtension);
    write_file(outputFilePathWithExtension, content, extractedMessageLength);
    
    free(outputFilePathWithExtension);
    free(bmpSecretData);
    free(extractedMessage);
    */
    free_parser(parser);

    return 0;
}

void example_encrypt_decrypt() {
    CipherParams *params = init_cipher_params("password", "aes256", "cbc");

    const char *message = "Hello, World! This is a test message.";
    uint8_t *plaintext = (uint8_t *)message;
    int plaintext_len = strlen(message);

    uint8_t *ciphertext = NULL;
    int ciphertext_len = encrypt(params, plaintext, plaintext_len, &ciphertext);
    if (ciphertext_len < 0) {
        fprintf(stderr, "Encryption failed\n");
        return;
    }

    printf("Original message: %s\n", message);
    printf("Encrypted message (hex): ");
    for (int i = 0; i < ciphertext_len; i++) {
        printf("%02x", ciphertext[i]);
    }
    printf("\n");

    uint8_t *decrypted_text = NULL;
    int decrypted_len = decrypt(params, ciphertext, ciphertext_len, &decrypted_text);
    if (decrypted_len < 0) {
        fprintf(stderr, "Decryption failed\n");
        free(ciphertext);
        return;
    }

    // print decrypted message
    printf("Decrypted message: %.*s\n", decrypted_len, decrypted_text);

    free(ciphertext);
    free(decrypted_text);
    free_cipher_params(params);
}