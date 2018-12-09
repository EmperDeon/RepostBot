/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#include <stdint.h>
#include "minizip/mz.h"
#include "minizip/mz_os.h"
#include "minizip/mz_strm.h"
#include "minizip/mz_strm_buf.h"
#include "minizip/mz_strm_split.h"
#include "minizip/mz_zip.h"
#include "minizip/mz_zip_rw.h"
#include "Zip.h"

void Utils::Zip::addFiles(const std::string &archive_path, const std::vector<std::string> &file_paths) {
    void *writer;
    int32_t err;
    int32_t err_close;

    /* Create zip writer */
    mz_zip_writer_create(&writer);
    mz_zip_writer_set_compress_method(writer, MZ_COMPRESS_METHOD_DEFLATE);
    mz_zip_writer_set_compress_level(writer, MZ_COMPRESS_LEVEL_FAST);
    mz_zip_writer_set_comment(writer, "MiniZip");

    err = mz_zip_writer_open_file(writer, archive_path.c_str(), 0, 1);

    if (err == MZ_OK) {
        for (const std::string file : file_paths) {
            /* Add file system path to zip */
            err = mz_zip_writer_add_path(writer, file.c_str(), NULL, 0, 1);
            if (err != MZ_OK)
                printf("Error %d adding path to zip %s\n", err, file.c_str());
        }
    } else {
        printf("Error %d opening zip for writing\n", err);
    }

    err_close = mz_zip_writer_close(writer);
    if (err_close != MZ_OK) {
        printf("Error %d closing zip for writing %s\n", err_close, archive_path.c_str());
    }

    mz_zip_writer_delete(&writer);
}
