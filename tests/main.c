

void test1(void);
void test2(void);
void test3(void);

int main(void) {
    test3();
}

void test1(void) {
    str *s = str_new_raw("Ciao come stai?");
    str_iter *s_iter = str_split(s, " ");

    str_iter_debug(s_iter, 1);

    // manual iteration
    str *s_next;
    str* s_list[10];
    int i = 0;
    while ((s_next = str_iter_next(s_iter)) != NULL) {
        str_iter_debug(s_iter, 1);
        s_list[i] = s_next;
        i++;
    }
    str_iter_debug(s_iter, 1);

    for (int j = 0; j < i; j++) {
        str_debug(s_list[j], 1);
        str_free(s_list[j]);
    }

    str_iter_debug(s_iter, 1);
    free(s_iter);
}

void test2(void) {
    str_iter *s_iter = str_split_raw("Ciao Pippo come stai?", " ");
    str_iter_debug(s_iter, 1);

    int i = 0;
    str **str_all = str_iter_collect(s_iter, &i);

    printf("\n\n\n======== outside ========\n");
    for (int j = 0; j < i; j++) str_debug(str_all[j], 1);
    free(str_all);

    str_iter_debug(s_iter, 1);
}


void test3(void) {
    str_iter *s_words_iter = str_split_raw("Hi Paul, how are you?", " ");
    if (s_words_iter == NULL) {
        // error handling
        exit(1);
    }

    int nw;
    str **s_words = str_iter_collect(s_words_iter, &nw);
    if (s_words == NULL) {
        // error handling
        exit(1);
    }

    for (int i = 0; i < nw; i++) {
        printf("%d: %s\n", i, s_words[i]->buf);
    }
}
