//
// Created by kreska on 25.03.17.
//

#ifndef UNTITLED7_NFTWF_H
#define UNTITLED7_NFTWF_H

int display_info(const char *fpath, const struct stat *file_stat,
                 int typeflag, struct FTW *ftwbuf);

void init_max(int max);

#endif //UNTITLED7_NFTWF_H
