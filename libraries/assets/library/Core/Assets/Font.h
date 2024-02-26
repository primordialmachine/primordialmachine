// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(CORE_ASSETS_FONT_H_INCLUDED)
#define CORE_ASSETS_FONT_H_INCLUDED

#include "Core/Assets/Def.h"

/* http://localhost/assets#core-assets-font */
Core_declareObjectType("Core.Assets.Font",
                       Core_Assets_Font,
                       Core_Assets_Def);

static inline Core_Assets_Font* CORE_ASSETS_FONT(void* p) {
  return (Core_Assets_Font*)p;
}

struct Core_Assets_Font {
  Core_Assets_Def _parent;

  /// @brief A pointer to the name of this font asset.
  Core_String* name;

  /// @brief The pathname of the font file.
  Core_String* fontFilePathName;

  /// @brief The size of the font.
  Core_Integer32 fontSize;
};

static inline Core_Assets_Font_Dispatch* CORE_ASSETS_FONT_DISPATCH(void* p) {
  return (Core_Assets_Font_Dispatch*)p;
}

struct Core_Assets_Font_Dispatch {
  Core_Assets_Def_Dispatch _parent;
};

#endif // CORE_ASSETS_FONT_H_INCLUDED
