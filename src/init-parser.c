#include "../include/init-parser.h"
#include "../include/constants/cli-arguments.h"

ArgParser* init_steganography_parser() {
    ArgParser *parser = create_parser();

    // Extract subcommand
    Subcommand *extract = add_subcommand(parser, CMD_EXTRACT);

    const char *steg_choices[] = {STEG_LSB1, STEG_LSB4, STEG_LSBI};
    const char steg_count = sizeof(steg_choices) / sizeof(steg_choices[0]);
    const char *encryption_choices[] = {ENC_AES128, ENC_AES192, ENC_AES256, ENC_3DES};
    const char enc_count = sizeof(encryption_choices) / sizeof(encryption_choices[0]);
    const char *mode_choices[] = {MODE_ECB, MODE_CFB, MODE_OFB, MODE_CBC};
    const char mode_count = sizeof(mode_choices) / sizeof(mode_choices[0]);

    add_argument(extract, ARG_OUTPUT, ARG_STRING, NULL, 0);
    add_argument(extract, ARG_PAYLOAD, ARG_STRING, NULL, 0);
    add_argument(extract, ARG_STEGANOGRAPHY, ARG_CHOICE, steg_choices, 3);
    add_argument(extract, ARG_ENCRYPTION, ARG_CHOICE, encryption_choices, 4);
    add_argument(extract, ARG_MODE, ARG_CHOICE, mode_choices, 4);
    add_argument(extract, ARG_PASSWORD, ARG_STRING, NULL, 0);

    // Embed subcommand
    Subcommand *embed = add_subcommand(parser, "-embed");

    add_argument(embed, ARG_INPUT, ARG_STRING, NULL, 0);
    add_argument(embed, ARG_PAYLOAD, ARG_STRING, NULL, 0);
    add_argument(embed, ARG_OUTPUT, ARG_STRING, NULL, 0);
    add_argument(embed, ARG_STEGANOGRAPHY, ARG_CHOICE, steg_choices, steg_count);
    add_argument(embed, ARG_ENCRYPTION, ARG_CHOICE, encryption_choices, enc_count);
    add_argument(embed, ARG_MODE, ARG_CHOICE, mode_choices, mode_count);
    add_argument(embed, ARG_PASSWORD, ARG_STRING, NULL, 0);

    return parser;
}