/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** PathHelper.cpp
*/

#include "pathhelper.hpp"
#include <iostream>

#if defined(_WIN32)
#    include <windows.h>
#elif defined(__APPLE__)
#    include <mach-o/dyld.h>
#elif defined(__linux__)
#    include <unistd.h>
#endif

PathHelper::PathHelper() : _baseDir(determineExecutableDir()) {}

std::string PathHelper::getBaseDir() const { return _baseDir; }

std::string PathHelper::getFullPath(const std::string &relativePath) const {
    return _baseDir + relativePath;
}

std::string PathHelper::getFontPath(const std::string &fontFileName) const {
    return _baseDir + "../../assets/fonts/" + fontFileName;
}

std::string PathHelper::getImagePath(const std::string &imageFileName) const {
    return _baseDir + "../../assets/images/" + imageFileName;
}

std::string PathHelper::getSoundPath(const std::string &soundFileName) const {
    return _baseDir + "../../assets/sounds/" + soundFileName;
}

std::string PathHelper::getAssetPath(const std::string &assetFileName) const {
    return _baseDir + "../../assets/" + assetFileName;
}

std::string PathHelper::getShaderPath(const std::string &shaderFileName) const {
    return _baseDir + "../../assets/shaders/" + shaderFileName;
}

std::string PathHelper::determineExecutableDir() {
    char buffer[1024];
#if defined(_WIN32)
    GetModuleFileNameA(NULL, buffer, sizeof(buffer));
#elif defined(__APPLE__)
    uint32_t size = sizeof(buffer);
    if (_NSGetExecutablePath(buffer, &size) != 0) {
        std::cerr << "Buffer too small; need size " << size << std::endl;
        return "";
    }
#elif defined(__linux__)
    ssize_t count = readlink("/proc/self/exe", buffer, sizeof(buffer));
    if (count == -1) {
        std::cerr << "Failed to get executable path" << std::endl;
        return "";
    }
    buffer[count] = '\0';
#endif
    std::string exePath(buffer);
    return exePath.substr(0, exePath.find_last_of("/\\")) + "/";
}
