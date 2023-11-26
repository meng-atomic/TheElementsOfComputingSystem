#pragma once

#include <filesystem>
#include <vector>
#include <string>


void get_all_files(const std::string& path, const std::string& ext,
  std::vector<std::string>& files) {
  if (std::filesystem::is_directory(path)) {
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
      get_all_jack_files(entry.path());
    }
  } else if (std::filesystem::is_regular_file(path)) {
    if (path.size() > 3) {
      if (path.substr(path.size() - 3) == std::string(ext)) {
        files.push_back(path);
      }
    }
  }
}

void get_file_name(const std::string& path, std::string& dir, std::string& name, std::string& ext) {
  auto pos_ext = path.find_last_of('.');
  if (pos_ext != std::string::npos) {
    ext = path.substr(pos_ext + 1);
  }
  auto pos_dash = path.find_last_of('/');
  if (pos_dash != std::string::npos) {
    if (pos_ext != std::string::npos) {
      name = path.substr(pos_dash + 1, pos_ext - pos_dash - 1);
    } else {
      name = path.substr(pos_dash + 1);
    }
    dir = path.substr(0, pos_dash);
  } else {
    dir = ".";
    if (pos_ext != std::string::npos) {
      name = path.substr(0, pos_ext);
    } else {
      name = path;
    }
  }
}