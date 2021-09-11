//
// Created by 匡涂上青 on 2021/6/21.
//

#ifndef CSUGL_FILE_SYSTEM_H
#define CSUGL_FILE_SYSTEM_H

#include <string>

namespace csugl::fs {
    /**
     * Get file name of path_file
     * @param path_file path/file_name.obj
     * @return file_name
     */
    std::string get_file_name(const std::string &path_file);

    /**
     * Get file name (with extend name) of path_file
     * @param path_file path/file_name.obj
     * @return file_name.obj
     */
    std::string get_file_total(const std::string &path_file);

    /**
     * Get file extend name of path_file
     * @param path_file path/file_name.obj
     * @return obj
     */
    std::string get_file_extend(const std::string &path_file);

    /**
     * Get file path of path_file
     * @param path_file path/file_name.obj
     * @return path
     */
    std::string get_file_path(const std::string &path_file);

    /**
     * Replace the filename
     * @param path_file path/file_name.obj
     * @param rename rename file_name
     * @return path/rename.obj
     */
    std::string replace_file_name(const std::string &path_file, const std::string &rename);

    /**
     * Replace the filename with origin path
     * @param path_file path/file_name.obj
     * @param rename rename file_name
     */
    void replace_file_name(std::string &path_file, const std::string &rename);

    std::string replace_file(const std::string&path_file, const std::string&re_file);

}


#endif //CSUGL_FILE_SYSTEM_H
