#include "../util/types.h"

//-
// The asset system is in charge of loading and passing around assets for other
// sub-systems of the engine to use. It bundles all the memory in one place for
// easy access and management and is initialized when the engine starts. If
// you're using the supplied Makefile, assets will be automatically prepared
// when you build the project.
//-

//*
// An AssetID is a simple and easy way to identify an asset, they should be
// unique and created by "src/fog_assets.cpp"

using AssetID = u64;

namespace Asset {

const u32 ASSET_ID_NO_ASSET = 0xFFFF;

enum class Type {
    NONE,
    TEXTURE,
    FONT,
    ATLAS,
    SOUND,
    SHADER,
    CONFIG,
    LEVEL
};

#pragma pack(4)
struct FileHeader {
    u64 number_of_assets;
    u64 size_of_headers;
    u64 size_of_strings;
    u64 size_of_data;
};

struct Header {
    static const char *string_list_base;

    static void set_string_list_ptr(const char *string_list) {
        string_list_base = string_list;
    }

    // Relative pointer until "rebuild_pointers".
    Type type;
    char *file_path;
    u64 file_path_length;
    u64 timestamp;
    u64 offset;
    u32 asset_size;
    u32 asset_id;
};

// NOTE(ed): Only ASCII is supported.
struct Font {
    struct Glyph {
        u8 id;
        f32 x, y;
        f32 w, h;
        f32 x_offset, y_offset;
        f32 advance;
    };

    struct Kerning {
        u16 key;
        f32 ammount;

        bool operator< (const Kerning &other) const {
            return key < other.key;
        }
    };

    f32 find_kerning(char a, char b) {
        u16 key = (a << 8) | b;
        s64 low = 0;
        s64 high = num_kernings;
        s64 last_guess = -1;
        while (low <= high) {
            s64 cur = (low + high) / 2;
            if (cur < 0 || num_kernings < cur || cur == last_guess)
                break;
            last_guess = cur;
            u16 cur_key = kernings[cur].key;
            if (cur_key > key)
                high = cur + 1;
            else if (cur_key < key)
                low = cur + 1;
            else
                return kernings[cur].ammount;
        }
        return 0;
    }

    u64 texture;
    f32 height;
    const s64 num_glyphs = 256;
    s64 num_kernings;
    bool monospace = false;

    Glyph *glyphs;
    Kerning *kernings;
};

struct Data {
    union {
        Image image;
        Sound sound;
        char *shader_source;
        Font font;
    };
};

//*
// Checks if the passed in "id" is mapped to an image,
// if it is an image is returned via pointer. It is
// not recommended to modify any data received from the
// asset system, as multiple threads could be reading
// from it and it's bound to cause headaches.
Image *fetch_image(AssetID id);

//*
// Checks if the passed in "id" is mapped to a font,
// if it is an image is returned via pointer. It is
// not recommended to modify any data received from the
// asset system, as multiple threads could be reading
// from it and it's bound to cause headaches.
Font *fetch_font(AssetID id);

};  // namespace Asset

#if _EXAMPLES_

////
// <h2>Loading a new asset</h2>
// Place the desired asset in the "res" folder, make sure it's 
// a compatible type for the engine, and rebuild the project.
// The "src/fog_assets.cpp" file has now been updated and the
// new constant can be used where "AssetID"s are asked for.
////

#endif

// The file format: 
//
// Number of Assets,
// Size of String list,
// Size of header,
// size of body
// =============================
// String list
// =============================
// Headers
// =============================
// Assets
