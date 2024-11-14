/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** pathhelper.hpp
*/

#ifndef PATH_HELPER_HPP_
#define PATH_HELPER_HPP_

#include <string>

class PathHelper {
  public:
    PathHelper();

    std::string getBaseDir() const;

    std::string getFullPath(const std::string &relativePath) const;

    std::string getFontPath(const std::string &fontFileName) const;

    std::string getImagePath(const std::string &imageFileName) const;

    std::string getSoundPath(const std::string &soundFileName) const;

    std::string getAssetPath(const std::string &assetFileName) const;

    std::string getShaderPath(const std::string &shaderFileName) const;

  private:
    std::string _baseDir;

    std::string determineExecutableDir();
};

#endif /* !PATH_HELPER_HPP_ */
