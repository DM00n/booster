// Copyright 2018 Your Name <your_email>

#ifndef INCLUDE_HEADER_HPP_
#define INCLUDE_HEADER_HPP_
#include <iostream>
#include <boost/filesystem.hpp>
#include <map>
#include <string>
#include <utility>

#define CURRENT_DIR "/home/rex/CLionProjects/boost/ftp"
using boost::filesystem::path;
using boost::filesystem::directory_entry;
using boost::filesystem::directory_iterator;
struct _account_info{
    std::string _broker = "";
    std::string _last_date = "";
    unsigned _count = 1;
};
class FTP_parser{
public:
    FTP_parser(){
        _path = CURRENT_DIR;
    }
    explicit FTP_parser(std::string path):_path(path){
        parse(path);
    }
    ~FTP_parser(){
        _path.clear();
        _MAS.clear();
    }
    void parse(const path& home){
        for (directory_entry& x : directory_iterator(home)){
            if (exists(x.path())) {
                if (is_directory(x.path())) parse(x.path());
                else
                    {
                    const path &p{x.path()};
                    if ((is_regular_file(p)) && (normal_file(p))) {
                        all_files_printer(home, p);
                        _MAS_set(home, p);
                    }
                }
            }
        }
    }
    void _MAS_set(const path& home, const path& p){
        std::string name = p.filename().string();
        std::string balance(name, 0, 7);
        std::string acc(name, 8, 8);
        std::string date(name, 17, 8);
        std::string expansion = p.extension().string();
        if (_MAS.count(acc) == 0){
            std::string key = acc;
            _account_info value;
            value._broker = home.filename().string();
            value._last_date = date;
            value._count = 1;
            _MAS.insert(std::pair<std::string, _account_info>(key, value));
        } else {
            auto it = _MAS.find(acc);
            _account_info new_value = it -> second;
            new_value._count++;
            if (date >= new_value._last_date) new_value._last_date = date;
            _MAS.at(acc) = new_value;
        }
    }
    static bool normal_file(const path& p){
        std::string name = p.filename().string();
        if (name.length() != 29) return false;
        const char c = '_';
        std::string balance(name, 0, 7);
        std::string acc(name, 8, 8);
        std::string date(name, 17, 8);
        std::string expansion = p.extension().string();
        if ((name[7] != c) && (name[16] != c)) return false;
        if ((acc < "00000000") && (acc > "99999999")) return false;
        if ((date < "00000000") && (date > "99999999")) return false;
        if (balance != "balance") return false;
        if (expansion != ".txt") return false;
        return true;
    }
    static void all_files_printer(const path& home, const path& p){
        std::cout << home.filename().string()
                  << " " << p.filename().string()
                  << std::endl;
    }
    void account_printer(){
        for (auto & it : _MAS){
            std::cout << "broker:" << it.second._broker << " "
                      << "account:" << it.first << " "
                      << "files:" << it.second._count << " "
                      << "lastdate:" << it.second._last_date << std::endl;
        }
    }

private:
    std::map<std::string, _account_info> _MAS;
    std::string _path;
};

#endif //INCLUDE_HEADER_HPP_
