#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <unistd.h>

#include <libevdev/libevdev.h>
#include <linux/input.h>

static int key_code(const char *code) {
    int ret = libevdev_event_code_from_name(EV_KEY, code);

    if (ret == -1) {
        char *endptr;
        long icode;

        errno = 0;
        icode = strtol(code, &endptr, 0);
        if (errno || *endptr || icode < 0 || icode > KEY_MAX) {
            ret = KEY_RESERVED;
        } else {
            ret = (int)icode;
        }
    }

    if (ret == KEY_RESERVED) {
        fprintf(stderr, "invalid key code: %s\n", code);
        exit(EXIT_FAILURE);
    }

    return ret;
}

static int read_event(struct input_event *event) {
    return fread(event, sizeof(struct input_event), 1, stdin) == 1;
}

static void write_event(const struct input_event *event) {
    if (fwrite(event, sizeof(struct input_event), 1, stdout) != 1)
        exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
    struct input_event input;
    int i, count, *keys_a, *keys_b;

    if (argc < 3 || ((argc - 1) % 2)) {
        fprintf(stderr, "Usage: %s KEY_A1 KEY_B1 [[KEY_A2 KEY_B2] ...]\n",
                argv[0]);
        exit(EXIT_FAILURE);
    }

    count = (argc - 1) / 2;
    keys_a = malloc(sizeof(int) * count);
    keys_b = malloc(sizeof(int) * count);

    for (i = 0; i < count; i++) {
        keys_a[i] = key_code(argv[1 + i * 2]);
        keys_b[i] = key_code(argv[2 + i * 2]);
    }

    setbuf(stdin, NULL), setbuf(stdout, NULL);

    while (read_event(&input)) {
        if (input.type == EV_MSC && input.code == MSC_SCAN)
            continue;

        if (input.type != EV_KEY) {
            write_event(&input);
            continue;
        }

        for (i = 0; i < count; i++) {
            if (input.code == keys_a[i]) {
                input.code = keys_b[i];
                break;
            } else if (input.code == keys_b[i]) {
                input.code = keys_a[i];
                break;
            }
        }

        write_event(&input);
    }

    return EXIT_SUCCESS;
}
