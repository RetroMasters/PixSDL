#pragma once

#include<string>

namespace pix
{

    // Returns the user- and application-specific directory where files can be written, such as preferences, logs, save files, and settings.
    // The returned path includes the trailing path separator.
    // 
    // On Windows, the returned path might look like:
    // C:\Users\Daniel\AppData\Roaming\companyName\appName\
    //
    // On failure, returns an empty string.
    std::string GetPrefPath(const std::string& companyName, const std::string& appName);

    // Returns the directory where the application executable is located.
    // The returned path includes the trailing path separator.
    // This is useful for loading assets that are shipped next to the executable.
    // On failure, returns an empty string.
    std::string GetBasePath();

}