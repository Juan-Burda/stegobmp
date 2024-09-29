#ifndef CLI_ARGUMENTS_H
#define CLI_ARGUMENTS_H

#define MAX_ARGS 16
#define MAX_ARG_LEN 32
#define MAX_SUBCOMMANDS 2

#define CMD_EMBED "-embed"
#define CMD_EXTRACT "-extract"

#define ARG_PASSWORD "pass"
#define ARG_STEGANOGRAPHY "steg"
#define ARG_ENCRYPTION "a"
#define ARG_MODE "m"
#define ARG_INPUT "in"
#define ARG_OUTPUT "out"
#define ARG_PAYLOAD "p"

#define ARG_FLAG 0
#define ARG_STRING 1
#define ARG_CHOICE 2

#define STEG_LSB1 "LSB1"
#define STEG_LSB4 "LSB4"
#define STEG_LSBI "LSBI"

#define ENC_AES128 "aes128"
#define ENC_AES192 "aes192"
#define ENC_AES256 "aes256"
#define ENC_3DES "3des"

#define MODE_ECB "ecb"
#define MODE_CFB "cfb"
#define MODE_OFB "ofb"
#define MODE_CBC "cbc"

#endif // CLI_ARGUMENTS_H