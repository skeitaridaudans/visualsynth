//
// Created by Guðmundur on 3/12/2023.
//

#include "FontAwesome.h"

const std::unique_ptr<fa::QtAwesome>& fontAwesome()
{
    static bool initialized = false;
    static const std::unique_ptr<fa::QtAwesome> fontAwesome = std::make_unique<fa::QtAwesome>();

    if (!initialized) {
    fontAwesome->initFontAwesome();
    initialized = true;
    }

    return fontAwesome;
}