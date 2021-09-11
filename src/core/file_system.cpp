//
// Created by 匡涂上青 on 2021/6/21.
//

#include "file_system.h"

#include <regex>

namespace csugl::fs {

    std::string get_file_name(const std::string &path_file) {
        std::string ret_name;
        // split by '/' or '\'
        std::regex regex_div_slash(R"(\\|\/)");
        std::vector<std::string> split_div_slash(
                std::sregex_token_iterator(path_file.begin(), path_file.end(), regex_div_slash, -1),
                std::sregex_token_iterator()
        );
        // split by '.'
        std::regex regex_dot("\\.");
        if (!split_div_slash.empty()) {
            if (split_div_slash.size() > 1 || (split_div_slash.size() == 1 &&
                                               path_file.find('/') == std::string::npos &&
                                               path_file.find('\\') == std::string::npos)) {
                auto &slashed = split_div_slash[split_div_slash.size() - 1];
                std::vector<std::string> split_dot(
                        std::sregex_token_iterator(slashed.begin(), slashed.end(), regex_dot, -1),
                        std::sregex_token_iterator()
                );
                if (!split_dot.empty()) {
                    ret_name = split_dot[0];
                    if (split_dot.size() > 2 || (split_dot.size() == 2 && split_dot[0] != "")) {
                        for (int i = 1; i < split_dot.size(); i++) {
                            if (i < split_dot.size() - 1) {
                                ret_name += ("." + split_dot[i]);
                            }
                        }
                    } else if (split_dot.size() > 1) {
                        for (int i = 1; i < split_dot.size(); i++) {
                            ret_name += ("." + split_dot[i]);
                        }
                    }
                }
            }
        }
        return ret_name;
    }

    std::string get_file_total(const std::string &path_file) {
        // split by '/' or '\'
        std::regex regex_div_slash(R"(\\|\/)");
        std::vector<std::string> split_div_slash(
                std::sregex_token_iterator(path_file.begin(), path_file.end(), regex_div_slash, -1),
                std::sregex_token_iterator()
        );
        if (!split_div_slash.empty()) {
            if (split_div_slash.size() > 1 || (split_div_slash.size() == 1 &&
                                               path_file.find('/') == std::string::npos &&
                                               path_file.find('\\') == std::string::npos))
                return split_div_slash[split_div_slash.size() - 1];
        }
        return "";
    }

    std::string get_file_extend(const std::string &path_file) {
        // split by '/' or '\'
        std::regex regex_div_slash(R"(\\|\/)");
        std::vector<std::string> split_div_slash(
                std::sregex_token_iterator(path_file.begin(), path_file.end(), regex_div_slash, -1),
                std::sregex_token_iterator()
        );
        // split by '.'
        std::regex regex_dot("\\.");
        if (!split_div_slash.empty()) {
            auto &slashed = split_div_slash[split_div_slash.size() - 1];
            std::vector<std::string> split_dot(
                    std::sregex_token_iterator(slashed.begin(), slashed.end(), regex_dot, -1),
                    std::sregex_token_iterator()
            );
            if ((split_dot.size() == 2 && !split_dot[0].empty()) || split_dot.size() > 2) {
                return "." + split_dot[split_dot.size() - 1];
            }
        }
        return "";
    }

    std::string get_file_path(const std::string &path_file) {
        // split by '/' or '\'
        std::regex regex_div_slash(R"(\\|\/)");
        std::vector<std::string> split_div_slash(
                std::sregex_token_iterator(path_file.begin(), path_file.end(), regex_div_slash, -1),
                std::sregex_token_iterator()
        );
        if (!split_div_slash.empty()) {
            if ((split_div_slash.size() == 1 &&
                path_file.find('/') != std::string::npos) ||
                path_file.find('\\') != std::string::npos) {
                // only path
                return split_div_slash[split_div_slash.size() - 1] + "/";
            } else if (split_div_slash.size() > 1) {
                std::string ret_name = split_div_slash[0];
                for (int i = 1; i < split_div_slash.size(); i++) {
                    if (i < split_div_slash.size() - 1) {
                        ret_name += ("/" + split_div_slash[i]);
                    }
                }
                return ret_name + "/";
            }
        }
        return "";
    }

    std::string replace_file_name(const std::string &path_file, const std::string &rename) {
        std::string path_name = get_file_path(path_file);
        std::string ext_name = get_file_extend(path_file);
        return path_name + rename + ext_name;
    }

    void replace_file_name(std::string &path_file, const std::string &rename) {
        std::string path_name = get_file_path(path_file);
        std::string ext_name = get_file_extend(path_file);
        path_file = path_name + rename + ext_name;
    }

    std::string replace_file(const std::string &path_file, const std::string &re_file) {
        std::string path_name = get_file_path(path_file);
        return path_name + re_file;
    }
}
